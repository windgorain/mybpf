/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2007-8-18
* Description: 相比于VTimer, 只创建一个Timer定时向一个线程发送消息
*              所有MTimer在这个线程中使用vclock调度
* History:     
******************************************************************************/
#include "bs.h"

#include "utl/vclock_utl.h"
#include "utl/atomic_once.h"
#include "utl/msgque_utl.h"
#include "utl/event_utl.h"
#include "utl/timerfd_utl.h"
#include "utl/thread_utl.h"


#define _MTimer_DFT_TIME_PER_TICK 100  

#define _VTIMER_MTimer_EVENT 1
#define _MTimer_MSG_TYPE 1



typedef struct
{
    HANDLE  hClockId;
    THREAD_ID ulThreadID;
    int timer_fd;
}_MTimer_HEAD_S;


static _MTimer_HEAD_S g_stMTimerHead;

static UINT _mtimer_Ms2Tick(UINT ms)
{
    return (ms + _MTimer_DFT_TIME_PER_TICK - 1)/ _MTimer_DFT_TIME_PER_TICK;
}

static void _MTimer_Main(IN USER_HANDLE_S *pstUserHandle)
{
    int ret;
    UINT64 data;

    while (1) {
        ret = read(g_stMTimerHead.timer_fd, &data, sizeof(data));
        if (ret > 0) {
            VCLOCK_Step(g_stMTimerHead.hClockId);
        }
    }

    return;
}

static UINT _MTimer_GetAdjustTick()
{
    
    return 1;
}

static BS_STATUS _mtimer_InitOnce(void *ud)
{
    Mem_Zero(&g_stMTimerHead, sizeof(_MTimer_HEAD_S));

    if (0 == (g_stMTimerHead.hClockId = VCLOCK_CreateInstance(TRUE))) {
        RETURN(BS_ERR);
    }

    g_stMTimerHead.timer_fd = TimerFd_Create(_MTimer_DFT_TIME_PER_TICK, 0);
    if (g_stMTimerHead.timer_fd < 0) {
        VCLOCK_DeleteInstance(g_stMTimerHead.hClockId);
        g_stMTimerHead.hClockId = 0;
        RETURN(BS_ERR);
    }

    g_stMTimerHead.ulThreadID = THREAD_Create("MTimer", NULL, _MTimer_Main, NULL);
    if (THREAD_ID_INVALID == g_stMTimerHead.ulThreadID) {
        VCLOCK_DeleteInstance(g_stMTimerHead.hClockId);
        g_stMTimerHead.hClockId = 0;
        TimerFd_Close(g_stMTimerHead.timer_fd);
        g_stMTimerHead.timer_fd = -1;
        BS_WARNNING(("Can't crete MTimer thread!"));
        RETURN(BS_ERR);
    }

    return BS_OK;
}

static void _mtimer_Init()
{
    static ATOM_ONCE_S once = ATOM_ONCE_INIT_VALUE;
    AtomOnce_WaitDo(&once, _mtimer_InitOnce, NULL);
}

int MTimer_AddExt(MTIMER_S *timer, UINT first_time_ms,UINT time_ms, UINT flag,
        PF_TIME_OUT_FUNC pfFunc, USER_HANDLE_S *pstUserHandle)
{
    UINT first_tick;
    UINT tick;

    _mtimer_Init();

    if (g_stMTimerHead.timer_fd < 0) {
        BS_WARNNING(("MTimer not init"));
        RETURN(BS_NOT_INIT);
    }

    tick = _mtimer_Ms2Tick(time_ms);
    first_tick = _mtimer_Ms2Tick(first_time_ms) + _MTimer_GetAdjustTick();

    return VCLOCK_AddTimer(g_stMTimerHead.hClockId, &timer->vclock,
            first_tick, tick, flag, pfFunc, pstUserHandle);
}

int MTimer_Add(MTIMER_S *timer, UINT time_ms, UINT flag,
        PF_TIME_OUT_FUNC pfFunc, USER_HANDLE_S *pstUserHandle)
{
    return MTimer_AddExt(timer, time_ms, time_ms, flag, pfFunc, pstUserHandle);
}

int MTimer_Del(MTIMER_S *timer)
{
    return VCLOCK_DelTimer(g_stMTimerHead.hClockId, &timer->vclock);
}

BOOL_T MTimer_IsRunning(MTIMER_S *timer)
{
    return VCLOCK_IsRunning(&timer->vclock);
}

BS_STATUS MTimer_Pause(MTIMER_S *timer)
{
    return VCLOCK_Pause(g_stMTimerHead.hClockId, &timer->vclock);
}

BS_STATUS MTimer_Resume(MTIMER_S *timer)
{
    return VCLOCK_Resume(g_stMTimerHead.hClockId, &timer->vclock, _MTimer_GetAdjustTick());
}

BS_STATUS MTimer_GetInfo(MTIMER_S *timer, OUT TIMER_INFO_S *pstTimerInfo)
{
    BS_STATUS eRet;
    
    eRet = VCLOCK_GetInfo(g_stMTimerHead.hClockId, &timer->vclock, pstTimerInfo);
    pstTimerInfo->ulTime *= _MTimer_DFT_TIME_PER_TICK;

    return eRet;
}

BS_STATUS MTimer_RestartWithTime(MTIMER_S *timer, UINT ulTime)
{
    UINT ulTick;

    ulTick = (ulTime + _MTimer_DFT_TIME_PER_TICK - 1) / _MTimer_DFT_TIME_PER_TICK;

    return VCLOCK_RestartWithTick(g_stMTimerHead.hClockId, &timer->vclock, _MTimer_GetAdjustTick(), ulTick);
}


U32 MTimer_GetTimeLeft(MTIMER_S *timer)
{
    UINT ulResTick;
    ulResTick = VCLOCK_GetTickLeft(g_stMTimerHead.hClockId, &timer->vclock);
    return ulResTick * _MTimer_DFT_TIME_PER_TICK;
}


