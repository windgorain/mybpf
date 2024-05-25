/******************************************************************************
* Copyright (C) LiXingang
* Author:      LiXingang  Version: 1.0  Date: 2013-4-8
* Description: 
* History:     
******************************************************************************/

#ifndef __NETINFO_UTL_H_
#define __NETINFO_UTL_H_
#include "utl/eth_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 

#define NETINFO_ADATER_NAME_MAX_LEN 63
#define NETINFO_ADATER_DES_MAX_LEN 127
#define NETINFO_ADPTER_MAX_IP_NUM  16

typedef struct
{
    CHAR szAdapterName[NETINFO_ADATER_NAME_MAX_LEN + 1];
    CHAR szDescription[NETINFO_ADATER_DES_MAX_LEN + 1];
    MAC_ADDR_S stMacAddr;
    UINT auiIpAddr[NETINFO_ADPTER_MAX_IP_NUM];
    UINT auiIpMask[NETINFO_ADPTER_MAX_IP_NUM];
    UINT uiGateWay;
    BOOL_T bDhcpEnabled;
    UINT uiDhcpServer;    
}NETINFO_ADAPTER_S;

typedef struct
{
    UINT uiAdapterNum;
    NETINFO_ADAPTER_S astAdapter[0];
}NETINFO_S;


NETINFO_S * NETINFO_GetAllInfo();
VOID NETINFO_Free(IN NETINFO_S *pstNetInfo);
BS_STATUS NETINFO_GetAdapterInfo(IN CHAR *pcAdapterName, OUT NETINFO_ADAPTER_S *pstAdapterInfo);

#ifdef __cplusplus
    }
#endif 

#endif 


