/******************************************************************************
* Copyright (C), 2000-2006,  Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2017-6-3
* Description: avl map
* History: rename from aggregate to map
******************************************************************************/
#include "bs.h"
#include "utl/jhash_utl.h"
#include "utl/mem_utl.h"
#include "utl/mem_cap.h"
#include "utl/map_utl.h"
#include "utl/avllib_utl.h"

typedef struct
{
    AVL_TREE avl_root;
    UINT count;
}_MAP_AVL_S;

static void map_avl_destroy(MAP_HANDLE map, PF_MAP_FREE_FUNC free_func, void *ud);
static void map_avl_reset(MAP_HANDLE map, PF_MAP_FREE_FUNC free_func, void *ud);
static int map_avl_add_node(MAP_HANDLE map, LDATA_S *key, void *pData, void *node, UINT flag);
static BS_STATUS map_avl_add(MAP_HANDLE map, VOID *pKey, UINT uiKeyLen, VOID *pData, UINT flag);
static MAP_ELE_S * map_avl_get_ele(MAP_HANDLE map, void *key, UINT key_len);
static void * map_avl_get(IN MAP_HANDLE map, IN VOID *pKey, IN UINT uiKeyLen);
static void * map_avl_del(IN MAP_HANDLE map, IN VOID *pKey, IN UINT uiKeyLen);
static void * map_avl_del_node(MAP_HANDLE map, void *n);
static void * map_avl_del_by_ele(IN MAP_HANDLE map, IN MAP_ELE_S *ele);
static void map_avl_del_all(MAP_HANDLE map, PF_MAP_FREE_FUNC func, void * pUserData);
static UINT map_avl_count(MAP_HANDLE map);
static void map_avl_walk(IN MAP_HANDLE map, IN PF_MAP_WALK_FUNC pfWalkFunc, IN VOID *pUserData);
static MAP_ELE_S * map_avl_getnext(MAP_HANDLE map, MAP_ELE_S *pstCurrent);

static MAP_FUNC_S g_map_avl_funcs = {
    .destroy_func = map_avl_destroy,
    .reset_func = map_avl_reset,
    .add_node_func = map_avl_add_node,
    .del_node_func = map_avl_del_node,
    .add_func = map_avl_add,
    .get_ele_func = map_avl_get_ele,
    .get_func = map_avl_get,
    .del_func = map_avl_del,
    .del_by_ele_func = map_avl_del_by_ele,
    .del_all_func = map_avl_del_all,
    .count_func = map_avl_count,
    .walk_func = map_avl_walk,
    .getnext_func = map_avl_getnext
};

static int _map_avl_cmp(void *key, void * pstCmpNode)
{
    MAP_AVL_NODE_S *pstNode = pstCmpNode;
    MAP_ELE_S *ele = key;

    if (pstNode->stEle.uiKeyLen == 0) {
        
        return (INT)HANDLE_UINT(ele->pKey) - (INT)HANDLE_UINT(pstNode->stEle.pKey);
    }

    return MEM_Cmp(ele->pKey, ele->uiKeyLen, pstNode->stEle.pKey, pstNode->stEle.uiKeyLen);
}

static MAP_AVL_NODE_S * _map_avl_find(IN MAP_CTRL_S *map, IN VOID *pKey, IN UINT uiKeyLen)
{
    _MAP_AVL_S *avl_map = map->impl_map;
    MAP_ELE_S ele;
    
    ele.pKey = pKey;
    ele.uiKeyLen = uiKeyLen;

    return avlSearch(avl_map->avl_root, &ele, _map_avl_cmp);
}

static int _map_avl_walk(void *node, void *ud)
{
    USER_HANDLE_S *pstUserHandle = ud;
    PF_MAP_WALK_FUNC pfWalkFunc = pstUserHandle->ahUserHandle[0];
    VOID *pUserHandleTmp = pstUserHandle->ahUserHandle[1];
    MAP_AVL_NODE_S *pstNode = node;

    return pfWalkFunc(&pstNode->stEle, pUserHandleTmp);
}

