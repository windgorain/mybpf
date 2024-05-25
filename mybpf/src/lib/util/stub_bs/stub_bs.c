/******************************************************************************
* Copyright (C), 2000-2006,  LiXingang
* Author:      LiXingang  Version: 1.0  Date: 2016-8-27
* Description: 用来对BS类函数进行封装成utl
* History:     
******************************************************************************/
#include "bs.h"
#include "utl/mem_utl.h"
#include "utl/mem_inline.h"

VOID * _mem_Malloc(IN UINT uiSize, const char *pcFileName, IN UINT uiLine)
{
#ifdef SUPPORT_MEM_MANAGED
    return MEM_MallocMem(uiSize, pcFileName, uiLine);
#else
    return malloc(uiSize);
#endif
}

VOID _mem_Free(IN VOID *pMem, const char *pcFileName, IN UINT uiLine)
{
#ifdef SUPPORT_MEM_MANAGED
    MEM_FreeMem(pMem, pcFileName, uiLine);
#else
    free(pMem);
#endif
}

void * _mem_Realloc(void *old_mem, UINT old_size, UINT new_size, char *filename, UINT line)
{
#ifdef SUPPORT_MEM_MANAGED
    void *mem;
    mem = _mem_MallocAndCopy(old_mem, old_size, new_size, filename, line);
    if (mem) {
        MEM_FreeMem(old_mem, filename, line);
    }
    return mem;
#else
    return realloc(old_mem, new_size);
#endif
}
