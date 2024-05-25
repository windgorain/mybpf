/*================================================================
*   Created by LiXingang: 2018.11.20
*   Description: 
*
================================================================*/
#ifndef _TIMERFD_UTL_H
#define _TIMERFD_UTL_H
#ifdef __cplusplus
extern "C"
{
#endif

#define TIMER_FD_FLAG_NOBLOCK 0x1

int TimerFd_Create(int interval, UINT flag);
int TimerFd_Create_Ext(int first_ts,int interval, UINT flag);
void TimerFd_Close(int fd);
void TimerFd_GetTime(int fd,int* val_sec,int* val_nsec,int* interval_sec,int* interval_nsec);

#ifdef __cplusplus
}
#endif
#endif 
