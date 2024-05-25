/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      lixingang  Version: 1.0  Date: 2007-7-2
* Description: 
* History:     
******************************************************************************/

#ifndef __BITMAP_UTL_H_
#define __BITMAP_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

typedef struct
{
    UINT *pulBitMap;
    UINT ulBitNum;
    UINT uiUintNum;
    UINT uiScanIndex;
}BITMAP_S;

#define BITMAP_NEED_UINTS(bitcount) (((bitcount) + 31)/32)
#define BITMAP_NEED_BYTES(bitcount) (BITMAP_NEED_UINTS(bitcount)*4)

#define BITMAP_GET_UINT_NUM(pstBitMap) ((pstBitMap)->uiUintNum)
#define BITMAP_GET_BIT_NUM(pstBitMap) ((pstBitMap)->ulBitNum)
#define BITMAP_ISSET(pstBitMap,uiIndex) \
    (((pstBitMap)->pulBitMap)[(uiIndex)/32] & (1 << ((uiIndex) % 32)))
#define BITMAP_SET(pstBitMap,uiIndex)   \
    (((pstBitMap)->pulBitMap)[(uiIndex)/32] |= (1 << ((uiIndex) % 32)))
#define BITMAP_CLR(pstBitMap,uiIndex)   \
    (((pstBitMap)->pulBitMap)[(uiIndex)/32] &= ~(1 << ((uiIndex) % 32)))

#define BITMAP_SCAN_SETTED_BEGIN(pstBitMap, uiIndex)  \
    do {\
        UINT _i, _j; \
        for (_i=0; _i<(pstBitMap)->uiUintNum; _i++) \
        {   \
            if ((pstBitMap)->pulBitMap[_i] == 0) continue; \
            for (_j=0; _j<32; _j++)    \
            {   \
                if (((pstBitMap)->pulBitMap[_i] & (1 << _j)) == 0) continue;  \
                uiIndex = _i*32 + _j;  \
                if (uiIndex >= (pstBitMap)->ulBitNum) break;

#define BITMAP_SCAN_UNSETTED_BEGIN(pstBitMap, uiIndex)  \
    do {\
        UINT _i, _j; \
        for (_i=0; _i<(pstBitMap)->uiUintNum; _i++) \
        {   \
            if ((pstBitMap)->pulBitMap[_i] == 0xffffffff) continue; \
            for (_j=0; _j<32; _j++)    \
            {   \
                if ((pstBitMap)->pulBitMap[_i] & (1 << _j)) continue;  \
                uiIndex = _i*32 + _j;  \
                if (uiIndex >= (pstBitMap)->ulBitNum) break;

    
#define BITMAP_SCAN_END()  \
            } \
        } \
    }while(0)

void BITMAP_Init(BITMAP_S *pstBitMap, UINT bitnum, UINT *bitmap_mem);
void BITMAP_Fini(BITMAP_S *pstBitMap);
BS_STATUS BITMAP_Create(IN BITMAP_S * pstBitMap, UINT ulBitNum);
BS_STATUS BITMAP_Destory(IN BITMAP_S * pstBitMap);


INT64 BITMAP_GetBusy(IN BITMAP_S * pstBitMap);


INT64 BITMAP_GetBusyFrom(BITMAP_S *pstBitMap, INT64 from);


INT64 BITMAP_GetFree(IN BITMAP_S * pstBitMap);

INT64 BITMAP_GetFreeFrom(BITMAP_S *pstBitMap, INT64 from);


INT64 BITMAP_GetFreeCycle(IN BITMAP_S *pstBitMap);


INT64 BITMAP_2GetUnsettedIndex(IN BITMAP_S * pstBitMap1, IN BITMAP_S * pstBitMap2);


INT64 BITMAP_2GetSettedIndex(IN BITMAP_S * pstBitMap1, IN BITMAP_S * pstBitMap2);

VOID BITMAP_Zero(IN BITMAP_S * pstBitMap);

void BITMAP_SetTo(BITMAP_S *pstBitMap, INT64 index, int to);

BS_STATUS BITMAP_SetUint(BITMAP_S *pstBitMap, UINT uiStartUint, UINT uiCount);
void BITMAP_ClrAll(BITMAP_S *pstBitMap);
BS_STATUS BITMAP_ClrUint(BITMAP_S *pstBitMap, UINT uiStartUint, UINT uiCount);
BS_STATUS BITMAP_SetBits(BITMAP_S *pstBitMap, INT64 start_bit, UINT uiCount);
BS_STATUS BITMAP_ClrBits(BITMAP_S *pstBitMap, INT64 start_bit, UINT uiCount);
BOOL_T BITMAP_IsSetBits(BITMAP_S *pstBitMap, INT64 start_bit, UINT uiCount);
BOOL_T BITMAP_IsAllSetted(BITMAP_S *pstBitMap);


void BITMAP_Copy(BITMAP_S *src, BITMAP_S *dst);

void BITMAP_Not(BITMAP_S *src, BITMAP_S *dst);

void BITMAP_Or(BITMAP_S *src1, BITMAP_S *src2, BITMAP_S *dst);

void BITMAP_And(BITMAP_S *src1, BITMAP_S *src2, BITMAP_S *dst);

void BITMAP_Xor(BITMAP_S *src1, BITMAP_S *src2, BITMAP_S *dst);

#ifdef __cplusplus
    }
#endif 

#endif


