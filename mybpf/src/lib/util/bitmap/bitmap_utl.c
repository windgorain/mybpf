/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2007-7-2
* Description: 
* History:     
******************************************************************************/
#include "bs.h"
#include "utl/mem_utl.h"
#include "utl/num_utl.h"
#include "utl/array_bit.h"
#include "utl/bitmap_utl.h"


void BITMAP_Init(BITMAP_S *pstBitMap, UINT bitnum, UINT *bitmap_mem)
{
    memset(pstBitMap, 0, sizeof(BITMAP_S));

    pstBitMap->pulBitMap = bitmap_mem;
    pstBitMap->ulBitNum = bitnum;
    pstBitMap->uiUintNum = BITMAP_NEED_UINTS(bitnum);

    memset(bitmap_mem, 0, pstBitMap->uiUintNum * 4);
}

void BITMAP_Fini(BITMAP_S *pstBitMap)
{
    memset(pstBitMap, 0, sizeof(BITMAP_S));
}


BS_STATUS BITMAP_Create(IN BITMAP_S * pstBitMap, IN UINT ulBitNum)
{
    if (pstBitMap == NULL)
    {
        RETURN(BS_NULL_PARA);
    }

    memset(pstBitMap, 0, sizeof(BITMAP_S));

    pstBitMap->pulBitMap = MEM_ZMalloc(BITMAP_NEED_BYTES(ulBitNum));
    if (pstBitMap->pulBitMap == NULL)
    {
        RETURN(BS_NO_MEMORY);
    }
    
    pstBitMap->ulBitNum = ulBitNum;
    pstBitMap->uiUintNum = BITMAP_NEED_UINTS(ulBitNum);

    return BS_OK;
}


BS_STATUS BITMAP_Destory(IN BITMAP_S * pstBitMap)
{
    if (NULL != pstBitMap->pulBitMap)
    {
        MEM_Free(pstBitMap->pulBitMap);
    }
    pstBitMap->pulBitMap = NULL;
    pstBitMap->ulBitNum = 0;
    pstBitMap->uiUintNum = 0;

    return BS_OK;
}


INT64 BITMAP_GetBusy(BITMAP_S *pstBitMap)
{
    return ArrayBit_GetBusy(pstBitMap->pulBitMap, pstBitMap->ulBitNum);
}


INT64 BITMAP_GetBusyFrom(BITMAP_S *pstBitMap, INT64 from)
{
    return ArrayBit_GetBusyFrom(pstBitMap->pulBitMap, pstBitMap->ulBitNum, from);
}


INT64 BITMAP_GetFree(BITMAP_S *pstBitMap)
{
    return ArrayBit_GetFree(pstBitMap->pulBitMap, pstBitMap->ulBitNum);
}


INT64 BITMAP_GetFreeFrom(BITMAP_S *pstBitMap, INT64 from)
{
    return ArrayBit_GetFreeFrom(pstBitMap->pulBitMap, pstBitMap->ulBitNum, from);
}


INT64 BITMAP_GetFreeCycle(BITMAP_S *pstBitMap)
{
    INT64 index = ArrayBit_GetFreeFrom(pstBitMap->pulBitMap, pstBitMap->ulBitNum, pstBitMap->uiScanIndex);
    if (index < 0) {
        index = ArrayBit_GetFree(pstBitMap->pulBitMap, pstBitMap->uiScanIndex);
        if (index < 0) {
            return -1;
        }
    }

    pstBitMap->uiScanIndex = index + 1;
    if (pstBitMap->uiScanIndex >= pstBitMap->ulBitNum) {
        pstBitMap->uiScanIndex = 0;
    }

    return index;
}


INT64 BITMAP_2GetSettedIndex(IN BITMAP_S * pstBitMap1, IN BITMAP_S * pstBitMap2)
{
    UINT uiIndex;
    
    BITMAP_SCAN_SETTED_BEGIN(pstBitMap1, uiIndex) {
        if (! BITMAP_ISSET(pstBitMap2, uiIndex)) {
            return uiIndex;
        }
    }BITMAP_SCAN_END();

    return -1;
}


