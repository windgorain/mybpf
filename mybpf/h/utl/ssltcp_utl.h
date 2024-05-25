/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2007-5-13
* Description: 
* History:     
******************************************************************************/

#ifndef __UTL_SSLTCP_H_
#define __UTL_SSLTCP_H_

#include "utl/socket_utl.h"
#include "utl/thread_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 


#define SSLTCP_MAX_TYPE_NAME_LEN 11
#define SSLTCP_MAX_SSLTCP_NUM    3072


#define SSLTCP_EVENT_ALL     0xffffffff
#define SSLTCP_EVENT_READ    0x1
#define SSLTCP_EVENT_WRITE   0x2
#define SSLTCP_EVENT_EXECPT  0x4


typedef enum
{
    SSLTCP_ASYN_MODE_LEVEL = 0,
    SSLTCP_ASYN_MODE_EDGE,
}SSLTCP_ASYN_MODE_E;

typedef BS_STATUS (*PF_SSLTCP_TRIGGER_FUNC)(IN USER_HANDLE_S *pstUserHandle);
typedef BS_STATUS (*PF_SSLTCP_INNER_CB_FUNC)(IN HANDLE hFileHandle, IN ULONG ulEvent, IN USER_HANDLE_S *pstUserHandle);
typedef BS_STATUS (*PF_SSLTCP_ASYN_FUNC)(IN UINT uiSslTcpId, IN UINT uiEvent, IN USER_HANDLE_S *pstUserHandle);

typedef BS_STATUS (*PF_SSLTCP_CREATE_FUNC)(IN UINT ulFamily, IN VOID *pParam, OUT HANDLE *hSslTcpId);
typedef BS_STATUS (*PF_SSLTCP_LISTEN_FUNC)(IN HANDLE hFileHandle, UINT ulLocalIp, IN USHORT usPort, IN UINT uiBacklog);
typedef BS_STATUS (*PF_SSLTCP_CONNECT_FUNC)(IN HANDLE hFileHandle, IN UINT ulIp, IN USHORT usPort);
typedef BS_STATUS (*PF_SSLTCP_ACCEPT_FUNC)(IN HANDLE hListenSocket, OUT HANDLE *phAcceptSocket);
typedef INT (*PF_SSLTCP_WRITE_FUNC)(IN HANDLE hFileHandle, IN UCHAR *pucBuf, IN UINT ulLen, IN UINT ulFlag);
typedef BS_STATUS (*PF_SSLTCP_READ_FUNC)(IN HANDLE hFileHandle, OUT UCHAR *pucBuf, IN UINT ulLen, OUT UINT *puiReadLen, IN UINT ulFlag);
typedef BS_STATUS (*PF_SSLTCP_CLOSE_FUNC)(IN HANDLE hFileHandle);
typedef HANDLE (*PF_SSLTCP_CREATE_ASYN_INSTANCE_FUNC)(IN SSLTCP_ASYN_MODE_E eMode);
typedef VOID (*PF_SSLTCP_DELETE_ASYN_INSTANCE_FUNC)(IN HANDLE hAsynHandle);
typedef BS_STATUS (*PF_SSLTCP_SET_ASYN_FUNC)
(
    IN HANDLE hAsynInstance,
    IN HANDLE hFileHandle,
    IN ULONG ulEvent,    
    IN PF_SSLTCP_INNER_CB_FUNC pfFunc,
    IN USER_HANDLE_S *pstUserHandle
);
typedef BS_STATUS (*PF_SSLTCP_UNSET_ASYN_FUNC)(IN HANDLE hAsynInstance, IN HANDLE hFileHandle);
typedef BS_STATUS (*PF_SSLTCP_SET_ASYN_TRIGGER_FUNC)(IN HANDLE hAsynInstance, PF_SSLTCP_TRIGGER_FUNC pfTriggerFunc, IN USER_HANDLE_S *pstUserHandle);
typedef BS_STATUS (*PF_SSLTCP_ASYN_TRIGGER_FUNC)(IN HANDLE hAsynInstance);
typedef BS_STATUS (*PF_SSLTCP_IOCTL_FUNC)(IN HANDLE hFileHandle, IN INT lCmd, IN UINT *argp);
typedef UINT (*PF_SSLTCP_GET_CONNECTION_NUM_FUNC)(IN HANDLE hAsynInstance);
typedef BS_STATUS (*PF_SSLTCP_DISPATCH_FUNC)(IN HANDLE hAsynInstance);
typedef BS_STATUS (*PF_SSLTCP_GET_HOST_IPPORT_FUNC)(IN HANDLE hFileHandle, OUT UINT *pulIp, OUT USHORT *pusPort);
typedef BS_STATUS (*PF_SSLTCP_GET_PEER_OPPORT_FUNC)(IN HANDLE hFileHandle, OUT UINT *pulIp, OUT USHORT *pusPort);
typedef struct
{
    BOOL_T bIsUsed;
    HANDLE hDftAsynInstance;
    THREAD_ID uiDftAsynThreadId;
    
    
    CHAR szProtoName[SSLTCP_MAX_TYPE_NAME_LEN + 1];
    PF_SSLTCP_CREATE_FUNC pfCreate;
    PF_SSLTCP_LISTEN_FUNC pfListen;
    PF_SSLTCP_CONNECT_FUNC pfConnect;
    PF_SSLTCP_ACCEPT_FUNC pfAccept;
    PF_SSLTCP_WRITE_FUNC pfWrite;
    PF_SSLTCP_READ_FUNC pfRead;
    PF_SSLTCP_CLOSE_FUNC pfClose;
    PF_SSLTCP_CREATE_ASYN_INSTANCE_FUNC pfCreateAsynInstance;
    PF_SSLTCP_DELETE_ASYN_INSTANCE_FUNC pfDeleteAsynInstance;
    PF_SSLTCP_SET_ASYN_FUNC pfSetAsyn;
    PF_SSLTCP_UNSET_ASYN_FUNC pfUnSetAsyn;
    PF_SSLTCP_DISPATCH_FUNC pfDispatch;
    PF_SSLTCP_SET_ASYN_TRIGGER_FUNC pfSetAsynTrigger;
    PF_SSLTCP_ASYN_TRIGGER_FUNC pfAsynTrigger;
    PF_SSLTCP_IOCTL_FUNC pfIoctl;
    PF_SSLTCP_GET_CONNECTION_NUM_FUNC pfGetConnectionNum;
    PF_SSLTCP_GET_HOST_IPPORT_FUNC pfGetHostIpPort;
    PF_SSLTCP_GET_PEER_OPPORT_FUNC pfGetPeerIpPort;
}SSLTCP_PROTO_S;


