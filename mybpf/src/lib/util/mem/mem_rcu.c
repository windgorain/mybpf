/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2007-11-3
* Description: 
* History:     
******************************************************************************/
#include "bs.h"

#include "utl/mutex_utl.h"
#include "utl/exec_utl.h"
#include "utl/num_utl.h"
#include "utl/txt_utl.h"
#include "utl/atomic_once.h"
#include "utl/mem_utl.h"
#include "utl/list_dtq.h"

static void _mem_rcu_free_callback(void *pstRcuNode)
{
    MEM_Free(pstRcuNode);
}

void * _mem_rcu_malloc(IN UINT uiSize, const char *file, int line)
{
    UINT uiNewSize;
    UCHAR *pucMem;

    uiNewSize = sizeof(RCU_NODE_S) + uiSize;

    pucMem = _mem_Malloc(uiNewSize, file, line);
    if (NULL == pucMem) {
        return NULL;
    }

    return pucMem + sizeof(RCU_NODE_S);
}

void * _mem_rcu_zmalloc(IN UINT uiSize, const char *file, int line)
{
    void *pMem = _mem_rcu_malloc(uiSize, file, line);
    if (pMem) {
        Mem_Zero(pMem, uiSize);
    }
    return pMem;
}

void * _mem_rcu_dup(void *mem, int size, const char *file, int line)
{
    void *buf = _mem_rcu_malloc(size, file, line);
    if (buf) {
        memcpy(buf, mem, size);
    }
    return buf;
}

void MEM_RcuFree(void *mem)
{
    UCHAR *pucMem = mem;
    pucMem -= sizeof(RCU_NODE_S);
    RcuEngine_Call((RCU_NODE_S*)pucMem, _mem_rcu_free_callback);
}

