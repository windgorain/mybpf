/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _ATOMIC_ONCE_H
#define _ATOMIC_ONCE_H
#include "utl/atomic_utl.h"
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    volatile S64 begin;
    volatile S64 end;
}ATOM_ONCE_S;

#define ATOM_ONCE_INIT_VALUE {0}

typedef int (*PF_ATOM_ONCE_CB)(void *ud);


static inline void AtomOnce_Do(ATOM_ONCE_S *once, PF_ATOM_ONCE_CB func, void *ud)
{
    S64 to = 0;

    if (once->end) {
        
        return;
    }

    if (ATOM_BOOL_COMP_SWAP(&once->begin, &to, 1)) {
        func(ud);
        ATOM_BARRIER();
        ATOM_SET(&once->end, 1);
    } 

    return;
}


static inline void AtomOnce_WaitDo(ATOM_ONCE_S *once, PF_ATOM_ONCE_CB func, void *ud)
{
    int to = 0;

    if (once->end) {
        
        return;
    }

    if (ATOM_BOOL_COMP_SWAP(&once->begin, &to, 1)) {
        func(ud);
        ATOM_BARRIER();
        once->end = 1;
    } else {
        while (0 == once->end) {
            
        }
    }

    return;
}




#ifdef __cplusplus
}
#endif
#endif 
