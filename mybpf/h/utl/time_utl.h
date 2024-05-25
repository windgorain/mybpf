/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2008-2-28
* Description: 
* History:     
******************************************************************************/

#ifndef __TIME_UTL_H_
#define __TIME_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

#include <time.h>

#define TM_STRING_TIME_LEN 47

#define DAYS_2_SECONDS(days) ((days) * 24 * 60 * 60)


static inline time_t TM_NowInSec(void)
{
	return time(0);
}


ULONG TM_SecondsFromInit(void);

UINT64 TM_MsFromInit(void);

UINT64 TM_UsFromInit(void);

UINT64 TM_NsFromInit(void);
U64 TM_SecondsFromUTC(void);


VOID TM_Utc2String(IN time_t ulUtcTime, OUT CHAR *szStringTime);

VOID TM_Utc2Tm(IN time_t ulUtcTime, OUT struct tm *pstTm);

BS_STATUS TM_Tm2Utc(IN struct tm *pstTm, OUT time_t *pulUtcTime);


char * TM_Tm2String(struct tm *pstTm, OUT char *szStringTime);

BS_STATUS TM_String2Utc(IN CHAR *pszStringTime, OUT time_t *pulUtcTime);
BS_STATUS TM_String2Tm(IN CHAR *pszStringTime, OUT struct tm *pstTm);


char * TM_Utc2Acstime(time_t seconds, OUT char *string);


char * TM_Utc2SimpleString(time_t ulUtcTime, OUT CHAR *szTimeString);

char * TM_Tm2SimpleString(struct tm *pstTm, OUT CHAR *szStringTime);

BS_STATUS TM_Utc2Gmt(IN time_t stTimeSrc, OUT CHAR *szDataStr);
time_t TM_Gmt2Utc(IN CHAR *pucValue, IN ULONG ulLen);

char * TM_GetTimeString(UINT input_time, OUT char *string, int size);


extern unsigned long TM_HZ; 
extern unsigned long TM_MS_HZ; 

unsigned long TM_GetTickPerSec(void);

static inline unsigned long TM_GetTick(void)
{
    unsigned long now_tick;
    struct tms buf;
    now_tick = times(&buf);
    return now_tick * 10;
}

#if 1 
static inline long TM_GetClock(void) {
        return clock();
}

static inline long TM_GetClockPerSec(void) {
        return CLOCKS_PER_SEC;
}
#endif


#ifdef __cplusplus
    }
#endif 

#endif 


