/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2012-9-10
* Description: 
* History:     
******************************************************************************/

#ifndef __MY_IP_HELPER_H_
#define __MY_IP_HELPER_H_

#ifdef __cplusplus
    extern "C" {
#endif 

typedef VOID (*PF_MY_IP_HELPER_NOTIFY_ADDR_CHANGE)(IN USER_HANDLE_S *pstUserHandle);

BS_STATUS My_IP_Helper_GetAdapterIndex(IN CHAR *pcAdapterGuid, OUT ULONG *index);
VOID My_IP_Helper_DeleteAllIpAddress(ULONG adapterIndex);
VOID My_IP_Helper_DeleteIpAddress(ULONG adapterIndex, UINT uiIp);
BS_STATUS My_IP_Helper_AddIPAddress(ULONG adapterIndex, UINT uiIp, UINT uiMask);
BS_STATUS My_IP_Helper_GetIPAddress(CHAR *pcAdapterName, OUT UINT *puiIp, OUT UINT *puiMask);
BS_STATUS My_Ip_Helper_RegAddrNotify(PF_MY_IP_HELPER_NOTIFY_ADDR_CHANGE pfFunc, USER_HANDLE_S *pstUserHandle);
VOID My_IP_Helper_DeleteArpsOfAdapter(ULONG adapter_index);


BS_STATUS My_IP_Helper_DelAllRouteByNexthop(IN UINT uiNexthop );


BS_STATUS My_IP_Helper_DelAllRouteByAdapterindex(ULONG adapter_index);


UINT My_IP_Helper_CountRouteByAdapterIndex(ULONG adapter_index);

VOID My_IP_Helper_DelRoute
(
    IN UINT uiDstIp,
    IN UINT uiMask,
    IN UINT uiNextHop,
    IN UINT uiOutIfIndex
);

BS_STATUS My_IP_Helper_AddRoute
(
    IN UINT uiDstIp,
    IN UINT uiMask,
    IN UINT uiNextHop,
    IN UINT uiOutIfIndex
);

#ifdef IN_WINDOWS
MIB_IPFORWARDTABLE * My_IP_Helper_GetRouteTbl();
VOID My_IP_Helper_FreeRouteTbl(IN MIB_IPFORWARDTABLE *pstRouteTbl);
MIB_IPFORWARDROW *My_IP_Helper_FindDftRouteByTbl(IN MIB_IPFORWARDTABLE *pstRouteTbl);
unsigned int My_IP_Helper_GetDftGateway();
VOID My_IP_Helper_DelRoute2(IN MIB_IPFORWARDROW *pstRoute);
BS_STATUS My_IP_Helper_AddRoute2(IN MIB_IPFORWARDROW *pstRoute);
BS_STATUS My_IP_Helper_ModifyRoute(IN MIB_IPFORWARDROW *pstRoute);
BS_STATUS My_IP_Helper_SetRoute2(IN MIB_IPFORWARDROW *pstRoute);

#define MY_IP_HELPER_SCAN_ROUTE_TBL_START(_pstRouteTbl, _pstRow) \
    do { \
        UINT _i;    \
        for (_i=0; _i<(_pstRouteTbl)->dwNumEntries; _i++)   \
        {   \
            (_pstRow) = &(_pstRouteTbl)->table[_i];

#define MY_IP_HELPER_SCAN_ROUTE_TBL_END()   }}while(0)
#endif

#ifdef __cplusplus
    }
#endif 

#endif 


