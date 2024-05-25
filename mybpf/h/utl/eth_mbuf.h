/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _ETH_MBUF_H
#define _ETH_MBUF_H
#include "utl/mbuf_utl.h"
#ifdef __cplusplus
extern "C"
{
#endif

BS_STATUS ETH_GetEthHeadInfoByMbuf(IN MBUF_S *pstMbuf, OUT ETH_PKT_INFO_S *pstHeadInfo);

int ETH_ToEthPkt(void *data, int len, OUT void **new_data, OUT int *new_len);
BOOL_T ETH_IsMulticastPktByMbuf(IN MBUF_S *pstMbuf);
BS_STATUS ETH_PadPacket(IN MBUF_S *pstMBuf, IN BOOL_T bHaveEthHead);

#ifdef __cplusplus
}
#endif
#endif 
