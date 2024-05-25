/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      lixingang  Version: 1.0  Date: 2007-2-8
* Description: 
* History:     
******************************************************************************/

#include "bs.h"

#include <pthread.h>
#include "utl/mutex_utl.h"

static inline void _mutex_init(MUTEX_S *pstMutex, void *attr)
{
#ifdef IN_UNIXLIKE
    pthread_mutex_init(&pstMutex->stMutex, attr);
#endif

#ifdef IN_WINDOWS
    InitializeCriticalSection(&pstMutex->stMutex);
#endif

    pstMutex->inited = 1;
}


VOID MUTEX_InitRecursive(IN MUTEX_S *pstMutex)
{
#ifdef IN_UNIXLIKE
    pthread_mutexattr_t mat;
    pthread_mutexattr_init(&mat);
    
    pthread_mutexattr_settype(&mat, PTHREAD_MUTEX_RECURSIVE);
    _mutex_init(pstMutex, &mat);
#endif

#ifdef IN_WINDOWS
    _mutex_init(pstMutex, 0);
#endif
}


void MUTEX_InitNormal(MUTEX_S *pstMutex)
{
    _mutex_init(pstMutex, 0);
}

VOID MUTEX_Final(IN MUTEX_S *pstMutex)
{
#ifdef IN_LINUX
    pthread_mutex_destroy(&pstMutex->stMutex);
#endif
#ifdef IN_WINDOWS
    DeleteCriticalSection(&pstMutex->stMutex);
#endif
}

void _MUTEX_P(IN MUTEX_S *pstMutex, const char *pcFile, IN UINT uiLine)
{
    BS_DBGASSERT(pstMutex->inited);

#ifdef IN_UNIXLIKE
    pthread_mutex_lock(&pstMutex->stMutex);
#endif

#ifdef IN_WINDOWS
    EnterCriticalSection(&pstMutex->stMutex);
#endif

    pstMutex->count ++;
    pstMutex->pcFile = pcFile;
    pstMutex->uiLine = uiLine;

    return;
}

BOOL_T _MUTEX_TryP(IN MUTEX_S *pstMutex, IN CHAR *pcFile, IN UINT uiLine)
{
    BOOL_T bSuccess = FALSE;

    BS_DBGASSERT(pstMutex->inited);
    
#ifdef IN_UNIXLIKE
    if (0 == pthread_mutex_trylock(&pstMutex->stMutex)) {
        bSuccess = TRUE;
    }
#endif

#ifdef IN_WINDOWS
    if (0 != TryEnterCriticalSection(&pstMutex->stMutex)) {
        bSuccess = TRUE;
    }
#endif

    if (bSuccess == TRUE) {
        pstMutex->count ++;
        pstMutex->pcFile = pcFile;
        pstMutex->uiLine = uiLine;
    }

    return bSuccess;
}

VOID MUTEX_V(IN MUTEX_S *pstMutex)
{
    pstMutex->pcFile = NULL;
    pstMutex->uiLine = 0;

    BS_DBGASSERT(pstMutex->count > 0);
    
    pstMutex->count --;

#ifdef IN_UNIXLIKE
    pthread_mutex_unlock(&pstMutex->stMutex);
#endif

#ifdef IN_WINDOWS
    LeaveCriticalSection(&pstMutex->stMutex);
#endif
}

UINT MUTEX_GetCount(IN MUTEX_S *pstMutex)
{
    return pstMutex->count;
}

void COND_Init(COND_S *cond)
{
#ifdef IN_UNIXLIKE
    pthread_cond_init(&cond->cond, NULL);
#endif
#ifdef IN_WINDOWS
    InitializeConditionVariable(&cond->cond);
#endif
}

void COND_Final(COND_S *cond)
{
#ifdef IN_UNIXLIKE
    pthread_cond_destroy(&cond->cond);
#endif
#ifdef IN_WINDOWS
#endif
}

void COND_Wait(COND_S *cond, MUTEX_S *mutex)
{
    mutex->count --;

#ifdef IN_UNIXLIKE
    pthread_cond_wait(&cond->cond, &mutex->stMutex);
#endif
#ifdef IN_WINDOWS
    SleepConditionVariableCS(&cond->cond, &mutex->stMutex, INFINITE);
#endif

    mutex->count ++;
}

void COND_Wake(COND_S *cond)
{
#ifdef IN_UNIXLIKE
    pthread_cond_signal(&cond->cond);
#endif
#ifdef IN_WINDOWS
    WakeConditionVariable(&cond->cond);
#endif
}

void COND_WakeAll(COND_S *cond)
{
#ifdef IN_UNIXLIKE
    pthread_cond_broadcast(&cond->cond);
#endif
#ifdef IN_WINDOWS
    WakeAllConditionVariable(&cond->cond);
#endif
}
