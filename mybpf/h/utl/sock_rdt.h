/******************************************************************************
* Copyright (C), LiXingang
* Author:      LiXingang  Version: 1.0  Date: 2008-3-27
* Description: 
* History:     
******************************************************************************/

#ifndef __SOCK_RDT_H_
#define __SOCK_RDT_H_

#ifdef __cplusplus
    extern "C" {
#endif 

#define SOCK_RDT_EVENT_BEFORE_CONNTION 0x1
#define SOCK_RDT_EVENT_AFTER_CONNTION  0x2

typedef BS_STATUS (*PF_SOCK_RDT_CALL_BACK)(IN UINT ulEventType, IN UINT ulSocketId, IN struct sockaddr_in *pstName, IN INT lNameLen, IN UINT ulUsrHandle);

BS_STATUS SockRDT_RegEvent(IN UINT ulEvent, IN PF_SOCK_RDT_CALL_BACK pfFunc, IN UINT ulUserHandle);

BS_STATUS SockRDT_RedirectTo(IN CHAR *pszSslTcpType, IN UINT ulRdtIp, IN USHORT usRdtPort);


#ifdef __cplusplus
    }
#endif 

#endif 


