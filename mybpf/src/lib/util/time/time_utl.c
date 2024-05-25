/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2008-2-28
* Description: 性能: rdtsc > gettimeofday > clock_gettime > times
* History:     
******************************************************************************/


#define RETCODE_FILE_NUM RETCODE_FILE_NUM_TIME

#include "bs.h"

#include "utl/time_utl.h"
#include "utl/txt_utl.h"

#define HTTP_TIME_REMAIN_MAX_LEN  18
#define HTTP_TIME_ISOC_REMAIN_LEN 14
#define HTTP_HEAD_FIELD_SPLIT_CHAR         ':'                     


typedef enum tagHTTP_TimeStandard
{
    NORULE = 0,
    RFC822,   
    RFC850,   
    ISOC,     
} HTTP_TIME_STANDARD_E;


static const char *g_aucTimeWeekDay[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
static const char *g_aucTimeMonth[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
static UCHAR g_aucDaysOfMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
unsigned long TM_HZ = 0;   
unsigned long TM_MS_HZ = 0; 

#ifdef IN_WINDOWS
static inline UINT64 _tm_os_GetNsFromInit(void)
{
    return (UINT64)GetTickCount();
}
#endif

#ifdef IN_UNIXLIKE
static inline UINT64 _tm_os_GetNsFromInit(void)
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    return ((UINT64)ts.tv_sec * 1000000000 + (UINT64)ts.tv_nsec);
}
#endif


VOID TM_Utc2String(IN time_t ulUtcTime, OUT CHAR *szStringTime)
{
    struct tm stTm;

    TM_Utc2Tm(ulUtcTime, &stTm);
    TM_Tm2String(&stTm, szStringTime);
}

BS_STATUS TM_String2Utc(IN CHAR *pszStringTime, OUT time_t *pulUtcTime)
{
    struct tm stTm;
    BS_STATUS eRet;

    if (BS_OK != (eRet = TM_String2Tm(pszStringTime, &stTm)))
    {
        return eRet;
    }

    if (BS_OK != (eRet = TM_Tm2Utc(&stTm, pulUtcTime)))
    {
        return eRet;
    }

    return BS_OK;
}


VOID TM_Utc2Tm(IN time_t ulUtcTime, OUT struct tm *pstTm)
{
#ifdef IN_WINDOWS
    {
         localtime_s(pstTm, &ulUtcTime);
    }
#else
    {
        localtime_r(&ulUtcTime, pstTm);
    }
#endif
}


BS_STATUS TM_Tm2Utc(IN struct tm *pstTm, OUT time_t *pulUtcTime)
{
    time_t lTime;

    lTime = mktime(pstTm);
    if (-1 == lTime)
    {
        RETURN(BS_ERR);
    }

    *pulUtcTime = lTime;

    return BS_OK;
}


BS_STATUS TM_String2Tm(IN CHAR *pszStringTime, OUT struct tm *pstTm)
{
    UINT i;
    CHAR szTmp[8];
    BS_STATUS eRet;

    if (strlen(pszStringTime) < TM_STRING_TIME_LEN)
    {
        RETURN(BS_BAD_PARA);
    }
    
    
    for (i=0; i<sizeof(g_aucTimeWeekDay)/sizeof(CHAR *); i++)
    {
        if (memcmp(g_aucTimeWeekDay[i], pszStringTime, 3) == 0)
        {
            pstTm->tm_wday = i;
            break;
        }
    }

    if (i >= sizeof(g_aucTimeWeekDay)/sizeof(CHAR *))
    {
        RETURN(BS_NOT_SUPPORT);
    }

    
    for (i=0; i<sizeof(g_aucTimeMonth)/sizeof(CHAR*); i++)
    {
        if (memcmp(g_aucTimeMonth[i], pszStringTime + 8, 3) == 0)
        {
            pstTm->tm_mon = i;
            break;
        }
    }

    if (i >= sizeof(g_aucTimeMonth)/sizeof(CHAR *))
    {
        RETURN(BS_NOT_SUPPORT);
    }

    
    memcpy(szTmp, pszStringTime + 5, 2);
    szTmp[2] = '\0';

    eRet = TXT_Atoui(szTmp, &i);
    if (BS_OK != eRet)
    {
        return eRet;
    }

    if ((i < 1) || (i > 31))
    {
        RETURN(BS_NOT_SUPPORT);
    }

    pstTm->tm_mday = i;

    
    memcpy(szTmp, pszStringTime + 12, 4);
    szTmp[4] = '\0';

    eRet = TXT_Atoui(szTmp, &i);
    if (BS_OK != eRet)
    {
        return eRet;
    }

    if (i < 1900)
    {
        RETURN(BS_NOT_SUPPORT);
    }

    pstTm->tm_year = i - 1900;

    
    pstTm->tm_yday = 0;
    for (i=0; i<(UINT)pstTm->tm_mon; i++)
    {
        pstTm->tm_yday += g_aucDaysOfMonth[i];
    }

    if (pstTm->tm_mon > 2)
    {
        
        pstTm->tm_yday += (pstTm->tm_year % 4 == 0) ? 1 : 0;
    }

    pstTm->tm_yday += pstTm->tm_mday;


    
    memcpy(szTmp, pszStringTime + 17, 2);
    szTmp[2] = '\0';

    eRet = TXT_Atoui(szTmp, &i);
    if (BS_OK != eRet)
    {
        return eRet;
    }

    if (i > 23)
    {
        RETURN(BS_NOT_SUPPORT);
    }

    pstTm->tm_hour = i;    

    
    
    memcpy(szTmp, pszStringTime + 20, 2);
    szTmp[2] = '\0';

    eRet = TXT_Atoui(szTmp, &i);
    if (BS_OK != eRet)
    {
        return eRet;
    }

    if (i > 59)
    {
        RETURN(BS_NOT_SUPPORT);
    }

    pstTm->tm_min = i;    

    
    
    memcpy(szTmp, pszStringTime + 23, 2);
    szTmp[2] = '\0';

    eRet = TXT_Atoui(szTmp, &i);
    if (BS_OK != eRet)
    {
        return eRet;
    }

    if (i > 59)
    {
        RETURN(BS_NOT_SUPPORT);
    }

    pstTm->tm_sec = i;

    pstTm->tm_isdst = 0;

    return BS_OK;
}


char * TM_Tm2String(IN struct tm *pstTm, OUT CHAR *szStringTime)
{
    sprintf(szStringTime, "%s, %02d %s %04d %02d:%02d:%02d",
        g_aucTimeWeekDay[pstTm->tm_wday], pstTm->tm_mday, 
        g_aucTimeMonth[pstTm->tm_mon], pstTm->tm_year + 1900,
        pstTm->tm_hour, pstTm->tm_min, pstTm->tm_sec);

    return szStringTime;
}


char * TM_Utc2Acstime(time_t seconds, OUT char *string)
{
    struct tm stTm;
    TM_Utc2Tm(seconds, &stTm);
    asctime_r(&stTm, string);
    string[strlen(string)-1] = '\0'; 
    return string;
}


char * TM_Utc2SimpleString(time_t ulUtcTime, OUT CHAR *szTimeString)
{
    struct tm stTm;

    TM_Utc2Tm(ulUtcTime, &stTm);
    return TM_Tm2SimpleString(&stTm, szTimeString);
}


char * TM_Tm2SimpleString(struct tm *pstTm, OUT CHAR *szStringTime)
{
    sprintf(szStringTime, "%04d-%02d-%02d %02d:%02d:%02d",
         pstTm->tm_year + 1900, pstTm->tm_mon + 1, pstTm->tm_mday, 
        pstTm->tm_hour, pstTm->tm_min, pstTm->tm_sec);

    return szStringTime;
}


BS_STATUS TM_Utc2Gmt(IN time_t stTimeSrc, OUT CHAR *szDataStr)
{    
    LONG   lYday;
    ULONG  ulTime, ulSec, ulMin, ulHour, ulMday, ulMon, ulYear, ulWday, ulDays, ulLeap;
    ULONG  ulTemp;
    

    

    ulTime =  stTimeSrc;

    ulDays = ulTime / 86400;

    

    ulWday = (4 + ulDays) % 7;

    ulTime %= 86400;
    ulHour = ulTime / 3600;
    ulTime %= 3600;
    ulMin = ulTime / 60;
    ulSec = ulTime % 60;

    

    
    ulDays = (ulDays - (31 + 28)) + 719527;

    

    ulYear = ((ulDays + 2) * 400 )/ (((365 * 400) + (100 - 4)) + 1);

    ulTemp = (365 * ulYear) + (ulYear / 4);
    ulTemp = ulTemp - (ulYear / 100);
    ulTemp = ulTemp + (ulYear / 400);
    lYday = (LONG)ulDays - (LONG)ulTemp;

    if (lYday < 0) {
        if((0 == (ulYear % 4)) && (((ulYear % 100) != 0) || ((ulYear % 400) == 0)))
        {
            ulLeap = 1;
        }
        else
        {
            ulLeap = 0;
        }
        lYday = (365 + (LONG)ulLeap) + lYday;
        ulYear--;
    }

    

    ulMon = (((ULONG)lYday + 31) * 10) / 306;

    

    ulMday = ((ULONG)lYday - (((367 * ulMon) / 12) - 30)) + 1;

    if (lYday >= 306) {

        ulYear++;
        ulMon -= 10;

        

    } else {

        ulMon += 2;

        
    }    

    if( ( ulWday >= 7 ) || ( ulMon > 12 ) || ( ulMon < 1 ) )
    {
        return BS_ERR;
    }

    
    (VOID)snprintf( szDataStr, TM_STRING_TIME_LEN + 1, "%3s, %02lu %3s %lu %02lu:%02lu:%02lu GMT",
                    g_aucTimeWeekDay[ulWday],
                    ulMday,
                    g_aucTimeMonth[ulMon - 1],
                    ulYear,
                    ulHour,
                    ulMin,
                    ulSec );
    
    return BS_OK;
}

time_t TM_Gmt2Utc(IN CHAR *pcValue, IN ULONG ulLen)
{
    UCHAR *pucPos;
    UCHAR *pucEnd;
    INT iMonth;
    UINT uiDay;
    UINT uiYear;
    UINT uiHour;
    UINT uiMin;
    UINT uiSec;
    ULONG ulTime;
    HTTP_TIME_STANDARD_E enFmt;
    UCHAR *pucValue = (UCHAR*)pcValue;

    
    if((NULL == pucValue)||(0 == ulLen))
    {
        return 0;
    }

    enFmt = NORULE;
    uiDay = 0;
    uiYear = 0;
    uiHour = 0;
    uiMin = 0;
    uiSec = 0;
    pucPos = pucValue;
    pucEnd = pucValue + ulLen;

    
    for (pucPos = pucValue; pucPos < pucEnd; pucPos++) 
    {
        if (',' == *pucPos) 
        {
            break;
        }

        if (' '==*pucPos) 
        {
            enFmt = ISOC;
            break;
        }
    }

    
    for (pucPos++; pucPos < pucEnd; pucPos++)
    {
        if (' ' != *pucPos) 
        {
            break;
        }
    }

    
    if (HTTP_TIME_REMAIN_MAX_LEN > (pucEnd - pucPos)) 
    {
        return 0;
    }

    if (ISOC != enFmt) 
    {
        if (('0' > *pucPos) || 
            ('9' < *pucPos) || 
            ('0' > *(pucPos + 1)) || 
            ('9' < *(pucPos + 1))) 
        {
            return 0;
        }
        
        uiDay = (((*pucPos - '0') * 10) + (*(pucPos + 1))) - '0';
        
        pucPos += 2; 
        if (' ' == *pucPos) 
        {
            if (HTTP_TIME_REMAIN_MAX_LEN > (pucEnd - pucPos)) 
            {
                return 0;
            }
            
            enFmt = RFC822;

        } 
        else if ('-' == *pucPos) 
        {
            
            enFmt = RFC850;
        } 
        else 
        {
            return 0;
        }

        pucPos++;
    }

    
    switch (*pucPos) 
    {

        case 'J':
        {
            iMonth = (*(pucPos + 1) == 'a' )? 0 : ((*(pucPos + 2) == 'n' )? 5 : 6);   
            break;
        }

        case 'F':
        {
            iMonth = 1;
            break;
        }

        case 'M':
        {
            iMonth = (*(pucPos + 2) == 'r') ? 2 : 4;  
            break;
        }

        case 'A':
        {
            iMonth = (*(pucPos + 1) == 'p') ? 3 : 7; 
            break;
        }
        case 'S':
        {
            iMonth = 8;
            break;
        }
        case 'O':
        {
            iMonth = 9;
            break;
        }
        case 'N':
        {
            iMonth = 10;
            break;
        }
        case 'D':
        {
            iMonth = 11;
            break;
        }
        default:
        {
            return 0;
        }
    }

    
    pucPos += 3;

    
    if (((RFC822 == enFmt) && (' ' != *pucPos)) || 
        ((RFC850 == enFmt) && ('-' != *pucPos))) 
    {
        return 0;
    }

    pucPos++;
    
    if (RFC822 == enFmt) 
    {
        if (('0' > *pucPos) || 
            ('9' < *pucPos) || 
            ('0' > *(pucPos + 1)) || 
            ('9' < *(pucPos + 1)) || 
            ('0' > *(pucPos + 2)) || 
            ('9' < *(pucPos + 2)) || 
            ('0' > *(pucPos + 3)) || 
            ('9' < *(pucPos + 3)))
        {
            return 0;
        }

        uiYear = ((*pucPos - '0') * 1000) + 
                 ((*(pucPos + 1) - '0') * 100 )+ 
                 ((*(pucPos + 2) - '0') * 10) + 
                  (*(pucPos + 3) - '0'); 
        pucPos += 4;

    } 
    else if (RFC850 == enFmt) 
    {
        if (('0' > *pucPos) || 
            ('9' < *pucPos) || 
            ('0' > *(pucPos + 1)) || 
            ('9' < *(pucPos + 1))) 
        {
            return 0;
        }

        uiYear = (((*pucPos - '0') * 10) + *(pucPos + 1)) - '0';
        uiYear += (uiYear < 70) ? 2000 : 1900; 
        pucPos += 2;
    }
    else
    {
        
    }

    if (ISOC == enFmt) 
    {
        
        if (' ' == *pucPos) 
        {
            pucPos++;
        }

        if (('0' > *pucPos) || 
            ('9' < *pucPos)) 
        {
            return 0;
        }

        uiDay = *pucPos++ - '0'; 

        if (' ' != *pucPos) 
        {
            if (('0' > *pucPos) || 
                ('9' < *pucPos)) 
            {
                return 0;
            }

            uiDay = ((uiDay * 10) + *pucPos++) - '0'; 
        }
        
        if (HTTP_TIME_ISOC_REMAIN_LEN > (pucEnd - pucPos)) 
        {
            return 0;
        }
    }

    if (' ' != *pucPos++) 
    {
        return 0;
    }

    
    if (('0' > *pucPos) || 
        ('9' < *pucPos) || 
        ('0' > *(pucPos + 1)) || 
        ('9' < *(pucPos + 1))) 
    {
        return 0;
    }

    
    uiHour = (((*pucPos - '0') * 10) + *(pucPos + 1)) - '0';
    pucPos += 2;

    if (HTTP_HEAD_FIELD_SPLIT_CHAR != *pucPos++) 
    {
        return 0;
    }

    if (('0' > *pucPos) || 
        ('9' < *pucPos) || 
        ('0' > *(pucPos + 1)) || 
        ('9' < *(pucPos + 1))) 
    {
        return 0;
    }
    
    uiMin = (((*pucPos - '0') * 10) + *(pucPos + 1)) - '0';
    pucPos += 2;

    if (HTTP_HEAD_FIELD_SPLIT_CHAR != *pucPos++) 
    {
        return 0;
    }

    if (('0' > *pucPos) || 
        ('9' < *pucPos) || 
        ('0' > *(pucPos + 1)) || 
        ('9' < *(pucPos + 1))) 
    {
        return 0;
    }
    
    uiSec = (((*pucPos - '0') * 10) + *(pucPos + 1)) - '0';

    
    if (ISOC == enFmt) 
    {
        pucPos += 2;

        if (' ' != *pucPos++) 
        {
            return 0;
        }

        if (('0' > *pucPos) || 
            ('9' < *pucPos) || 
            ('0' > *(pucPos + 1)) || 
            ('9' < *(pucPos + 1)) || 
            ('0' > *(pucPos + 2)) || 
            ('9' < *(pucPos + 2)) || 
            ('0' > *(pucPos + 3)) || 
            ('9' < *(pucPos + 3)))
        {
            return 0;
        }

        uiYear = (((*pucPos - '0') * 1000) + 
                 ((*(pucPos + 1) - '0') * 100 )+ 
                 ((*(pucPos + 2) - '0') * 10) + 
                  *(pucPos + 3)) - '0';
    }

    if ((23 < uiHour) || 
        (59 < uiMin) || 
        (59 < uiSec)) 
    {
         return 0;
    }

    
    if ((29 == uiDay) && 
        (1 == iMonth)) 
    {
        if ((0 < (uiYear & 3)) || 
            ((0 == (uiYear % 100)) && (0 != (uiYear % 400)))) 
        {
            return 0;
        }

    }
    
    else if (uiDay > g_aucDaysOfMonth[iMonth]) 
    {
        return 0;
    }
    else
    {
        
    }

    

    if (0 >= --iMonth) 
    {
        
        iMonth += 12;
        uiYear -= 1;
    }

    

    ulTime = (ULONG)365 * uiYear;
    ulTime += uiYear / 4;
    ulTime -= uiYear / 100;
    ulTime += uiYear / 400;
    ulTime += ((367 * (ULONG)((UINT)iMonth)) / 12);
    ulTime = (ulTime + uiDay) - 719499;
    ulTime = ulTime * 86400;
    ulTime += ((uiHour * 3600UL) + (uiMin * 60UL) + uiSec);

    return (time_t) ulTime;
}


char * TM_GetTimeString(UINT input_time, OUT char *string, int size)
{
    struct tm *tm;
    static char tmp[64];

    if (! string) {
        string = tmp;
        size = sizeof(tmp);
    }

    if(input_time == 0){
        struct timeval tv;
        gettimeofday(&tv, NULL);
        tm = localtime(&tv.tv_sec);
    } else {
        time_t now=(time_t)input_time;
        tm = localtime(&now);
    }

    strftime(string, size, "%Y-%m-%d %H:%M:%S", tm);


    return string;
}


UINT64 TM_NsFromInit(void)
{
    return _tm_os_GetNsFromInit();
}


UINT64 TM_UsFromInit(void)
{
    return _tm_os_GetNsFromInit() / 1000;
}


UINT64 TM_MsFromInit(void)
{
    return _tm_os_GetNsFromInit() / 1000000;
}


ULONG TM_SecondsFromInit(void)
{
    return TM_UsFromInit()/1000000;
}


U64 TM_SecondsFromUTC(void)
{
    return time(NULL);
}

unsigned long TM_GetTickPerSec(void)
{
    unsigned long clocks;

    clocks = sysconf(_SC_CLK_TCK);

    return clocks * 10;
}

static void tm_HZInit(void)
{
    TM_HZ = TM_GetTickPerSec();
    TM_MS_HZ = TM_HZ/1000;
}

CONSTRUCTOR(init) {
    tm_HZInit();
}
