/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2014-7-6
* Description: 
* History:     
******************************************************************************/

#ifndef __MYPOLL_INNER_H_
#define __MYPOLL_INNER_H_

#include "utl/mutex_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 

#define _MYPOLL_FLAG_RESTART 0x1
#define _MYPOLL_FLAG_PROCESSING_EVENT 0x2 
#define _MYPOLL_FLAG_LOOP_ODD         0x4 

#define _MYPOLL_MAX_SINGAL_NUM  32

typedef struct {
    UINT uiCount;
    PF_MYPOLL_SIGNAL_FUNC pfFunc;
}_MYPOLL_SIGNAL_S;

typedef struct
{
    VOID *pProto;
    VOID *pProtoHandle;

    INT iSocketSrc;
    INT iSocketDst;

    UINT uiFlag;

    PF_MYPOLL_USER_EVENT_FUNC pfUserEventFunc;
    USER_HANDLE_S stUserEventUserHandle;
    UINT uiUserEvent;
    MUTEX_S lock;

    _MYPOLL_SIGNAL_S singal_processers[_MYPOLL_MAX_SINGAL_NUM];

    DARRAY_HANDLE hFdInfoTbl;
}_MYPOLL_CTRL_S;

#define MYPOLL_PROTO_FLAG_AT_ONCE 0x1  

typedef BS_STATUS (*PF_Mypoll_Proto_Init)(IN _MYPOLL_CTRL_S *pstMyPoll);
typedef VOID (*PF_Mypoll_Proto_Fini)(IN _MYPOLL_CTRL_S *pstMyPoll);
typedef BS_STATUS (*PF_Mypoll_Proto_Add)
(
    IN _MYPOLL_CTRL_S *pstMyPoll,
    IN INT iSocketId,
    IN UINT uiEvent,
    IN PF_MYPOLL_EV_NOTIFY pfNotifyFunc
);
typedef BS_STATUS (*PF_Mypoll_Proto_Set)
(
    IN _MYPOLL_CTRL_S *pstMyPoll,
    IN INT iSocketId,
    IN UINT uiEvent,
    IN PF_MYPOLL_EV_NOTIFY pfNotifyFunc
);
typedef VOID (*PF_Mypoll_Proto_Del)
(
    IN _MYPOLL_CTRL_S *pstMyPoll,
    IN INT iSocketId
);
typedef int (*PF_Mypoll_Proto_Run)(IN _MYPOLL_CTRL_S *pstMyPoll);

typedef struct
{
    UINT uiFlag;
    PF_Mypoll_Proto_Init pfInit;
    PF_Mypoll_Proto_Fini pfFini;
    PF_Mypoll_Proto_Add pfAdd;
    PF_Mypoll_Proto_Set pfSet;
    PF_Mypoll_Proto_Del pfDel;
    PF_Mypoll_Proto_Run pfRun;
}MYPOLL_PROTO_S;

#define _MYPOLL_FDINFO_LOOP_ODD 0x4 

typedef struct
{
    UINT uiEvent;
    UINT flag;
    PF_MYPOLL_EV_NOTIFY pfNotifyFunc;
    USER_HANDLE_S stUserHandle;
}MYPOLL_FDINFO_S;

#ifdef __cplusplus
    }
#endif 

#endif 