static void _map_avl_free(void *pNode, void *ud)
{
    USER_HANDLE_S *pstUserHandle = ud;
    PF_MAP_FREE_FUNC func = pstUserHandle->ahUserHandle[0];
    VOID *pUserHandleTmp = pstUserHandle->ahUserHandle[1];
    MAP_CTRL_S *map = pstUserHandle->ahUserHandle[2];
    MAP_AVL_NODE_S *pstNode = pNode;

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

static void map_avl_reset(MAP_HANDLE map, PF_MAP_FREE_FUNC free_func, void *ud)
{
    map_avl_del_all(map, free_func, ud);
}

static void map_avl_destroy(MAP_HANDLE map, PF_MAP_FREE_FUNC free_func, void *ud)
{
    _MAP_AVL_S *avl_map = map->impl_map;

    map_avl_del_all(map, free_func, ud);
    MemCap_Free(map->memcap, avl_map);
    MemCap_Free(map->memcap, map);
}

static inline int _map_avl_add(MAP_HANDLE map, VOID *pKey, UINT uiKeyLen,
        void *pData, MAP_AVL_NODE_S *pstNode, UINT flag)
{
    _MAP_AVL_S *avl_map = map->impl_map;

    if ((flag & MAP_FLAG_DUP_KEY) && (uiKeyLen != 0)) {
        pstNode->stEle.pKey = MemCap_Dup(map->memcap, pKey, uiKeyLen);
        if (NULL == pstNode->stEle.pKey) {
            RETURN(BS_NO_MEMORY);
        }
        pstNode->stEle.dup_key = 1;
    } else {
        pstNode->stEle.dup_key = 0;
        pstNode->stEle.pKey = pKey;
    }

    pstNode->stEle.uiKeyLen = uiKeyLen;
    pstNode->stEle.pData = pData;

    if (0 != avlInsert(&avl_map->avl_root, pstNode, &pstNode->stEle, _map_avl_cmp)) {
        if (pstNode->stEle.dup_key) {
            MemCap_Free(map->memcap, pstNode->stEle.pKey);
        }
        RETURN(BS_ALREADY_EXIST);
    }

    avl_map->count ++;

    return BS_OK;
}

static int map_avl_add_node(MAP_HANDLE map, LDATA_S *key, void *pData, void *node, UINT flag)
{
    _MAP_AVL_S *avl_map = map->impl_map;
    MAP_AVL_NODE_S *pstNode = node;

    BS_DBGASSERT(0 == (flag & MAP_FLAG_PERMIT_DUPLICATE));

    if ((map->uiCapacity != 0) && (map->uiCapacity <= avl_map->count)) {
        RETURN(BS_NO_RESOURCE);
    }

    pstNode->stEle.link_alloced = 0;

    return _map_avl_add(map, key->data, key->len, pData, pstNode, flag);
}

static BS_STATUS map_avl_add(MAP_HANDLE map, VOID *pKey, UINT uiKeyLen, VOID *pData, UINT flag)
{
    _MAP_AVL_S *avl_map = map->impl_map;
    MAP_AVL_NODE_S *pstNode;

    BS_DBGASSERT(0 == (flag & MAP_FLAG_PERMIT_DUPLICATE));

    if ((map->uiCapacity != 0) && (map->uiCapacity <= avl_map->count)) {
        RETURN(BS_NO_RESOURCE);
    }

    pstNode = MemCap_ZMalloc(map->memcap, sizeof(MAP_AVL_NODE_S));
    if (NULL == pstNode) {
        RETURN(BS_NO_MEMORY);
    }
    pstNode->stEle.link_alloced = 1;

    int ret = _map_avl_add(map, pKey, uiKeyLen, pData, pstNode, flag);
    if (ret < 0) {
        MemCap_Free(map->memcap, pstNode);
        return ret;
    }

    return BS_OK;
}

static MAP_ELE_S * map_avl_get_ele(MAP_HANDLE map, void *key, UINT key_len)
{
    MAP_AVL_NODE_S *pstFind;

    pstFind = _map_avl_find(map, key, key_len);
    if (NULL == pstFind) {
        return NULL;
    }

    return &pstFind->stEle;
}

static void * map_avl_get(IN MAP_HANDLE map, IN VOID *pKey, IN UINT uiKeyLen)
{
    MAP_AVL_NODE_S *pstFind;

    pstFind = _map_avl_find(map, pKey, uiKeyLen);
    if (NULL == pstFind) {
        return NULL;
    }

    return pstFind->stEle.pData;
}

static void * _map_avl_del_node(IN MAP_HANDLE map, IN MAP_AVL_NODE_S *pstNode)
{
    void *pData = pstNode->stEle.pData;
    _MAP_AVL_S *avl_map = map->impl_map;

    if (pstNode->stEle.dup_key) {
        MemCap_Free(map->memcap, pstNode->stEle.pKey);
    }
    if (pstNode->stEle.link_alloced) {
        MemCap_Free(map->memcap, pstNode);
    }

    avl_map->count --;

    return pData;
}

static void * map_avl_del_by_ele(IN MAP_HANDLE map, IN MAP_ELE_S *ele)
{
    MAP_AVL_NODE_S *node = container_of(ele, MAP_AVL_NODE_S, stEle);
    return _map_avl_del_node(map, node);
}


static void * map_avl_del(IN MAP_HANDLE map, IN VOID *pKey, IN UINT uiKeyLen)
{
    _MAP_AVL_S *avl_map = map->impl_map;
    MAP_ELE_S ele;

    ele.pKey = pKey;
    ele.uiKeyLen = uiKeyLen;

    MAP_AVL_NODE_S *pstNode = avlDelete(&avl_map->avl_root, &ele, _map_avl_cmp);
    if (! pstNode) {
        return NULL;
    }

    return _map_avl_del_node(map, pstNode);
}


static void * map_avl_del_node(MAP_HANDLE map, void *n)
{
    MAP_AVL_NODE_S *pstNode = n;
    return _map_avl_del_node(map, pstNode);
}

static void map_avl_del_all(MAP_HANDLE map, PF_MAP_FREE_FUNC func, void * pUserData)
{
    _MAP_AVL_S *avl_map = map->impl_map;
    USER_HANDLE_S stUserHandle;

    stUserHandle.ahUserHandle[0] = func;
    stUserHandle.ahUserHandle[1] = pUserData;
    stUserHandle.ahUserHandle[2] = map;

    avlTreeErase(&avl_map->avl_root, _map_avl_free, &stUserHandle);

    avl_map->count = 0;
}

static UINT map_avl_count(MAP_HANDLE map)
{
    _MAP_AVL_S *avl_map = map->impl_map;
    return avl_map->count;
}

static void map_avl_walk(IN MAP_HANDLE map, IN PF_MAP_WALK_FUNC pfWalkFunc, IN VOID *pUserData)
{
    _MAP_AVL_S *avl_map = map->impl_map;
    USER_HANDLE_S stUserHandle;

    stUserHandle.ahUserHandle[0] = pfWalkFunc;
    stUserHandle.ahUserHandle[1] = pUserData;

    avlTreeWalk(&avl_map->avl_root, _map_avl_walk, &stUserHandle);
}


static MAP_ELE_S * map_avl_getnext(MAP_HANDLE map, MAP_ELE_S *pstCurrent)
{
    _MAP_AVL_S *avl_map = map->impl_map;
    MAP_AVL_NODE_S *node;

    if (NULL == pstCurrent) {
        node = avlMinimumGet(avl_map->avl_root);
    } else {
        node = avlSuccessorGet(avl_map->avl_root, pstCurrent, _map_avl_cmp);
    }

    if (! node) {
        return NULL;
    }

    return &node->stEle;
}

MAP_HANDLE MAP_AvlCreate(void *memcap)
{
    MAP_CTRL_S *ctrl;
    _MAP_AVL_S *avl_map;

    ctrl = MemCap_ZMalloc(memcap, sizeof(MAP_CTRL_S));
    if (! ctrl) {
        return NULL;
    }
    ctrl->memcap = memcap;
    ctrl->funcs = &g_map_avl_funcs;

    avl_map = MemCap_ZMalloc(memcap, sizeof(_MAP_AVL_S));
    if (NULL == avl_map) {
        MemCap_Free(memcap, ctrl);
        return NULL;
    }
    ctrl->impl_map = avl_map;

    return ctrl;
}


