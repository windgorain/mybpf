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

VOID VNIC_Agent_Stop(IN VNIC_AGENT_HANDLE hVnicAgent);

BS_STATUS VNIC_Agent_Write(IN VNIC_AGENT_HANDLE hVnicAgent, IN MBUF_S *pstMbuf);

BS_STATUS VNIC_Agent_WriteData(IN VNIC_AGENT_HANDLE hVnicAgent, IN UCHAR *pucData, IN UINT uiDataLen);

extern HANDLE VNIC_Agent_GetUserData (IN VNIC_AGENT_HANDLE hVnicAgent);

extern BS_STATUS VNIC_Agent_SetUserData (IN VNIC_AGENT_HANDLE hVnicAgent, IN HANDLE hUserHandle);

extern BS_STATUS VNIC_Agent_Close (IN VNIC_AGENT_HANDLE hVnicAgent);

extern VOID VNIC_Agent_SetVnic(IN VNIC_AGENT_HANDLE hVnicAgent, IN VNIC_HANDLE hVnic);

extern VNIC_HANDLE VNIC_Agent_GetVnic(IN VNIC_AGENT_HANDLE hVnicAgent);

#ifdef __cplusplus
    }
#endif 

#endif 


