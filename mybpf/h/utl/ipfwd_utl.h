/******************************************************************************
* Copyright (C) LiXingang
* Author:      LiXingang  Version: 1.0  Date: 2013-1-10
* Description: 
* History:     
******************************************************************************/

#ifndef __IPFWD_UTL_H_
#define __IPFWD_UTL_H_

#include "utl/fib_utl.h"
#include "utl/ipfwd_service.h"

#ifdef __cplusplus
    extern "C" {
#endif 


#define IP_FWD_DBG_PACKET 0x1


typedef HANDLE IPFWD_HANDLE;

typedef BS_STATUS (*PF_IPFWD_LINK_OUTPUT)(IN UINT uiIfIndex, IN MBUF_S *pstMbuf, IN USHORT usProtoType);
typedef BS_STATUS (*PF_IPFWD_DeliverUp)(IN IP_HEAD_S *pstIpHead, IN MBUF_S *pstMbuf);

typedef enum
{
    IPFWD_HOW_DEBUG_SET = 0,    
    IPFWD_HOW_DEBUG_ADD,        
    IPFWD_HOW_DEBUG_DEL         
}IPFWD_HOW_DEBUG_E;


IPFWD_HANDLE IPFWD_Create
(
    IN FIB_HANDLE hFib,
    IN IPFWD_SERVICE_HANDLE hIpFwdService,
    IN PF_IPFWD_LINK_OUTPUT pfLinkOutput,
    IN PF_IPFWD_DeliverUp pfDeliverUp
);

VOID IPFWD_Destory(IN IPFWD_HANDLE hIpFwd);


BS_STATUS IPFWD_Send
(
    IN IPFWD_HANDLE hIpFwd,
    IN MBUF_S *pstMbuf 
);

BS_STATUS IPFWD_Output
(
    IN IPFWD_HANDLE hIpFwd,
    IN MBUF_S *pstMbuf, 
    IN UINT uiDstIp,    
    IN UINT uiSrcIp,    
    IN UCHAR ucProto
);

BS_STATUS IPFWD_Input (IN IPFWD_HANDLE hIpFwd, IN MBUF_S *pstMbuf);

VOID IPFWD_SetDebug(IN IPFWD_HANDLE hIpFwd, IN IPFWD_HOW_DEBUG_E eHow, IN UINT uiDbgFlag);


#ifdef __cplusplus
    }
#endif 

#endif 


