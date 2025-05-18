/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _MEM_INLINE_H
#define _MEM_INLINE_H

#ifdef __cplusplus
extern "C" {
#endif

static inline VOID * _mem_MallocAndCopy
(
    IN VOID *pSrc,
    IN U64 uiSrcLen,
    IN U64 uiMallocLen,
    const CHAR *pcFileName,
    IN UINT uiLine
)
{
    VOID *pMem;
    U64 uiCopyLen;

    (VOID)pcFileName;
    (VOID)uiLine;

    uiCopyLen = MIN(uiSrcLen, uiMallocLen);

    pMem = MEM_Malloc(uiMallocLen);
    if (NULL == pMem) {
        return NULL;
    }

    if (uiCopyLen != 0) {
        memcpy(pMem, pSrc, uiCopyLen);
    }

    return pMem;
}

#ifdef __cplusplus
}
#endif
#endif 
