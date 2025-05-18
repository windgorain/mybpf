/******************************************************************************
* Copyright (C), 2000-2006,  LiXingang
* Author:      LiXingang  Version: 1.0  Date: 2016-8-31
* Description: Number List: 支持以字符串形式描述的数字列表
*              格式例如: 6,2-5,8-7
* History:     
******************************************************************************/
#include "bs.h"

#include "utl/txt_utl.h"
#include "utl/lstr_utl.h"
#include "utl/num_list.h"


VOID NumList_Init(IN NUM_LIST_S *pstList)
{
    DLL_INIT(&pstList->stList);
}

VOID NumList_Finit(IN NUM_LIST_S *pstList)
{
    NUM_LIST_NODE_S *pstNode, *pstNodeTmp;

    DLL_SAFE_SCAN(&pstList->stList, pstNode, pstNodeTmp)
    {
        DLL_DEL(&pstList->stList, pstNode);
        MEM_Free(pstNode);
    }
}


int NumList_ParseElement(LSTR_S *lstr, OUT UINT *min, OUT UINT *max)
{
    LSTR_S stBegin;
    LSTR_S stEnd;

    LSTR_MSplit(lstr, "-/", &stBegin, &stEnd);

    LSTR_Strim(&stBegin, " \t\r\n", &stBegin);
    LSTR_Strim(&stEnd, " \t\r\n", &stEnd);
    if (stBegin.uiLen == 0) {
        RETURN(BS_ERR);
    }

    if (BS_OK != LSTR_Atoui(&stBegin, min)) {
        RETURN(BS_ERR);
    }

    if (stEnd.uiLen == 0) {
        *max = *min;
        return 0;
    }

    if (BS_OK != LSTR_Atoui(&stEnd, max)) {
        RETURN(BS_ERR);
    }

    if (*min > *max) {
        RETURN(BS_ERR);
    }

    return 0;
}

static int _numlist_ParseElement(IN NUM_LIST_S *pstList, IN LSTR_S *pstEle)
{
    UINT uiNumBegin;
    UINT uiNumEnd;

    int ret = NumList_ParseElement(pstEle, &uiNumBegin, &uiNumEnd);
    if (ret < 0) {
        return ret;
    }

    return NumList_AddRange(pstList, uiNumBegin, uiNumEnd);
}

BS_STATUS NumList_AddRange(IN NUM_LIST_S *pstList, INT64 iBegin, INT64 iEnd)
{
    NUM_LIST_NODE_S *pstNode;

    pstNode = MEM_ZMalloc(sizeof(NUM_LIST_NODE_S));
    if (NULL == pstNode) {
        RETURN(BS_NO_MEMORY);
    }

    pstNode->iNumBegin = iBegin;
    pstNode->iNumEnd = iEnd;

    DLL_ADD(&pstList->stList, pstNode);

    return BS_OK;
}


BS_STATUS NumList_DelRange(IN NUM_LIST_S *pstList, INT64 iBegin, INT64 iEnd)
{
    NUM_LIST_NODE_S *pstNode = NumList_FindRange(pstList, iBegin, iEnd);
    if (pstNode) {
        DLL_DEL(&pstList->stList, pstNode);
        MEM_Free(pstNode);
    }

    return BS_OK;
}

void NumList_DelNode(IN NUM_LIST_S *pstList, IN NUM_LIST_NODE_S *node)
{
    DLL_DEL(&pstList->stList, node);
    MEM_Free(node);
}


NUM_LIST_NODE_S * NumList_FindRange(IN NUM_LIST_S *pstList, INT64 begin, INT64 end)
{
    NUM_LIST_NODE_S *pstNode;

    DLL_SCAN(&pstList->stList, pstNode) {
        if ((pstNode->iNumBegin == begin) && (pstNode->iNumEnd == end)) {
            return pstNode;
        }
    }

    return NULL;
}

