/******************************************************************************
* Copyright (C), 2000-2006,  Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2017-6-3
* Description: hash map
* History: rename from aggregate to map
******************************************************************************/
#include "bs.h"
#include "utl/jhash_utl.h"
#include "utl/hash_utl.h"
#include "utl/mem_utl.h"
#include "utl/mem_cap.h"
#include "utl/map_utl.h"

#define _MAP_HASH_BUCKET_NUM 1024

typedef struct
{
    HASH_S * hHash;
}_MAP_HASH_S;

static void map_hash_destroy(MAP_HANDLE map, PF_MAP_FREE_FUNC free_func, void *ud);
static void map_hash_reset(MAP_HANDLE map, PF_MAP_FREE_FUNC free_func, void *ud);
static int map_hash_add_node(MAP_HANDLE map, LDATA_S *key, void *pData, void *node, UINT flag);
static void * map_hash_del_node(MAP_HANDLE map, void *node);
static int map_hash_add(MAP_HANDLE map, VOID *pKey, UINT uiKeyLen, VOID *pData, UINT flag);
static MAP_ELE_S * map_hash_get_ele(MAP_HANDLE map, void *key, UINT key_len);
static void * map_hash_get(IN MAP_HANDLE map, IN VOID *pKey, IN UINT uiKeyLen);
static void * map_hash_del(IN MAP_HANDLE map, IN VOID *pKey, IN UINT uiKeyLen);
static void * map_hash_del_by_ele(IN MAP_HANDLE map, IN MAP_ELE_S *ele);
static void map_hash_del_all(MAP_HANDLE map, PF_MAP_FREE_FUNC func, void * pUserData);
static UINT map_hash_count(MAP_HANDLE map);
static void map_hash_walk(IN MAP_HANDLE map, IN PF_MAP_WALK_FUNC pfWalkFunc, IN VOID *pUserData);
static MAP_ELE_S * map_hash_getnext(MAP_HANDLE map, MAP_ELE_S *pstCurrent);

static MAP_FUNC_S g_map_hash_funcs = {
    .destroy_func = map_hash_destroy,
    .reset_func = map_hash_reset,
    .add_node_func = map_hash_add_node,
    .del_node_func = map_hash_del_node,
    .add_func = map_hash_add,
    .get_ele_func = map_hash_get_ele,
    .get_func = map_hash_get,
    .del_func = map_hash_del,
    .del_by_ele_func = map_hash_del_by_ele,
    .del_all_func = map_hash_del_all,
    .count_func = map_hash_count,
    .walk_func = map_hash_walk,
    .getnext_func = map_hash_getnext
};

static inline UINT _map_key_hash_factor(IN void *key, UINT key_len)
{
    if (key_len == 0) {
        return HANDLE_UINT(key);
    }

    return JHASH_GeneralBuffer(key, key_len, 0);
}

static UINT _map_hash_factor(IN VOID *pstHashNode)
{
    MAP_HASH_NODE_S *pstNode = pstHashNode;

    return _map_key_hash_factor(pstNode->stEle.pKey, pstNode->stEle.uiKeyLen);
}

static int _map_hash_cmp(IN VOID * pstHashNode1, IN VOID * pstHashNode2)
{
    MAP_HASH_NODE_S *pstNode1 = pstHashNode1;
    MAP_HASH_NODE_S *pstNode2 = pstHashNode2;

    if (pstNode1->stEle.uiKeyLen != pstNode2->stEle.uiKeyLen) {
        return -1;
    }

    if (pstNode1->stEle.uiKeyLen == 0) {
        
        return (INT)HANDLE_UINT(pstNode1->stEle.pKey) - (INT)HANDLE_UINT(pstNode2->stEle.pKey);
    }

    return MEM_Cmp(pstNode1->stEle.pKey, pstNode1->stEle.uiKeyLen, pstNode2->stEle.pKey, pstNode2->stEle.uiKeyLen);
}

