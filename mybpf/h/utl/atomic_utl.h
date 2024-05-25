/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2014-8-22
* Description: 
* History:     
******************************************************************************/

#ifndef __ATOMIC_UTL_H_
#define __ATOMIC_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

#ifdef __ATOMIC_RELAXED

#define ATOM_GET(ptr)  __atomic_load_n(ptr, __ATOMIC_RELAXED)
#define ATOM_SET(ptr,val)  __atomic_store_n(ptr, val, __ATOMIC_RELAXED)

#define ATOM_FETCH_SET(ptr,val)  (__sync_val_compare_and_swap(ptr, *(ptr), (val)))
#define ATOM_FETCH_ADD(ptr,val)  (__atomic_fetch_add(ptr,val,__ATOMIC_RELAXED))
#define ATOM_FETCH_SUB(ptr,val)  (__atomic_fetch_sub(ptr,val,__ATOMIC_RELAXED))
#define ATOM_FETCH_AND(ptr,val)  (__atomic_fetch_and(ptr,val,__ATOMIC_RELAXED))
#define ATOM_FETCH_OR(ptr,val)   (__atomic_fetch_or(ptr,val,__ATOMIC_RELAXED))
#define ATOM_FETCH_XOR(ptr,val)  (__atomic_fetch_xor(ptr,val,__ATOMIC_RELAXED))
#define ATOM_ADD_FETCH(ptr,val)  (__atomic_add_fetch(ptr,val,__ATOMIC_RELAXED))
#define ATOM_SUB_FETCH(ptr,val)  (__atomic_sub_fetch(ptr,val,__ATOMIC_RELAXED))
#define ATOM_AND_FETCH(ptr,val)  (__atomic_and_fetch(ptr,val,__ATOMIC_RELAXED))
#define ATOM_OR_FETCH(ptr,val)   (__atomic_or_fetch(ptr,val,__ATOMIC_RELAXED))
#define ATOM_XOR_FETCH(ptr,val)  (__atomic_xor_fetch(ptr,val,__ATOMIC_RELAXED))
#define ATOM_INC_FETCH(ptr)      ATOM_ADD_FETCH(ptr,1)
#define ATOM_DEC_FETCH(ptr)      ATOM_SUB_FETCH(ptr,1)

#else

#define ATOM_GET(ptr)  (*(ptr))
#define ATOM_SET(ptr,val)  (*(ptr) = (val))

#define ATOM_FETCH_SET(ptr,val) (__sync_val_compare_and_swap(ptr, *(ptr), (val)))
#define ATOM_FETCH_ADD(ptr,val)  (__sync_fetch_and_add(ptr,val))
#define ATOM_FETCH_SUB(ptr,val)  (__sync_fetch_and_sub(ptr,val))
#define ATOM_FETCH_AND(ptr,val)  (__sync_fetch_and_and(ptr,val))
#define ATOM_FETCH_OR(ptr,val)   (__sync_fetch_and_or(ptr,val))
#define ATOM_FETCH_XOR(ptr,val)  (__sync_fetch_and_xor(ptr,val))
#define ATOM_ADD_FETCH(ptr,val)  (__sync_add_and_fetch(ptr,val))
#define ATOM_SUB_FETCH(ptr,val)  (__sync_sub_and_fetch(ptr,val))
#define ATOM_AND_FETCH(ptr,val)  (__sync_and_and_fetch(ptr,val))
#define ATOM_OR_FETCH(ptr,val)   (__sync_or_and_fetch(ptr,val))
#define ATOM_XOR_FETCH(ptr,val)  (__sync_xor_and_fetch(ptr,val))

#define ATOM_INC_FETCH(ptr)        ATOM_ADD_FETCH(ptr,1)
#define ATOM_DEC_FETCH(ptr)        ATOM_SUB_FETCH(ptr,1)

#endif


#define ATOM_BOOL_COMP_SWAP(ptr,ifptr,newval) __sync_bool_compare_and_swap((ptr), *(ifptr), (newval))
#define ATOM_BARRIER() (__sync_synchronize())

#ifdef __cplusplus
    }
#endif 

#endif 


