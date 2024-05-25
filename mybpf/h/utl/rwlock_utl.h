/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2014-8-22
* Description: 
* History:     
******************************************************************************/

#ifndef __RWLOCK_UTL_H_
#define __RWLOCK_UTL_H_

#include "utl/mutex_utl.h"
#include "utl/spinlock_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 

#ifdef IN_WINDOWS

typedef struct
{
    SPINLOCK_S stSpinLock;
    volatile UINT uiReaderCount;
    volatile UINT uiWriterCount;
    MUTEX_S stWriterMutex;
}RWLOCK_S;

VOID RWLOCK_Init(IN RWLOCK_S *pstLock);
VOID RWLOCK_Fini(IN RWLOCK_S *pstLock);
VOID RWLOCK_ReadLock(IN RWLOCK_S *pstLock);
VOID RWLOCK_ReadUnLock(IN RWLOCK_S *pstLock);
VOID RWLOCK_WriteLock(IN RWLOCK_S *pstLock);
VOID RWLOCK_WriteUnLock(IN RWLOCK_S *pstLock);

#endif



#ifdef IN_UNIXLIKE

typedef struct
{
    pthread_rwlock_t stRwLock;
}RWLOCK_S;

static inline VOID RWLOCK_Init(IN RWLOCK_S *pstLock)
{
    pthread_rwlock_init(&pstLock->stRwLock, NULL);
}

static inline VOID RWLOCK_Fini(IN RWLOCK_S *pstLock)
{
    pthread_rwlock_destroy(&pstLock->stRwLock);
}

static inline VOID RWLOCK_ReadLock(IN RWLOCK_S *pstLock)
{
    pthread_rwlock_rdlock(&pstLock->stRwLock);
}

static inline VOID RWLOCK_ReadUnLock(IN RWLOCK_S *pstLock)
{
    pthread_rwlock_unlock (&pstLock->stRwLock);
}

static inline VOID RWLOCK_WriteLock(IN RWLOCK_S *pstLock)
{
    pthread_rwlock_wrlock(&pstLock->stRwLock);
}

static inline VOID RWLOCK_WriteUnLock(IN RWLOCK_S *pstLock)
{
    pthread_rwlock_unlock(&pstLock->stRwLock);
}

#endif

#ifdef __cplusplus
    }
#endif 

#endif 