static MAP_HASH_NODE_S * _map_hash_find(IN MAP_CTRL_S *map, IN VOID *pKey, IN UINT uiKeyLen)
{
    _MAP_HASH_S *hash_map = map->impl_map;
    MAP_HASH_NODE_S stNode;
    
    stNode.stEle.pKey = pKey;
    stNode.stEle.uiKeyLen = uiKeyLen;

    return HASH_Find(hash_map->hHash, _map_hash_cmp, &stNode);
}

static int _map_hash_walk(IN void *hHashId, IN VOID *pNode, IN VOID * pUserHandle)
{
    USER_HANDLE_S *pstUserHandle = pUserHandle;
    PF_MAP_WALK_FUNC pfWalkFunc = pstUserHandle->ahUserHandle[0];
    VOID *pUserHandleTmp = pstUserHandle->ahUserHandle[1];
    MAP_HASH_NODE_S *pstNode = pNode;

    return pfWalkFunc(&pstNode->stEle, pUserHandleTmp);
}

static void _map_hash_free_all(IN void * hHashId, IN VOID *pNode, IN VOID * pUserHandle)
{
    USER_HANDLE_S *pstUserHandle = pUserHandle;
    PF_MAP_FREE_FUNC func = pstUserHandle->ahUserHandle[0];
    VOID *pUserHandleTmp = pstUserHandle->ahUserHandle[1];
    MAP_CTRL_S *map = pstUserHandle->ahUserHandle[2];
    MAP_HASH_NODE_S *pstNode = pNode;

    if (func) {
        func(pstNode->stEle.pData, pUserHandleTmp);
    }
    if (pstNode->stEle.dup_key) {
        MemCap_Free(map->memcap, pstNode->stEle.pKey);
    }
    if (pstNode->stEle.link_alloced) {
        MemCap_Free(map->memcap, pstNode);
    }
}

static int _map_hash_getnext_cmp(IN MAP_ELE_S *pstNode, IN MAP_ELE_S *current)
{
    INT iCmpRet;

    if (current->uiKeyLen == 0) {
        
        return (INT)HANDLE_UINT(pstNode->pKey) - (INT)HANDLE_UINT(current->pKey);
    }

    iCmpRet = MEM_Cmp(pstNode->pKey, pstNode->uiKeyLen, current->pKey, current->uiKeyLen);

    return iCmpRet;
}

static int _map_hash_getnext_each(IN MAP_ELE_S *pstEle, IN HANDLE hUserHandle)
{
    USER_HANDLE_S *pstUserHandle = hUserHandle;
    MAP_ELE_S *pstEleCurrent = pstUserHandle->ahUserHandle[0];
    MAP_ELE_S *pstEleNext = pstUserHandle->ahUserHandle[1];

    if ((pstEleCurrent != NULL) && (_map_hash_getnext_cmp(pstEle, pstEleCurrent) <= 0)) {
        return 0;
    }

    if ((pstEleNext != NULL) && (_map_hash_getnext_cmp(pstEle, pstEleNext) >= 0)) {
        return 0;
    }

    pstUserHandle->ahUserHandle[1] = pstEle;

    return 0;
}


static void map_hash_reset(MAP_HANDLE map, PF_MAP_FREE_FUNC free_func, void *ud)
{
    map_hash_del_all(map, free_func, ud);
}

static void map_hash_destroy(MAP_HANDLE map, PF_MAP_FREE_FUNC free_func, void *ud)
{
    _MAP_HASH_S *hash_map = map->impl_map;

    map_hash_del_all(map, free_func, ud);
    HASH_DestoryInstance(hash_map->hHash);
    MemCap_Free(map->memcap, hash_map);
    MemCap_Free(map->memcap, map);
}

