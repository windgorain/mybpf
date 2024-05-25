/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2014-1-23
* Description: 
* History:     
******************************************************************************/

#ifndef __KA_UTL_H_
#define __KA_UTL_H_

#include "utl/vclock_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 

struct tag_KA;

typedef VOID (*PF_KA_SEND_KEEP_ALIVE)(IN struct tag_KA *pstKa);
typedef VOID (*PF_KA_FAILED)(IN struct tag_KA *pstKa);

typedef struct
{
    USHORT usIdle;   
    USHORT usIntval; 
    USHORT usMaxProbeCount; 
    PF_KA_SEND_KEEP_ALIVE pfSendKeepAlive;
    PF_KA_FAILED pfKeepAliveFailed;
}KA_SET_S;

typedef struct
{
    VCLOCK_INSTANCE_HANDLE hVClockInstance;
    USHORT usProbeCount; 
    VCLOCK_NODE_S vclock;
}_KA_INNER_VAR_S;

typedef struct tag_KA
{
    KA_SET_S stSet; 
    USER_HANDLE_S stUserHandle;
    _KA_INNER_VAR_S stInnerVar; 
}KA_S;


BS_STATUS KA_Start
(
    IN VCLOCK_INSTANCE_HANDLE hVClockInstance,
    IN KA_S *pstKaConfig
);
VOID KA_TrigerKeepAlive(IN KA_S *pstKaConfig);
VOID KA_Stop(IN KA_S *pstKaConfig);
VOID KA_Reset(IN KA_S *pstKaConfig);

#ifdef __cplusplus
    }
#endif 

#endif 


