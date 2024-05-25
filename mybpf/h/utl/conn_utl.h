/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2014-8-1
* Description: 
* History:     
******************************************************************************/

#ifndef __CONN_UTL_H_
#define __CONN_UTL_H_

#include "utl/mypoll_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 

typedef VOID* CONN_HANDLE;

typedef enum
{
    CONN_USER_DATA_INDEX_0 = 0,
    CONN_USER_DATA_INDEX_1,

    CONN_USER_DATA_INDEX_MAX
}CONN_USER_DATA_INDEX_E;

CONN_HANDLE CONN_New(IN INT iFd);
VOID CONN_Free(IN CONN_HANDLE hConn);
VOID CONN_SetPoller(IN CONN_HANDLE hConn, IN MYPOLL_HANDLE hPollHandle);
MYPOLL_HANDLE CONN_GetPoller(IN CONN_HANDLE hConn);
BS_STATUS CONN_SetSsl(IN CONN_HANDLE hConn, IN VOID *pstSsl);
VOID * CONN_GetSsl(IN CONN_HANDLE hConn);
VOID CONN_SetUserData(IN CONN_HANDLE hConn, IN CONN_USER_DATA_INDEX_E eIndex, IN HANDLE hUserData);
HANDLE CONN_GetUserData(IN CONN_HANDLE hConn, IN CONN_USER_DATA_INDEX_E eIndex);
INT CONN_GetFD(IN CONN_HANDLE hConn);

BS_STATUS CONN_SetEvent
(
    IN CONN_HANDLE hConn,
    IN UINT uiEvent,
    IN PF_MYPOLL_EV_NOTIFY pfFunc,
    IN USER_HANDLE_S *pstUserHandle
);

BS_STATUS CONN_AddEvent(IN CONN_HANDLE hConn, IN UINT uiEvent);

BS_STATUS CONN_DelEvent(IN CONN_HANDLE hConn, IN UINT uiEvent);

BS_STATUS CONN_ClearEvent(IN CONN_HANDLE hConn);

BS_STATUS CONN_ModifyEvent(IN CONN_HANDLE hConn, IN UINT uiEvent);


INT CONN_Read(IN CONN_HANDLE hConn, OUT UCHAR *pucBuf, IN UINT uiBufLen);


INT CONN_Write(IN CONN_HANDLE hConn, IN VOID *pBuf, IN UINT uiLen);


INT CONN_WriteAll(IN CONN_HANDLE hConn, IN UCHAR *pucBuf, IN UINT uiLen);

INT CONN_WriteString(IN CONN_HANDLE hConn, IN CHAR *pcString);

INT CONN_SslPending(IN CONN_HANDLE hConn);

#ifdef __cplusplus
    }
#endif 

#endif 