BS_STATUS NumList_ParseLstr(IN NUM_LIST_S *pstList, IN LSTR_S *pstNumListString)
{
    LSTR_S stElement;
    BS_STATUS eRet = BS_OK;
    
    LSTR_SCAN_ELEMENT_BEGIN(pstNumListString->pcData, pstNumListString->uiLen, ',', &stElement) {
        eRet |= _numlist_ParseElement(pstList, &stElement);
    }LSTR_SCAN_ELEMENT_END();

	return eRet;
}

BS_STATUS NumList_ParseStr(IN NUM_LIST_S *pstList, IN char *str)
{
    LSTR_S lstr;

    lstr.pcData = str;
    lstr.uiLen = strlen(str);

    return NumList_ParseLstr(pstList, &lstr);
}


VOID NumList_Cat(INOUT NUM_LIST_S *pstListDst, INOUT NUM_LIST_S *pstListSrc)
{
    DLL_Cat(&pstListDst->stList, &pstListSrc->stList);
}

static INT _numlist_Cmp(IN DLL_NODE_S *pstDllNode1, IN DLL_NODE_S *pstDllNode2, IN HANDLE hHandle)
{
    NUM_LIST_NODE_S *pstNode1 = (VOID*)pstDllNode1;
    NUM_LIST_NODE_S *pstNode2 = (VOID*)pstDllNode2;

    if (pstNode1->iNumBegin > pstNode2->iNumBegin)
    {
        return 1;
    }

    if (pstNode1->iNumBegin < pstNode2->iNumBegin)
    {
        return -1;
    }

    if (pstNode1->iNumEnd < pstNode2->iNumEnd)
    {
        return 1;
    }

    if (pstNode1->iNumEnd > pstNode2->iNumEnd)
    {
        return -1;
    }

    return 0;
}


VOID NumList_Sort(IN NUM_LIST_S *pstList)
{
    DLL_Sort(&pstList->stList, _numlist_Cmp, 0);
}


VOID NumList_Compress(IN NUM_LIST_S *pstList)
{
    NUM_LIST_NODE_S *pstNode, *pstNodeNext;
    
    
    NumList_Sort(pstList);

    
    pstNode = DLL_FIRST(&pstList->stList);

    if (NULL == pstNode)
    {
        return;
    }

    while(1)
    {
        pstNodeNext = DLL_NEXT(&pstList->stList, pstNode);
        if (NULL == pstNodeNext)
        {
            break;
        }

        if (pstNodeNext->iNumEnd <= pstNode->iNumEnd)
        {
            
            DLL_DEL(&pstList->stList, pstNodeNext);
            MEM_Free(pstNodeNext);
            continue;
        }

        if (pstNodeNext->iNumBegin <= pstNode->iNumBegin)
        {
            pstNodeNext->iNumBegin = pstNode->iNumEnd + 1;
        }

        pstNode = pstNodeNext;
    }

    return;
}


VOID NumList_Continue(IN NUM_LIST_S *pstList)
{
    NUM_LIST_NODE_S *pstNode, *pstNodeNext;

    
    NumList_Compress(pstList);

    
    pstNode = DLL_FIRST(&pstList->stList);

    if (NULL == pstNode)
    {
        return;
    }

    while(1)
    {
        pstNodeNext = DLL_NEXT(&pstList->stList, pstNode);
        if (NULL == pstNodeNext)
        {
            break;
        }

        if (pstNodeNext->iNumBegin == pstNode->iNumEnd + 1)
        {
            pstNode->iNumEnd = pstNodeNext->iNumEnd;
            DLL_DEL(&pstList->stList, pstNodeNext);
            MEM_Free(pstNodeNext);
            continue;
        }

        pstNode = pstNodeNext;
    }

    return;
}


BOOL_T NumList_IsNumInTheList(IN NUM_LIST_S *pstList, IN INT iNum)
{
    INT64 iStart, iEnd;

    NUM_LIST_SCAN_BEGIN(pstList, iStart, iEnd) {
        if ((iNum >= iStart) && (iNum <= iEnd)) {
            return TRUE;
        }
    }NUM_LIST_SCAN_END();

    return FALSE;
}


