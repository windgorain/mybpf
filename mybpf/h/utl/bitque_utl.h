/******************************************************************************
* Copyright (C) LiXingang
* Author:      LiXingang  Version: 1.0  Date: 2013-3-27
* Description: 
* History:     
******************************************************************************/

#ifndef __BITQUE_UTL_H_
#define __BITQUE_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

typedef struct
{
    BITMAP_S stBitMap;
    int size;    
    int count;   
    int read_index;
    int write_index;
}BITQUE_S;

BS_STATUS BITQUE_Init(BITQUE_S *pstBitQue, int size);
VOID BITQUE_Fini(IN BITQUE_S *pstBitQue);

int BITQUE_Enque(BITQUE_S *pstBitQue, int bit_value);

int BITQUE_Deque(BITQUE_S *pstBitQue);
int BITQUE_Get(BITQUE_S *pstBitQue, int index);

int BITQUE_TestDeque(BITQUE_S *pstBitQue);


#ifdef __cplusplus
    }
#endif 

#endif 


