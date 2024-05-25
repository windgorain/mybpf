/******************************************************************************
* Copyright (C), LiXingang
* Author:      LiXingang  Date: 2016-6-15
* Description: 
* History:     
******************************************************************************/

#ifndef __SOCKET_IN_H_
#define __SOCKET_IN_H_

#include "utl/net.h"

#ifdef __cplusplus
    extern "C" {
#endif 


#ifndef INET_ADDRSTRLEN
#define INET_ADDRSTRLEN 15    
#endif

#ifndef INET6_ADDRSTRLEN
#define INET6_ADDRSTRLEN 46   
#endif


#define INET_ADDR_STR_LEN      INET6_ADDRSTRLEN


typedef struct
{
    USHORT   usFamily;             
    USHORT   usReserved;           
    union
    {
        IN6ADDR_S stIP6Addr;
        INADDR_S  stIP4Addr;
    } un_addr;                      

    #define uIP6_Addr       un_addr.stIP6Addr
    #define uIP4_Addr       un_addr.stIP4Addr
}INET_ADDR_S;


#define INET_ADDR_FAMILY(pstAddrM)      ((pstAddrM)->usFamily)
#define INET_ADDR_IP4ADDR(pstAddrM)     ((pstAddrM)->uIP4_Addr)
#define INET_ADDR_IP4ADDRUINT(pstAddrM) INET_ADDR_IP4ADDR(pstAddrM).S_addr
#define INET_ADDR_IP6ADDR(pstAddrM)     ((pstAddrM)->uIP6_Addr)
#define INET_ADDR_IP6ADDRUINT(pstAddrM) INET_ADDR_IP6ADDR(pstAddrM).s6_addr32

static inline VOID INADDR_LenToMask(IN UINT uiLen, OUT UINT *puiMask)
{
    if (0 == uiLen)
    {
        *puiMask = 0;
    }
    else
    {
        *puiMask = INADDR_NONE<<(32-uiLen);
    }

    return;
}


static inline VOID IN6ADDR_LenToMask(IN UINT uiLen, OUT IN6ADDR_S *pstMask)
{
    UINT uiUintLen;
    UINT uiBitLen;
    UINT uiLoop;
    UINT *puiMask;

    puiMask = pstMask->net_s6_addr32;
    
    puiMask[0] = 0;
    puiMask[1] = 0;
    puiMask[2] = 0;
    puiMask[3] = 0;

    uiUintLen = uiLen >> 5;     
    uiBitLen = uiLen & 31;      
    for (uiLoop = 0; uiLoop < uiUintLen; uiLoop++)
    {
        puiMask[uiLoop] = 0xffffffff;
    }
    
    if (uiBitLen != 0)
    {
        puiMask[uiUintLen] = htonl(((UINT)0xffffffff) << (32-uiBitLen));
    }

    return;
}


static inline VOID IN6ADDR_GetPrefix(IN const IN6ADDR_S *pstAddr, IN const IN6ADDR_S *pstMask, 
                                            OUT IN6ADDR_S *pstPrefix)
{
    UINT *puiPrefix;
    const UINT *puiAddr;
    const UINT *puiMask;
    
    puiPrefix = pstPrefix->net_s6_addr32;
    puiAddr = pstAddr->net_s6_addr32;
    puiMask = pstMask->net_s6_addr32;

    puiPrefix[0] = puiAddr[0] & puiMask[0];
    puiPrefix[1] = puiAddr[1] & puiMask[1];
    puiPrefix[2] = puiAddr[2] & puiMask[2];
    puiPrefix[3] = puiAddr[3] & puiMask[3];
    
    return;
}


static inline INT IN6ADDR_Cmp(IN const IN6ADDR_S *pstAddr1, IN const IN6ADDR_S *pstAddr2)
{
    UINT i;
    INT iRet;

    for (i = 0; i < NS_IN6ADDRSZ; i++)
    {
        iRet = pstAddr1->net_s6_addr[i] - pstAddr2->net_s6_addr[i];
        if (0 != iRet)
        {
            break;
        }
    }
    
    return iRet;
}


static inline INT INET_ADDR_Cmp(IN const INET_ADDR_S *pstAddr1, IN const INET_ADDR_S *pstAddr2)
{
    INT iRet = 0;
    
    if(AF_INET == INET_ADDR_FAMILY(pstAddr1))
    {
        UINT uiIP4Addr1 = ntohl(INET_ADDR_IP4ADDR(pstAddr1).S_addr);
        UINT uiIP4Addr2 = ntohl(INET_ADDR_IP4ADDR(pstAddr2).S_addr);

        if (uiIP4Addr1 > uiIP4Addr2)
        {
            iRet = 1;
        }
        else if(uiIP4Addr1 < uiIP4Addr2)
        {
            iRet = -1;
        }
        else
        {
            iRet = 0;
        }
    }
    else
    {
        iRet = IN6ADDR_Cmp(&(INET_ADDR_IP6ADDR(pstAddr1)), &(INET_ADDR_IP6ADDR(pstAddr2)));
    }

    return iRet;
}


BS_STATUS INET_ADDR_Str2IP(IN USHORT usFamily, IN const CHAR *pcStr, OUT INET_ADDR_S *pstAddr);
BS_STATUS INET_ADDR_N_Str2IP(IN USHORT usFamily, IN const CHAR *pcStr, IN UINT uiStrLen, OUT INET_ADDR_S *pstAddr);

#ifdef __cplusplus
    }
#endif 

#endif 


