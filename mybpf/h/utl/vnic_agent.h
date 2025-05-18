/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2007-8-27
* Description: 
* History:     
******************************************************************************/

#ifndef __VNIC_AGENT_H_
#define __VNIC_AGENT_H_

#ifdef __cplusplus
    extern "C" {
#endif 

#include "utl/tun_utl.h"
#include "utl/vnic_lib.h"
#include "utl/mbuf_utl.h"

typedef HANDLE VNIC_AGENT_HANDLE;

typedef BS_STATUS (*VNIC_AGENT_CB_FUNC)(IN VNIC_AGENT_HANDLE hVnicAgent, IN MBUF_S *pstMbuf, IN HANDLE hUserHandle);

extern VNIC_AGENT_HANDLE VNIC_Agent_Create();

extern BS_STATUS VNIC_Agent_Start
(
    IN VNIC_AGENT_HANDLE hVnicAgent,
    IN VNIC_AGENT_CB_FUNC pfFunc, 
    IN HANDLE hUserHandle
);

void VNIC_Agent_Stop(VNIC_AGENT_HANDLE hVnicAgent);
BS_STATUS VNIC_Agent_Write(VNIC_AGENT_HANDLE hVnicAgent, MBUF_S *pstMbuf);
BS_STATUS VNIC_Agent_WriteData(VNIC_AGENT_HANDLE hVnicAgent, UCHAR *pucData, UINT uiDataLen);
HANDLE VNIC_Agent_GetUserData(VNIC_AGENT_HANDLE hVnicAgent);
BS_STATUS VNIC_Agent_SetUserData (VNIC_AGENT_HANDLE hVnicAgent, HANDLE hUserHandle);
BS_STATUS VNIC_Agent_Close(VNIC_AGENT_HANDLE hVnicAgent);
void VNIC_Agent_SetVnic(VNIC_AGENT_HANDLE hVnicAgent, TUN_FD hVnic);
TUN_FD VNIC_Agent_GetVnic(VNIC_AGENT_HANDLE hVnicAgent);

#ifdef __cplusplus
    }
#endif 

#endif 


