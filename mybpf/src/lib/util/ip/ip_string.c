/******************************************************************************
* Copyright (C), 2000-2006,  LiXingang
* Author:      LiXingang  Version: 1.0  Date: 2017-5-20
* Description: IP String库
* History:     
******************************************************************************/
#include "bs.h"
#include "utl/net.h"
#include "utl/eth_utl.h"
#include "utl/lstr_utl.h"
#include "utl/txt_utl.h"
#include "utl/data2hex_utl.h"
#include "utl/ip_utl.h"
#include "utl/socket_utl.h"
#include "utl/ip_string.h"


int IPString_IpPrefixString2IpMask(CHAR *pcIpPrefixString, OUT IP_MASK_S *pstIpMask)
{
    BS_STATUS enRet;
    IP_PREFIX_S stPrefix;

    enRet = IPString_ParseIpPrefix(pcIpPrefixString, &stPrefix);
    if (BS_OK != enRet) {
        return enRet;
    }

    pstIpMask->uiIP = stPrefix.uiIP;
    pstIpMask->uiMask = PREFIX_2_MASK(stPrefix.ucPrefix);

    return BS_OK;
}


BS_STATUS IPString_ParseIpMask(CHAR *pcIpMaskString, OUT IP_MASK_S *pstIpMask)
{
    LSTR_S stIP;
    LSTR_S stMask;
    CHAR szTmp[32];
    
    TXT_StrSplit(pcIpMaskString, '/', &stIP, &stMask);

    LSTR_Strim(&stIP, TXT_BLANK_CHARS, &stIP);
    LSTR_Strim(&stMask, TXT_BLANK_CHARS, &stMask);

    if ((stIP.uiLen > 15) || (stMask.uiLen > 15))
    {
        return BS_BAD_PARA;
    }

    LSTR_Strlcpy(&stIP, sizeof(szTmp), szTmp);
    pstIpMask->uiIP = Socket_Ipsz2IpHost(szTmp);

    LSTR_Strlcpy(&stMask, sizeof(szTmp), szTmp);
    pstIpMask->uiMask = Socket_Ipsz2IpHost(szTmp);

    return BS_OK;
}



UINT IPString_ParseIpMaskList(IN CHAR *pcIpMaskString, IN CHAR cSplitChar, IN UINT uiIpMaskMaxNum, OUT IP_MASK_S *pstIpMasks)
{
    CHAR *pcIpMask;
    UINT uiNum = 0;

    TXT_SCAN_ELEMENT_BEGIN(pcIpMaskString, cSplitChar, pcIpMask)
    {
        if (uiNum >= uiIpMaskMaxNum)
        {
            TXT_SCAN_ELEMENT_STOP();
            return uiNum;
        }

        IPString_ParseIpMask(pcIpMask, &pstIpMasks[uiNum]);

        uiNum ++;
    }TXT_SCAN_ELEMENT_END();

    return uiNum;
}

CHAR * IPString_IP2String(IN UINT ip, OUT CHAR *str)
{
    UCHAR *pucData = (void*)&ip;
    sprintf(str, "%u.%u.%u.%u", pucData[0], pucData[1], pucData[2], pucData[3]);
    return str;
}

INT IPString_IpMask2String_OutIpPrefix(IN IP_MASK_S *pstIpMask, IN INT size, OUT CHAR *str)
{
    int len;
    UCHAR *pucData;
    UINT uiIpNet;
    UCHAR ucPrefix;

    uiIpNet = ntohl(pstIpMask->uiIP);
    pucData = (VOID *)&uiIpNet;
    ucPrefix = MASK_2_PREFIX(pstIpMask->uiMask);

    len = SNPRINTF(str, size, "%u.%u.%u.%u", pucData[0], pucData[1], pucData[2], pucData[3]);
    if (len < 0) {
        return len;
    }

    return SNPRINTF(str+len, size - len, "/%hhu", ucPrefix);
}

CHAR * IPString_IPHeader2String(IN VOID *ippkt, OUT CHAR *info, IN UINT infosize)
{
    IP_HEAD_S *iph;
    char sip[32];
    char dip[32];

    iph = ippkt;

    scnprintf(info, infosize,
            "\"ip_head_len\":%u,\"ip_tos\":%u,\"ip_total_length\":%u,\"ip_pkt_id\":%d,"
            "\"ip_flag\":%u,\"ip_frag_offset\":%u,\"ip_ttl\":%u,\"ip_protocol\":%u,\"ip_head_chksum\":\"%04x\","
            "\"sip\":\"%s\",\"dip\":\"%s\"",
            iph->ucHLen*4, iph->ucTos, ntohs(iph->usTotlelen), ntohs(iph->usIdentification),
            IP_HEAD_FLAG(iph), IP_HEAD_FRAG_OFFSET(iph), iph->ucTtl, iph->ucProto, iph->usCrc,
            IPString_IP2String(iph->unSrcIp.uiIp, sip), IPString_IP2String(iph->unDstIp.uiIp, dip));

    return info;
}

CHAR * IPString_IPHeader2Hex(IN VOID *ippkt, OUT CHAR *info)
{
    IP_HEAD_S *iph = ippkt;
    int len = iph->ucHLen*4;

    DH_Data2HexString(ippkt, len, info);

    return info;
}

