/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2014-8-22
* Description: 
* History:     
******************************************************************************/

#ifndef __SPINLOCK_UTL_H_
#define __SPINLOCK_UTL_H_

#include "utl/atomic_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 

#define SPINLOCK_INIT_VALUE {0,0}

typedef struct {
    volatile U64 count;
}SPINLOCK_S;

static inline void SpinLock_Init(IN SPINLOCK_S *lock)
{
    lock->count = 0;
}

static inline void SpinLock_Lock(IN SPINLOCK_S *lock)
{
    while (! __sync_bool_compare_and_swap(&lock->count, 0, 1));
}

static inline void SpinLock_UnLock(IN SPINLOCK_S *lock)
{
    lock->count = 0;
}

static inline BOOL_T SpinLock_TryLock(IN SPINLOCK_S *lock)
{
    return __sync_bool_compare_and_swap(&lock->count, 0, 1);
}

#ifdef __cplusplus
    }
#endif 

#endif 


