/******************************************************************************
* Copyright (C),    LiXingang
* Author:      LiXingang  Version: 1.0  Date: 2014-8-1
* Description: 
* History:     
******************************************************************************/
#include "bs.h"

#include "utl/ssl_utl.h"
#include "utl/socket_utl.h"
#include "utl/conn_utl.h"

typedef struct
{
    INT iSocketID;
    VOID *pstSsl;
    MYPOLL_HANDLE hPollHandle;
    HANDLE ahUserData[CONN_USER_DATA_INDEX_MAX];
}CONN_S;

CONN_HANDLE CONN_New(IN INT iFd)
{
    CONN_S *pstConn;

    pstConn = MEM_ZMalloc(sizeof(CONN_S));
    if (NULL == pstConn)
    {
        return NULL;
    }

    pstConn->iSocketID = iFd;

    return pstConn;
}

VOID CONN_Free(IN CONN_HANDLE hConn)
{
    CONN_S *pstConn = hConn;

    if (pstConn == NULL)
    {
        return;
    }

    if (NULL != pstConn->hPollHandle)
    {
        MyPoll_Del(pstConn->hPollHandle, pstConn->iSocketID);
    }

    if (pstConn->pstSsl != NULL)
    {
        SSL_UTL_Free(pstConn->pstSsl);
    }

    if (pstConn->iSocketID >= 0)
    {
        Socket_Close(pstConn->iSocketID);
    }

    MEM_Free(pstConn);
}

VOID CONN_SetPoller(IN CONN_HANDLE hConn, IN MYPOLL_HANDLE hPollHandle)
{
    CONN_S *pstConn = hConn;

    if (NULL != pstConn)
    {
        pstConn->hPollHandle = hPollHandle;
    }
}

MYPOLL_HANDLE CONN_GetPoller(IN CONN_HANDLE hConn)
{
    CONN_S *pstConn = hConn;

    if (NULL == pstConn)
    {
        return NULL;
    }

    return pstConn->hPollHandle;
}

BS_STATUS CONN_SetSsl(IN CONN_HANDLE hConn, IN VOID *pstSsl)
{
    CONN_S *pstConn = hConn;

    if (NULL == pstConn)
    {
        return BS_NULL_PARA;
    }

    pstConn->pstSsl = pstSsl;

    return BS_OK;
}

VOID * CONN_GetSsl(IN CONN_HANDLE hConn)
{
    CONN_S *pstConn = hConn;

    if (NULL == pstConn)
    {
        return NULL;
    }

    return pstConn->pstSsl;
}

VOID CONN_SetUserData(IN CONN_HANDLE hConn, IN CONN_USER_DATA_INDEX_E eIndex, IN HANDLE hUserData)
{
    CONN_S *pstConn = hConn;

    if (eIndex >= CONN_USER_DATA_INDEX_MAX)
    {
        return;
    }

    if (NULL != pstConn)
    {
        pstConn->ahUserData[eIndex] = hUserData;
    }
}

HANDLE CONN_GetUserData(IN CONN_HANDLE hConn, IN CONN_USER_DATA_INDEX_E eIndex)
{
    CONN_S *pstConn = hConn;

    if (eIndex >= CONN_USER_DATA_INDEX_MAX)
    {
        return NULL;
    }

    if (NULL == pstConn)
    {
        return NULL;
    }

    return pstConn->ahUserData[eIndex];
}

INT CONN_GetFD(IN CONN_HANDLE hConn)
{
    CONN_S *pstConn = hConn;

    if (pstConn == NULL)
    {
        return -1;
    }

    return pstConn->iSocketID;
}

BS_STATUS CONN_SetEvent
(
    IN CONN_HANDLE hConn,
    IN UINT uiEvent,
    IN PF_MYPOLL_EV_NOTIFY pfFunc,
    IN USER_HANDLE_S *pstUserHandle
)
{
    CONN_S *pstConn = hConn;

    if (NULL == pstConn)
    {
        return BS_ERR;
    }
    
    return MyPoll_SetEvent(pstConn->hPollHandle, pstConn->iSocketID, uiEvent, pfFunc, pstUserHandle);
}

BS_STATUS CONN_AddEvent(IN CONN_HANDLE hConn, IN UINT uiEvent)
{
    CONN_S *pstConn = hConn;

    if (NULL == pstConn)
    {
        return BS_ERR;
    }
    
    return MyPoll_AddEvent(pstConn->hPollHandle, pstConn->iSocketID, uiEvent);
}

BS_STATUS CONN_DelEvent(IN CONN_HANDLE hConn, IN UINT uiEvent)
{
    CONN_S *pstConn = hConn;

    if (NULL == pstConn)
    {
        return BS_ERR;
    }

    return MyPoll_DelEvent(pstConn->hPollHandle, pstConn->iSocketID, uiEvent);
}


BS_STATUS CONN_ClearEvent(IN CONN_HANDLE hConn)
{
    CONN_S *pstConn = hConn;

    if (NULL == pstConn)
    {
        return BS_ERR;
    }

    return MyPoll_ClearEvent(pstConn->hPollHandle, pstConn->iSocketID);
}

BS_STATUS CONN_ModifyEvent(IN CONN_HANDLE hConn, IN UINT uiEvent)
{
    CONN_S *pstConn = hConn;

    if (NULL == pstConn)
    {
        return BS_ERR;
    }

    return MyPoll_ModifyEvent(pstConn->hPollHandle, pstConn->iSocketID, uiEvent);
}


INT CONN_Read(IN CONN_HANDLE hConn, OUT UCHAR *pucBuf, IN UINT uiBufLen)
{
    CONN_S *pstConn = hConn;

    if (NULL == pstConn)
    {
        return SOCKET_E_ERR;
    }

    if (pstConn->pstSsl == NULL)
    {
        return Socket_Read(pstConn->iSocketID, pucBuf, uiBufLen, 0);
    }

    return SSL_UTL_Read(pstConn->pstSsl, pucBuf, uiBufLen);
}

INT CONN_Write(IN CONN_HANDLE hConn, IN VOID *pBuf, IN UINT uiLen)
{
    CONN_S *pstConn = hConn;

    if (NULL == pstConn)
    {
        return SOCKET_E_ERR;
    }

    if (pstConn->pstSsl == NULL)
    {
        return Socket_Write(pstConn->iSocketID, pBuf, uiLen, 0);
    }

    return SSL_UTL_Write(pstConn->pstSsl, pBuf, uiLen);
}


INT CONN_WriteAll(IN CONN_HANDLE hConn, IN UCHAR *pucBuf, IN UINT uiLen)
{
    INT iLen;
    UINT uiRemainLen = uiLen;
    UCHAR *pucData = pucBuf;

    while (uiRemainLen > 0)
    {
        iLen = CONN_Write(hConn, pucData, uiRemainLen);
        if (iLen <= 0)
        {
            return iLen;
        }

        pucData += iLen;
        uiRemainLen -= iLen;
    }

    return (INT)uiLen;
}

INT CONN_WriteString(IN CONN_HANDLE hConn, IN CHAR *pcString)
{
    return CONN_Write(hConn, pcString, strlen(pcString));
}

INT CONN_SslPending(IN CONN_HANDLE hConn)
{
    CONN_S *pstConn = hConn;

    if (NULL == pstConn)
    {
        return 0;
    }

    if (pstConn->pstSsl == NULL)
    {
        return 0;
    }

    return SSL_UTL_Pending(pstConn->pstSsl);
}

