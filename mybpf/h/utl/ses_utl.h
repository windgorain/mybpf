/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2013-10-31
* Description: 
* History:     
******************************************************************************/

#ifndef __SES_UTL_H_
#define __SES_UTL_H_

#include "mbuf_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 

#define SES_INVALID_ID 0

#define SES_DBG_FLAG_PROTOCOL_PKT 0x1
#define SES_DBG_FLAG_DATA_PKT     0x2

#define SES_EVENT_CONNECT 1
#define SES_EVENT_ACCEPT  2
#define SES_EVENT_CONNECT_FAILED  3
#define SES_EVENT_PEER_CLOSED 4


typedef enum
{
    SES_STATUS_INIT = 0,
    SES_STATUS_SYN_SEND,
    SES_STATUS_SYN_RCVD,
    SES_STATUS_ESTABLISH,
    SES_STATUS_CLOSED,

    SES_STATUS_MAX
}SES_STATUS_E;

typedef enum
{
    SES_OPT_KEEP_ALIVE_TIME = 0,  

    SES_OPT_MAX
}SES_OPT_E;

typedef struct
{
    USHORT usIdle;    
    USHORT usIntval;  
    USHORT usMaxProbeCount; 
}SES_OPT_KEEP_ALIVE_TIME_S;

typedef HANDLE SES_HANDLE;

typedef BS_STATUS (*PF_SES_SEND_PKT)(IN MBUF_S *pstMbuf, IN VOID *pUserContext);
typedef BS_STATUS (*PF_SES_RECV_PKT)(IN UINT uiSesID, IN MBUF_S *pstMbuf);
typedef BS_STATUS (*PF_SES_DFT_EVENT_NOTIFY)(IN UINT uiSesID, IN UINT uiEvent);
typedef BS_STATUS (*PF_SES_EVENT_NOTIFY)(IN UINT uiSesID, IN UINT uiEvent, IN USER_HANDLE_S *pstUserHandle);
typedef int (*PF_SES_WALK_FUNC)(IN UINT uiSesID, IN HANDLE hUserHandle);

typedef VOID (*PF_SES_CLOSE_NOTIFY_FUNC)(IN UINT uiSesID, IN HANDLE *phPropertys, IN USER_HANDLE_S *pstUserHandle);

SES_HANDLE SES_CreateInstance
(
    IN UINT uiMaxSesNum,
    IN UINT uiUserContextSize,
    IN UINT uiMaxProperty,
    IN PF_SES_RECV_PKT pfRecvPktFunc,
    IN PF_SES_SEND_PKT pfSendPktFunc,
    IN PF_SES_DFT_EVENT_NOTIFY pfEventNotify
);
VOID SES_DestroyInstance(IN SES_HANDLE hSesHandle);
BS_STATUS SES_SetDftKeepAlive(IN SES_HANDLE hSesHandle, IN SES_OPT_KEEP_ALIVE_TIME_S *pstKeepAlive);

BS_STATUS SES_RegCloseNotifyEvent
(
    IN SES_HANDLE hSesHandle,
    IN PF_SES_CLOSE_NOTIFY_FUNC pfFunc,
    IN USER_HANDLE_S *pstUserHandle
);


UINT SES_CreateClient(IN SES_HANDLE hSesHandle, IN VOID *pContext);

typedef enum
{
    SES_TYPE_CONNECTER = 0,
    SES_TYPE_APPECTER,

    SES_TYPE_MAX
}SES_TYPE_E;
SES_TYPE_E SES_GetType(IN SES_HANDLE hSesHandle, IN UINT uiSesID);


BS_STATUS SES_SetEventNotify(IN SES_HANDLE hSesHandle, IN UINT uiSesID, IN PF_SES_EVENT_NOTIFY pfEventNotify, IN USER_HANDLE_S *pstUserHandle);
VOID SES_Close(IN SES_HANDLE hSesHandle, IN UINT uiSesID);
BS_STATUS SES_Connect(IN SES_HANDLE hSesHandle, IN UINT uiSesID);
BS_STATUS SES_PktInput(IN SES_HANDLE hSesHandle, IN MBUF_S *pstMbuf, IN VOID *pUserContext);
BS_STATUS SES_SendPkt(IN SES_HANDLE hSesHandle, IN UINT uiSesID, IN MBUF_S *pstMbuf);
VOID * SES_GetUsrContext(IN SES_HANDLE hSesHandle, IN UINT uiSesID);
VOID SES_TimerStep(IN SES_HANDLE hSesHandle);
UINT SES_GetStatus(IN SES_HANDLE hSesHandle, IN UINT uiSesID);
UINT SES_GetPeerSESID(IN SES_HANDLE hSesHandle, IN UINT uiSesID);
BS_STATUS SES_SetProperty(IN SES_HANDLE hSesHandle, IN UINT uiSesID, IN UINT uiPropertyIndex, IN HANDLE hValue);
HANDLE SES_GetProperty(IN SES_HANDLE hSesHandle, IN UINT uiSesID, IN UINT uiPropertyIndex);
HANDLE * SES_GetAllProperty(IN SES_HANDLE hSesHandle, IN UINT uiSesID);
BS_STATUS SES_SetOpt(IN SES_HANDLE hSesHandle, IN UINT uiSesID, IN UINT uiOpt, IN VOID *pValue);
VOID SES_Walk(IN SES_HANDLE hSesHandle, IN PF_SES_WALK_FUNC pfFunc, IN HANDLE hUserHandle);
CHAR * SES_GetStatusString(IN UINT uiStatus);
VOID SES_AddDbgFlag(IN SES_HANDLE hSesHandle, IN UINT uiDbgFlag);
VOID SES_ClrDbgFlag(IN SES_HANDLE hSesHandle, IN UINT uiDbgFlag);

#ifdef __cplusplus
    }
#endif 

#endif 


