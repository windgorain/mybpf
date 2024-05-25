/******************************************************************************
* Copyright (C) LiXingang
* Author:      LiXingang  Version: 1.0  Date: 2013-3-27
* Description: 
* History:     
******************************************************************************/
#include "bs.h"

#include "utl/bitmap1_utl.h"
#include "utl/bitque_utl.h"

BS_STATUS BITQUE_Init(BITQUE_S *pstBitQue, int size)
{
    BS_STATUS eRet;

    BS_DBGASSERT(NULL != pstBitQue);

    Mem_Zero(pstBitQue, sizeof(BITQUE_S));

    eRet = BITMAP_Create(&pstBitQue->stBitMap, size);
    if (BS_OK != eRet) {
        return eRet;
    }

    return BS_OK;
}

VOID BITQUE_Fini(IN BITQUE_S *pstBitQue)
{
    BITMAP_Destory(&pstBitQue->stBitMap);
}


int BITQUE_Enque(BITQUE_S *pstBitQue, int bit_value)
{
    int index;
    
    if (pstBitQue->count >= pstBitQue->size) {
        return -1;
    }

    if (bit_value) {
        BITMAP_SET(&pstBitQue->stBitMap, pstBitQue->write_index);
    } else {
        BITMAP_CLR(&pstBitQue->stBitMap, pstBitQue->write_index);
    }

    index = pstBitQue->write_index;

    pstBitQue->write_index++;

    if (pstBitQue->write_index >= pstBitQue->size) {
        pstBitQue->write_index = 0;
    }

    pstBitQue->count ++;

    return index;
}


int BITQUE_Deque(BITQUE_S *pstBitQue)
{
    int ret;
    
    if (pstBitQue->count == 0) {
        return -1;
    }

    ret = 0;
    if (BITMAP_ISSET(&pstBitQue->stBitMap, pstBitQue->read_index)) {
        ret = 1;
    }

    pstBitQue->read_index ++;
    if (pstBitQue->read_index >= pstBitQue->size) {
        pstBitQue->read_index = 0;
    }

    pstBitQue->count --;

    return ret;
}

int BITQUE_Get(BITQUE_S *pstBitQue, int index)
{
    if (index >= pstBitQue->size) {
        return -1;
    }

    if (pstBitQue->write_index > pstBitQue->read_index) {
        if (index >= pstBitQue->write_index) {
            return -1;
        }
        if (index < pstBitQue->read_index) {
            return -1;
        }
    } else if (pstBitQue->write_index < pstBitQue->read_index) {
        if ((index >= pstBitQue->write_index) &&
                (index < pstBitQue->read_index)) {
            return -1;
        }
    }

    if (BITMAP_ISSET(&pstBitQue->stBitMap, index)) {
        return 1;
    }

    return 0;
}


int BITQUE_TestDeque(BITQUE_S *pstBitQue)
{
    int ret;
    
    if (pstBitQue->count == 0) {
        return -1;
    }

    ret = 0;
    if (BITMAP1_ISSET(&pstBitQue->stBitMap, pstBitQue->read_index)) {
        ret = 1;
    }

    return ret;
}

