/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2008-9-30
* Description: 
* History:     
******************************************************************************/

#ifndef __ARP_UTL_H_
#define __ARP_UTL_H_

#include "utl/mbuf_utl.h"
#include "utl/eth_utl.h"
#include "utl/sif_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 

#define ARP_HARDWARE_TYPE_ETH 0x0001

#define ARP_REQUEST   0x0001
#define ARP_REPLY     0x0002

typedef HANDLE ARP_HANDLE;

typedef enum
{
    ARP_TYPE_STATIC = 0,
    ARP_TYPE_DYNAMIC    
}ARP_TYPE_E;

#pragma pack(1)
typedef struct
{
    
    UCHAR  aucDMac[6];          
    UCHAR  aucSMac[6];          
    USHORT usProto;             

    
    USHORT usHardWareType;      
    USHORT usProtoType;         
    UCHAR  ucHardAddrLen;       
    UCHAR  ucProtoAddrLen;      
    USHORT usOperation;         
    UCHAR  aucSenderHA[6];      
    UINT  ulSenderIp;          
    UCHAR  aucDstHA[6];         
    UINT  ulDstIp;             
}ETH_ARP_PACKET_S;

typedef struct
{
    USHORT usHardWareType;      
    USHORT usProtoType;         
    UCHAR  ucHardAddrLen;       
    UCHAR  ucProtoAddrLen;      
    USHORT usOperation;         
    UCHAR  aucSenderHA[6];      
    UINT  ulSenderIp;          
    UCHAR  aucDstHA[6];         
    UINT  ulDstIp;             
}ARP_HEADER_S;

#pragma pack()

typedef struct
{
    UINT uiIp;  
    MAC_ADDR_S stMac;
    ARP_TYPE_E eType;
}ARP_NODE_S;

typedef BS_STATUS (*PF_ARP_SEND_PACKET_FUNC)(IN MBUF_S *pstMbuf, IN USER_HANDLE_S *pstUserHandle);
typedef BOOL_T (*PF_ARP_IS_HOST_IP_FUNC)(IF_INDEX uiIfIndex, ARP_HEADER_S *pstArpHeader, USER_HANDLE_S *pstUserHandle);
typedef UINT (*PF_ARP_GET_HOST_IP_FUNC)(IN IF_INDEX ifIndex, IN UINT uiDstIP, IN USER_HANDLE_S *pstUserHandle);  
typedef int (*PF_ARP_WALK_CALL_BACK_FUNC)(IN ARP_NODE_S *pstArpNode, IN HANDLE hUserHandle);


BOOL_T ARP_IsArpPacket(IN MBUF_S *pstMbuf);

MBUF_S * ARP_BuildPacket
(
    IN UINT uiSrcIp,
    IN UINT uiDstIp, 
    IN UCHAR *pucSrcMac,
    IN UCHAR *pucDstMac,  
    IN USHORT usArpType
);

MBUF_S * ARP_BuildPacketWithEthHeader
(
    IN UINT uiSrcIp,  
    IN UINT uiDstIp, 
    IN UCHAR *pucSrcMac,
    IN UCHAR *pucDstMac,
    IN USHORT usArpType
);

ARP_HANDLE ARP_CreateInstance
(
    IN UINT uiTimeOutTick,  
    IN BOOL_T bIsCreateSem
);

VOID ARP_DestoryInstance(IN ARP_HANDLE hArpInstance);
VOID ARP_SetSendPacketFunc
(
    IN ARP_HANDLE hArpInstance,
    IN PF_ARP_SEND_PACKET_FUNC pfSendPacketFunc,
    IN USER_HANDLE_S *pstUserHandle
);
VOID ARP_SetIsHostIpFunc
(
    IN ARP_HANDLE hArpInstance,
    IN PF_ARP_IS_HOST_IP_FUNC pfIsHostIp,
    IN USER_HANDLE_S *pstUserHandle
);
VOID ARP_SetGetHostIpFunc
(
    IN ARP_HANDLE hArpInstance,
    IN PF_ARP_GET_HOST_IP_FUNC pfGetHostIp,
    IN USER_HANDLE_S *pstUserHandle
);
BS_STATUS ARP_SetHostMac(IN ARP_HANDLE hArpInstance, IN MAC_ADDR_S *pstHostMac);
BS_STATUS ARP_AddStaticARP
(
    IN ARP_HANDLE hArpInstance,
    IN UINT uiIP, 
    IN MAC_ADDR_S *pstMac
);

BS_STATUS ARP_GetMacByIp
(
    IN ARP_HANDLE hArpInstance,
    IN UINT uiIfIndex,
    IN UINT ulIpToResolve ,
    IN MBUF_S *pstMbuf,
    OUT MAC_ADDR_S *pstMacAddr
);

VOID ARP_Walk
(
    IN ARP_HANDLE hArpInstance,
    IN PF_ARP_WALK_CALL_BACK_FUNC pfFunc,
    IN HANDLE hUserHandle
);

BS_STATUS ARP_PacketInput(IN ARP_HANDLE hArpInstance, IN MBUF_S *pstArpPacket);

VOID ARP_TimerStep(IN ARP_HANDLE hArpInstance);

#ifdef __cplusplus
    }
#endif 

#endif 