INT64 BITMAP_2GetUnsettedIndex(IN BITMAP_S * pstBitMap1, IN BITMAP_S * pstBitMap2)
{
    UINT uiIndex;
    
    BITMAP_SCAN_UNSETTED_BEGIN(pstBitMap1, uiIndex) {
        if (! BITMAP_ISSET(pstBitMap2, uiIndex)) {
            return uiIndex;
        }
    }BITMAP_SCAN_END();

    return -1;
}


void BITMAP_Copy(BITMAP_S *src, BITMAP_S *dst)
{
    BS_DBGASSERT(src->uiUintNum == dst->uiUintNum);

    memcpy(dst->pulBitMap, src->pulBitMap, dst->uiUintNum * 4);
}


void BITMAP_Not(BITMAP_S *src, BITMAP_S *dst)
{
    BS_DBGASSERT(src->uiUintNum == dst->uiUintNum);

    UINT i;
    for (i=0; i<dst->uiUintNum; i++) {
        dst->pulBitMap[i] = ~src->pulBitMap[i];
    }
}


void BITMAP_Or(BITMAP_S *src1, BITMAP_S *src2, BITMAP_S *dst)
{
    BS_DBGASSERT(src1->uiUintNum == src2->uiUintNum);
    BS_DBGASSERT(src1->uiUintNum == dst->uiUintNum);

    UINT i;
    for (i=0; i<dst->uiUintNum; i++) {
        dst->pulBitMap[i] = src1->pulBitMap[i] | src2->pulBitMap[i];
    }
}


void BITMAP_And(BITMAP_S *src1, BITMAP_S *src2, BITMAP_S *dst)
{
    BS_DBGASSERT(src1->uiUintNum == src2->uiUintNum);
    BS_DBGASSERT(src1->uiUintNum == dst->uiUintNum);

    UINT i;
    for (i=0; i<dst->uiUintNum; i++) {
        dst->pulBitMap[i] = src1->pulBitMap[i] & src2->pulBitMap[i];
    }
}


void BITMAP_Xor(BITMAP_S *src1, BITMAP_S *src2, BITMAP_S *dst)
{
    BS_DBGASSERT(src1->uiUintNum == src2->uiUintNum);
    BS_DBGASSERT(src1->uiUintNum == dst->uiUintNum);

    UINT i;
    for (i=0; i<dst->uiUintNum; i++) {
        dst->pulBitMap[i] = src1->pulBitMap[i] ^ src2->pulBitMap[i];
    }
}

VOID BITMAP_Zero(IN BITMAP_S * pstBitMap)
{
    UINT i;

    for (i=0; i<pstBitMap->uiUintNum; i++) {
        pstBitMap->pulBitMap[i] = 0;
    }
}

static inline BS_STATUS bitmap_SetUintTo(BITMAP_S *pstBitMap, UINT uiStartUint,
        UINT uiCount, UINT to)
{
    UINT i;
    UINT count;

    if (uiStartUint >= pstBitMap->uiUintNum) {
        return BS_OUT_OF_RANGE;
    }

    count = MIN(pstBitMap->uiUintNum - uiStartUint, uiCount);

    for (i=0; i<count; i++) {
        pstBitMap->pulBitMap[uiStartUint + i] = to;
    }

    return BS_OK;
}

static inline BOOL_T bitmap_TestUint(BITMAP_S *pstBitMap, UINT uiStartUint,
        UINT uiCount)
{
    UINT i;
    UINT count;

    if (uiStartUint >= pstBitMap->uiUintNum) {
        BS_DBGASSERT(0);
        return FALSE;
    }

    count = MIN(pstBitMap->uiUintNum - uiStartUint, uiCount);

    for (i=0; i<count; i++) {
        if (pstBitMap->pulBitMap[uiStartUint + i] != 0xffffffff) {
            return FALSE;
        }
    }

    return TRUE;
}

void BITMAP_SetTo(BITMAP_S *pstBitMap, INT64 index, int to)
{
    BS_DBGASSERT(((to == 0) || (to == 1)));

    if (to) {
        BITMAP_SET(pstBitMap, index);
    } else {
        BITMAP_CLR(pstBitMap, index);
    }
}

