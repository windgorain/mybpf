/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _IP46_UTL_H
#define _IP46_UTL_H

#include "utl/ip6_utl.h"
#include "utl/sprintf_utl.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef union
{
    IP6_HEAD_S *ip6;
    IP_HEAD_S *ip4;
}IP46_HEAD_U;

typedef struct
{
    USHORT    family;
    IP46_HEAD_U iph;
}IP46_HEAD_S;

int IP46_GetIPHeader(IP46_HEAD_S *pstIpHead, UCHAR *pucData, UINT uiDataLen, NET_PKT_TYPE_E enPktType);
int IP46_Header2String(IP46_HEAD_S *iph, VOID *ippkt, CHAR *buf, UINT bufsize, CHAR dump_hex);

static inline int IP46_GetIPDataLen(IP46_HEAD_S *ipheader, int pktlen) 
{
    int payload_len = 0;

    if (ipheader->family == ETH_P_IP) {
        payload_len = ntohs(ipheader->iph.ip4->usTotlelen);
        payload_len = MIN(payload_len, pktlen);

        if (payload_len > IP_HEAD_LEN(ipheader->iph.ip4)) {
            payload_len -= IP_HEAD_LEN(ipheader->iph.ip4);
        }
    }else if (ipheader->family == ETH_P_IP6) {
        payload_len = ipheader->iph.ip6->len;
        payload_len = MIN(payload_len, pktlen - IP6_HDR_LEN);
    }

    return payload_len;
}

static inline char * IP46_IP2Str(UCHAR family, void *ip, OUT char *buf, int buf_size)
{
    unsigned char *ch;

    switch (family) {
        case AF_INET:
            ch = ip;
            scnprintf(buf, buf_size, "%u.%u.%u.%u", ch[0], ch[1], ch[2], ch[3]);
            break;
        case AF_INET6:
            inet_ntop6_full(ip, buf, buf_size);
            break;
    }

    return buf;
}

#ifdef __cplusplus
}
#endif
#endif 
