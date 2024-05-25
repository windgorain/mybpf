/*================================================================
*   Created by LiXingang: 2018.11.20
*   Description: 
*
================================================================*/
#include "bs.h"
#include "utl/timerfd_utl.h"
#include "utl/socket_utl.h"

#if (defined IN_WINDOWS) || (defined IN_MAC)

static void _win_timerfd_Main(IN USER_HANDLE_S *pstUserHandle)
{
    int fd = HANDLE_UINT(pstUserHandle->ahUserHandle[0]);
    int interval = HANDLE_UINT(pstUserHandle->ahUserHandle[1]);

    for (;;) {
        Sleep(interval);
        Socket_Write(fd, "0", 1, 0);
    }
}

static int _os_timerfd_create_ext(int first_ts,int interval, UINT flag)
{
    int fds[2];
    USER_HANDLE_S user_data;

    if (BS_OK != Socket_Pair(SOCK_STREAM, fds)) {
        return -1;
    }

    user_data.ahUserHandle[0] = UINT_HANDLE(fds[1]);
    user_data.ahUserHandle[1] = UINT_HANDLE(interval);
    user_data.ahUserHandle[2] = UINT_HANDLE(first_ts);

    THREAD_Create("timer_fd", NULL, _win_timerfd_Main, &user_data);

    return fds[0];
}

static void _os_timerfd_close(int fd)
{
    Socket_Close(fd);
}

static void _os_timerfd_getime(int fd,int* val_sec,int* val_nsec,int* interval_sec,int* interval_nsec){
    return;
}
#endif

#ifdef IN_LINUX
#include <sys/timerfd.h>
static int _os_timerfd_create_ext(int first_ts,int interval, UINT flag)
{
    UINT os_flag = 0;
    int timer_fd;
    struct itimerspec new_value;
    int first_sec=first_ts/1000;
    int first_nsec=(first_ts % 1000) * 1000000;
    int sec = interval / 1000;
    int nsec = (interval % 1000) * 1000000;

    new_value.it_value.tv_sec = first_sec;
    new_value.it_value.tv_nsec = first_nsec;
    new_value.it_interval.tv_sec = sec;
    new_value.it_interval.tv_nsec = nsec;

    if (flag & TIMER_FD_FLAG_NOBLOCK) {
        os_flag |= TFD_NONBLOCK;
    }

    timer_fd = timerfd_create(CLOCK_MONOTONIC, os_flag | TFD_CLOEXEC);
    if (timer_fd < 0) {
        return -1;
    }

    timerfd_settime(timer_fd, 0, &new_value, NULL);

    return timer_fd;
}

static void _os_timerfd_close(int fd)
{
    close(fd);
}

static void _os_timerfd_getime(int fd,int* val_sec,int* val_nsec,int* interval_sec,int* interval_nsec){
    struct itimerspec curr_value;
    int ret=timerfd_gettime(fd,&curr_value);
    if(ret<0){
        return;
    }
    *val_sec=curr_value.it_value.tv_sec;
    *val_nsec=curr_value.it_value.tv_nsec;
    *interval_sec=curr_value.it_interval.tv_sec;
    *interval_nsec=curr_value.it_interval.tv_nsec;
}
#endif

int TimerFd_Create(int interval, UINT flag)
{
    return _os_timerfd_create_ext(interval, interval, flag);
}

int TimerFd_Create_Ext(int first_ts,int interval, UINT flag)
{
    return _os_timerfd_create_ext(first_ts,interval, flag);
}

void TimerFd_GetTime(int fd,int* val_sec,int* val_nsec,int* interval_sec,int* interval_nsec)
{
    return _os_timerfd_getime(fd,val_sec,val_nsec,interval_sec,interval_nsec);
}

void TimerFd_Close(int fd)
{
    _os_timerfd_close(fd);
}
