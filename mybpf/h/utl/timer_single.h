/*================================================================
*   Created by LiXingang: 2018.11.20
*   Description: 
*
================================================================*/
#ifndef _TIMER_SINGLE_H
#define _TIMER_SINGLE_H
#ifdef __cplusplus
extern "C"
{
#endif

HANDLE TimerSingle_Create(UINT time, UINT flag, PF_TIME_OUT_FUNC func, USER_HANDLE_S *user_data);
BS_STATUS TimerSingle_Delete(HANDLE timer);
BS_STATUS TimerSingle_GetInfo(HANDLE timer, OUT TIMER_INFO_S *info);
BS_STATUS TimerSingle_Pause(HANDLE timer);
BS_STATUS TimerSingle_Resume(HANDLE timer);
BS_STATUS TimerSingle_ReSetTime(HANDLE timer, UINT ulTime);

#ifdef __cplusplus
}
#endif
#endif 
