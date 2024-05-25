/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2013-1-10
* Description: 
* History:     
******************************************************************************/

#ifndef __IPFWD_SERVICE_H_
#define __IPFWD_SERVICE_H_

#include "utl/ip_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 


typedef HANDLE IPFWD_SERVICE_HANDLE;

typedef enum
{
    IPFWD_SERVICE_PRE_ROUTING,               
    IPFWD_SERVICE_LOCAL_IN,                  
    IPFWD_SERVICE_NO_FIB,                    
    IPFWD_SERVICE_BEFORE_FORWARD,            
    IPFWD_SERVICE_LOCAL_OUT,                 
    IPFWD_SERVICE_POST_ROUTING_BEFOREFRAG,   
    IPFWD_SERVICE_POST_ROUTING_AFTERFRAG,    
    IPFWD_SERVICE_BEFORE_DELIVER_UP,         

    IPFWD_SERVICE_PHASE_MAX
}IPFWD_SERVICE_PHASE_E;

typedef enum
{
    IPFWD_SERVICE_RET_CONTINUE = 0,   
    IPFWD_SERVICE_RET_TAKE_OVER,      

    IPFWD_SERVICE_RET_MAX
}IPFWD_SERVICE_RET_E;

typedef IPFWD_SERVICE_RET_E (*PF_IPFWD_SERVICE_FUNC)(IN IP_HEAD_S *pstIpHead, IN MBUF_S *pstMbuf, IN USER_HANDLE_S *pstUserHandle);


IPFWD_SERVICE_HANDLE IPFWD_Service_Create();
VOID IPFWD_Service_Destory(IN IPFWD_SERVICE_HANDLE hIpFwdService);
IPFWD_SERVICE_RET_E IPFWD_Service_Handle
(
    IN IPFWD_SERVICE_HANDLE hIpFwdService,
    IN IPFWD_SERVICE_PHASE_E ePhase,
    IN IP_HEAD_S *pstIpHead,
    IN MBUF_S *pstMbuf
);


BS_STATUS IPFWD_Service_Reg
(
    IN IPFWD_SERVICE_HANDLE hIpFwdService,
    IN IPFWD_SERVICE_PHASE_E ePhase,
    IN UINT uiOrder, 
    IN CHAR *pcName, 
    IN PF_IPFWD_SERVICE_FUNC pfFunc,
    IN USER_HANDLE_S *pstUserHandle
);

#ifdef __cplusplus
    }
#endif 

#endif 


