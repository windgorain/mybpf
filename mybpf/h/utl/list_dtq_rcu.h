/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _LIST_DTQ_RCU_H
#define _LIST_DTQ_RCU_H

#include "utl/list_dtq.h"
#include "utl/atomic_utl.h"

#ifdef __cplusplus
extern "C" {
#endif

static inline void DTQ_AddBeforeRCU(DTQ_NODE_S* pstNext, DTQ_NODE_S* pstInst)
{
    pstInst->pstPrev = pstNext->pstPrev;
    pstInst->pstNext = pstNext;
    ATOM_BARRIER();
    pstInst->pstPrev->pstNext = pstInst;
    pstInst->pstNext->pstPrev = pstInst;
    return;
}

static inline void DTQ_AddTailRCU(DTQ_HEAD_S* pstList, DTQ_NODE_S* pstNode)
{
    DTQ_AddBeforeRCU(&pstList->stHead, pstNode);
    return;
}

#ifdef __cplusplus
}
#endif
#endif 
