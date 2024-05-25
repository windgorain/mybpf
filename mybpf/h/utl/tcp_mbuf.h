/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _TCP_MBUF_H
#define _TCP_MBUF_H
#include "utl/mbuf_utl.h"
#ifdef __cplusplus
extern "C"
{
#endif

TCP_HEAD_S * TCP_GetTcpHeaderByMbuf(IN MBUF_S *pstMbuf, IN NET_PKT_TYPE_E enPktType);

#ifdef __cplusplus
}
#endif
#endif 
