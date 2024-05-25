/******************************************************************************
* Copyright (C), LiXingang
* Author:      LiXingang  Date: 2016-3-9
* Description: 
* History:     
******************************************************************************/

#ifndef __SIGNAL_SLOT_UTL_H_
#define __SIGNAL_SLOT_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

typedef HANDLE SIG_SLOT_HANDLE;

typedef struct
{
    HANDLE ahParams[4];
}SIG_SLOT_PARAM_S;

typedef BS_STATUS (*PF_SIG_SLOT_RECEIVER_FUNC)
(
    IN CHAR *pcSender,
    IN CHAR *pcSignal,
    IN SIG_SLOT_PARAM_S *pstParam,
    IN USER_HANDLE_S *pstSlotUserHandle
);

SIG_SLOT_HANDLE SigSlot_Create();
BS_STATUS SigSlot_Connect
(
    IN SIG_SLOT_HANDLE hSigSlot,
    IN CHAR *pcSender,
    IN CHAR *pcSignal,
    IN CHAR *pcReceiver,
    IN PF_SIG_SLOT_RECEIVER_FUNC pfSlotFunc,
    IN USER_HANDLE_S *pstSlotUserHandle
);
VOID SigSlot_DisConnect
(
    IN SIG_SLOT_HANDLE hSigSlot,
    IN CHAR *pcSender,
    IN CHAR *pcSignal,
    IN CHAR *pcReceiver,
    IN PF_SIG_SLOT_RECEIVER_FUNC pfSlotFunc
);
BS_STATUS SigSlot_SendSignal
(
    IN SIG_SLOT_HANDLE hSigSlot,
    IN CHAR *pcSender,
    IN CHAR *pcSignal,
    IN SIG_SLOT_PARAM_S *pstParam
);

#ifdef __cplusplus
    }
#endif 

#endif 


