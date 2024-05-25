/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      lixingang  Version: 1.0  Date: 2007-2-3
* Description: 
* History:     
******************************************************************************/
#ifndef __NET_H_
#define __NET_H_

#ifdef __cplusplus
extern "C" {
#endif 


#define LOOPBACK_IP 0x7f000001    

#ifdef IN_WINDOWS
#define NET_GET_ADDR_BY_SOCKADDR(pstSockAddr) (((struct sockaddr_in *)(pstSockAddr))->sin_addr.S_un.S_addr)
#endif

#ifdef IN_UNIXLIKE
#define NET_GET_ADDR_BY_SOCKADDR(pstSockAddr) (((struct sockaddr_in *)(pstSockAddr))->sin_addr.s_addr)
#endif

#define  PRINT_HIP(x)\
    (((x) >>  24) & 0xFF),\
    (((x) >>  16) & 0xFF),\
    (((x) >> 8) & 0xFF),\
    (((x) >> 0) & 0xFF)

#define PREFIX_2_COUNT(depth) (1UL << (depth)) 


static inline UINT PREFIX_2_MASK(IN UCHAR ucPrefixLen)
{
    
    if (ucPrefixLen == 32) {
        return 0xffffffff;
    }

    return ~(((UINT)0xFFFFFFFF) >> ucPrefixLen);
}


static inline UINT PREFIX_2_CAPACITY(IN UCHAR ucPrefixLen)
{
	return 1 << (32 - ucPrefixLen);
}

static inline UCHAR MASK_2_PREFIX(IN UINT uiMask)
{
    UINT i;

    for (i=0; i<32; i++) {
        if ((uiMask & (1 << (31 - i))) == 0) {
            return i;
        }
    }

    return 32;
}


static inline BOOL_T MASK_IS_VALID(IN UINT uiMask)
{
    if (uiMask == PREFIX_2_MASK(MASK_2_PREFIX(uiMask))) {
        return TRUE;
    }

    return FALSE;
}


static inline VOID IpMask_2_Range(IN UINT uiIp, IN UINT uiMask, OUT UINT *puiBeginIP, OUT UINT *puiEndIP)
{
    *puiBeginIP = uiIp & uiMask;
    *puiEndIP = uiIp | (~uiMask);
}


static inline BOOL_T IP_IsSubNetEdge(IN UINT uiIP, IN UINT uiMask)
{
    if ((uiIP & uiMask) == uiIP) {
        return TRUE;
    }
    if ((uiIP | (~uiMask)) == uiIP) {
        return TRUE;
    }
    return FALSE;
}


static inline UINT SubNet_2_Mask(IN UINT uiSubNetSelf, IN UINT uiSubNetBroadIP)
{
    return ~(uiSubNetSelf ^ uiSubNetBroadIP);
}


static inline BOOL_T IP_IsSubNetEdge2(IN UINT uiStart, IN UINT uiEnd)
{
    UINT uiMask = SubNet_2_Mask(uiStart, uiEnd);
    if(MASK_IS_VALID(uiMask)) {
        return TRUE;
    }
    return FALSE;
}


static inline UINT IP_GetMiniMask(IN UINT uiStartIP, IN UINT uiEndIP)
{
    UINT uiPrefix;
    UINT uiMask;

    for (uiPrefix=31; uiPrefix>0; uiPrefix--) {
        uiMask = PREFIX_2_MASK(uiPrefix);
        if ((uiStartIP & uiMask) == (uiEndIP & uiMask)) {
            return uiMask;
        }
    }

    return 0;
}


static inline UINT Range_GetFirstMask(IN UINT uiBeginIP, IN UINT uiEndIP)
{
    UINT uiPrefix;
    UINT uiMask = 0;
    UINT uiStart;
    UINT uiStop;

    for (uiPrefix=0; uiPrefix<=32; uiPrefix++) {
        uiMask = PREFIX_2_MASK(uiPrefix);
        IpMask_2_Range(uiBeginIP, uiMask, &uiStart, &uiStop);
        if ((uiStart >= uiBeginIP) && (uiStop <= uiEndIP)) {
            break;
        }
    }

    return uiMask;
}


static inline UINT IP_GetCommonPrefix(IN UINT uiIP1, IN UINT uiIP2)
{
    UINT i;

    for (i=0; i<32; i++) {
        if ((uiIP1 & (0x1 << (32- i))) != (uiIP2 & (0x1 << (32- i)))) {
            return i;
        }
    }

    return i;
}


#define IP_SPLIT_SUBNET_BY_MASK_BEGIN(_uiStartIP, _uiEndIP, _uiMask, _uiNetStart, _uiNetEnd) \
    do { \
        UINT _uiBeginTmp = _uiStartIP;  \
        UINT _uiNetStartTmp, _uiNetEndTmp;  \
        while (_uiBeginTmp <= _uiEndIP) {   \
            IpMask_2_Range(_uiBeginTmp, _uiMask, &_uiNetStartTmp, &_uiNetEndTmp);   \
            if (_uiNetStartTmp < _uiBeginTmp){ _uiNetStartTmp = _uiBeginTmp;} \
            if (_uiNetEndTmp > _uiEndIP){_uiNetEndTmp = _uiEndIP;} \
            _uiNetStart = _uiNetStartTmp;   \
            _uiNetEnd = _uiNetEndTmp;  \
            {

#define IP_SPLIT_SUBNET_BY_MASK_END() } _uiBeginTmp = _uiNetEndTmp + 1; }} while(0)


typedef struct
{
    UCHAR aucMac[6];
}MAC_ADDR_S;

typedef union
{
    MAC_ADDR_S stMacAddr;
    USHORT ausMacAddr[3];
}MAC_ADDR_U;

typedef union
{
    UCHAR aucIp[4];
    UINT uiIp;
}IP_ADDR_U;

#define IP_ADDR_4_CHAR(ip_addr) (ip_addr)->aucIp[0], (ip_addr)->aucIp[1], (ip_addr)->aucIp[2], (ip_addr)->aucIp[3]
#define IP_4_CHAR(aucIp) (aucIp)[0], (aucIp)[1], (aucIp)[2], (aucIp)[3]

#define IPv4(a, b, c, d) ((uint32_t)(((a) & 0xff) << 24) | \
				(((b) & 0xff) << 16) |     \
				(((c) & 0xff) << 8)  |     \
				((d) & 0xff))

#define NS_IN6ADDRSZ     16
#define IN6ADDR_SIZE8    NS_IN6ADDRSZ
#define IN6ADDR_SIZE16   8
#define IN6ADDR_SIZE32   4

typedef struct net_in6_addr {
    union
    {
        unsigned char   __u6_addr8[IN6ADDR_SIZE8];
        unsigned short  __u6_addr16[IN6ADDR_SIZE16];
        unsigned int    __u6_addr32[IN6ADDR_SIZE32];
    } __u6_addr;            
    
    #define net_s6_addr   __u6_addr.__u6_addr8
    #define net_s6_addr16 __u6_addr.__u6_addr16
    #define net_s6_addr32 __u6_addr.__u6_addr32
}IN6ADDR_S;

typedef struct
{
    UINT S_addr;
}INADDR_S;

typedef enum
{
    NET_PKT_TYPE_ETH = 0,
    NET_PKT_TYPE_IP,
    NET_PKT_TYPE_IP6,
    NET_PKT_TYPE_UDP,
    NET_PKT_TYPE_TCP,
    NET_PKT_TYPE_ICMP,
    NET_PKT_TYPE_VXLAN,

    NET_PKT_TYPE_MAX
}NET_PKT_TYPE_E;

#ifdef __cplusplus
}
#endif 

#endif 

