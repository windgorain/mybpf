/******************************************************************************
* Copyright (C), 2000-2006,  Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2017-6-3
* Description: red black tree map
* History: rename from aggregate to map
******************************************************************************/
#include "bs.h"
#include "utl/jhash_utl.h"
#include "utl/mem_utl.h"
#include "utl/mem_cap.h"
#include "utl/map_utl.h"
#include "utl/rb_tree.h"

typedef struct
{
    RB_TREE_CTRL_S root;
    UINT count;
}_MAP_RBTREE_S;

static void map_rbtree_destroy(MAP_HANDLE map, PF_MAP_FREE_FUNC free_func, void *ud);
static void map_rbtree_reset(MAP_HANDLE map, PF_MAP_FREE_FUNC free_func, void *ud);
static int map_rbtree_add_node(MAP_HANDLE map, LDATA_S *key, void *pData, void *node, UINT flag);
static BS_STATUS map_rbtree_add(MAP_HANDLE map, VOID *pKey, UINT uiKeyLen, VOID *pData, UINT flag);
static MAP_ELE_S * map_rbtree_get_ele(MAP_HANDLE map, void *key, UINT key_len);
static void * map_rbtree_get(IN MAP_HANDLE map, IN VOID *pKey, IN UINT uiKeyLen);
static void * map_rbtree_del(IN MAP_HANDLE map, IN VOID *pKey, IN UINT uiKeyLen);
static void * map_rbtree_del_node(MAP_HANDLE map, void *node);
static void * map_rbtree_del_by_ele(IN MAP_HANDLE map, IN MAP_ELE_S *ele);
static void map_rbtree_del_all(MAP_HANDLE map, PF_MAP_FREE_FUNC func, void * pUserData);
static UINT map_rbtree_count(MAP_HANDLE map);
static void map_rbtree_walk(IN MAP_HANDLE map, IN PF_MAP_WALK_FUNC pfWalkFunc, IN VOID *pUserData);
static MAP_ELE_S * map_rbtree_getnext(MAP_HANDLE map, MAP_ELE_S *pstCurrent);

static MAP_FUNC_S g_map_rbtree_funcs = {
    .destroy_func = map_rbtree_destroy,
    .reset_func = map_rbtree_reset,
    .add_node_func = map_rbtree_add_node,
    .del_node_func = map_rbtree_del_node,
    .add_func = map_rbtree_add,
    .get_ele_func = map_rbtree_get_ele,
    .get_func = map_rbtree_get,
    .del_func = map_rbtree_del,
    .del_by_ele_func = map_rbtree_del_by_ele,
    .del_all_func = map_rbtree_del_all,
    .count_func = map_rbtree_count,
    .walk_func = map_rbtree_walk,
    .getnext_func = map_rbtree_getnext
};

static int _map_rbtree_cmp(void *key, RB_TREE_NODE_S *pstCmpNode)
{
    MAP_RBTREE_NODE_S *pstNode = (void*)pstCmpNode;
    MAP_ELE_S *ele = key;

    if (pstNode->stEle.uiKeyLen == 0) {
        
        return (INT)HANDLE_UINT(ele->pKey) - (INT)HANDLE_UINT(pstNode->stEle.pKey);
    }

    return MEM_Cmp(ele->pKey, ele->uiKeyLen, pstNode->stEle.pKey, pstNode->stEle.uiKeyLen);
}

static MAP_RBTREE_NODE_S * _map_rbtree_find(IN MAP_CTRL_S *map, IN VOID *pKey, IN UINT uiKeyLen)
{
    _MAP_RBTREE_S *rbtree_map = map->impl_map;
    MAP_ELE_S ele;
    
    ele.pKey = pKey;
    ele.uiKeyLen = uiKeyLen;

    return (void*) RBTree_Search(&rbtree_map->root, &ele, _map_rbtree_cmp);
}

static int _map_rbtree_walk(RB_TREE_NODE_S *node, void *ud)
{
    USER_HANDLE_S *pstUserHandle = ud;
    PF_MAP_WALK_FUNC pfWalkFunc = pstUserHandle->ahUserHandle[0];
    VOID *pUserHandleTmp = pstUserHandle->ahUserHandle[1];
    MAP_RBTREE_NODE_S *pstNode = (void*)node;

    return pfWalkFunc(&pstNode->stEle, pUserHandleTmp);
}

