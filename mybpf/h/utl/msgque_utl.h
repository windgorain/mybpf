/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2007-5-8
* Description: 
* History:     
******************************************************************************/

#ifndef __MSGQUE_H_
#define __MSGQUE_H_

#ifdef __cplusplus
    extern "C" {
#endif 

#define MSGQUE_EVENT_READ    0x1
#define MSGQUE_EVENT_WRITE   0x2

#define MSGQUE_DEPETH_NO_LIMIT 0xffffffff

#define MSGQUE_NAME_MAX_LEN  11

typedef HANDLE MSGQUE_HANDLE;

typedef VOID (*PF_QUE_EVENT_FUNC)(IN MSGQUE_HANDLE hQueId, IN UINT ulEvent, IN UINT ulUserHandle);

typedef struct
{
    HANDLE ahMsg[4];  
}MSGQUE_MSG_S;

typedef struct{
    UINT      capacity;
    UINT      mask;
    volatile  UINT prod;
    volatile  UINT cons;
    MSGQUE_MSG_S msgs[0];
}MSGQUE_S;

MSGQUE_S * MSGQUE_Create(UINT capacity);
void MSGQUE_Delete(MSGQUE_S *q);
BS_STATUS MSGQUE_WriteMsg(MSGQUE_S *q, MSGQUE_MSG_S *pstMsg);
BS_STATUS MSGQUE_ReadMsg(MSGQUE_S *q, OUT MSGQUE_MSG_S *pstMsg);
UINT MSGQUE_Count(MSGQUE_S *q);
UINT MSGQUE_FreeCount(MSGQUE_S *q);
int MSGQUE_Full(MSGQUE_S *q);
int MSGQUE_Empty(MSGQUE_S *q);

#ifdef __cplusplus
    }
#endif 

#endif 


