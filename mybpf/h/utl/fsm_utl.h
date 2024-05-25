/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2014-5-22
* Description: 
* History:     
******************************************************************************/

#ifndef __FSM_UTL_H_
#define __FSM_UTL_H_

#include "utl/que_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 

#define FSM_DBG_FLAG_EVENT         0x1
#define FSM_DBG_FLAG_STATE_CHANGE  0x2
#define FSM_DBG_FLAG_ERR           0x4

#define FSM_DBG_FLAG_ALL           0xffffffff

#define FSM_INVALID_STATE 0xffffffff
#define FSM_INVALID_EVENT 0xffffffff
#define FSM_STATE_NO_CHANGE 0xfffffffe
#define FSM_STATE_ANY       0xfffffffd


#define FSM_STATE_NO_CHANGE_STRING "@"  
#define FSM_STATE_ANY_STRING "*" 

typedef VOID* FSM_SWITCH_TBL;


typedef struct
{
    CHAR *pcStateName;
    UINT uiState;
}FSM_STATE_MAP_S;


typedef struct
{
    CHAR *pcEventName;
    UINT uiEvent;
}FSM_EVENT_MAP_S;

typedef struct
{
    FSM_SWITCH_TBL hSwitchTbl;

    QUE_HANDLE hEventQue;       

    UINT uiCurState;
    UINT uiOldState;

    UINT uiDbgFlag;

    HANDLE hUserPrivateData;
}FSM_S;


typedef BS_STATUS (*PF_FSM_EVENT_FUNC)(IN FSM_S *pstFsm, IN UINT uiEvent);


typedef VOID (*PF_FSM_STATE_LISTEN)(IN FSM_S *pstFsm, IN UINT uiOldState, IN UINT uiNewState, IN USER_HANDLE_S *pstUserHandle);


typedef struct
{
    CHAR *pcState;  
    CHAR *pcEvent;  
    CHAR *pcNextState;  
    PF_FSM_EVENT_FUNC pfEventFunc;
}FSM_SWITCH_MAP_S;

FSM_SWITCH_TBL FSM_CreateSwitchTbl
(
    IN FSM_STATE_MAP_S *pstStateMap,
    IN UINT uiStateMapCount,
    IN FSM_EVENT_MAP_S *pstEventMap,
    IN UINT uiEventMapCount,
    IN FSM_SWITCH_MAP_S *pstSwitchMap,
    IN UINT uiSwitchMapCount
);

#define FSM_CREATE_SWITCH_TBL(astStateMap, astEventMap, astSwitchMap) \
    FSM_CreateSwitchTbl(astStateMap, sizeof(astStateMap)/sizeof(FSM_STATE_MAP_S), \
                        astEventMap, sizeof(astEventMap)/sizeof(FSM_EVENT_MAP_S), \
                        astSwitchMap, sizeof(astSwitchMap)/sizeof(FSM_SWITCH_MAP_S))

VOID FSM_DestorySwitchTbl(IN FSM_SWITCH_TBL hSwitchTbl);
BS_STATUS FSM_RegStateListener(IN FSM_SWITCH_TBL hSwitchTbl, IN PF_FSM_STATE_LISTEN pfListenFunc, IN USER_HANDLE_S *pstUserHandle);


VOID FSM_Init(INOUT FSM_S *pstFsm, IN FSM_SWITCH_TBL hSwitchTbl);
VOID FSM_Finit(IN FSM_S *pstFsm);
BS_STATUS FSM_InitEventQue(IN FSM_S *pstFsm, IN UINT uiCapacity);
VOID FSM_InitState(IN FSM_S *pstFsm, IN UINT uiInitState);
VOID FSM_SetState(IN FSM_S *pstFsm, IN UINT uiState);
BS_STATUS FSM_PushEvent(IN FSM_S *pstFsm, IN UINT uiEvent);
UINT FSM_PopEvent(IN FSM_S *pstFsm);
BS_STATUS FSM_EventHandle(IN FSM_S *pstFsm, IN UINT uiEvent);

CHAR * FSM_GetStateName(IN FSM_SWITCH_TBL hSwitchTbl, IN UINT uiState);
CHAR * FSM_GetEventName(IN FSM_SWITCH_TBL hSwitchTbl, IN UINT uiEvent);

static inline UINT FSM_GetCurrentState(IN FSM_S *pstFsm)
{
    return pstFsm->uiCurState;
}

static inline VOID FSM_SetPrivateData(IN FSM_S *pstFsm, IN HANDLE hPrivateData)
{
    pstFsm->hUserPrivateData = hPrivateData;
}

static inline HANDLE FSM_GetPrivateData(IN FSM_S *pstFsm)
{
    return pstFsm->hUserPrivateData;
}

static inline VOID FSM_SetDbgFlag(IN FSM_S *pstFsm, IN UINT uiDbgFlag)
{
    pstFsm->uiDbgFlag |= uiDbgFlag;
}

static inline VOID FSM_ClrDbgFlag(IN FSM_S *pstFsm, IN UINT uiDbgFlag)
{
    pstFsm->uiDbgFlag &= (~uiDbgFlag);
}

#ifdef __cplusplus
    }
#endif 

#endif 


