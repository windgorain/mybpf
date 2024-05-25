/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _IP_UTL_H_
#define _IP_UTL_H_

#include "utl/net.h"
#include "utl/eth_utl.h"
#include "utl/ip4_utl.h"
#include "utl/ip6_utl.h"
#include "utl/ip46_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 

#define IP_INVALID_HEAD_OFFSET 0xffffffff

typedef struct {
    UINT uiIP;
    UINT uiMask;
}IP_MASK_S;

typedef struct {
    UINT uiIP;
    UCHAR ucPrefix;
}IP_PREFIX_S;

typedef struct {
    UINT ip;
    USHORT port;
}IP_PORT_S;

USHORT IP_CheckSum (IN UCHAR *pucBuf, IN UINT ulLen);
IP_HEAD_S * IP_GetIPHeader(IN UCHAR *pucData, IN UINT uiDataLen, IN NET_PKT_TYPE_E enPktType);
BOOL_T IPUtl_IsExistInIpArry(IN IP_MASK_S *pstIpMask, IN UINT uiNum, IN UINT uiIP, IN UINT uiMask);
BOOL_T IP_IsPrivateIp(UINT ip);

#ifdef __cplusplus
}
#endif 

#endif 

