/*================================================================
*   Created by LiXingang: 2018.11.20
*   Description: 
*
================================================================*/
#ifndef _TIMER_COMMON_H
#define _TIMER_COMMON_H
#ifdef __cplusplus
extern "C"
{
#endif


#define TIME_ONE_SECOND   1000
#define TIME_ONE_MINUTE   (60*TIME_ONE_SECOND)
#define TIME_ONE_HOUR     (60*TIME_ONE_MINUTE)
#define TIME_ONE_DAY      (24*TIME_ONE_HOUR)
#define TIME_ONE_WEEK     (7*TIME_ONE_WEEK)
#define TIME_30_DAY       (30*TIME_ONE_DAY)

#define TIMER_FLAG_CYCLE 0x1  
#define TIMER_FLAG_PAUSE 0x2  

#define TIMER_IS_CYCLE(flag) ((flag) & TIMER_FLAG_CYCLE)
#define TIMER_IS_PAUSE(flag) ((flag) & TIMER_FLAG_PAUSE)

#define TIMER_SET_PAUSE(flag) ((flag) |= TIMER_FLAG_PAUSE)
#define TIMER_CLR_PAUSE(flag) ((flag) &= (~TIMER_FLAG_PAUSE))

typedef void (*PF_TIME_OUT_FUNC)(HANDLE timer, USER_HANDLE_S *ud);

typedef struct {
    UINT time_value;
    void *pfFunc;
    UINT flag;
}TIMER_INFO_S;

#ifdef __cplusplus
}
#endif
#endif 
