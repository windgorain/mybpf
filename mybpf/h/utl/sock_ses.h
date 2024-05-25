/******************************************************************************
* Copyright (C), LiXingang
* Author:      LiXingang  Version: 1.0  Date: 2008-3-23
* Description: 
* History:     
******************************************************************************/

#ifndef __SOCK_SES_H_
#define __SOCK_SES_H_

#ifdef __cplusplus
    extern "C" {
#endif 

#define SOCK_SES_READABLE     0x1
#define SOCK_SES_WRITEABLE    0x2
#define SOCK_SES_CLOSED       0x4

typedef struct
{
    UINT ulSslTcpId;
    UINT ulStatus;  
    HANDLE hRbufId;
}SOCK_SES_SIDE_S;


typedef struct
{
    DLL_NODE_S stDllNode;
    SOCK_SES_SIDE_S stSideA;
    SOCK_SES_SIDE_S stSideB;
    UINT ulFwdHandle1;
    HANDLE hUserHandle1;
    HANDLE hUserHandle2;
}SOCK_SES_S;


extern BS_STATUS SockSes_CreateInstance(IN BOOL_T bNeedSem, OUT HANDLE *phSockSesId);
extern VOID SockSes_DeleteInstance(IN HANDLE hSockSesId);
extern BS_STATUS SockSes_AddNode(IN HANDLE hSockSesId, IN UINT ulSslTcpSideA, IN UINT ulSslTcpSideB);
extern BS_STATUS SockSes_DelNode(IN HANDLE hSockSesId, IN SOCK_SES_S *pstDelNode);
extern SOCK_SES_S * SockSes_GetNodeBySideA(IN HANDLE hSockSesId, IN UINT ulSslTcpSideA);
extern SOCK_SES_S * SockSes_GetNodeBySideB(IN HANDLE hSockSesId, IN UINT ulSslTcpSideA);
extern VOID SockSes_Lock(IN HANDLE hSockSesId);
extern VOID SockSes_UnLock(IN HANDLE hSockSesId);

#ifdef __cplusplus
    }
#endif 

#endif 




