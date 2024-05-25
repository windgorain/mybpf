/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2007-8-15
* Description: 
* History:     
******************************************************************************/

#ifndef __VBUF_UTL_H_
#define __VBUF_UTL_H_

#include "utl/mem_inline.h"

#ifdef __cplusplus
    extern "C" {
#endif 

typedef struct {
    ULONG ulTotleLen;
    ULONG ulUsedLen;
    ULONG ulOffset;  
    UINT  double_mem: 1; 
    UCHAR *pucData;
}VBUF_S;

int VBUF_AddHead(INOUT VBUF_S *vbuf, ULONG len);
int VBUF_AddHeadBuf(INOUT VBUF_S *vbuf, void *buf, ULONG len);
int VBUF_AddTail(INOUT VBUF_S *vbuf, ULONG len);
BS_STATUS VBUF_CatFromVBuf(IN VBUF_S *pstVBufDst, IN VBUF_S *pstVBufSrc);
BS_STATUS VBUF_CpyFromVBuf(IN VBUF_S *pstVBufDst, IN VBUF_S *pstVBufSrc);
INT VBUF_CmpByBuf(IN VBUF_S *pstVBuf, IN void *buf, IN ULONG ulLen);
INT VBUF_CmpByVBuf(IN VBUF_S *pstVBuf1, IN VBUF_S *pstVBuf2);
long VBUF_Ptr2Offset(VBUF_S *vbuf, void *ptr);
int VBUF_Insert(VBUF_S *vbuf, U64 data_offset, U64 len);
int VBUF_InsertBuf(VBUF_S *vbuf, U64 data_offset, void *buf, U64 buf_len);
int VBUF_WriteFile(char *filename, VBUF_S *vbuf);
int VBUF_ReadFP(void *fp, OUT VBUF_S *vbuf);
int VBUF_ReadFile(char *filename, OUT VBUF_S *vbuf);

static inline int _vbuf_expand_to(IN VBUF_S *pstVBuf, IN ULONG ulLen)
{
    UCHAR *pucTmp;
    UINT uiNewTotleLen = ulLen;

    if (uiNewTotleLen <= pstVBuf->ulTotleLen) {
        return BS_OK;
    }

    pucTmp = MEM_MallocAndCopy(pstVBuf->pucData + pstVBuf->ulOffset, pstVBuf->ulUsedLen, uiNewTotleLen);
    if (NULL == pucTmp) {
        RETURN(BS_NO_MEMORY);
    }

    if (pstVBuf->pucData != NULL) {
        MEM_Free(pstVBuf->pucData);
    }

    pstVBuf->ulTotleLen = uiNewTotleLen;
    pstVBuf->pucData = pucTmp;
    pstVBuf->ulOffset = 0;

    return BS_OK;
}

static inline int _vbuf_resize_double_up_to(VBUF_S *pstVBuf, ULONG min)
{
    ULONG new_size = pstVBuf->ulTotleLen;

    if (new_size == 0) {
        new_size = 1;
    }

    while (new_size < min) {
        new_size  = new_size * 2;
    }

    return _vbuf_expand_to(pstVBuf, new_size);
}


static inline int _vbuf_resize_up_to(VBUF_S *pstVBuf, ULONG len)
{
    if (pstVBuf->double_mem) {
        return _vbuf_resize_double_up_to(pstVBuf, len);
    }
    return _vbuf_expand_to(pstVBuf, len);
}

static inline int _vbuf_resize_up(VBUF_S *pstVBuf, ULONG len)
{
    return _vbuf_resize_up_to(pstVBuf, pstVBuf->ulTotleLen + len);
}


static inline int _vbuf_move_data(IN VBUF_S *pstVBuf, IN ULONG ulOffset)
{
    if (pstVBuf->ulOffset == ulOffset) {
        return BS_OK;
    }

    if (pstVBuf->ulUsedLen + ulOffset > pstVBuf->ulTotleLen) {
        if (BS_OK != _vbuf_resize_up(pstVBuf, pstVBuf->ulUsedLen + ulOffset - pstVBuf->ulTotleLen)) {
            return BS_NO_MEMORY;
        }
    }

    if (pstVBuf->ulUsedLen > 0) {
        memmove(pstVBuf->pucData + ulOffset, pstVBuf->pucData + pstVBuf->ulOffset, pstVBuf->ulUsedLen);
    }

    pstVBuf->ulOffset = ulOffset;

    return BS_OK;
}

static inline int _vbuf_pre_cat(IN VBUF_S *pstVBuf, IN ULONG ulLen)
{
    ULONG ulTailLen;

    ulTailLen = (pstVBuf->ulTotleLen - pstVBuf->ulOffset) - pstVBuf->ulUsedLen;

    if (ulTailLen >= ulLen) {
        return 0;
    }

    if (pstVBuf->ulUsedLen + ulLen <= pstVBuf->ulTotleLen) {
        return _vbuf_move_data(pstVBuf, 0);
    }

    return _vbuf_resize_up_to(pstVBuf, pstVBuf->ulUsedLen + ulLen);
}


static inline void VBUF_Init(OUT VBUF_S *pstVBuf)
{
    Mem_Zero(pstVBuf, sizeof(VBUF_S));
}

static inline void VBUF_Finit(IN VBUF_S *pstVBuf)
{
    if (pstVBuf->pucData) {
        MEM_Free(pstVBuf->pucData);
        Mem_Zero(pstVBuf, sizeof(VBUF_S));
    }
}


static inline void * VBUF_Steal(INOUT VBUF_S *vbuf)
{
    _vbuf_move_data(vbuf, 0);
    void *mem = vbuf->pucData;
    VBUF_Init(vbuf);
    return mem;
}

static inline void VBUF_SetData(INOUT VBUF_S *pstVBuf, void *data, int data_len)
{
    VBUF_Finit(pstVBuf);
    pstVBuf->pucData = data;
    pstVBuf->ulTotleLen = data_len;
    pstVBuf->ulUsedLen = data_len;
}

static inline void * VBUF_GetData(IN VBUF_S *pstVBuf)
{
    BS_DBGASSERT(pstVBuf != 0);
    return pstVBuf->pucData + pstVBuf->ulOffset;
}

static inline void * VBUF_GetTailFreeBuf(IN VBUF_S *pstVBuf)
{
    return pstVBuf->pucData + pstVBuf->ulOffset + pstVBuf->ulUsedLen;
}


static inline ULONG VBUF_GetDataLength(IN VBUF_S *pstVBuf)
{
    BS_DBGASSERT(0 != pstVBuf);
    return pstVBuf->ulUsedLen;
}


static inline void VBUF_ToM(INOUT VBUF_S *vbuf, OUT LLDATA_S *m)
{
    m->len = VBUF_GetDataLength(vbuf);
    m->data = VBUF_Steal(vbuf);
}


static inline void VBUF_FromM(OUT VBUF_S *vbuf, INOUT LLDATA_S *m)
{
    VBUF_SetData(vbuf, m->data, m->len);
    m->data = NULL;
    m->len = 0;
}


static inline ULONG VBUF_GetHeadFreeLength(IN VBUF_S *pstVBuf)
{
    return pstVBuf->ulOffset;
}


static inline ULONG VBUF_GetTailFreeLength(IN VBUF_S *pstVBuf)
{
    BS_DBGASSERT(pstVBuf->ulTotleLen >= (pstVBuf->ulOffset + pstVBuf->ulUsedLen));
    return (pstVBuf->ulTotleLen - pstVBuf->ulOffset) - pstVBuf->ulUsedLen;
}

static inline int VBUF_CpyBuf(IN VBUF_S *pstVBuf, IN void *buf, IN ULONG ulLen)
{
    BS_DBGASSERT(0 != pstVBuf);

    if (pstVBuf->ulTotleLen < ulLen) {
        UCHAR *pucTmp = MEM_Malloc(ulLen);
        if (NULL == pucTmp) {
            RETURN(BS_NO_MEMORY);
        }

        if (NULL != pstVBuf->pucData) {
            MEM_Free(pstVBuf->pucData);
        }

        pstVBuf->ulTotleLen = ulLen;
        pstVBuf->pucData = pucTmp;
    }

    memcpy(pstVBuf->pucData, buf, ulLen);
    pstVBuf->ulUsedLen = ulLen;
    pstVBuf->ulOffset = 0;

    return BS_OK;
}

static inline int VBUF_CatBuf(INOUT VBUF_S *pstVBuf, IN VOID *buf, IN ULONG ulLen)
{
    int ret;

    BS_DBGASSERT(0 != pstVBuf);
    BS_DBGASSERT(NULL != buf);

    if (pstVBuf->ulUsedLen == 0) {
        return VBUF_CpyBuf(pstVBuf, buf, ulLen);
    }

    ret = _vbuf_pre_cat(pstVBuf, ulLen);
    if (ret < 0) {
        return ret;
    }

    memcpy(pstVBuf->pucData + pstVBuf->ulOffset + pstVBuf->ulUsedLen, buf, ulLen);
    pstVBuf->ulUsedLen += ulLen;

    return BS_OK;
}


static inline int VBUF_ExpandTo(IN VBUF_S *pstVBuf, IN ULONG ulLen)
{
    return _vbuf_expand_to(pstVBuf, ulLen);
}


static inline int VBUF_Expand(IN VBUF_S *pstVBuf, IN ULONG ulLen)
{
    UINT uiNewTotleLen;
    uiNewTotleLen = ulLen + pstVBuf->ulTotleLen;
    return VBUF_ExpandTo(pstVBuf, uiNewTotleLen);
}


static inline int VBUF_MoveData(IN VBUF_S *pstVBuf, IN ULONG ulOffset)
{
    return _vbuf_move_data(pstVBuf, ulOffset);
}

static inline void VBUF_SetMemDouble(OUT VBUF_S *pstVBuf, BOOL_T enable)
{
    pstVBuf->double_mem = enable;
}


static inline void VBUF_Clear(IN VBUF_S *pstVBuf)
{
    if (! pstVBuf) {
        return;
    }

    if (pstVBuf->pucData) {
        MEM_Free(pstVBuf->pucData);
    }

    Mem_Zero(pstVBuf, sizeof(VBUF_S));
}


static inline void VBUF_ClearData(IN VBUF_S *pstVBuf)
{
    pstVBuf->ulUsedLen = 0;
    pstVBuf->ulOffset = 0;
}


static inline int VBUF_SetDataLength(INOUT VBUF_S *vbuf, ULONG ulDataLength)
{
    BS_DBGASSERT(NULL != vbuf);

    ULONG new_len = vbuf->ulOffset + ulDataLength;

    if (unlikely(new_len > vbuf->ulTotleLen)) {
        RETURN(BS_OUT_OF_RANGE);
    }

    vbuf->ulUsedLen = ulDataLength;

    return 0;
}


static inline int VBUF_AddDataLength(INOUT VBUF_S *vbuf, ULONG add_len)
{
    BS_DBGASSERT(0 != vbuf);

    ULONG new_len = (vbuf->ulOffset + vbuf->ulUsedLen) + add_len;

    if (unlikely(new_len > vbuf->ulTotleLen)) {
        RETURN(BS_OUT_OF_RANGE);
    }

    vbuf->ulUsedLen += add_len;

    return 0;
}


static inline int VBUF_Cut(VBUF_S *vbuf, ULONG offset, ULONG cut_len)
{
    if (offset + cut_len >= vbuf->ulUsedLen) {
        vbuf->ulUsedLen = offset;
        return 0;
    }

    char *data = VBUF_GetData(vbuf);
    char *dst = data + offset;
    char *src = dst + cut_len;
    ULONG src_len = vbuf->ulUsedLen - (offset + cut_len);

    memmove(dst, src, src_len);

    vbuf->ulUsedLen -= cut_len;

    return 0;
}


static inline int VBUF_CutHead(IN VBUF_S *pstVBuf, IN ULONG ulCutLen)
{
    if (ulCutLen < pstVBuf->ulUsedLen) {
        pstVBuf->ulUsedLen -= ulCutLen;
        memmove(pstVBuf->pucData, pstVBuf->pucData + pstVBuf->ulOffset + ulCutLen, pstVBuf->ulUsedLen);
    } else {
        pstVBuf->ulUsedLen = 0;
    }

    pstVBuf->ulOffset = 0;

    return 0;
}


static inline int VBUF_EarseHead(IN VBUF_S *pstVBuf, IN ULONG ulCutLen)
{
    BS_DBGASSERT(0 != pstVBuf);

    if (ulCutLen < pstVBuf->ulUsedLen) {
        pstVBuf->ulUsedLen -= ulCutLen;
        pstVBuf->ulOffset = pstVBuf->ulOffset + ulCutLen;
    } else {
        pstVBuf->ulUsedLen = 0;
        pstVBuf->ulOffset = 0;
    }

    return 0;
}

static inline void VBUF_CutAll(IN VBUF_S *pstVBuf)
{
    BS_DBGASSERT(0 != pstVBuf);

    pstVBuf->ulUsedLen = 0;
    pstVBuf->ulOffset = 0;
}

static inline int VBUF_CutTail(IN VBUF_S *pstVBuf, IN ULONG ulCutLen)
{
    BS_DBGASSERT(0 != pstVBuf);

    if (ulCutLen < pstVBuf->ulUsedLen) {
        pstVBuf->ulUsedLen -= ulCutLen;
    } else {
        pstVBuf->ulUsedLen = 0;
    }

    return 0;
}


#ifdef __cplusplus
    }
#endif 

#endif 


