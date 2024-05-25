/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2008-3-26
* Description: 
* History:     
******************************************************************************/

#ifndef __SOCK_FWD_H_
#define __SOCK_FWD_H_

#include "utl/sock_ses.h"

#ifdef __cplusplus
    extern "C" {
#endif 



#define SOCK_FWD_EVENT_ACCEPT 0x1
typedef BS_STATUS (*PF_SOCK_FWD_ACCEPT_EVENT)(IN UINT ulEventType, IN UINT ulSslTcpId, IN UINT ulUsrHandle);

#define SOCK_FWD_EVENT_HOST_BEFORE_READ  0x1
#define SOCK_FWD_EVENT_HOST_AFTER_READ   0x2
#define SOCK_FWD_EVENT_SERVER_BEFORE_READ  0x4
#define SOCK_FWD_EVENT_SERVER_AFTER_READ   0x8
#define SOCK_FWD_EVENT_SERVER_CLOSE        0x10
#define SOCK_FWD_EVENT_HOST_CLOSE        0x20
typedef BS_STATUS (*PF_SOCK_FWD_TRANS_EVENT)(IN UINT ulEventType, IN SOCK_SES_S *pstSes, IN UINT ulUsrHandle);

extern BS_STATUS SockFwd_AddSend2ServerData(IN SOCK_SES_S *pstSesNode, IN UCHAR *pucData, IN UINT ulDataLen, OUT UINT *pulWriteLen);
extern BS_STATUS SockFwd_RegAcceptEvent(IN UINT ulEvent, IN PF_SOCK_FWD_ACCEPT_EVENT pfFunc, IN UINT ulUserHandle);
extern BS_STATUS SockFwd_RegTransEvent(IN UINT ulEvent, IN PF_SOCK_FWD_TRANS_EVENT pfFunc, IN UINT ulUserHandle);
extern BS_STATUS SockFwd_Init(IN UINT ulServerIp, IN USHORT usServerPort);
extern USHORT SockFwd_GetRdtListenPort();


#ifdef __cplusplus
    }
#endif 

#endif 


