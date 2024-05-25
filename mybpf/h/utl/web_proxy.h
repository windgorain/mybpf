/******************************************************************************
* Copyright (C), LiXingang
* Author:      LiXingang  Version: 1.0  Date: 2015-10-21
* Description: 在线代理
* History:     
******************************************************************************/

#ifndef __WEB_PROXY_H_
#define __WEB_PROXY_H_

#include "utl/mutex_utl.h"
#include "utl/mypoll_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 

typedef VOID* WEB_PROXY_HANDLE;

#define WEB_PROXY_DBG_FLAG_PACKET 0x1
#define WEB_PROXY_DBG_FLAG_FSM    0x2

#define WEB_PROXY_FLAG_ERRTYPE_COMPATIBLE 0x2 

typedef struct
{
    MUTEX_S stMutex;
    VOID *pSslCtx;
}WEB_PROXY_SSL_CTX_NODE_S;

WEB_PROXY_HANDLE WEB_Proxy_Create(IN UINT uiFlag);
VOID WEB_Proxy_Destroy(IN WEB_PROXY_HANDLE hWebProxy);
VOID WEB_Proxy_SetDgbFlag(IN WEB_PROXY_HANDLE hWebProxy, IN UINT uiDbgFlag);
VOID WEB_Proxy_ClrDgbFlag(IN WEB_PROXY_HANDLE hWebProxy, IN UINT uiDbgFlag);

BOOL_T WEB_Proxy_IsRwedUrl(IN WEB_PROXY_HANDLE hWebProxy, IN CHAR *pcUrl);
BS_STATUS WEB_Proxy_ReferInput
(
    IN WEB_PROXY_HANDLE hWebProxy,
    IN WS_TRANS_HANDLE hTrans,
    IN UINT uiEvent
);
BS_STATUS WEB_Proxy_AuthAgentInput
(
    IN WEB_PROXY_HANDLE hWebProxy,
    IN WS_TRANS_HANDLE hTrans,
    IN UINT uiEvent
);
BS_STATUS WEB_Proxy_RequestIn
(
    IN WEB_PROXY_HANDLE hWebProxy,
    IN MYPOLL_HANDLE hPoller,
    IN void *sslctx,
    IN WS_TRANS_HANDLE hTrans,
    IN UINT uiEvent
);

BS_STATUS WEB_Proxy_ParseServerUrl
(
    IN CHAR *pcUrl,
    OUT LSTR_S *pstTag,   
    OUT LSTR_S *pstProto, 
    OUT LSTR_S *pstHost,
    OUT LSTR_S *pstPort,
    OUT LSTR_S *pstPath
);

#ifdef __cplusplus
    }
#endif 

#endif 


