/*================================================================
*   Created by LiXingang：2018.11.06
*   Description：
*
================================================================*/
#ifndef _ROUTE_UTL_H
#define _ROUTE_UTL_H
#ifdef __cplusplus
extern "C"
{
#endif

UINT Route_GetDefaultGw();
int RouteCmd_Add(unsigned int dst, unsigned int prefix_len, unsigned int nexthop);
int RouteCmd_Del(unsigned int dst, unsigned int prefix_len);

#ifdef __cplusplus
}
#endif
#endif 
