/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2013-4-25
* Description: 
* History:     
******************************************************************************/

#ifndef __ARP_CHEAT_UTL_H_
#define __ARP_CHEAT_UTL_H_

#include "utl/eth_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 

typedef HANDLE ARP_CHEAT_HANDLE;

ARP_CHEAT_HANDLE ARPCheat_Create();
VOID ARPCheat_SetPcapIndex(IN ARP_CHEAT_HANDLE hArpCheat, IN UINT uiPcapIndex);
UINT ARPCheat_GetPcapIndex(IN ARP_CHEAT_HANDLE hArpCheat);
VOID ARPCheat_SetCheatIP(IN ARP_CHEAT_HANDLE hArpCheat, IN UINT uiCheatIP);
UINT ARPCheat_GetCheatIP(IN ARP_CHEAT_HANDLE hArpCheat);
VOID ARPCheat_SetCheatMAC(IN ARP_CHEAT_HANDLE hArpCheat, IN MAC_ADDR_S *pstMacAddr);
MAC_ADDR_S * ARPCheat_GetCheatMAC(IN ARP_CHEAT_HANDLE hArpCheat);
BS_STATUS ARPCheat_SendCheatPkt(IN ARP_CHEAT_HANDLE hArpCheat);

#ifdef __cplusplus
    }
#endif 

#endif 




