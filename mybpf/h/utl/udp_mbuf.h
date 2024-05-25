/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _UDP_MBUF_H
#define _UDP_MBUF_H

#include "utl/mbuf_utl.h"

#ifdef __cplusplus
extern "C"
{
#endif

UDP_HEAD_S * UDP_GetUDPHeaderByMbuf(IN MBUF_S *pstMbuf, IN NET_PKT_TYPE_E enPktType);


BS_STATUS UDP_BuilderHeader
(
    IN MBUF_S *pstMbuf,
    IN USHORT usSrcPort,
    IN USHORT usDstPort,
    IN UINT uiSrcIp,
    IN UINT uiDstIp
);

#ifdef __cplusplus
}
#endif
#endif 
