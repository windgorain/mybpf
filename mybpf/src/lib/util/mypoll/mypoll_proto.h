/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2014-7-5
* Description: 
* History:     
******************************************************************************/

#ifndef __MYPOLL_PROTO_H_
#define __MYPOLL_PROTO_H_

#ifdef __cplusplus
    extern "C" {
#endif 

 BS_STATUS _Mypoll_Proto_Init(IN _MYPOLL_CTRL_S *pstMyPoll);
VOID _MyPoll_Proto_Fini(IN _MYPOLL_CTRL_S *pstMyPoll);
BS_STATUS _Mypoll_Proto_Add
(
    IN _MYPOLL_CTRL_S *pstMyPoll,
    IN INT iSocketId,
    IN UINT uiEvent,
    IN PF_MYPOLL_EV_NOTIFY pfNotifyFunc
);
BS_STATUS _Mypoll_Proto_Set
(
    IN _MYPOLL_CTRL_S *pstMyPoll,
    IN INT iSocketId,
    IN UINT uiEvent,
    IN PF_MYPOLL_EV_NOTIFY pfNotifyFunc
);
VOID _Mypoll_Proto_Del
(
    IN _MYPOLL_CTRL_S *pstMyPoll,
    IN INT iSocketId
);

int _Mypoll_Proto_Run(IN _MYPOLL_CTRL_S *pstMyPoll);

extern MYPOLL_PROTO_S * Mypoll_Select_GetProtoTbl(void);
extern MYPOLL_PROTO_S * Mypoll_Epoll_GetProtoTbl(void);

#ifdef __cplusplus
    }
#endif 

#endif 


