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

static int _hash_cmp_node(PF_HASH_CMP_FUNC pfCmpFunc, HASH_NODE_S *node1, HASH_NODE_S *node2)
{
    if (! node1) {
        return -1;
    }

    if (! node2) {
        return 1;
    }

    return pfCmpFunc(node1, node2);
}

void HASH_Init(OUT HASH_S *hash, DLL_HEAD_S *buckets, U32 bucket_num, PF_HASH_INDEX_FUNC pfFunc)
{
    BS_DBGASSERT(NUM_IS2N(bucket_num));

    hash->mask = bucket_num - 1;
    hash->pfHashIndexFunc = pfFunc;
    hash->pstBuckets = buckets;

    for (int i=0; i<bucket_num; i++) {
        DLL_INIT(&buckets[i]);
    }
}

void HASH_AddWithFactor(IN HASH_S * hHashId, IN VOID *pstNode , UINT hash_factor)
{
    HASH_S *pstHashCtrl = (HASH_S*)hHashId;
    HASH_NODE_S *node = pstNode;
    UINT ulHashIndex = hash_factor & pstHashCtrl->mask;

    node->hash_factor = hash_factor;

    DLL_ADD(&pstHashCtrl->pstBuckets[ulHashIndex], pstNode);

    pstHashCtrl->uiNodeCount ++;
}

VOID HASH_Add(IN HASH_S * hHashId, IN VOID *pstNode )
{
    HASH_S *pstHashCtrl = (HASH_S*)hHashId;

    BS_DBGASSERT(0 != hHashId);

    HASH_AddWithFactor(hHashId, pstNode, pstHashCtrl->pfHashIndexFunc(pstNode));
}

VOID HASH_Del(IN HASH_S * hHashId, IN VOID *pstNode)
{
    UINT ulHashIndex;
    HASH_S *pstHashCtrl;

    BS_DBGASSERT(0 != hHashId);

    pstHashCtrl = (HASH_S*)hHashId;

    ulHashIndex = pstHashCtrl->pfHashIndexFunc(pstNode);
    ulHashIndex &= pstHashCtrl->mask;

    DLL_DEL(&pstHashCtrl->pstBuckets[ulHashIndex], pstNode);

    pstHashCtrl->uiNodeCount --;
}

VOID HASH_DelAll(IN HASH_S * hHashId, IN PF_HASH_FREE_FUNC pfFreeFunc, IN VOID *pUserHandle)
{
    HASH_S *pstHashCtrl;
    HASH_NODE_S *pstNodeFind, *pstNodeTmp;
    UINT i;

    BS_DBGASSERT(0 != hHashId);

    pstHashCtrl = (HASH_S*)hHashId;

    if (pstHashCtrl->uiNodeCount == 0)
    {
        return;
    }

    for (i=0; i<=pstHashCtrl->mask; i++)
    {
        if (NULL != pfFreeFunc)
        {
            DLL_SAFE_SCAN(&pstHashCtrl->pstBuckets[i], pstNodeFind, pstNodeTmp)
            {
                DLL_DEL(&pstHashCtrl->pstBuckets[i], pstNodeFind);
                pfFreeFunc(hHashId, pstNodeFind, pUserHandle);
            }
        }
    }

    pstHashCtrl->uiNodeCount = 0;
}

VOID * HASH_FindWithFactor(IN HASH_S * hHashId, UINT hash_factor, IN PF_HASH_CMP_FUNC pfCmpFunc, IN VOID *pstNodeToFind)
{
    HASH_S *pstHashCtrl = (HASH_S*)hHashId;
    UINT ulHashIndex = hash_factor & pstHashCtrl->mask;
    HASH_NODE_S *pstNodeFind;

    DLL_SCAN(&pstHashCtrl->pstBuckets[ulHashIndex], pstNodeFind) {
        if ((hash_factor == pstNodeFind->hash_factor)
                && (pfCmpFunc(pstNodeFind, pstNodeToFind) == 0)) {
            return pstNodeFind;
        }
    }

    return NULL;
}

VOID * HASH_Find(IN HASH_S * hHashId, IN PF_HASH_CMP_FUNC pfCmpFunc, IN VOID *pstNodeToFind)
{
    UINT hash_factor;
    HASH_S *pstHashCtrl;

    BS_DBGASSERT(0 != hHashId);

    pstHashCtrl = (HASH_S*)hHashId;

    hash_factor = pstHashCtrl->pfHashIndexFunc(pstNodeToFind);

    return HASH_FindWithFactor(hHashId, hash_factor, pfCmpFunc, pstNodeToFind);
}

UINT HASH_Count(IN HASH_S * hHashId)
{
    HASH_S *pstHashCtrl;

    BS_DBGASSERT(0 != hHashId);

    pstHashCtrl = (HASH_S*)hHashId;

    return pstHashCtrl->uiNodeCount;
}

VOID HASH_Walk(IN HASH_S * hHashId, IN PF_HASH_WALK_FUNC pfWalkFunc, IN VOID * pUserHandle)
{
    HASH_S *pstHashCtrl;
    HASH_NODE_S *pstNodeFind, *pstNodeTmp;
    UINT i;

    BS_DBGASSERT(0 != hHashId);

    pstHashCtrl = (HASH_S*)hHashId;

    for (i=0; i<=pstHashCtrl->mask; i++) {
        DLL_SAFE_SCAN(&pstHashCtrl->pstBuckets[i], pstNodeFind, pstNodeTmp) {
            if (pfWalkFunc(hHashId, pstNodeFind, pUserHandle) < 0) {
                return;
            }
        }
    }

    return;
}


HASH_NODE_S * HASH_GetNext(HASH_S * hHash, HASH_NODE_S *curr_node )
{
    HASH_S *pstHashCtrl;
    UINT i;
    HASH_NODE_S *node = NULL;
    UINT hash_factor;
    UINT index = 0;

    BS_DBGASSERT(0 != hHash);

    pstHashCtrl = (HASH_S*)hHash;

    
    if (curr_node) {
        hash_factor = pstHashCtrl->pfHashIndexFunc(curr_node);
        index = hash_factor & pstHashCtrl->mask;
        node = DLL_NEXT(&pstHashCtrl->pstBuckets[index], curr_node);
        if (node) {
            return node;
        }
        index ++;
    }

    for (i=index; i<=pstHashCtrl->mask; i++) {
        node = DLL_FIRST(&pstHashCtrl->pstBuckets[i]);
        if (node) {
            break;
        }
    }

    return node;
}


HASH_NODE_S * HASH_GetNextDict(HASH_S * hHash, PF_HASH_CMP_FUNC pfCmpFunc, HASH_NODE_S *curr_node )
{
    HASH_S *pstHashCtrl;
    HASH_NODE_S *pstNodeFind;
    HASH_NODE_S *pstNext = NULL;
    UINT i;

    BS_DBGASSERT(0 != hHash);

    pstHashCtrl = (HASH_S*)hHash;

    for (i=0; i<=pstHashCtrl->mask; i++) {
        DLL_SCAN(&pstHashCtrl->pstBuckets[i], pstNodeFind) {
            if (_hash_cmp_node(pfCmpFunc, pstNodeFind, curr_node) > 0) {
                if (! pstNext) {
                    pstNext = pstNodeFind;
                } else if (_hash_cmp_node(pfCmpFunc, pstNodeFind, pstNext) < 0) {
                    pstNext = pstNodeFind;
                }
            }
        }
    }

    return pstNext;
}


