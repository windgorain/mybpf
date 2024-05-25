/*================================================================
* Author：LiXingang. Data: 2019.07.30
* Description: 
*
================================================================*/
#ifndef _ICMP_MBUF_H
#define _ICMP_MBUF_H
#ifdef __cplusplus
extern "C"
{
#endif

ICMP_HEAD_S * ICMP_GetIcmpHeaderByMbuf(IN MBUF_S *pstMbuf, IN NET_PKT_TYPE_E enPktType);

#ifdef __cplusplus
}
#endif
#endif 
