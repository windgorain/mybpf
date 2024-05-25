/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2007-10-25
* Description: 
* History:     
******************************************************************************/

#ifndef _MEM_UTL_H
#define _MEM_UTL_H

#include "utl/num_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 

#define MEM_Set(pBuf, ucTo, uiLen) memset(pBuf, ucTo, uiLen)
#define Mem_Zero(pMem,ulSize)  memset(pMem, 0, ulSize)

#if defined(USE_BS)
#define SUPPORT_MEM_MANAGED
#endif


#define MEM_Malloc(uiSize)  _mem_Malloc(uiSize, __FILE__,  __LINE__)
#define MEM_ZMalloc(ulSize)  _mem_MallocWithZero(ulSize, __FILE__,  __LINE__)
#define MEM_MallocAndCopy(pSrc,uiSrcLen,uiMallocLen) _mem_MallocAndCopy(pSrc,uiSrcLen,uiMallocLen,__FILE__,__LINE__)
#define MEM_Free(pMem)  _mem_Free((VOID*)(pMem), __FILE__, __LINE__)
#define MEM_SafeFree(pMem) do {if (pMem) {MEM_Free(pMem);}}while(0) 


#define MEM_Kalloc(uiSize) MEM_Malloc(uiSize)
#define MEM_ZKalloc(uiSize) MEM_ZMalloc(uiSize)
#define MEM_KallocAndCopy(uiSize) MEM_MallocAndCopy(uiSize)
#define MEM_KFree(pMem)  MEM_Free(pMem)
#define MEM_SafeKFree(pMem) MEM_SafeFree(pMem)

#define MEM_ZMallocAndCopy(pSrc,uiSrcLen,uiMallocLen) ({ \
        char *_mem = MEM_MallocAndCopy(pSrc,uiSrcLen, uiMallocLen); \
        if (_mem && (uiMallocLen > uiSrcLen)) \
            memset(_mem + uiSrcLen, 0, uiMallocLen - uiSrcLen); \
        (void*)_mem; })

#define MEM_Realloc(pSrc,uiSrcLen,uiMallocLen) _mem_Realloc(pSrc,uiSrcLen,uiMallocLen,__FILE__,__LINE__)

#define MEM_ZRealloc(pSrc,uiSrcLen,uiMallocLen) ({ \
        char *_mem = MEM_Realloc(pSrc,uiSrcLen, uiMallocLen); \
        if (_mem && (uiMallocLen > uiSrcLen)) \
            memset(_mem + uiSrcLen, 0, uiMallocLen - uiSrcLen); \
        (void*)_mem; })

#define MEM_Dup(data, len) MEM_MallocAndCopy(data, len, len)

void * _mem_Malloc(IN UINT uiSize, const char *pcFileName, IN UINT uiLine);
void _mem_Free(IN VOID *pMem, const char *pcFileName, IN UINT uiLine);
void * _mem_Realloc(void *old_mem, UINT old_size, UINT new_size, char *filename, UINT line);

static inline VOID * _mem_MallocWithZero(IN UINT uiSize, const char *pszFileName, IN UINT ulLine)
{
    VOID *pMem;

    (VOID)pszFileName;
    (VOID)ulLine;

    pMem = _mem_Malloc(uiSize, pszFileName, ulLine);
    if (pMem) {
        Mem_Zero(pMem, uiSize);
    }

    return pMem;
}

#ifdef IN_DEBUG 
#define MEM_Copy(d,s,l) MEM_CopyWithCheck(d,s,l)
#else 
#define MEM_Copy(d,s,l) memcpy(d,s,l)
#endif


static inline int MEM_Cmp(IN UCHAR *pucMem1, IN UINT uiMem1Len, IN UCHAR *pucMem2, IN UINT uiMem2Len)
{
    UINT uiCmpLen = MIN(uiMem1Len, uiMem2Len);

    int ret = memcmp(pucMem1, pucMem2, uiCmpLen);
    if (ret) {
        return ret;
    }

    return (int)uiMem1Len - (int)uiMem2Len;
}

void * _mem_rcu_malloc(IN UINT uiSize, const char *file, int line);
#define MEM_RcuMalloc(size) _mem_rcu_malloc(size, __FILE__, __LINE__)

void * _mem_rcu_zmalloc(IN UINT uiSize, const char *file, int line);
#define MEM_RcuZMalloc(size) _mem_rcu_zmalloc(size, __FILE__, __LINE__)

void * _mem_rcu_dup(void *mem, int size, const char *file, int line);
#define MEM_RcuDup(mem, size) _mem_rcu_dup(mem, size, __FILE__, __LINE__)

void MEM_RcuFree(void *mem);

void * MEM_Find(IN VOID *pMem, IN UINT ulMemLen, IN VOID *pMemToFind, IN UINT ulMemToFindLen);

void * MEM_CaseFind(void *pMem, UINT ulMemLen, void *pMemToFind, UINT ulMemToFindLen);

void * MEM_FindOne(void *mem, UINT mem_len, UCHAR to_find);

void * MEM_FindOneOf(void *mem, UINT mem_len, void *to_finds, UINT to_finds_len);

int MEM_CaseCmp(UCHAR *pucMem1, UINT uiMem1Len, UCHAR *pucMem2, UINT uiMem2Len);

typedef struct
{
    UCHAR *pucPattern;     
    UINT uiPatternLen;     
    UINT uiPatternCmpLen;  
    UINT uiCmpMemOffset;   
    UINT uiPreMemTotleSize;
}MEM_FIND_INFO_S;

VOID MEM_DiscreteFindInit(INOUT MEM_FIND_INFO_S *pstFindInfo, IN UCHAR *pucPattern, IN UINT uiPatternLen);

BS_STATUS MEM_DiscreteFind
(
    INOUT MEM_FIND_INFO_S *pstFindInfo,
    IN UCHAR *pucData,
    IN UINT uiDataLen,
    OUT UINT *puiFindOffset
);


void MEM_Invert(void *in, int len, void *out);

int MEM_IsZero(void *data, int size);

int MEM_IsFF(void *data, int size);

int MEM_SprintCFromat(void *mem, UINT len, OUT char *buf, int buf_size);
int MEM_Sprint(void *pucMem, UINT uiLen, OUT char *buf, int buf_size);
typedef void (*PF_MEM_PRINT_FUNC)(const char *fmt, ...);
void MEM_Print(void *pucMem, int len, PF_MEM_PRINT_FUNC print_func);
void MEM_PrintCFormat(void *mem, int len, PF_MEM_PRINT_FUNC print_func);


int MEM_ReplaceChar(void *data, int len, UCHAR src, UCHAR dst);


int MEM_ReplaceOneChar(void *data, int len, UCHAR src, UCHAR dst);

void MEM_Swap(void *buf1, void *buf2, int len);
int MEM_SwapByOff(void *buf, int buf_len, int off);
int MEM_MoveData(void *data, S64 len, S64 offset);
int MEM_MoveDataTo(void *data, U64 len, void *dst);

void MEM_CopyWithCheck(void *dst, void *src, U32 len);

#ifdef __cplusplus
}
#endif
#endif 
