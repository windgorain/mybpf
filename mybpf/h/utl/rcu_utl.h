
#ifndef __RCU_UTL_H_
#define __RCU_UTL_H_

#include "utl/list_sl.h"
#include "utl/spinlock_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 


typedef void (*PF_RCU_FREE_FUNC)(void *pstRcuNode);

typedef struct
{
    SPINLOCK_S stLock;
    int uiCurrentPhase;
    UINT auiCounter[2];
    SL_HEAD_S free_list[2];
}RCU_S;

typedef RCU_S* RCU_HANDLE;

typedef struct
{
    SL_NODE_S node;
    PF_RCU_FREE_FUNC pfFunc;
}RCU_NODE_S;

void RCU_Init(INOUT RCU_S *rcu);

VOID RCU_Call
(
    IN RCU_HANDLE hRcuHandle,
    IN RCU_NODE_S *pstRcuNode,
    IN PF_RCU_FREE_FUNC pfFreeFunc
);

int RCU_Lock(IN RCU_HANDLE hRcuHandle);
VOID RCU_UnLock(IN RCU_HANDLE hRcuHandle, IN UINT uiPhase);

#ifdef __cplusplus
    }
#endif 

#endif 


