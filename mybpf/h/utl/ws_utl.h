/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2014-5-24
* Description: 
* History:     
******************************************************************************/

#ifndef __WS_UTL_H_
#define __WS_UTL_H_

#include "utl/mbuf_utl.h"
#include "utl/http_lib.h"
#include "utl/mime_utl.h"
#include "utl/mypoll_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 


#define WS_DBG_PACKET  0x1
#define WS_DBG_EVENT   0x2
#define WS_DBG_PROCESS 0x4
#define WS_DBG_ERR     0x8
#define WS_DBG_ALL     0xffffffff


#define WS_VHOST_MAX_LEN  63
#define WS_DOMAIN_MAX_LEN 63

typedef VOID* WS_HANDLE;
typedef VOID * WS_CONN_HANDLE;
typedef VOID * WS_TRANS_HANDLE;

#if 1

typedef BS_STATUS (*PF_WS_CONN_SET_EVENT)(IN WS_CONN_HANDLE hWsConn, IN UINT uiEvent);
typedef VOID (*PF_WS_CONN_CLOSE)(IN WS_CONN_HANDLE hWsConn);
typedef INT (*PF_WS_CONN_READ)(IN WS_CONN_HANDLE hWsConn, OUT UCHAR *pucData, IN UINT uiDataLen);
typedef INT (*PF_WS_CONN_WRITE)(IN WS_CONN_HANDLE hWsConn, IN UCHAR *pucData, IN UINT uiDataLen);

typedef struct
{
    PF_WS_CONN_SET_EVENT pfSetEvent;
    PF_WS_CONN_CLOSE pfClose;
    PF_WS_CONN_READ pfRead;
    PF_WS_CONN_WRITE pfWrite;
}WS_FUNC_TBL_S;

WS_HANDLE WS_Create(IN WS_FUNC_TBL_S *pstFuncTbl);
VOID WS_Destory(IN WS_HANDLE hWs);
#endif

#if 1

#define WS_CONN_EVENT_READ  MYPOLL_EVENT_IN  
#define WS_CONN_EVENT_WRITE MYPOLL_EVENT_OUT
#define WS_CONN_EVENT_ERROR MYPOLL_EVENT_ERR

BS_STATUS WS_Conn_Add(IN WS_HANDLE hWs, IN HANDLE hRawConn, IN USER_HANDLE_S *pstPrivateData);
VOID WS_Conn_Destory(IN WS_CONN_HANDLE hWsConn);
HANDLE WS_Conn_GetRawConn(IN WS_CONN_HANDLE hWsConn);
VOID WS_Conn_ClearRawConn(IN WS_CONN_HANDLE hWsConn);
BS_STATUS WS_Conn_SetEvent(IN WS_CONN_HANDLE hWsConn, IN UINT uiEvent);
VOID WS_Conn_EventHandler(IN WS_CONN_HANDLE hWsConn, IN UINT uiEvent);
USER_HANDLE_S * WS_Conn_GetPrivateData(IN WS_CONN_HANDLE hWsConn);
#endif

#if 1
typedef VOID* WS_VHOST_HANDLE;
WS_VHOST_HANDLE WS_VHost_Add(IN WS_HANDLE hWs, IN CHAR *pcVHost);
VOID WS_VHost_Del(IN WS_VHOST_HANDLE hVHost);
UINT WS_VHost_GetContextCount(IN WS_VHOST_HANDLE hVHost);

WS_VHOST_HANDLE WS_VHost_Find(IN WS_HANDLE hWs, IN CHAR *pcVHost);

WS_VHOST_HANDLE WS_VHost_Match(IN WS_HANDLE hWs, IN CHAR *pcVHost, IN UINT uiVHostLen);
#endif

#if 1

typedef HANDLE WS_DELIVER_TBL_HANDLE;

typedef enum
{
    WS_DELIVER_TYPE_METHOD = 0,
    WS_DELIVER_TYPE_FILE,
    WS_DELIVER_TYPE_PATH,
    WS_DELIVER_TYPE_EXT_NAME,
    WS_DELIVER_TYPE_REFER_PATH,
    WS_DELIVER_TYPE_QUERY,
    WS_DELIVER_TYPE_CALL_BACK,  
}WS_DELIVER_TYPE_E;

#define WS_DELIVER_FLAG_DROP_BODY           0x1   
#define WS_DELIVER_FLAG_DELIVER_BODY        0x2   
#define WS_DELIVER_FLAG_PARSE_BODY_AS_MIME  0x4   

