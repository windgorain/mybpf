/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2007-7-31
* Description: 环形数组
* History:     
******************************************************************************/

#define RETCODE_FILE_NUM RETCODE_FILE_NUM_RARRAY

#include "bs.h"


#define _RARRY_IS_FULL(pstCtrl)  ((pstCtrl)->ulCount == (pstCtrl)->ulRowsCount)

typedef struct
{
    UINT ulRowsCount;
    UINT ulColCount;
    UINT ulReadIndex;
    UINT ulWriteIndex;
    UINT ulCount;
    UCHAR *pucData;
}_RARRAY_CTRL_S;

HANDLE RArray_Create(IN UINT ulRowsCount, IN UINT ulColCount)
{
    _RARRAY_CTRL_S *pstRArryCtrl = NULL;
    
    if ((ulRowsCount == 0) || (ulColCount == 0))
    {
        BS_WARNNING(("Null ptr!"));
        return NULL;
    }

    pstRArryCtrl = MEM_Malloc(ulRowsCount*(ulColCount + sizeof(UINT)) + sizeof(_RARRAY_CTRL_S));
    if (NULL == pstRArryCtrl)
    {
        return NULL;
    }
    Mem_Zero(pstRArryCtrl, sizeof(_RARRAY_CTRL_S));

    pstRArryCtrl->pucData = (UCHAR*)(pstRArryCtrl + 1);
    pstRArryCtrl->ulColCount = ulColCount + sizeof(UINT); 
    pstRArryCtrl->ulRowsCount = ulRowsCount;
    pstRArryCtrl->ulReadIndex = 0;
    pstRArryCtrl->ulWriteIndex = 0;

    return pstRArryCtrl;    
}

void RArray_Reset(IN HANDLE hHandle)
{
    _RARRAY_CTRL_S *pstRArryCtrl = (_RARRAY_CTRL_S*)hHandle;
    if (pstRArryCtrl) {
        pstRArryCtrl->ulReadIndex = 0;
        pstRArryCtrl->ulWriteIndex = 0;
        pstRArryCtrl->ulCount = 0;
    }
}

BS_STATUS RArray_Delete(IN HANDLE hHandle)
{
    if (hHandle == 0)
    {
        BS_WARNNING(("Null ptr!"));
        RETURN(BS_NULL_PARA);
    }

    MEM_Free(hHandle);
    
    return BS_OK;
}

BS_STATUS RArray_WriteForce(IN HANDLE hHandle, IN UCHAR *pucData, IN UINT ulDataLen)
{
    _RARRAY_CTRL_S *pstRArryCtrl = (_RARRAY_CTRL_S*)hHandle;

    if ((hHandle == 0) || (pucData == NULL))
    {
        BS_WARNNING(("Null ptr!"));
        RETURN(BS_NULL_PARA);
    }

    if (ulDataLen > pstRArryCtrl->ulColCount - sizeof(UINT))
    {
        BS_WARNNING(("Too long data!"));
        RETURN(BS_TOO_LONG);
    }

    if (ulDataLen > 0)
    {
        MEM_Copy((pstRArryCtrl->pucData + pstRArryCtrl->ulWriteIndex * pstRArryCtrl->ulColCount + sizeof(UINT)),
                pucData, ulDataLen);
    }
    *((UINT*)(pstRArryCtrl->pucData + pstRArryCtrl->ulWriteIndex * pstRArryCtrl->ulColCount)) = ulDataLen;

    pstRArryCtrl->ulWriteIndex = (pstRArryCtrl->ulWriteIndex + 1) % (pstRArryCtrl->ulRowsCount);

    if (_RARRY_IS_FULL(pstRArryCtrl))
    {
        pstRArryCtrl->ulReadIndex = pstRArryCtrl->ulWriteIndex;
    }
    else
    {
        pstRArryCtrl->ulCount++;
    }

    return BS_OK;
}

BS_STATUS RArray_Write(IN HANDLE hHandle, IN UCHAR *pucData, IN UINT ulDataLen)
{
    _RARRAY_CTRL_S *pstRArryCtrl = (_RARRAY_CTRL_S*)hHandle;

    if ((hHandle == 0) || (pucData == NULL))
    {
        BS_WARNNING(("Null ptr!"));
        RETURN(BS_NULL_PARA);
    }

    if (ulDataLen > pstRArryCtrl->ulColCount - sizeof(UINT))
    {
        BS_WARNNING(("Too long data!"));
        RETURN(BS_TOO_LONG);
    }

    if (_RARRY_IS_FULL(pstRArryCtrl))
    {
        RETURN(BS_FULL);        
    }

    return RArray_WriteForce(hHandle, pucData, ulDataLen);
}


