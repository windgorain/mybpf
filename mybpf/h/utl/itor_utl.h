/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2009-1-30
* Description: 
* History:     
******************************************************************************/

#ifndef __ITOR_UTL_H_
#define __ITOR_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

typedef VOID (*PF_ITOR_DEL_NOTIFY_FUNC) \
        (IN VOID *pstNodeToDel, IN HANDLE hItorInstanceHandle, IN HANDLE hItorId, IN USER_HANDLE_S *pstUserHandle);

BS_STATUS ITOR_CreateInstance
(
    IN PF_ITOR_DEL_NOTIFY_FUNC pfFunc,
    IN USER_HANDLE_S *pstUserHandle,
    OUT HANDLE *phInstanceHandle
);
BS_STATUS ITOR_DelInstace(IN HANDLE ulInstanceId);
VOID ITOR_NotifyDelNode(IN HANDLE ulInstanceId, IN VOID *pstNode2Del);
BS_STATUS ITOR_CreateItor(IN HANDLE ulInstanceId, OUT HANDLE *pulItorId);
BS_STATUS ITOR_DelItor(IN HANDLE ulInstanceId, IN HANDLE ulItorId);
BS_STATUS ITOR_SetHandle(IN HANDLE ulItorId, IN HANDLE ulHandle);
HANDLE ITOR_GetHandle(IN HANDLE hItor);

#ifdef __cplusplus
    }
#endif 

#endif 


