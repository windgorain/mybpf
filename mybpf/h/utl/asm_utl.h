/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2014-8-22
* Description: 
* History:     
******************************************************************************/
#ifndef __ASM_UTL_H_
#define __ASM_UTL_H_

#include "utl/atomic_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 

typedef struct { volatile int counter; } atomic_t;
typedef struct { volatile long long counter; } atomic64_t;

#define ATOMIC_INIT(i)	{ (i) }

#define mb() ATOM_BARRIER()

#define atomic_read(v)		((v)->counter)
#define atomic_set(v,i)		(((v)->counter) = (i))

static inline void atomic_add(int i, atomic_t *v)
{
    ATOM_FETCH_ADD(&v->counter, i);
}

static inline void atomic_sub(int i, atomic_t *v)
{
    ATOM_FETCH_SUB(&v->counter, i);
}

static inline int atomic_sub_and_test(int i, atomic_t *v)
{
    return (ATOM_SUB_FETCH(&v->counter, i) == 0) ? TRUE : FALSE;
}

static inline void atomic_inc(atomic_t *v)
{
    ATOM_ADD_FETCH(&v->counter, 1);
}

static inline void atomic_dec(atomic_t *v)
{
    ATOM_SUB_FETCH(&v->counter, 1);
}

static inline int atomic_dec_and_test(atomic_t *v)
{
    return (ATOM_SUB_FETCH(&v->counter, 1) == 0) ? TRUE : FALSE;
}

static inline int atomic_inc_and_test(atomic_t *v)
{
    return (ATOM_ADD_FETCH(&v->counter, 1) == 0) ? TRUE : FALSE;
}

static inline int atomic_add_negative(int i, atomic_t *v)
{
    int count = ATOM_ADD_FETCH(&v->counter, i);
    if (count < 0) {
        return TRUE;
    }
    return FALSE;
}

static inline int atomic_add_return(int i, atomic_t *v)
{
    return ATOM_ADD_FETCH(&v->counter, i);
}

static inline int atomic_sub_return(int i, atomic_t *v)
{
    return ATOM_SUB_FETCH(&v->counter, i);
}

#ifdef __cplusplus
    }
#endif 

#endif 