BS_STATUS RArray_ReadNoDel(IN HANDLE hHandle, OUT UCHAR **ppucData, OUT UINT *pulDataLen)
{
    _RARRAY_CTRL_S *pstRArryCtrl = (_RARRAY_CTRL_S*)hHandle;

    if ((hHandle == 0) || (ppucData == NULL))
    {
        BS_WARNNING(("Null ptr!"));
        RETURN(BS_NULL_PARA);
    }

    if (pstRArryCtrl->ulCount == 0)
    {
        
        RETURN(BS_EMPTY);
    }

    *ppucData = pstRArryCtrl->pucData + pstRArryCtrl->ulReadIndex * pstRArryCtrl->ulColCount + sizeof(UINT);

    if (pulDataLen)
    {
        *pulDataLen = *((UINT*)(pstRArryCtrl->pucData + pstRArryCtrl->ulReadIndex * pstRArryCtrl->ulColCount));
    }

    return BS_OK;    
}


BS_STATUS RArray_Read(IN HANDLE hHandle, OUT UCHAR **ppucData, OUT UINT *pulDataLen)
{
    _RARRAY_CTRL_S *pstRArryCtrl = (_RARRAY_CTRL_S*)hHandle;
    UINT ulRet;

    ulRet = RArray_ReadNoDel(hHandle, ppucData, pulDataLen);

    if (BS_OK == ulRet)
    {
        pstRArryCtrl->ulReadIndex = (pstRArryCtrl->ulReadIndex + 1) % (pstRArryCtrl->ulRowsCount);
        pstRArryCtrl->ulCount--;
    }

    return ulRet;
}


BS_STATUS RArray_ReadIndex(IN HANDLE hHandle, IN UINT ulIndex, OUT UCHAR **ppucData, OUT UINT *pulDataLen)
{
    _RARRAY_CTRL_S *pstRArryCtrl = (_RARRAY_CTRL_S*)hHandle;
    UINT ulWillReadIndex;

    if ((hHandle == 0) || (ppucData == NULL))
    {
        BS_WARNNING(("Null ptr!"));
        RETURN(BS_NULL_PARA);
    }

    if (ulIndex >= pstRArryCtrl->ulCount)
    {
        RETURN(BS_EMPTY);
    }

    ulWillReadIndex = (pstRArryCtrl->ulReadIndex + ulIndex) % (pstRArryCtrl->ulRowsCount);

    *ppucData = pstRArryCtrl->pucData + ulWillReadIndex * pstRArryCtrl->ulColCount + sizeof(UINT);
    if (pulDataLen)
    {
        *pulDataLen = *((UINT*)(pstRArryCtrl->pucData + ulWillReadIndex * pstRArryCtrl->ulColCount));
    }

    return BS_OK;
}


BS_STATUS RArray_ReadReversedIndex(IN HANDLE hHandle, IN UINT ulIndex, OUT UCHAR **ppucData, OUT UINT *pulDataLen)
{
    _RARRAY_CTRL_S *pstRArryCtrl = (_RARRAY_CTRL_S*)hHandle;
    UINT ulWillReadIndex;

    if ((hHandle == 0) || (ppucData == NULL))
    {
        BS_WARNNING(("Null ptr!"));
        RETURN(BS_NULL_PARA);
    }

    if (ulIndex >= pstRArryCtrl->ulCount)
    {
        RETURN(BS_EMPTY);
    }

    ulWillReadIndex = (pstRArryCtrl->ulWriteIndex + pstRArryCtrl->ulRowsCount - 1 - ulIndex) % (pstRArryCtrl->ulRowsCount);

    *ppucData = pstRArryCtrl->pucData + ulWillReadIndex * pstRArryCtrl->ulColCount + sizeof(UINT);
    if (pulDataLen)
    {
        *pulDataLen = *((UINT*)(pstRArryCtrl->pucData + ulWillReadIndex * pstRArryCtrl->ulColCount));
    }

    return BS_OK;
}

