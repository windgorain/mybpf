/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2008-9-20
* Description: 
* History:     
******************************************************************************/
#include "bs.h"
#include "utl/hash_utl.h"
#include "utl/num_utl.h"
#include "utl/mem_cap.h"

static int _hash_cmp_node(PF_CMP_EXT_FUNC pfCmpFunc, HASH_NODE_S *node1, HASH_NODE_S *node2, void *ud)
{
    if (! node1) {
        return -1;
    }

    if (! node2) {
        return 1;
    }

    return pfCmpFunc(node1, node2, ud);
}

void HASH_Init(OUT HASH_S *hash, DL_HEAD_S *buckets, U32 bucket_num, PF_HASH_BY_NODE hash_by_node)
{
    BS_DBGASSERT(NUM_IS2N(bucket_num));

    hash->mask = bucket_num - 1;
    hash->hash_by_node = hash_by_node;
    hash->pstBuckets = buckets;

    for (int i=0; i<bucket_num; i++) {
        DL_Init(&buckets[i]);
    }
}

void HASH_AddWithFactor(HASH_S * hHashId, void *pstNode , UINT hash_factor)
{
    HASH_S *pstHashCtrl = (HASH_S*)hHashId;
    HASH_NODE_S *node = pstNode;
    UINT ulHashIndex = hash_factor & pstHashCtrl->mask;

    node->hash_factor = hash_factor;

    DL_AddHead(&pstHashCtrl->pstBuckets[ulHashIndex], pstNode);

    pstHashCtrl->uiNodeCount ++;
}

void HASH_Add(IN HASH_S * hHashId, IN VOID *pstNode )
{
    HASH_S *pstHashCtrl = (HASH_S*)hHashId;
    BS_DBGASSERT(0 != hHashId);
    HASH_AddWithFactor(hHashId, pstNode, pstHashCtrl->hash_by_node(pstNode, hHashId));
}

VOID HASH_Del(HASH_S * hHashId, void *pstNode)
{
    HASH_S *pstHashCtrl = hHashId;
    DL_Del(pstNode);
    pstHashCtrl->uiNodeCount --;
}

VOID HASH_DelAll(IN HASH_S * hHashId, IN PF_HASH_FREE_FUNC pfFreeFunc, IN VOID *pUserHandle)
{
    HASH_S *pstHashCtrl;
    DL_NODE_S *pstNodeFind, *pstNodeTmp;
    UINT i;

    BS_DBGASSERT(0 != hHashId);

    pstHashCtrl = (HASH_S*)hHashId;

    if (pstHashCtrl->uiNodeCount == 0) {
        return;
    }

    for (i=0; i<=pstHashCtrl->mask; i++) {
        if (NULL != pfFreeFunc) {
            DL_FOREACH_SAFE(&pstHashCtrl->pstBuckets[i], pstNodeFind, pstNodeTmp) {
                DL_Del(pstNodeFind);
                pfFreeFunc(hHashId, pstNodeFind, pUserHandle);
            }
        }
    }

    pstHashCtrl->uiNodeCount = 0;
}


void * HASH_FindWithFactor(HASH_S *hash, U32 hash_factor, PF_CMP_EXT_FUNC cmp_func, void *key, void *ud)
{
    UINT ulHashIndex = hash_factor & hash->mask;
    HASH_NODE_S *node;

    DL_FOREACH_ENTRY(&hash->pstBuckets[ulHashIndex], node, stNode) {
        if ((hash_factor == node->hash_factor) && (cmp_func(key, node, ud) == 0)) {
            return node;
        }
    }

    return NULL;
}


void * HASH_FindKey(HASH_S *hash, PF_CMP_EXT_FUNC cmp_func, PF_HASH_BY_KEY hash_by_key, void *key, void *ud)
{
    BS_DBGASSERT(NULL != hash);
    U32 hash_factor = hash_by_key(key, ud);
    return HASH_FindWithFactor(hash, hash_factor, cmp_func, key, ud);
}


void * HASH_FindNode(HASH_S *hash, PF_CMP_EXT_FUNC cmp_func, void *node, void *ud)
{
    BS_DBGASSERT(NULL != hash);
    U32 hash_factor = hash->hash_by_node(node, hash);
    return HASH_FindWithFactor(hash, hash_factor, cmp_func, node, ud);
}

UINT HASH_Count(IN HASH_S * hHashId)
{
    BS_DBGASSERT(0 != hHashId);
    HASH_S *pstHashCtrl = (HASH_S*)hHashId;
    return pstHashCtrl->uiNodeCount;
}

void HASH_Walk(HASH_S * hHashId, PF_HASH_WALK_FUNC pfWalkFunc, void *ud)
{
    HASH_S *pstHashCtrl;
    DL_NODE_S *pstNodeFind, *pstNodeTmp;
    UINT i;

    BS_DBGASSERT(0 != hHashId);

    pstHashCtrl = (HASH_S*)hHashId;

    for (i=0; i<=pstHashCtrl->mask; i++) {
        DL_FOREACH_SAFE(&pstHashCtrl->pstBuckets[i], pstNodeFind, pstNodeTmp) {
            if (pfWalkFunc(hHashId, pstNodeFind, ud) < 0) {
                return;
            }
        }
    }

    return;
}


HASH_NODE_S * HASH_GetNext(HASH_S *hash, HASH_NODE_S *curr_node )
{
    HASH_S *pstHashCtrl;
    UINT i;
    HASH_NODE_S *node = NULL;
    UINT hash_factor;
    UINT index = 0;

    BS_DBGASSERT(0 != hash);

    pstHashCtrl = (HASH_S*)hash;

    
    if (curr_node) {
        hash_factor = pstHashCtrl->hash_by_node(curr_node, hash);
        index = hash_factor & pstHashCtrl->mask;
        node = (void*)DL_Next(&curr_node->stNode);
        if (node) {
            return node;
        }
        index ++;
    }

    for (i=index; i<=pstHashCtrl->mask; i++) {
        node = (void*)DL_First(&pstHashCtrl->pstBuckets[i]);
        if (node) {
            break;
        }
    }

    return node;
}


HASH_NODE_S * HASH_GetNextDict(HASH_S * hHash, PF_CMP_EXT_FUNC pfCmpFunc, HASH_NODE_S *curr_node, void *ud)
{
    HASH_S *pstHashCtrl;
    HASH_NODE_S *node;
    HASH_NODE_S *pstNext = NULL;
    UINT i;

    BS_DBGASSERT(0 != hHash);

    pstHashCtrl = (HASH_S*)hHash;

    for (i=0; i<=pstHashCtrl->mask; i++) {
        DL_FOREACH_ENTRY(&pstHashCtrl->pstBuckets[i], node, stNode) {
            if (_hash_cmp_node(pfCmpFunc, curr_node, node, ud) < 0) {
                if (! pstNext) {
                    pstNext = node;
                } else if (_hash_cmp_node(pfCmpFunc, pstNext, node, ud) > 0) {
                    pstNext = node;
                }
            }
        }
    }

    return pstNext;
}


