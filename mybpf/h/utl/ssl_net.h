/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2008-5-13
* Description: 
* History:     
******************************************************************************/

#ifndef __SSL_NET_H_
#define __SSL_NET_H_

#include "utl/ssltcp_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 

BS_STATUS SSLNET_Create(IN UINT ulFamily, IN CHAR *pszSslPolicy, OUT HANDLE *phSslHandle);
INT SSLNET_Send(IN HANDLE hSslHandle, IN UCHAR *pucBuf, IN UINT ulLen, IN UINT ulFlag);
INT SSLNET_Recv(IN HANDLE hSslHandle, OUT UCHAR *pucBuf, IN UINT ulLen, IN UINT ulFlag);
BS_STATUS SSLNET_Close(IN HANDLE hSslHandle);
BS_STATUS SSLNET_Accept(IN HANDLE hListenSslHandle, OUT HANDLE *phAcceptSslHandle);

BS_STATUS SSLNET_Listen(IN HANDLE hSslHandle, UINT ulLocalIp, IN USHORT usPort, IN USHORT usBacklog);
BS_STATUS SSLNET_SetAsyn(IN HANDLE hSslHandle, IN PF_SSLTCP_ASYN_FUNC pfFunc, IN USER_HANDLE_S *pstUserHandle);
BS_STATUS SSLNET_UnSetAsyn(IN HANDLE hSslHandle);

BS_STATUS SSLNET_GetHostIpPort(IN HANDLE hSslHandle, OUT UINT *pulIp, OUT USHORT *pusPort);

BS_STATUS SSLNET_GetPeerIpPort(IN HANDLE hSslHandle, OUT UINT *pulIp, OUT USHORT *pusPort);


#ifdef __cplusplus
    }
#endif 

#endif 


