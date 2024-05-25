/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2010-5-20
* Description: 
* History:     
******************************************************************************/

#ifndef __QUE_UTL_H_
#define __QUE_UTL_H_

#include "utl/list_stq.h"
#ifdef __cplusplus
    extern "C" {
#endif 

#define QUE_FLAG_OVERWRITE 0x1 

#if 1
typedef HANDLE QUE_HANDLE;
QUE_HANDLE QUE_Create(IN UINT uiCapacity, IN UINT uiFlag);
VOID QUE_Destory(IN QUE_HANDLE hQue);
UINT QUE_Count(IN QUE_HANDLE hQue);
BS_STATUS QUE_Push(IN QUE_HANDLE hQue, IN HANDLE hItem);
BS_STATUS QUE_Pop(IN QUE_HANDLE hQue, OUT HANDLE *phItem);
BS_STATUS QUE_ExtractPop(IN QUE_HANDLE hQue, IN HANDLE hItem, OUT HANDLE *phItemFound);
#endif

#if 1
typedef HANDLE BYTE_QUE_HANDLE;
#define BYTE_QUE_POP_ERR 0xFFFFFFFF
BYTE_QUE_HANDLE ByteQue_Create(IN UINT uiCapacity, IN UINT uiFlag);
VOID ByteQue_Destory(IN BYTE_QUE_HANDLE hQue);
VOID ByteQue_Clear(IN BYTE_QUE_HANDLE hQue);
UINT ByteQue_Count(IN BYTE_QUE_HANDLE hQue);
BOOL_T ByteQue_IsFull(IN BYTE_QUE_HANDLE hQue);
BS_STATUS ByteQue_Push(IN BYTE_QUE_HANDLE hQue, IN UCHAR ucChar);
UINT ByteQue_Pop(IN QUE_HANDLE hQue);

UINT ByteQue_PopTail(IN QUE_HANDLE hQue);
UINT ByteQue_PopSpec(IN QUE_HANDLE hQue, IN UINT uiIndex);
UINT ByteQue_PeekOne(IN IN QUE_HANDLE hQue, IN UINT uiIndex);

UINT ByteQue_Peek
(
    IN IN QUE_HANDLE hQue, 
    IN UINT uiIndex,
    OUT UCHAR *pucBuf,
    IN UINT uiCount
);
#endif

#if 1 
typedef struct _qnode {
    void *data;
    struct _qnode *next;
} SQUE_NODE_S;

typedef struct _queue {
    SQUE_NODE_S * head, *tail;
    int count;
} SQUE_S;
void SQUE_Init(SQUE_S * s);
void SQUE_Final(SQUE_S * s);
int SQUE_Push(SQUE_S * s, void *data);
void * SQUE_Pop(SQUE_S *s);
int SQUE_Count(SQUE_S * s);
#endif


#if 1 

typedef void (*PF_BLOCK_QUE_FREE)(void *node, void *ud);

typedef struct BLOCK_QUE_STRUCT *BLOCKQUE_HANDLE;

BLOCKQUE_HANDLE BlockQue_Create();
void BlockQue_Destroy(BLOCKQUE_HANDLE q, PF_BLOCK_QUE_FREE free_func, void *ud);
void BlockQue_DelAll(BLOCKQUE_HANDLE q, PF_BLOCK_QUE_FREE free_func, void *ud);
BOOL_T BlockQue_IsEmpty(BLOCKQUE_HANDLE q);
BOOL_T BlockQue_IsNeedWake(BLOCKQUE_HANDLE q);
void BlockQue_SetNeedWait(BLOCKQUE_HANDLE q, BOOL_T need);
void BlockQue_Put(BLOCKQUE_HANDLE q, STQ_NODE_S *node);

void * BlockQue_Poll(BLOCKQUE_HANDLE q);

void * BlockQue_Take(BLOCKQUE_HANDLE q);

void * BlockQue_CondTake(BLOCKQUE_HANDLE q);
void * BlockQue_Peek(BLOCKQUE_HANDLE q);
int BlockQue_Count(BLOCKQUE_HANDLE q);

#endif

#ifdef __cplusplus
    }
#endif 

#endif 


