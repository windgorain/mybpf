/******************************************************************************
* Copyright (C), LiXingang
* Author:      LiXingang  Date: 2016-3-17
* Description: 
* History:     
******************************************************************************/

#ifndef __NETFW_UTL_H_
#define __NETFW_UTL_H_

BS_STATUS WindowsFirewallInitialize(OUT HANDLE *phFwProfile);
VOID WindowsFirewallCleanup(IN HANDLE hFwProfile);
BS_STATUS WindowsFirewallIsOn(IN HANDLE hFwProfile, OUT BOOL_T *bfwOn);
BS_STATUS WindowsFirewallTurnOn(IN HANDLE hFwProfile);
BS_STATUS WindowsFirewallTurnOff(IN HANDLE hFwProfile);
BS_STATUS WindowsFirewallAppIsEnabled
(
    IN HANDLE hFwProfile,
    IN wchar_t* fwProcessImageFileName,
    OUT BOOL_T *bFwAppEnabled
);
BS_STATUS WindowsFirewallAddApp
(
    IN HANDLE hFwProfile,
    IN const wchar_t* fwProcessImageFileName,
    IN const wchar_t* fwName
);
BS_STATUS WindowsFirewallPortIsEnabled
(
    IN HANDLE hFwProfile,
    IN LONG portNumber,
    IN UINT ipProtocol, 
    OUT BOOL_T * bFwPortEnabled
);
BS_STATUS WindowsFirewallPortAdd
(
    IN HANDLE hFwProfile,
    IN LONG portNumber,
    IN UINT ipProtocol,
    IN const wchar_t* name
);


#endif 


