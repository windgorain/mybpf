/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2007-8-15
* Description: 动态扩展空间的buf
* History:     
******************************************************************************/
#include "bs.h"

#include "utl/vbuf_utl.h"



int VBUF_AddHead(INOUT VBUF_S *vbuf, ULONG len)
{
    if (vbuf->ulOffset < len) {
        int ret = VBUF_MoveData(vbuf, len);
        if (ret < 0) {
            return ret;
        }
    }

    vbuf->ulOffset -= len;
    vbuf->ulUsedLen += len;

    return 0;
}


int VBUF_AddHeadBuf(INOUT VBUF_S *vbuf, void *buf, ULONG len)
{
    int ret;

    ret = VBUF_AddHead(vbuf, len);
    if (ret < 0) {
        return ret;
    }

    void *d = VBUF_GetData(vbuf);
    memcpy(d, buf, len);

    return 0;
}


int VBUF_AddTail(INOUT VBUF_S *vbuf, ULONG len)
{
    int ret = _vbuf_pre_cat(vbuf, len);
    if (ret < 0) {
        return ret;
    }

    vbuf->ulUsedLen += len;

    return 0;
}

BS_STATUS VBUF_CatFromVBuf(IN VBUF_S *pstVBufDst, IN VBUF_S *pstVBufSrc)
{
    BS_DBGASSERT(0 != pstVBufDst);
    BS_DBGASSERT(0 != pstVBufSrc);

    return VBUF_CatBuf(pstVBufDst, pstVBufSrc->pucData, pstVBufSrc->ulUsedLen);
}

BS_STATUS VBUF_CpyFromVBuf(IN VBUF_S *pstVBufDst, IN VBUF_S *pstVBufSrc)
{
    BS_DBGASSERT(0 != pstVBufDst);
    BS_DBGASSERT(0 != pstVBufSrc);

    return VBUF_CpyBuf(pstVBufDst, pstVBufSrc->pucData, pstVBufSrc->ulUsedLen);
}

INT VBUF_CmpByBuf(IN VBUF_S *pstVBuf, IN void *buf, IN ULONG ulLen)
{
    BS_DBGASSERT(0 != pstVBuf);

    return memcmp(pstVBuf->pucData + pstVBuf->ulOffset, buf, MIN(pstVBuf->ulUsedLen, ulLen));
}

INT VBUF_CmpByVBuf(IN VBUF_S *pstVBuf1, IN VBUF_S *pstVBuf2)
{
    if ((pstVBuf1 == 0) && (pstVBuf2 == 0))
    {
        return 0;
    }
    
    if (pstVBuf1 == 0) 
    {
        return -1;
    }

    if (pstVBuf2 == 0)
    {
        return 1;
    }

    return VBUF_CmpByBuf(pstVBuf1, pstVBuf2->pucData, pstVBuf2->ulUsedLen);
}


int VBUF_Insert(VBUF_S *vbuf, U64 data_offset, U64 len)
{
    if (data_offset > vbuf->ulUsedLen) {
        RETURNI(BS_ERR, "Insert out of range");
    }

    if (data_offset == 0) {
        return VBUF_AddHead(vbuf, len);
    }

    if (data_offset == vbuf->ulUsedLen) {
        return VBUF_AddTail(vbuf, len);
    }

    int ret = VBUF_AddTail(vbuf, len);
    if (ret < 0) {
        return ret;
    }

    char *src = (char*)vbuf->pucData + vbuf->ulOffset + data_offset;
    char *dst = src + len;

    memmove(dst, src, vbuf->ulUsedLen - data_offset);

    return 0;
}


int VBUF_InsertBuf(VBUF_S *vbuf, U64 data_offset, void *buf, U64 buf_len)
{
    int ret = VBUF_Insert(vbuf, data_offset, buf_len);
    if (ret < 0) {
        return ret;
    }

    char *data = VBUF_GetData(vbuf);
    memcpy(data + data_offset, buf, buf_len);

    return 0;
}


long VBUF_Ptr2Offset(VBUF_S *vbuf, void *ptr)
{
    long offset;
    char *data = VBUF_GetData(vbuf);
    char *tmp = ptr;

    if (data > tmp) {
        return -1;
    }

    offset = tmp - data;
    if (offset >= vbuf->ulUsedLen) {
        return -1;
    }

    return offset;
}

