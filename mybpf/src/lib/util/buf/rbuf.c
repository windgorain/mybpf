/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2007-7-31
* Description: 环形buf
* History:     
******************************************************************************/

#define RETCODE_FILE_NUM RETCODE_FILE_NUM_RBUF

#include "bs.h"

#include "utl/num_utl.h"

typedef struct
{
    UINT ulMaxDataLen;
    UINT ulDataLen;    
    UINT ulReadIndex;  
    UINT ulWriteIndex; 
    UCHAR *pucData;
}_RBUF_CTRL_S;

BS_STATUS RBUF_Create(IN UINT ulBufLen, OUT HANDLE *phHandle)
{
    _RBUF_CTRL_S *pstCtrl = NULL;
    
    if ((phHandle == NULL) || (ulBufLen == 0))
    {
        BS_WARNNING(("Null ptr!"));
        RETURN(BS_NULL_PARA);
    }

    pstCtrl = MEM_Malloc(sizeof(_RBUF_CTRL_S) + ulBufLen);
    if (NULL == pstCtrl)
    {
        RETURN(BS_NO_MEMORY);
    }
    Mem_Zero(pstCtrl, sizeof(_RBUF_CTRL_S));

    pstCtrl->ulMaxDataLen = ulBufLen;
    pstCtrl->pucData = (UCHAR*)(pstCtrl + 1);

    *phHandle = pstCtrl;

    return BS_OK;
}

BS_STATUS RBUF_Delete(IN HANDLE hHandle)
{
    if (hHandle == 0)
    {
        BS_WARNNING(("Null ptr!"));
        RETURN(BS_NULL_PARA);
    }

    MEM_Free(hHandle);
    
    return BS_OK;
}


BS_STATUS RBUF_WriteForce(IN HANDLE hHandle, IN UCHAR *pucBuf, IN UINT ulBufLen)
{
    _RBUF_CTRL_S *pstCtrl = (_RBUF_CTRL_S*)hHandle;
    UINT ulWriteLenFirst = 0;

    if ((pstCtrl == NULL) || (pucBuf == NULL))
    {
        BS_WARNNING(("Null ptr!"));
        RETURN(BS_NULL_PARA);
    }

    if (ulBufLen == 0)
    {
        return BS_OK;
    }

    
    if (ulBufLen > pstCtrl->ulMaxDataLen)
    {
        ulBufLen = pstCtrl->ulMaxDataLen;
        pstCtrl->ulDataLen = 0;
        pstCtrl->ulReadIndex = pstCtrl->ulWriteIndex = 0;
        pucBuf += ulBufLen - pstCtrl->ulMaxDataLen;
    }

    if (ulBufLen > (pstCtrl->ulMaxDataLen - pstCtrl->ulDataLen))
    {
        pstCtrl->ulReadIndex = 
            (pstCtrl->ulReadIndex + ulBufLen - (pstCtrl->ulMaxDataLen - pstCtrl->ulDataLen)) % (pstCtrl->ulMaxDataLen);
    }

    if ((pstCtrl->ulWriteIndex + ulBufLen) <= pstCtrl->ulMaxDataLen)  
    {
        MEM_Copy(pstCtrl->pucData + pstCtrl->ulWriteIndex, pucBuf, ulBufLen);
    }
    else    
    {
        ulWriteLenFirst = pstCtrl->ulMaxDataLen - pstCtrl->ulWriteIndex;
        MEM_Copy(pstCtrl->pucData + pstCtrl->ulWriteIndex, pucBuf, ulWriteLenFirst);
        MEM_Copy(pstCtrl->pucData, pucBuf + ulWriteLenFirst, ulBufLen - ulWriteLenFirst);
    }

    pstCtrl->ulWriteIndex  = (pstCtrl->ulWriteIndex + ulBufLen) % (pstCtrl->ulMaxDataLen);
    pstCtrl->ulDataLen += ulBufLen;

    return BS_OK;
}


BS_STATUS RBUF_Write(IN HANDLE hHandle, IN UCHAR *pucBuf, IN UINT ulBufLen, OUT UINT *pulWriteLen)
{
    _RBUF_CTRL_S *pstCtrl = (_RBUF_CTRL_S*)hHandle;
    UINT ulWriteLen = 0;
    UINT ulRet;
    
    if ((pstCtrl == NULL) || (pucBuf == NULL))
    {
        BS_WARNNING(("Null ptr!"));
        RETURN(BS_NULL_PARA);
    }

    if (ulBufLen == 0)
    {
        if (pulWriteLen)
        {
            *pulWriteLen = 0;
        }
        return BS_OK;
    }

    ulWriteLen = MIN(ulBufLen, (pstCtrl->ulMaxDataLen - pstCtrl->ulDataLen));

    ulRet = RBUF_WriteForce(hHandle, pucBuf, ulWriteLen);

    if (BS_OK == ulRet)
    {
        pstCtrl->ulWriteIndex  = (pstCtrl->ulWriteIndex + ulWriteLen) % (pstCtrl->ulMaxDataLen);
        pstCtrl->ulDataLen += ulWriteLen;

        if (pulWriteLen)
        {
            *pulWriteLen = ulWriteLen;
        }
    }
    
    return ulRet;
}

