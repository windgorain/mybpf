/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2012-8-19
* Description: 
* History:     
******************************************************************************/

#ifndef __TASK_MASTER_H_
#define __TASK_MASTER_H_

#include "utl/vclock_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 

typedef HANDLE TASK_MASTER_HANDLE;

typedef VOID (*PF_TASK_MASTER_FUNC)(void *ud);

TASK_MASTER_HANDLE TASK_Master_Create(IN UINT uiTime);

BS_STATUS TASK_Master_SetEvent
(
    IN TASK_MASTER_HANDLE hTaskMaster,
    IN UINT uiEventOffset, 
    IN PF_TASK_MASTER_FUNC pfFunc,
    IN USER_HANDLE_S *pstUserHandle
);

BS_STATUS TASK_Master_EventInput
(
    IN TASK_MASTER_HANDLE hTaskMaster,
    IN UINT uiEventOffset 
);

BS_STATUS TASK_Master_MsgInput
(
    IN TASK_MASTER_HANDLE hTaskMaster,
    IN PF_TASK_MASTER_FUNC pfFunc,
    IN void *ud
);

VCLOCK_HANDLE TASK_Master_AddTimer
(
    IN TASK_MASTER_HANDLE hTaskMaster,
    IN UINT uiTime,    
    IN UINT flag,
    IN PF_TIME_OUT_FUNC pfFunc,
    IN USER_HANDLE_S *pstUserHandle
);

VOID TASK_Master_DelTimer(IN TASK_MASTER_HANDLE hTaskMaster, IN VCLOCK_HANDLE hTimerHandle);

VOID TASK_Master_RefreshTimer(IN TASK_MASTER_HANDLE hTaskMaster, IN VCLOCK_HANDLE hTimerHandle);

#ifdef __cplusplus
    }
#endif 

#endif 


