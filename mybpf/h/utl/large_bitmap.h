/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2014-8-15
* Description: bitIndex从0开始
* History:     
******************************************************************************/

#ifndef __BITLEVEL_UTL_H_
#define __BITLEVEL_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

typedef VOID* LBITMAP_HANDLE;

typedef struct {
    void *memcap;
}LBITMAP_PARAM_S;

LBITMAP_HANDLE LBitMap_Create(LBITMAP_PARAM_S *p);
VOID LBitMap_Destory(IN LBITMAP_HANDLE hLBitMap);
void LBitMap_Reset(LBITMAP_HANDLE hLBitMap);

BS_STATUS LBitMap_AllocByRange
(
    IN LBITMAP_HANDLE hLBitMap,
    IN UINT uiRangeMin,
    IN UINT uiRangeMax,
    OUT UINT *puiBitIndex
);

BS_STATUS LBitMap_TryByRange
(
    IN LBITMAP_HANDLE hLBitMap,
    IN UINT uiRangeMin,
    IN UINT uiRangeMax,
    OUT UINT *puiBitIndex
);
BS_STATUS LBitMap_SetBit(IN LBITMAP_HANDLE hLBitMap, IN UINT uiBitIndex);
VOID LBitMap_ClrBit(IN LBITMAP_HANDLE hLBitMap, IN UINT uiBitIndex);
BOOL_T LBitMap_IsBitSetted(IN LBITMAP_HANDLE hLBitMap, IN UINT uiBitIndex);
BS_STATUS LBitMap_GetFirstBusyBit(IN LBITMAP_HANDLE hLBitMap, OUT UINT *puiBitIndex);
BS_STATUS LBitMap_GetNextBusyBit(IN LBITMAP_HANDLE hLBitMap, IN UINT uiCurrentBitIndex, OUT UINT *puiBitIndex);

#ifdef __cplusplus
    }
#endif 

#endif 


