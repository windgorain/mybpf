/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _IP6_UTL_H
#define _IP6_UTL_H

#include "utl/ip4_utl.h"
#include "os/linux_kernel_pile.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define IP6_ADDR_LEN 16
#define IP6_HDR_LEN 40

#pragma pack(1)

#ifndef s6_addr
struct in6_addr
{
    union
    {
        uint8_t u6_addr8[16];
        uint16_t u6_addr16[8];
        uint32_t u6_addr32[4];
    } in6_u;
#define s6_addr         in6_u.u6_addr8
#define s6_addr16       in6_u.u6_addr16
#define s6_addr32       in6_u.u6_addr32
};
#endif

#ifdef IN_MAC
#define s6_addr32	__u6_addr.__u6_addr32
#endif

typedef struct ip6 {
    UINT            vcl;      
    USHORT           len;      
    UCHAR           next;     
    UCHAR           hop_lmt;  
    struct in6_addr ip6_src;  
    struct in6_addr ip6_dst;  
} IP6_HEAD_S;

typedef struct {
    UCHAR next;
    UCHAR len;
}IP6_OPT_S;

typedef struct {
    UCHAR  next;        
    UCHAR  len;        
    UCHAR  type;       
    UCHAR  seg_left;    
    
}IP6_OPT_ROUTE_S;


typedef struct tagIP6_FRAG {
    UCHAR  next;        
    UCHAR  reserved;   
    USHORT offset_flag;      
    UINT id;      
}IP6_FRAG_S;
#pragma pack()

typedef struct {
    void *data;
    int len;
    UCHAR protocol;
}IP6_UPLAYER_S;

char * inet_ntop6_full(const struct in6_addr *addr, char *dst, socklen_t size);
IP6_HEAD_S * IP6_GetIPHeader(UCHAR *pucData, UINT uiDataLen, NET_PKT_TYPE_E enPktType);
int IP6_GetUpLayer(IP6_HEAD_S *ip6_header, int len, OUT IP6_UPLAYER_S *uplayer);

static inline UCHAR IP6_HEAD_DSCP(IP6_HEAD_S *ip6_header)
{
    UINT vcl = ntohl(ip6_header->vcl);
    UCHAR dscp = (vcl >> 22) & 0x3f;

    return dscp;
}

static inline UINT IP6_HEAD_FLOW_LABLE(IP6_HEAD_S *ip6_header)
{
    UINT vcl = ntohl(ip6_header->vcl);
    return vcl & 0xfffff;
}

static inline UINT IP6_HEAD_FLOW_INFO(IP6_HEAD_S *ip6_header)
{
    UINT vcl = ntohl(ip6_header->vcl);
    return vcl & 0x0fffffff;
}

static inline UINT IP6_HEAD_VERSION(IP6_HEAD_S *ip6_header)
{
    UINT vcl = ntohl(ip6_header->vcl);
    return (vcl >>28) & 0xf;
}

static inline void IP6_ADDR_COPY(void *dst, void *src) 
{
    UINT *a1 = dst;
    UINT *a2 = src;

    a1[0] = a2[0];
    a1[1] = a2[1];
    a1[2] = a2[2];
    a1[3] = a2[3];
}

static inline int IP6_ADDR_CMP(void *addr1, void *addr2)
{
    UINT *a1 = addr1;
    UINT *a2 = addr2;
    int i;

    for (i=0; i<4; i++) {
        if (a1[i] < a2[i]) {
            return -1;
        } else if (a1[i] > a2[i]) {
            return 1;
        }
    }

    return 0;
}

static inline void * IP6_ADDR_MIN(void *addr1, void *addr2) {
    if (IP6_ADDR_CMP(addr1, addr2) > 0) {
        return addr2;
    }
    return addr1;
}

static inline void * IP6_ADDR_MAX(void *addr1, void *addr2) {
    if (IP6_ADDR_CMP(addr1, addr2) < 0) {
        return addr2;
    }
    return addr1;
}

#ifdef __cplusplus
}
#endif
#endif 