typedef enum
{
    WS_EV_RET_CONTINUE = 0, 
    WS_EV_RET_BREAK,        
    WS_EV_RET_STOP,         
    WS_EV_RET_INHERIT,      
    WS_EV_RET_ERR
}WS_EV_RET_E;

typedef enum
{
    WS_DELIVER_RET_OK = 0,
    WS_DELIVER_RET_INHERIT,
    WS_DELIVER_RET_ERR
}WS_DELIVER_RET_E;

typedef BOOL_T (*PF_WS_Deliver_MatchCB)(IN WS_TRANS_HANDLE hTrans);
typedef WS_DELIVER_RET_E (*PF_WS_Deliver_Func)(IN WS_TRANS_HANDLE hTrans, IN UINT uiEvent);

WS_DELIVER_TBL_HANDLE WS_Deliver_Create();

BS_STATUS WS_Deliver_Reg
(
    IN WS_DELIVER_TBL_HANDLE hDeliverTbl,
    IN UINT uiPriority,
    IN WS_DELIVER_TYPE_E enType,
    IN VOID *pKey,  
    IN PF_WS_Deliver_Func pfFunc,
    IN UINT uiFlag
);

BS_STATUS WS_Deliver_SetDeliverFunc(IN WS_TRANS_HANDLE hWsTrans, IN PF_WS_Deliver_Func pfDeliverFunc);

#endif


#if 1
#define WS_CONTEXT_MAX_INDEX_LEN 63

typedef VOID* WS_CONTEXT_HANDLE;

WS_CONTEXT_HANDLE WS_Context_Add(IN WS_VHOST_HANDLE hVHost, IN CHAR *pcContext);
VOID WS_Context_Del(IN WS_CONTEXT_HANDLE hContext);
WS_CONTEXT_HANDLE WS_Context_Find
(
    IN WS_VHOST_HANDLE hVHost,
    IN CHAR *pcContext
);
WS_CONTEXT_HANDLE WS_Context_GetDftContext(IN WS_VHOST_HANDLE hVHost);
VOID WS_Context_DelAll(IN WS_VHOST_HANDLE hVHost);
BS_STATUS WS_Context_SetUserData(IN WS_CONTEXT_HANDLE hWsContext, IN VOID *pUserData);
VOID * WS_Context_GetUserData(IN WS_CONTEXT_HANDLE hWsContext);
BS_STATUS WS_Context_SetRootPath(IN WS_CONTEXT_HANDLE hWsContext, IN CHAR *pcPath);
BS_STATUS WS_Context_SetSecRootPath(IN WS_CONTEXT_HANDLE hWsContext, IN CHAR *pcPath);
CHAR * WS_Context_GetRootPath(IN WS_CONTEXT_HANDLE hWsContext);
CHAR * WS_Context_GetSecRootPath(IN WS_CONTEXT_HANDLE hWsContext);
CHAR * WS_Context_File2RootPathFile
(
    IN WS_CONTEXT_HANDLE hWsContext,
    IN CHAR *pcFilePath,
    OUT CHAR *pcRootPathFile,
    IN UINT uiRootPathFileSize
);
BS_STATUS WS_Context_SetIndex(IN WS_CONTEXT_HANDLE hWsContext, IN CHAR *pcIndex);
CHAR * WS_Context_GetIndex(IN WS_CONTEXT_HANDLE hWsContext);
WS_VHOST_HANDLE WS_Context_GetVHost(IN WS_CONTEXT_HANDLE hWsContext);
CHAR * WS_Context_GetDomainName(IN WS_CONTEXT_HANDLE hWsContext);
CHAR * WS_Context_GetNext(IN WS_VHOST_HANDLE hVHost, IN CHAR *pcCurrentContext);
VOID WS_Context_BindDeliverTbl(IN WS_CONTEXT_HANDLE hWsContext, IN WS_DELIVER_TBL_HANDLE hDeliverTbl);
#endif


#if 1
VOID * WS_TransMemPool_Alloc(IN WS_TRANS_HANDLE hTrans, IN UINT uiSize);
VOID * WS_TransMemPool_ZAlloc(IN WS_TRANS_HANDLE hTrans, IN UINT uiSize);
VOID WS_TransMemPool_Free(IN WS_TRANS_HANDLE hTrans, IN VOID *pMem);
#endif

