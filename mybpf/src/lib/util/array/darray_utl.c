/******************************************************************************
* Copyright (C) LiXingang
* Author:      LiXingang  Version: 1.0  Date: 2012-11-16
* Description: 动态增长的数组, 有效值不为0，如果将值设置为0则认为是空闲位置
* History:     
******************************************************************************/

#include "bs.h"

#include "utl/mem_inline.h"
#include "utl/darray_utl.h"

#define _DARRAY_DFT_STEP_SIZE 128

typedef struct
{
    UINT uiSize;  
    USHORT step_num;  
    USHORT reserved;
    HANDLE * phArray;
}_DARRAY_CTRL_S;


STATIC BS_STATUS darray_Expand(IN _DARRAY_CTRL_S *pstCtrl, IN UINT uiNewSize)
{
    VOID *pMem;
    UINT uiOldSize;

    if (pstCtrl->uiSize >= uiNewSize)
    {
        return BS_OK;
    }

    uiOldSize = pstCtrl->uiSize;

    pMem = MEM_MallocAndCopy(pstCtrl->phArray, pstCtrl->uiSize * sizeof(HANDLE), uiNewSize * sizeof(HANDLE));
    if (NULL == pMem)
    {
        return BS_FULL;
    }

    Mem_Zero((UCHAR*)pMem + uiOldSize * sizeof(HANDLE), (uiNewSize - uiOldSize) * sizeof(HANDLE));

    if (NULL != pstCtrl->phArray)
    {
        MEM_Free(pstCtrl->phArray);
    }

    pstCtrl->phArray = pMem;
    pstCtrl->uiSize = uiNewSize;

    return BS_OK;
}

static UINT darray_AddToSpace(_DARRAY_CTRL_S *pstCtrl, IN HANDLE hData)
{
    UINT i;

    for (i=0; i<pstCtrl->uiSize; i++)
    {
        if (pstCtrl->phArray[i] == NULL)
        {
            pstCtrl->phArray[i] = hData;
            return i;
        }
    }

    return DARRAY_INVALID_INDEX;
}


static UINT darray_ExpandAndAdd(IN _DARRAY_CTRL_S *pstCtrl, IN HANDLE hData)
{
    UINT uiIndex;

    BS_DBGASSERT(NULL != pstCtrl);

    uiIndex = pstCtrl->uiSize;

    if (BS_OK != darray_Expand(pstCtrl, pstCtrl->uiSize + pstCtrl->step_num))
    {
        return DARRAY_INVALID_INDEX;
    }

    pstCtrl->phArray[uiIndex] = hData;

    return uiIndex;
}

DARRAY_HANDLE DARRAY_Create(UINT init_num, USHORT step_num)
{
    _DARRAY_CTRL_S *pstCtrl;

    pstCtrl = MEM_ZMalloc(sizeof(_DARRAY_CTRL_S));
    if (NULL == pstCtrl)
    {
        return NULL;
    }

    if (init_num != 0)
    {
        pstCtrl->phArray = MEM_ZMalloc(sizeof(HANDLE) * init_num);
        if (NULL == pstCtrl->phArray)
        {
            MEM_Free(pstCtrl);
            return NULL;
        }
        pstCtrl->uiSize = init_num;
    }

    pstCtrl->step_num = step_num;
    if (step_num == 0) {
        pstCtrl->step_num = _DARRAY_DFT_STEP_SIZE;
    }

    return pstCtrl;
}

VOID DARRAY_Destory(IN DARRAY_HANDLE hDArray)
{
    _DARRAY_CTRL_S *pstCtrl = hDArray;

    if (NULL != pstCtrl)
    {
        if (NULL != pstCtrl->phArray)
        {
            MEM_Free(pstCtrl->phArray);
        }

        MEM_Free(pstCtrl);
    }
}


UINT DARRAY_Add(IN DARRAY_HANDLE hDArray, IN HANDLE hData)
{
    UINT uiIndex;
    _DARRAY_CTRL_S *pstCtrl = hDArray;

    BS_DBGASSERT(NULL != pstCtrl);
    BS_DBGASSERT(NULL != hData);

    uiIndex = darray_AddToSpace(pstCtrl, hData);
    if (uiIndex == DARRAY_INVALID_INDEX)
    {
        uiIndex = darray_ExpandAndAdd(hDArray, hData);
    }

    return uiIndex;
}

BS_STATUS DARRAY_Set(IN DARRAY_HANDLE hDArray, IN UINT uiIndex, IN HANDLE hData)
{
    _DARRAY_CTRL_S *pstCtrl = hDArray;

    if (uiIndex >= pstCtrl->uiSize)
    {
        if (BS_OK != darray_Expand(pstCtrl, uiIndex + 1))
        {
            return BS_NO_MEMORY;
        }
    }

    pstCtrl->phArray[uiIndex] = hData;

    return BS_OK;
}

HANDLE DARRAY_Get(IN DARRAY_HANDLE hDArray, IN UINT uiIndex)
{
    _DARRAY_CTRL_S *pstCtrl = hDArray;

    if (unlikely((uiIndex >= pstCtrl->uiSize)))
    {
        return NULL;
    }

    return pstCtrl->phArray[uiIndex];
}

UINT DARRAY_FindIntData(IN DARRAY_HANDLE hDArray, IN HANDLE hData)
{
    _DARRAY_CTRL_S *pstCtrl = hDArray;
    UINT uiIndex = 0; 

    for(uiIndex = 0; uiIndex <= pstCtrl->uiSize; uiIndex++) {
        if(hData == pstCtrl->phArray[uiIndex]) {
            return (uiIndex);
        }
    }
    return DARRAY_INVALID_INDEX;
}


HANDLE DARRAY_Clear(IN DARRAY_HANDLE hDArray, IN UINT uiIndex)
{
    _DARRAY_CTRL_S *pstCtrl = hDArray;
    HANDLE hData;

    if (uiIndex >= pstCtrl->uiSize)
    {
        return NULL;
    }

    hData = pstCtrl->phArray[uiIndex];
    pstCtrl->phArray[uiIndex] = NULL;

    return hData;
}


UINT DARRAY_GetSize(IN DARRAY_HANDLE hDArray)
{
    _DARRAY_CTRL_S *pstCtrl = hDArray;

    return pstCtrl->uiSize;
}


