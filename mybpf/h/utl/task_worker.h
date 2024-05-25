/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2012-8-17
* Description: 
* History:     
******************************************************************************/

#ifndef __TASK_WORKER_H_
#define __TASK_WORKER_H_

#include "utl/vclock_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 

#define TASK_WORKER_ID_AUTO 0xffffffff 

typedef HANDLE TASK_WORKER_HANDLE;

typedef VOID (*PF_TASK_WORKER_FUNC)(void *ud);

TASK_WORKER_HANDLE TASK_Worker_Create(char *name_prefix, UINT worker_num, UINT time);
BS_STATUS TASK_Worker_SetEvent
(
    IN TASK_WORKER_HANDLE hTaskWorker,
    IN UINT uiEventOffset, 
    IN PF_TASK_WORKER_FUNC pfFunc,
    IN USER_HANDLE_S *pstUserHandle
);
BS_STATUS TASK_Worker_EventInput
(
    IN TASK_WORKER_HANDLE hTaskWorker,
    IN UINT uiWokerId,  
    IN UINT uiEventOffset 
);
BS_STATUS TASK_Worker_MsgInput
(
    TASK_WORKER_HANDLE hTaskWorker,
    UINT uiWokerId,  
    PF_TASK_WORKER_FUNC pfFunc,
    void *ud
);

VCLOCK_HANDLE TASK_Worker_AddTimer
(
    IN TASK_WORKER_HANDLE hTaskWorker,
    IN UINT uiWokerId,
    IN UINT uiTime,    
    IN PF_TIME_OUT_FUNC pfFunc,
    IN USER_HANDLE_S *pstUserHandle
);

VOID TASK_Worker_DelTimer
(
    IN TASK_WORKER_HANDLE hTaskWorker,
    IN UINT uiWokerId,
    IN VCLOCK_HANDLE hTimerHandle
);


#ifdef __cplusplus
    }
#endif 

#endif 