#if 1
#define WS_TRANS_EVENT_CREATE              0x1
#define WS_TRANS_EVENT_RECV_HEAD_OK        0x2
#define WS_TRANS_EVENT_RECV_BODY           0x4
#define WS_TRANS_EVENT_RECV_BODY_OK        0x8
#define WS_TRANS_EVENT_PRE_BUILD_HEAD      0x10
#define WS_TRANS_EVENT_SEND_HEAD_OK        0x20
#define WS_TRANS_EVENT_BUILD_BODY          0x40  
#define WS_TRANS_EVENT_FORMAT_BODY         0x80  
#define WS_TRANS_EVENT_SEND_BODY_OK        0x100
#define WS_TRANS_EVENT_DESTORY             0x200

CHAR * WS_Trans_GetRequestFile(IN WS_TRANS_HANDLE hTrans);
WS_CONTEXT_HANDLE WS_Trans_GetContext(IN WS_TRANS_HANDLE hTrans);
BS_STATUS WS_Trans_SetUserHandle(IN WS_TRANS_HANDLE hTrans, IN CHAR *pcKey, IN HANDLE hUserHandle);
HANDLE WS_Trans_GetUserHandle(IN WS_TRANS_HANDLE hTrans, IN CHAR *pcKey);
MBUF_S * WS_Trans_GetBodyData(IN WS_TRANS_HANDLE hTrans);

#define WS_TRANS_REPLY_FLAG_WITHOUT_BODY 0x1  
#define WS_TRANS_REPLY_FLAG_IMMEDIATELY  0x2  

BS_STATUS WS_Trans_Reply(IN WS_TRANS_HANDLE hTrans, IN UINT uiStatusCode, IN UINT uiFlag);
BS_STATUS WS_Trans_Redirect(IN WS_TRANS_HANDLE hTrans, IN CHAR *pcRedirectTo);
VOID WS_Trans_Pause(IN WS_TRANS_HANDLE hTrans);
VOID WS_Trans_Continue(IN WS_TRANS_HANDLE hTrans);
VOID WS_Trans_SetHeadFieldFinish(IN WS_TRANS_HANDLE hTrans);
VOID WS_Trans_Close(IN WS_TRANS_HANDLE hTrans);
VOID WS_Trans_Err(IN WS_TRANS_HANDLE hTrans);
HTTP_HEAD_PARSER WS_Trans_GetHttpRequestParser(IN WS_TRANS_HANDLE hTrans);
HTTP_HEAD_PARSER WS_Trans_GetHttpEncap(IN WS_TRANS_HANDLE hTrans);
MIME_HANDLE WS_Trans_GetQueryMime(IN WS_TRANS_HANDLE hTrans);
MIME_HANDLE WS_Trans_GetBodyMime(IN WS_TRANS_HANDLE hTrans);
MIME_HANDLE WS_Trans_GetCookieMime(IN WS_TRANS_HANDLE hTrans);
BS_STATUS WS_Trans_AddReplyBody(IN WS_TRANS_HANDLE hTrans, IN MBUF_S *pstMbuf);
BS_STATUS WS_Trans_AddReplyBodyByBuf(IN WS_TRANS_HANDLE hTrans, IN void *buf, IN UINT uiDataLen);
static inline BS_STATUS WS_Trans_AddReplyBodyByString(IN WS_TRANS_HANDLE hTrans, IN UCHAR *pcData)
{
    return WS_Trans_AddReplyBodyByBuf(hTrans, pcData, strlen((CHAR*)pcData));
}
VOID WS_Trans_ReplyBodyFinish(IN WS_TRANS_HANDLE hTrans);
WS_CONN_HANDLE WS_Trans_GetConn(IN WS_TRANS_HANDLE hTrans);
#endif

CHAR * WS_GetEventName(IN UINT uiEvent);
VOID WS_SetDbg(IN WS_HANDLE hWs, IN UINT uiDbgFlag);
VOID WS_ClrDbg(IN WS_HANDLE hWs, IN UINT uiDbgFlag);
UINT WS_GetDbgFlagByName(IN CHAR *pcFlagName);
VOID WS_SetDbgFlagByName(IN WS_HANDLE hWs, IN CHAR *pcFlagName);
VOID WS_ClrDbgFlagByName(IN WS_HANDLE hWs, IN CHAR *pcFlagName);

#ifdef __cplusplus
    }
#endif 

#endif 


