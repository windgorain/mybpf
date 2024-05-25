/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2007-8-19
* Description: 
* History:     
******************************************************************************/

#ifndef __MTimer_BS_H_
#define __MTimer_BS_H_

#include "utl/vclock_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 

typedef struct {
    VCLOCK_NODE_S vclock;
}MTIMER_S;

PLUG_API int MTimer_Add(MTIMER_S *timer, UINT time_ms, UINT flag,
        PF_TIME_OUT_FUNC pfFunc, USER_HANDLE_S *pstUserHandle);
PLUG_API int MTimer_AddExt(MTIMER_S *timer, UINT first_time_ms, UINT time_ms, UINT flag,
        PF_TIME_OUT_FUNC pfFunc, USER_HANDLE_S *pstUserHandle);     
PLUG_API int MTimer_Del(MTIMER_S *timer);
PLUG_API BOOL_T MTimer_IsRunning(MTIMER_S *timer);
PLUG_API BS_STATUS MTimer_Pause(MTIMER_S *timer);
PLUG_API BS_STATUS MTimer_Resume(MTIMER_S *timer);
PLUG_API BS_STATUS MTimer_GetInfo(MTIMER_S *timer, OUT TIMER_INFO_S *pstTimerInfo);


PLUG_API BS_STATUS MTimer_RestartWithTime(MTIMER_S *timer, UINT ulTime);


#ifdef __cplusplus
    }
#endif 

#endif 