BS_STATUS RBUF_ReadNoDel(IN HANDLE hHandle, OUT UCHAR **ppucData, OUT UINT *pulDataLen)
{
    _RBUF_CTRL_S *pstCtrl = (_RBUF_CTRL_S*)hHandle;

    if ((pstCtrl->ulWriteIndex < pstCtrl->ulReadIndex) && (pstCtrl->ulWriteIndex != 0))
    {
        
        UCHAR *pucTmp = MEM_Malloc(pstCtrl->ulWriteIndex - 1);
        if (pucTmp == NULL)
        {
            RETURN(BS_NO_MEMORY);
        }

        MEM_Copy(pucTmp, pstCtrl->pucData, pstCtrl->ulWriteIndex - 1);
        memmove(pstCtrl->pucData, pstCtrl->pucData + pstCtrl->ulReadIndex, pstCtrl->ulMaxDataLen - pstCtrl->ulReadIndex);
        MEM_Copy(pstCtrl->pucData + pstCtrl->ulMaxDataLen - pstCtrl->ulReadIndex, pucTmp, pstCtrl->ulWriteIndex - 1);
        pstCtrl->ulReadIndex = 0;
        pstCtrl->ulWriteIndex = pstCtrl->ulDataLen;
        
        MEM_Free(pucTmp);
    }

    *ppucData = pstCtrl->pucData + pstCtrl->ulReadIndex;
    if (pulDataLen)
    {
        *pulDataLen = pstCtrl->ulDataLen;
    }

    return BS_OK;    
}

BS_STATUS RBUF_Read(IN HANDLE hHandle, OUT UCHAR **ppucData, OUT UINT *pulDataLen)
{
    _RBUF_CTRL_S *pstCtrl = (_RBUF_CTRL_S*)hHandle;
    UINT ulRet;

    ulRet = RBUF_ReadNoDel(hHandle, ppucData, pulDataLen);
    if (BS_OK == ulRet)
    {
        pstCtrl->ulDataLen = 0;
        pstCtrl->ulReadIndex = pstCtrl->ulWriteIndex = 0;
    }

    return ulRet;
}



BS_STATUS RBUF_GetContinueWritePtr(IN HANDLE hHandle, OUT UCHAR **ppucWritePtr, OUT UINT *pulContinueMemLen)
{
    _RBUF_CTRL_S *pstCtrl = (_RBUF_CTRL_S*)hHandle;

    *ppucWritePtr = NULL;
    *pulContinueMemLen = 0;
    
    if (pstCtrl->ulDataLen >= pstCtrl->ulMaxDataLen)
    {
        RETURN(BS_NO_MEMORY);
    }

    *ppucWritePtr = pstCtrl->pucData + pstCtrl->ulWriteIndex;
    if (pstCtrl->ulReadIndex <= pstCtrl->ulWriteIndex)
    {
        *pulContinueMemLen = pstCtrl->ulMaxDataLen - pstCtrl->ulWriteIndex;
    }
    else
    {
        *pulContinueMemLen = pstCtrl->ulReadIndex - pstCtrl->ulWriteIndex;
    }

    return BS_OK;
}


VOID RBUF_MoveWriteIndex(IN HANDLE hHandle, IN INT lIndexMovOffset)
{
    _RBUF_CTRL_S *pstCtrl = (_RBUF_CTRL_S*)hHandle;

    pstCtrl->ulWriteIndex = (pstCtrl->ulWriteIndex + lIndexMovOffset) % pstCtrl->ulMaxDataLen;
}


VOID RBUF_MoveReadIndex(IN HANDLE hHandle, IN INT lIndexMovOffset)
{
    _RBUF_CTRL_S *pstCtrl = (_RBUF_CTRL_S*)hHandle;

    pstCtrl->ulWriteIndex = (pstCtrl->ulReadIndex + lIndexMovOffset) % pstCtrl->ulMaxDataLen;
}

BOOL_T RBUF_IsFull(IN HANDLE hHandle)
{
    _RBUF_CTRL_S *pstCtrl = (_RBUF_CTRL_S*)hHandle;

    if (pstCtrl->ulDataLen >= pstCtrl->ulMaxDataLen)
    {
        return TRUE;
    }

    return FALSE;
}

BOOL_T RBUF_IsEmpty(IN HANDLE hHandle)
{
    _RBUF_CTRL_S *pstCtrl = (_RBUF_CTRL_S*)hHandle;

    if (pstCtrl->ulDataLen == 0)
    {
        return TRUE;
    }

    return FALSE;
}

