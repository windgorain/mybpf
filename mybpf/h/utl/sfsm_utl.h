/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2014-7-15
* Description: Simple fsm
* History:     
******************************************************************************/

#ifndef __SFSM_UTL_H_
#define __SFSM_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

#define SFSM_STATE_ANY 0xffffffff

typedef VOID (*PF_SFSM_EVENT_HANDLER)(IN UINT uiEvent, IN VOID *pUserContext);

typedef struct
{
    UINT uiState;
    UINT uiEvent;
    PF_SFSM_EVENT_HANDLER pfEventHandler;
}SFSM_EVENT_S;

typedef struct
{
    SFSM_EVENT_S *pstEventTbl;
    UINT uiEventTblSize;
    UINT uiState;
}SFSM_S;

VOID SFSM_EventHandle(IN SFSM_S *pstFsm, IN UINT uiEvent, IN VOID *pUserContext);
static inline VOID SFSM_SetState(IN SFSM_S *pstFsm, IN UINT uiState)
{
    pstFsm->uiState = uiState;
}
static inline UINT SFSM_GetState(IN SFSM_S *pstFsm)
{
    return pstFsm->uiState;
}

#ifdef __cplusplus
    }
#endif 

#endif 