static void _map_rbtree_free(RB_TREE_NODE_S *pNode, void *ud)
{
    USER_HANDLE_S *pstUserHandle = ud;
    PF_MAP_FREE_FUNC func = pstUserHandle->ahUserHandle[0];
    VOID *pUserHandleTmp = pstUserHandle->ahUserHandle[1];
    MAP_CTRL_S *map = pstUserHandle->ahUserHandle[2];
    MAP_RBTREE_NODE_S *pstNode = (void*)pNode;

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

static void map_rbtree_reset(MAP_HANDLE map, PF_MAP_FREE_FUNC free_func, void *ud)
{
    map_rbtree_del_all(map, free_func, ud);
}

static void map_rbtree_destroy(MAP_HANDLE map, PF_MAP_FREE_FUNC free_func, void *ud)
{
    _MAP_RBTREE_S *rbtree_map = map->impl_map;

    map_rbtree_del_all(map, free_func, ud);
    MemCap_Free(map->memcap, rbtree_map);
    MemCap_Free(map->memcap, map);
}

static inline int _map_rbtree_add(MAP_HANDLE map, VOID *pKey, UINT uiKeyLen,
        void *pData, MAP_RBTREE_NODE_S *pstNode, UINT flag)
{
    _MAP_RBTREE_S *rbtree_map = map->impl_map;

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

    if (0 != RBTree_Insert(&rbtree_map->root, &pstNode->rb_node, &pstNode->stEle, _map_rbtree_cmp)) {
        if (pstNode->stEle.dup_key) {
            MemCap_Free(map->memcap, pstNode->stEle.pKey);
        }
        RETURN(BS_ALREADY_EXIST);
    }

    rbtree_map->count ++;

    return BS_OK;
}

static int map_rbtree_add_node(MAP_HANDLE map, LDATA_S *key, void *pData, void *node, UINT flag)
{
    _MAP_RBTREE_S *rbtree_map = map->impl_map;
    MAP_RBTREE_NODE_S *pstNode = node;

    BS_DBGASSERT(0 == (flag & MAP_FLAG_PERMIT_DUPLICATE));

    if ((map->uiCapacity != 0) && (map->uiCapacity <= rbtree_map->count)) {
        RETURN(BS_NO_RESOURCE);
    }

    pstNode->stEle.link_alloced = 0;

    return _map_rbtree_add(map, key->data, key->len, pData, pstNode, flag);
}

static BS_STATUS map_rbtree_add(MAP_HANDLE map, VOID *pKey, UINT uiKeyLen, VOID *pData, UINT flag)
{
    _MAP_RBTREE_S *rbtree_map = map->impl_map;
    MAP_RBTREE_NODE_S *pstNode;

    BS_DBGASSERT(0 == (flag & MAP_FLAG_PERMIT_DUPLICATE));

    if ((map->uiCapacity != 0) && (map->uiCapacity <= rbtree_map->count)) {
        RETURN(BS_NO_RESOURCE);
    }

    pstNode = MemCap_ZMalloc(map->memcap, sizeof(MAP_RBTREE_NODE_S));
    if (NULL == pstNode) {
        RETURN(BS_NO_MEMORY);
    }
    pstNode->stEle.link_alloced = 1;

    int ret = _map_rbtree_add(map, pKey, uiKeyLen, pData, pstNode, flag);
    if (ret < 0) {
        MemCap_Free(map->memcap, pstNode);
        return ret;
    }

    return 0;
}

static MAP_ELE_S * map_rbtree_get_ele(MAP_HANDLE map, void *key, UINT key_len)
{
    MAP_RBTREE_NODE_S *pstFind;

    pstFind = _map_rbtree_find(map, key, key_len);
    if (NULL == pstFind) {
        return NULL;
    }

    return &pstFind->stEle;
}

static void * map_rbtree_get(IN MAP_HANDLE map, IN VOID *pKey, IN UINT uiKeyLen)
{
    MAP_RBTREE_NODE_S *pstFind;

    pstFind = _map_rbtree_find(map, pKey, uiKeyLen);
    if (NULL == pstFind) {
        return NULL;
    }

    return pstFind->stEle.pData;
}

static void * _map_rbtree_del_node(IN MAP_HANDLE map, IN MAP_RBTREE_NODE_S *pstNode)
{
    _MAP_RBTREE_S *rbtree_map = map->impl_map;
    void *pData = pstNode->stEle.pData;

    if (pstNode->stEle.dup_key) {
        MemCap_Free(map->memcap, pstNode->stEle.pKey);
    }

    if (pstNode->stEle.link_alloced) {
        MemCap_Free(map->memcap, pstNode);
    }

    rbtree_map->count --;

    return pData;
}

static void * map_rbtree_del_by_ele(IN MAP_HANDLE map, IN MAP_ELE_S *ele)
{
    MAP_RBTREE_NODE_S *node = container_of(ele, MAP_RBTREE_NODE_S, stEle);
    return _map_rbtree_del_node(map, node);
}


static void * map_rbtree_del(IN MAP_HANDLE map, IN VOID *pKey, IN UINT uiKeyLen)
{
    _MAP_RBTREE_S *rbtree_map = map->impl_map;
    MAP_RBTREE_NODE_S *pstNode;
    MAP_ELE_S ele;

    ele.pKey = pKey;
    ele.uiKeyLen = uiKeyLen;

    pstNode = (void*)RBTree_Del(&rbtree_map->root, &ele, _map_rbtree_cmp);
    if (! pstNode) {
        return NULL;
    }

    return _map_rbtree_del_node(map, pstNode);
}


static void * map_rbtree_del_node(MAP_HANDLE map, void *node)
{
    MAP_RBTREE_NODE_S *pstNode = node;
    return _map_rbtree_del_node(map, pstNode);
}

static void map_rbtree_del_all(MAP_HANDLE map, PF_MAP_FREE_FUNC func, void * pUserData)
{
    _MAP_RBTREE_S *rbtree_map = map->impl_map;
    USER_HANDLE_S stUserHandle;

    stUserHandle.ahUserHandle[0] = func;
    stUserHandle.ahUserHandle[1] = pUserData;
    stUserHandle.ahUserHandle[2] = map;

    RBTree_Finit(&rbtree_map->root, _map_rbtree_free, &stUserHandle);

    rbtree_map->count = 0;
}

static UINT map_rbtree_count(MAP_HANDLE map)
{
    _MAP_RBTREE_S *rbtree_map = map->impl_map;
    return rbtree_map->count;
}

static void map_rbtree_walk(IN MAP_HANDLE map, IN PF_MAP_WALK_FUNC pfWalkFunc, IN VOID *pUserData)
{
    _MAP_RBTREE_S *rbtree_map = map->impl_map;
    USER_HANDLE_S stUserHandle;

    stUserHandle.ahUserHandle[0] = pfWalkFunc;
    stUserHandle.ahUserHandle[1] = pUserData;

    RBTree_InOrderWalk(&rbtree_map->root, _map_rbtree_walk, &stUserHandle);
}


static MAP_ELE_S * map_rbtree_getnext(MAP_HANDLE map, MAP_ELE_S *pstCurrent)
{
    _MAP_RBTREE_S *rbtree_map = map->impl_map;
    MAP_RBTREE_NODE_S *node;

    if (NULL == pstCurrent) {
        node = (void*)RBTree_Min(&rbtree_map->root);
    } else {
        node = (void*)RBTree_SuccessorGet(&rbtree_map->root, pstCurrent, _map_rbtree_cmp);
    }

    if (! node) {
        return NULL;
    }

    return &node->stEle;
}

MAP_HANDLE MAP_RBTreeCreate(void *memcap)
{
    MAP_CTRL_S *ctrl;
    _MAP_RBTREE_S *rbtree_map;

    ctrl = MemCap_ZMalloc(memcap, sizeof(MAP_CTRL_S));
    if (! ctrl) {
        return NULL;
    }
    ctrl->memcap = memcap;
    ctrl->funcs = &g_map_rbtree_funcs;

    rbtree_map = MemCap_ZMalloc(memcap, sizeof(_MAP_RBTREE_S));
    if (NULL == rbtree_map) {
        MemCap_Free(memcap, ctrl);
        return NULL;
    }
    ctrl->impl_map = rbtree_map;

    return ctrl;
}