static int _map_hash_add_check(MAP_HANDLE map, VOID *pKey, UINT uiKeyLen, UINT flag, UINT hash_factor)
{
    _MAP_HASH_S *hash_map = map->impl_map;
    MAP_HASH_NODE_S *pstFind;
    MAP_HASH_NODE_S stNodeToFind;

    if ((map->uiCapacity != 0) && (map->uiCapacity <= HASH_Count(hash_map->hHash))) {
        RETURN(BS_NO_RESOURCE);
    }

    if (0 == (flag & MAP_FLAG_PERMIT_DUPLICATE)) {
        stNodeToFind.stEle.pKey = pKey;
        stNodeToFind.stEle.uiKeyLen = uiKeyLen;
        pstFind = HASH_FindWithFactor(hash_map->hHash, hash_factor, _map_hash_cmp, &stNodeToFind);
        if (pstFind) {
            RETURN(BS_ALREADY_EXIST);
        }
    }

    return 0;
}

static inline int _map_hash_add(MAP_HANDLE map, void *pKey, UINT uiKeyLen, void *pData,
        MAP_HASH_NODE_S *pstNode, UINT flag, UINT hash_factor)
{
    _MAP_HASH_S *hash_map = map->impl_map;

    if ((flag & MAP_FLAG_DUP_KEY) && (uiKeyLen != 0)) {
        pstNode->stEle.pKey = MemCap_Dup(map->memcap, pKey, uiKeyLen);
        if (NULL == pstNode->stEle.pKey) {
            RETURN(BS_NO_MEMORY);
        }
        pstNode->stEle.dup_key = 1;
    } else {
        pstNode->stEle.pKey = pKey;
    }

    pstNode->stEle.uiKeyLen = uiKeyLen;
    pstNode->stEle.pData = pData;

    HASH_AddWithFactor(hash_map->hHash, pstNode, hash_factor);

    return BS_OK;
}

static int map_hash_add_node(MAP_HANDLE map, LDATA_S *key, void *pData, void *node, UINT flag)
{
    MAP_HASH_NODE_S *pstNode = node;
    UINT hash_factor;
    int ret;

    hash_factor = _map_key_hash_factor(key->data, key->len);

    ret = _map_hash_add_check(map, key->data, key->len, flag, hash_factor);
    if (ret < 0) {
        return ret;
    }

    pstNode->stEle.link_alloced = 0;

    ret = _map_hash_add(map, key->data, key->len, pData, pstNode, flag, hash_factor);
    if (ret < 0) {
        return ret;
    }

    return 0;
}

static BS_STATUS map_hash_add(MAP_HANDLE map, VOID *pKey, UINT uiKeyLen, VOID *pData, UINT flag)
{
    MAP_HASH_NODE_S *pstNode;
    UINT hash_factor;
    int ret;

    hash_factor = _map_key_hash_factor(pKey, uiKeyLen);

    ret = _map_hash_add_check(map, pKey, uiKeyLen, flag, hash_factor);
    if (ret < 0) {
        return ret;
    }

    pstNode = MemCap_ZMalloc(map->memcap, sizeof(MAP_HASH_NODE_S));
    if (NULL == pstNode) {
        RETURN(BS_NO_MEMORY);
    }

    pstNode->stEle.link_alloced = 1;

    ret = _map_hash_add(map, pKey, uiKeyLen, pData, pstNode, flag, hash_factor);
    if (ret < 0) {
        MemCap_Free(map->memcap, pstNode);
        return ret;
    }

    return BS_OK;
}

static MAP_ELE_S * map_hash_get_ele(MAP_HANDLE map, void *key, UINT key_len)
{
    MAP_HASH_NODE_S *pstFind;

    pstFind = _map_hash_find(map, key, key_len);
    if (NULL == pstFind) {
        return NULL;
    }

    return &pstFind->stEle;
}

static void * map_hash_get(IN MAP_HANDLE map, IN VOID *pKey, IN UINT uiKeyLen)
{
    MAP_HASH_NODE_S *pstFind;

    pstFind = _map_hash_find(map, pKey, uiKeyLen);
    if (NULL == pstFind) {
        return NULL;
    }

    return pstFind->stEle.pData;
}

static void * _map_hash_del_node(IN MAP_HANDLE map, IN MAP_HASH_NODE_S *pstNode)
{
    _MAP_HASH_S *hash_map = map->impl_map;

    HASH_Del(hash_map->hHash, pstNode);

    void *pData = pstNode->stEle.pData;

    if (pstNode->stEle.dup_key) {
        MemCap_Free(map->memcap, pstNode->stEle.pKey);
    }

    if (pstNode->stEle.link_alloced) {
        MemCap_Free(map->memcap, pstNode);
    }

    return pData;
}

