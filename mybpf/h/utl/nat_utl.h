
#ifndef __NAT_H_
#define __NAT_H_

#include "utl/mbuf_utl.h"

#ifdef __cplusplus
extern "C" {
#endif 

#define NAT_MAX_PUB_IP_NUM 16 

#define NAT_DBG_PACKET 0x1

typedef HANDLE NAT_HANDLE;


typedef enum
{
    NAT_TCP_STATUS_SYN_SEND = 0,    
    NAT_TCP_STATUS_SYN_RECEIVED,    
    NAT_TCP_STATUS_ESTABLISHED,     

    NAT_TCP_STATUS_I_FIN_WAIT_1,      
    NAT_TCP_STATUS_I_FIN_WAIT_2,      
    NAT_TCP_STATUS_I_TIME_WAIT,       
                                      

    NAT_TCP_STATUS_O_FIN_WAIT_1,      
    NAT_TCP_STATUS_O_FIN_WAIT_2,      
    NAT_TCP_STATUS_O_TIME_WAIT,       
                                      

    NAT_TCP_STATUS_TIME_WAIT,         

    NAT_TCP_STATUS_CLOSED,

    NAT_TCP_STATUS_MAX
}NAT_TCP_STATUS_E;


typedef enum
{
    NAT_UDP_STATUS_SYN_SEND = 0,    
    NAT_UDP_STATUS_SYN_RECEIVED,    
    NAT_UDP_STATUS_ESTABLISHED,     

    NAT_UDP_STATUS_MAX
}NAT_UDP_STATUS_E;

typedef struct
{
    UINT uiPrivateIp;       
    UINT uiPubIp;           
    USHORT usPrivatePort;   
    USHORT usPubPort;       
    UINT uiDomainId;
    UCHAR ucType;
    UCHAR ucStatus;         
    UCHAR aucReserved[2];
}NAT_NODE_S;

typedef int (*PF_NAT_WALK_CALL_BACK_FUNC)(IN NAT_NODE_S *pstNatNode, IN HANDLE hUserHandle);

NAT_HANDLE NAT_Create
(
    IN USHORT usMinPort,   
    IN USHORT usMaxPort,    
    IN UINT   uiMsInTick,  
    IN BOOL_T bCreateMutex
);

VOID NAT_Destory(IN NAT_HANDLE hNatHandle);

VOID NAT_SetPubIp
(
    IN NAT_HANDLE hNatHandle,
    IN UINT auiPubIp[NAT_MAX_PUB_IP_NUM] 
);

BS_STATUS NAT_SetTcpTimeOutTick
(
    IN NAT_HANDLE hNatHandle,
    IN UINT uiTick
);

BS_STATUS NAT_SetUdpTimeOutTick
(
    IN NAT_HANDLE hNatHandle,
    IN UINT uiTick
);

BS_STATUS NAT_PacketTranslate
(
    IN NAT_HANDLE hNatHandle,
    INOUT UCHAR *pucData, 
    IN UINT uiDataLen,
    IN BOOL_T bFromPub,
    INOUT UINT *puiDomainId   
);


BS_STATUS NAT_PacketTranslateByMbuf
(
    IN NAT_HANDLE hNatHandle,
    INOUT MBUF_S *pstMbuf, 
    IN BOOL_T bFromPub,
    INOUT UINT *puiDomainId   
);

VOID NAT_TimerStep(IN NAT_HANDLE hNatHandle);

VOID NAT_Walk
(
    IN NAT_HANDLE hNatHandle,
    IN PF_NAT_WALK_CALL_BACK_FUNC pfFunc,
    IN HANDLE hUserHandle
);

VOID NAT_SetDbgFlag(IN NAT_HANDLE hNatHandle, IN UINT uiDbgFlag);

VOID NAT_ClrDbgFlag(IN NAT_HANDLE hNatHandle, IN UINT uiDbgFlag);

CHAR * NAT_GetStatusString(IN UCHAR ucType, IN UINT uiStatus);

#ifdef __cplusplus
}
#endif 

#endif 


