/*================================================================
*   Created by LiXingang: 2018.11.19
*   Description: 
*
================================================================*/
#ifndef _IPCMD_UTL_H
#define _IPCMD_UTL_H
#ifdef __cplusplus
extern "C"
{
#endif


BS_STATUS IPCMD_AddIP(char *ifname, UINT ip, UINT mask, UINT peer);
BS_STATUS IPCMD_AddDns(char *ifname, UINT dnsip, UINT index);
BS_STATUS IPCMD_DelDns(char *ifname, UINT dnsip);
BS_STATUS IPCMD_SetDns(char *ifname, UINT dnsip);
BS_STATUS IPCMD_SetMtu(char *ifname, UINT mtu);


#ifdef __cplusplus
}
#endif
#endif 
