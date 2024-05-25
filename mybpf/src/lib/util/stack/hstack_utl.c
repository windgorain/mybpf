/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2010-5-20
* Description:  HANDLE Stack. Stack中存放的是Handle
* History:     
******************************************************************************/
#include "bs.h"
#include "utl/mem_inline.h"
#include "utl/stack_utl.h"

#define _HSTACK_DFT_STEP_SIZE 128

typedef struct
{
    BOOL_T bIsDynamic;
    UINT uiStackSize;
    UINT uiNextIndex;    
    HANDLE *pHandleStack;
}_HSTACK_CTRL_S;

HANDLE HSTACK_Create(IN UINT uiStackSize)
{
    UINT uiStackSizeTmp = uiStackSize;
    _HSTACK_CTRL_S *pstCtrl;

    pstCtrl = MEM_ZMalloc(sizeof(_HSTACK_CTRL_S));
    if (NULL == pstCtrl)
    {
        return NULL;
    }

    if (uiStackSizeTmp == 0)
    {
        uiStackSizeTmp = _HSTACK_DFT_STEP_SIZE;
        pstCtrl->bIsDynamic = TRUE;
    }

    pstCtrl->pHandleStack = MEM_Malloc(sizeof(HANDLE) * uiStackSizeTmp);
    if (NULL == pstCtrl->pHandleStack)
    {
        MEM_Free(pstCtrl);
        return NULL;
    }

    pstCtrl->uiStackSize = uiStackSizeTmp;

    return pstCtrl;
}

void HSTACK_Reset(IN HANDLE hHandle)
{
    _HSTACK_CTRL_S *pstCtrl = hHandle;
    pstCtrl->uiNextIndex = 0;
}

VOID HSTACK_Destory(IN HANDLE hHandle)
{
    _HSTACK_CTRL_S *pstCtrl = hHandle;

    if (pstCtrl != NULL)
    {
        if (pstCtrl->pHandleStack != NULL)
        {
            MEM_Free(pstCtrl->pHandleStack);
        }

        MEM_Free(pstCtrl);
    }
}

UINT HSTACK_GetStackSize(IN HANDLE hHandle)
{
    _HSTACK_CTRL_S *pstCtrl = hHandle;
    return pstCtrl->uiStackSize;
}

BOOL_T HSTACK_IsDynamic(IN HANDLE hHandle)
{
    _HSTACK_CTRL_S *pstCtrl = hHandle;
    return pstCtrl->bIsDynamic;
}

UINT HSTACK_GetCount(IN HANDLE hHandle)
{
    _HSTACK_CTRL_S *pstCtrl = hHandle;
    return pstCtrl->uiNextIndex;
}

STATIC BS_STATUS hstack_Expand(IN _HSTACK_CTRL_S *pstCtrl, IN UINT uiNewSize)
{
    VOID *pMem;

    if (pstCtrl->uiStackSize >= uiNewSize)
    {
        return BS_OK;
    }

    if (pstCtrl->bIsDynamic == FALSE)
    {
        return BS_FULL;
    }

    pMem = MEM_MallocAndCopy(pstCtrl->pHandleStack, pstCtrl->uiStackSize * sizeof(HANDLE), uiNewSize * sizeof(HANDLE));
    if (NULL == pMem)
    {
        return BS_FULL;
    }

    MEM_Free(pstCtrl->pHandleStack);
    pstCtrl->pHandleStack = pMem;
    pstCtrl->uiStackSize = uiNewSize;

    return BS_OK;
}


BS_STATUS HSTACK_Push(IN HANDLE hHandle, IN HANDLE hUserHandle)
{
    _HSTACK_CTRL_S *pstCtrl = hHandle;
    
    if (pstCtrl->uiNextIndex >= pstCtrl->uiStackSize)    
    {
        if (BS_OK != hstack_Expand(pstCtrl, pstCtrl->uiStackSize + _HSTACK_DFT_STEP_SIZE))
        {
            return BS_FULL;
        }
    }

    pstCtrl->pHandleStack[pstCtrl->uiNextIndex] = hUserHandle;
    pstCtrl->uiNextIndex ++;

    return BS_OK;
}


BS_STATUS HSTACK_Pop(IN HANDLE hHandle, OUT HANDLE *phUserHandle)
{
    _HSTACK_CTRL_S *pstCtrl = hHandle;

    if (NULL != phUserHandle)
    {
        *phUserHandle = NULL;
    }

    if (pstCtrl->uiNextIndex == 0)
    {
        return BS_EMPTY;
    }

    pstCtrl->uiNextIndex --;

    if (NULL != phUserHandle)
    {
        *phUserHandle = pstCtrl->pHandleStack[pstCtrl->uiNextIndex];
    }

    return BS_OK;
}


HANDLE HSTACK_GetValueByIndex(IN HANDLE hHandle, IN UINT uiIndex)
{
    _HSTACK_CTRL_S *pstCtrl = hHandle;

    if (uiIndex >= pstCtrl->uiNextIndex)
    {
        BS_DBGASSERT(0);
        return NULL;
    }

    return pstCtrl->pHandleStack[uiIndex];
}


BS_STATUS HSTACK_SetValueByIndex(IN HANDLE hHandle, IN UINT uiIndex, IN HANDLE hValue)
{
    _HSTACK_CTRL_S *pstCtrl = hHandle;

    if (uiIndex >= pstCtrl->uiStackSize)    
    {
        return BS_OUT_OF_RANGE;
    }

    pstCtrl->pHandleStack[uiIndex] = hValue;

    return BS_OK;
}



BS_STATUS HSTACK_GetTop(IN HANDLE hHandle, OUT HANDLE *phUserHandle)
{
    UINT uiCurrentSize = HSTACK_GetCount(hHandle);
    
    if (uiCurrentSize == 0)
    {
        return BS_EMPTY;
    }

    *phUserHandle = HSTACK_GetValueByIndex(hHandle, uiCurrentSize - 1);
    return BS_OK;
}



