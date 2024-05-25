/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2008-9-4
* Description: 
* History:     
******************************************************************************/

#ifndef __IPPOOL_UTL_H_
#define __IPPOOL_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

typedef HANDLE IPPOOL_HANDLE;

typedef int (*PF_IPPOOL_WALK_FUNC)(IN UINT uiIp, IN HANDLE hUserHandle);

IPPOOL_HANDLE IPPOOL_Create();

VOID IPPOOL_Destory(IN IPPOOL_HANDLE hIpPoolHandle);


BS_STATUS IPPOOL_AddRange
(
    IN IPPOOL_HANDLE hIpPoolHandle,
    IN UINT uiBeginIP,
    IN UINT uiEndIP
);

BS_STATUS IPPOOL_DelRange
(
    IN IPPOOL_HANDLE hIpPoolHandle,
    IN UINT uiBeginIP,
    IN UINT uiEndIP
);

BS_STATUS IPPOOL_ModifyRange
(
    IN IPPOOL_HANDLE hIpPoolHandle,
    IN UINT uiOldBeginIP,
    IN UINT uiOldEndIP,
    IN UINT uiBeginIP,
    IN UINT uiEndIP
);


BOOL_T IPPOOL_IsOverlap
(
    IN IPPOOL_HANDLE hIpPoolHandle,
    IN UINT uiBeginIP,
    IN UINT uiEndIP
);


VOID IPPOOL_Deny(IN IPPOOL_HANDLE hIpPoolHandle, IN UINT uiDenyIp );

VOID IPPOOL_Permit(IN IPPOOL_HANDLE hIpPoolHandle, IN UINT uiIP );

VOID IPPOOL_PermitAll(IN IPPOOL_HANDLE hIpPoolHandle);


UINT IPPOOL_AllocIP (IN IPPOOL_HANDLE hIpPoolHandle, IN UINT uiRequestIp);


BS_STATUS IPPOOL_AllocSpecIP(IN IPPOOL_HANDLE hIpPoolHandle, IN UINT uiSpecIp);


VOID IPPOOL_FreeIP (IN IPPOOL_HANDLE hIpPoolHandle, IN UINT ulIp);

VOID IPPOOL_WalkBusy
(
    IN IPPOOL_HANDLE hIpPoolHandle,
    IN PF_IPPOOL_WALK_FUNC pfFunc,
    IN HANDLE hUserHandle
);

#ifdef __cplusplus
    }
#endif 

#endif 