PLUG_API BS_STATUS SSLTCP_Display (IN UINT ulArgc, IN UCHAR **argv);

PLUG_API BS_STATUS SSLTCP_RegProto(IN SSLTCP_PROTO_S *pstProto);
PLUG_API UINT SSLTCP_Create(IN CHAR *pszProtoName, IN UINT ulFamily, IN VOID *pParam);
PLUG_API BOOL_T SSLTCP_IsValid (IN UINT ulSslTcpId);

PLUG_API BS_STATUS SSLTCP_Listen(IN UINT uiSslTcpId, IN UINT uiIp, IN USHORT usPort, IN UINT uiBackLog);

PLUG_API BS_STATUS SSLTCP_Accept(IN UINT hListenSslTcpId, OUT UINT *puiAcceptSslTcpId);
PLUG_API BS_STATUS SSLTCP_Connect(IN UINT ulSslTcpId, IN UINT ulIp, IN USHORT usPort);

PLUG_API BS_STATUS SSLTCP_Write(IN UINT ulSslTcpId, IN UCHAR * pucBuf, IN UINT ulSize, OUT UINT *puiWriteSize);

PLUG_API BS_STATUS SSLTCP_WriteUntilFinish(IN UINT ulSslTcpId, IN UCHAR * pucBuf, IN UINT ulSize);
PLUG_API BS_STATUS SSLTCP_Read(IN UINT ulSslTcpId, OUT void* pucBuf, IN UINT ulBufSize, OUT UINT *ulReadSize);
PLUG_API BS_STATUS SSLTCP_Close(IN UINT ulSslTcpId);
PLUG_API UINT SSLTCP_GetPeerIP(IN UINT ulSslTcpId);
PLUG_API USHORT SSLTCP_GetPeerPort(IN UINT ulSslTcpId);
PLUG_API HANDLE SSLTCP_CreateAsynInstance(IN SSLTCP_ASYN_MODE_E eAsynMode);
PLUG_API VOID SSLTCP_DeleteAsynInstance(IN HANDLE hAsynHandle);
PLUG_API BS_STATUS SSLTCP_Dispatch(IN HANDLE hAsynInstance);
PLUG_API UINT SSLTCP_GetConnectionNum(IN HANDLE hAsynInstance);
PLUG_API BS_STATUS SSLTCP_SetAsyn
(
    IN HANDLE hAsynInstance,
    IN UINT uiSslTcpId,
    IN ULONG ulEvent,
    IN PF_SSLTCP_ASYN_FUNC pfFunc,
    IN USER_HANDLE_S *pstUserHandle
);
PLUG_API BS_STATUS SSLTCP_GetAsyn(IN UINT ulSslTcpId, OUT USER_HANDLE_S **ppstUserHandle);
PLUG_API HANDLE SSLTCP_GetAsynInstanceHandle(IN UINT uiSsltcpId);
PLUG_API BS_STATUS SSLTCP_UnSetAsyn(IN UINT ulSslTcpId);
PLUG_API BS_STATUS SSLTCP_SetAsynTrigger(IN HANDLE hAsynInstance, IN PF_SSLTCP_TRIGGER_FUNC pfFunc, IN USER_HANDLE_S *pstUserHandle);
PLUG_API BS_STATUS SSLTCP_AsynTrigger(IN HANDLE hAsynInstance);
PLUG_API BS_STATUS SSLTCP_Ioctl(IN UINT uiSslTcpId, IN INT lCmd, IN VOID *pParam);
PLUG_API UINT SSLTCP_GetFamily(IN UINT ulSslTcpId);
PLUG_API USHORT SSLTCP_GetHostPort(IN UINT ulSslTcpId);
PLUG_API UINT SSLTCP_GetHostIP(IN UINT ulSslTcpId);


#ifdef __cplusplus
    }
#endif 

#endif 


