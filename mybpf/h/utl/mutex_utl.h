/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2007-5-13
* Description: 
* History:     
******************************************************************************/

#ifndef __UTL_MUTEX_H_
#define __UTL_MUTEX_H_

#ifdef __cplusplus
    extern "C" {
#endif 

typedef struct
{
#ifdef IN_UNIXLIKE
    pthread_mutex_t stMutex;
#endif
#ifdef IN_WINDOWS
    CRITICAL_SECTION stMutex;
#endif

    UINT uiLine: 15;
    UINT inited: 1;
    UINT count: 16; 
    const char *pcFile;
}MUTEX_S;

typedef struct {
#ifdef IN_UNIXLIKE
    pthread_cond_t cond;
#endif
#ifdef IN_WINDOWS
    CONDITION_VARIABLE cond;
#endif
}COND_S;

#define MUTEX_P(pstMutex) _MUTEX_P(pstMutex, __FILE__, __LINE__)
#define MUTEX_TryP(pstMutex) _MUTEX_TryP(pstMutex, __FILE__, __LINE__)

#define MUTEX_Init(_x) MUTEX_InitRecursive(_x)

VOID MUTEX_InitRecursive(IN MUTEX_S *pstMutex);
void MUTEX_InitNormal(MUTEX_S *pstMutex);

VOID MUTEX_Final(IN MUTEX_S *pstMutex);
void _MUTEX_P(IN MUTEX_S *pstMutex, const char *pcFile, IN UINT uiLine);
BOOL_T _MUTEX_TryP(IN MUTEX_S *pstMutex, IN CHAR *pcFile, IN UINT uiLine);
VOID MUTEX_V(IN MUTEX_S *pstMutex);
UINT MUTEX_GetCount(IN MUTEX_S *pstMutex);

void COND_Init(COND_S *cond);
void COND_Final(COND_S *cond);
void COND_Wait(COND_S *cond, MUTEX_S *mutex);
void COND_Wake(COND_S *cond);
void COND_WakeAll(COND_S *cond);

#ifdef __cplusplus
    }
#endif 

#endif 


