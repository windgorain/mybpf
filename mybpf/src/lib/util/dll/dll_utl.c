/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2008-6-6
* Description: 
* History:     
******************************************************************************/
#include "bs.h"


VOID DLL_Sort(IN DLL_HEAD_S *pstDllHead, IN PF_DLL_CMP_FUNC pfFunc, IN HANDLE hUserHandle)
{
    DLL_NODE_S *pstNode1, *pstNode2;
    DLL_HEAD_S stHead;
    BOOL_T bIsInserted;

    DLL_INIT(&stHead);
    DLL_CAT(&stHead, pstDllHead);

    while (NULL != (pstNode1 = (DLL_NODE_S*)DLL_Get(&stHead)))
    {
        bIsInserted = FALSE;
        
        DLL_SCAN(pstDllHead, pstNode2)
        {
            if (pfFunc(pstNode1, pstNode2, hUserHandle) < 0)
            {
                DLL_INSERT_BEFORE(pstDllHead, pstNode1, pstNode2);
                bIsInserted = TRUE;
                break;
            }
        }

        if (bIsInserted == FALSE)
        {        
            DLL_ADD(pstDllHead, pstNode1);
        }
    }
}


VOID DLL_SortAdd
(
    IN DLL_HEAD_S *pstDllHead,
    IN DLL_NODE_S *pstNewNode,
    IN PF_DLL_CMP_FUNC pfFunc,
    IN HANDLE hUserHandle
)
{
    DLL_NODE_S *pstNodeTmp;
    
    DLL_SCAN(pstDllHead, pstNodeTmp) {
        if (pfFunc(pstNewNode, pstNodeTmp, hUserHandle) <= 0) {
            DLL_INSERT_BEFORE(pstDllHead, pstNewNode, pstNodeTmp);
            return;
        }
    }

    DLL_ADD(pstDllHead, pstNewNode);

    return;
}


int DLL_UniqueSortAdd(DLL_HEAD_S *head, DLL_NODE_S *node, PF_DLL_CMP_FUNC cmp_func, void *user_data)
{
    DLL_NODE_S *pstNodeTmp;
    int cmp_ret;
    
    DLL_SCAN(head, pstNodeTmp)
    {
        cmp_ret = cmp_func(node, pstNodeTmp, user_data);
        if (cmp_ret == 0) {
            return -1;
        }

        if (cmp_ret < 0) {
            DLL_INSERT_BEFORE(head, node, pstNodeTmp);
            return 0;
        }
    }

    DLL_ADD(head, node);

    return 0;
}

VOID DLL_Cat (IN DLL_HEAD_S *pstDllHeadDst, IN DLL_HEAD_S *pstDllHeadSrc)
{
    DLL_NODE_S *pstDllNodeFirst, *pstDllNodeLast;
    DLL_NODE_S *pstNode;
    
    if (0 == DLL_COUNT (pstDllHeadSrc)) {
        return;
    }

    DLL_SCAN(pstDllHeadSrc, pstNode) {
        pstNode->pstHead = pstDllHeadDst;
    }

    pstDllNodeFirst = (DLL_NODE_S*) DLL_FIRST (pstDllHeadSrc);
    pstDllNodeLast  = (DLL_NODE_S*) DLL_LAST (pstDllHeadSrc);

    pstDllHeadDst->prev->next = pstDllNodeFirst;
    pstDllNodeFirst->prev = pstDllHeadDst->prev;
    pstDllHeadDst->prev = pstDllNodeLast;
    pstDllNodeLast->next = (DLL_NODE_S*)pstDllHeadDst;

    pstDllHeadDst->ulCount += DLL_COUNT(pstDllHeadSrc);
    DLL_INIT (pstDllHeadSrc);

    return;
}

VOID * DLL_Find(IN DLL_HEAD_S *pstDllHead, IN PF_DLL_CMP_FUNC pfCmpFunc, IN VOID *pstNodeToFind, IN HANDLE hUserHandle)
{
    DLL_NODE_S *pstNode;
    
    DLL_SCAN(pstDllHead, pstNode) {
        if (pfCmpFunc(pstNode, pstNodeToFind, hUserHandle) == 0) {
            return pstNode;
        }
    }

    return NULL;
}