static inline BS_STATUS bitmap_SetBitsTo(BITMAP_S *pstBitMap, INT64 start_bit, UINT uiCount, int to)
{
    UINT uiEndBit = start_bit + uiCount;
    UINT uiTmpBitEnd;
    UINT uiStartUint, uiEndUint;
    UINT i;

    BS_DBGASSERT(((to == 0) || (to == 1)));

    if (start_bit >= pstBitMap->ulBitNum) {
        return BS_OUT_OF_RANGE;
    }

    if (uiEndBit > pstBitMap->ulBitNum) {
        uiEndBit = pstBitMap->ulBitNum;
    }

    uiTmpBitEnd = NUM_UP_ALIGN(start_bit, 32);
    uiTmpBitEnd = MIN(uiTmpBitEnd, uiEndBit);

    for (i=start_bit; i<uiTmpBitEnd; i++) {
        BITMAP_SetTo(pstBitMap, i, to);
    }

    uiStartUint = uiTmpBitEnd/32;
    uiEndUint = uiEndBit/32;

    if (uiEndUint - uiStartUint) {
        bitmap_SetUintTo(pstBitMap, uiStartUint, uiEndUint - uiStartUint, to);
    }

    for (i=uiEndUint*32; i<uiEndBit; i++) {
        BITMAP_SetTo(pstBitMap, i, to);
    }

    return BS_OK;
}

static inline BOOL_T bitmap_TestBits(BITMAP_S *pstBitMap, INT64 start_bit, UINT uiCount)
{
    UINT uiEndBit = start_bit + uiCount;
    UINT uiTmpBitEnd;
    UINT uiStartUint, uiEndUint;
    UINT i;

    if (start_bit >= pstBitMap->ulBitNum) {
        BS_DBGASSERT(0);
        return FALSE;
    }

    if (uiEndBit > pstBitMap->ulBitNum) {
        uiEndBit = pstBitMap->ulBitNum;
    }

    uiTmpBitEnd = NUM_UP_ALIGN(start_bit, 32);
    uiTmpBitEnd = MIN(uiTmpBitEnd, uiEndBit);

    for (i=start_bit; i<uiTmpBitEnd; i++) {
        if (! BITMAP_ISSET(pstBitMap, i)) {
            return FALSE;
        }
    }

    if (uiEndBit >= 32) {
        uiStartUint = uiTmpBitEnd/32;
        uiEndUint = uiEndBit/32;

        if (uiEndUint - uiStartUint) {
            if (! bitmap_TestUint(pstBitMap, uiStartUint,
                    uiEndUint - uiStartUint)) {
                return FALSE;
            }
        }

        for (i=uiEndUint*32; i<uiEndBit; i++) {
            if (! BITMAP_ISSET(pstBitMap, i)) {
                return FALSE;
            }
        }
    }

    return TRUE;
}

BS_STATUS BITMAP_SetUint(BITMAP_S * pstBitMap, UINT uiStartUint, UINT uiCount)
{
    return bitmap_SetUintTo(pstBitMap, uiStartUint, uiCount, 0xffffffff);
}

void BITMAP_ClrAll(BITMAP_S *pstBitMap)
{
    memset(pstBitMap->pulBitMap, 0, pstBitMap->uiUintNum * sizeof(UINT));
}

BS_STATUS BITMAP_ClrUint(BITMAP_S * pstBitMap, UINT uiStartUint, UINT uiCount)
{
    return bitmap_SetUintTo(pstBitMap, uiStartUint, uiCount, 0);
}

BS_STATUS BITMAP_SetBits(BITMAP_S *pstBitMap, INT64 start_bit, UINT uiCount)
{
    return bitmap_SetBitsTo(pstBitMap, start_bit, uiCount, 0xffffffff);
}

BS_STATUS BITMAP_ClrBits(BITMAP_S *pstBitMap, INT64 start_bit, UINT uiCount)
{
    return bitmap_SetBitsTo(pstBitMap, start_bit, uiCount, 0);
}

BOOL_T BITMAP_IsSetBits(BITMAP_S *pstBitMap, INT64 start_bit, UINT uiCount)
{
    return bitmap_TestBits(pstBitMap, start_bit, uiCount);
}

BOOL_T BITMAP_IsAllSetted(BITMAP_S *pstBitMap)
{
    return BITMAP_IsSetBits(pstBitMap, 0, pstBitMap->ulBitNum);
}
