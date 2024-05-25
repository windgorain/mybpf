/*================================================================
*   Created by LiXingang: 2018.11.20
*   Description: 
*
================================================================*/
#ifndef _TIMER_UTL_H
#define _TIMER_UTL_H
#include "utl/nap_utl.h"
#include "utl/vclock_utl.h"
#include "utl/mutex_utl.h"
#ifdef __cplusplus
extern "C"
{
#endif


typedef struct {
    MUTEX_S lock;
    NAP_HANDLE node_pool;
    VCLOCK_INSTANCE_HANDLE vclock;
    int precision; 
}TIMER_UTL_CTRL_S;

BS_STATUS TimerUtl_Init(TIMER_UTL_CTRL_S *ctrl, int precision);
HANDLE TimerUtl_Create(TIMER_UTL_CTRL_S *ctrl, UINT ulTime, UINT flag, PF_TIME_OUT_FUNC pfFunc, USER_HANDLE_S *pstUserHandle);
BS_STATUS TimerUtl_Delete(TIMER_UTL_CTRL_S *ctrl, HANDLE timer);
BS_STATUS TimerUtl_GetInfo(TIMER_UTL_CTRL_S *ctrl, HANDLE timer, OUT TIMER_INFO_S *pstTimerInfo);
BS_STATUS TimerUtl_Pause(TIMER_UTL_CTRL_S *ctrl, HANDLE timer);
BS_STATUS TimerUtl_Resume(TIMER_UTL_CTRL_S *ctrl, HANDLE timer);
BS_STATUS TimerUtl_ReSetTime(TIMER_UTL_CTRL_S *ctrl, HANDLE timer, UINT ulTime);

#ifdef __cplusplus
}
#endif
#endif 
