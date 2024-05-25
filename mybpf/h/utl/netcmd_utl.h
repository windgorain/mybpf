/*================================================================
*   Description: 
*
================================================================*/
#ifndef _NETCMD_UTL_H
#define _NETCMD_UTL_H
#ifdef __cplusplus
extern "C"
{
#endif


BS_STATUS NETCMD_getIfMac(char *pifName, unsigned char macAddr[MAC_ADDR_LEN]);
BS_STATUS NETCMD_setIfMac(char *pifName, unsigned char macAddr[MAC_ADDR_LEN]);
BS_STATUS NETCMD_setIfUp(char *pifName);
BS_STATUS NETCMD_setIfDown(char *pifName);
BS_STATUS NETCMD_getIfIp(char *pifName, char *ipAddr);
BS_STATUS NETCMD_getIfMask(char *pifName, char *ipMask);
BS_STATUS NETCMD_setIfIp(char *pifName, char *ipAddr, char *ipMask);
BS_STATUS NETCMD_delIfIp(char *pifName);
BS_STATUS NETCMD_addRoute(char *dstAddr, char *netmask, char *ifName, char *nextHop, unsigned char weight);
BS_STATUS NETCMD_delRoute(char *dstAddr, char *netmask, char *ifName);


#ifdef __cplusplus
}
#endif
#endif 