static void * map_hash_del_by_ele(IN MAP_HANDLE map, IN MAP_ELE_S *ele)
{
    MAP_HASH_NODE_S *node = container_of(ele, MAP_HASH_NODE_S, stEle);
    return _map_hash_del_node(map, node);
}


static void * map_hash_del(IN MAP_HANDLE map, IN VOID *pKey, IN UINT uiKeyLen)
{
    MAP_HASH_NODE_S *pstNode;

    pstNode = _map_hash_find(map, pKey, uiKeyLen);
    if (NULL == pstNode) {
        return NULL;
    }

    return _map_hash_del_node(map, pstNode);
}


static void * map_hash_del_node(MAP_HANDLE map, void *node)
{
    MAP_HASH_NODE_S *pstNode = node;
    return _map_hash_del_node(map, pstNode);
}

static void map_hash_del_all(MAP_HANDLE map, PF_MAP_FREE_FUNC func, void * pUserData)
{
    _MAP_HASH_S *hash_map = map->impl_map;
    USER_HANDLE_S stUserHandle;

    stUserHandle.ahUserHandle[0] = func;
    stUserHandle.ahUserHandle[1] = pUserData;
    stUserHandle.ahUserHandle[2] = map;

    HASH_DelAll(hash_map->hHash, _map_hash_free_all, &stUserHandle);
}

static UINT map_hash_count(MAP_HANDLE map)
{
    _MAP_HASH_S *hash_map = map->impl_map;
    return HASH_Count(hash_map->hHash);
}

static void map_hash_walk(IN MAP_HANDLE map, IN PF_MAP_WALK_FUNC pfWalkFunc, IN VOID *pUserData)
{
    _MAP_HASH_S *hash_map = map->impl_map;
    USER_HANDLE_S stUserHandle;

    stUserHandle.ahUserHandle[0] = pfWalkFunc;
    stUserHandle.ahUserHandle[1] = pUserData;

    HASH_Walk(hash_map->hHash, _map_hash_walk, &stUserHandle);
}


static MAP_ELE_S * map_hash_getnext(MAP_HANDLE map, MAP_ELE_S *pstCurrent)
{
    USER_HANDLE_S stUserHandle;

    stUserHandle.ahUserHandle[0] = pstCurrent;
    stUserHandle.ahUserHandle[1] = NULL;
  
    map_hash_walk(map, _map_hash_getnext_each, &stUserHandle);

    return stUserHandle.ahUserHandle[1];
}

static MAP_PARAM_S g_hash_map_dft_param = {0};


MAP_HANDLE MAP_HashCreate(MAP_PARAM_S *p)
{
    MAP_CTRL_S *ctrl;
    _MAP_HASH_S *hash_map;

    if (p == NULL) {
        p = &g_hash_map_dft_param;
    }

    ctrl = MemCap_ZMalloc(p->memcap, sizeof(MAP_CTRL_S));
    if (! ctrl) {
        return NULL;
    }
    ctrl->memcap = p->memcap;
    ctrl->funcs = &g_map_hash_funcs;

    hash_map = MemCap_ZMalloc(p->memcap, sizeof(_MAP_HASH_S));
    if (NULL == hash_map) {
        MemCap_Free(p->memcap, ctrl);
        return NULL;
    }
    ctrl->impl_map = hash_map;

    UINT bucket_num = p->bucket_num;
    if (bucket_num == 0) {
        bucket_num = _MAP_HASH_BUCKET_NUM;
    }

    hash_map->hHash = HASH_CreateInstance(p->memcap, bucket_num, _map_hash_factor);
    if (NULL == hash_map->hHash) {
        MemCap_Free(p->memcap, hash_map);
        MemCap_Free(p->memcap, ctrl);
        return NULL;
    }

    return ctrl;
}



