/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#include "bs.h"
#include "utl/net.h"
#include "utl/eth_utl.h"
#include "utl/lstr_utl.h"
#include "utl/txt_utl.h"
#include "utl/data2hex_utl.h"
#include "utl/ip_utl.h"
#include "utl/socket_utl.h"
#include "utl/ip_string.h"


int IPString_ParseIpPrefix(CHAR *pcIpPrefixString, OUT IP_PREFIX_S *pstIpPrefix)
{
    LSTR_S stIP;
    LSTR_S stPrefix;
    CHAR szTmp[32];
    UINT uiPrefix = 0;

    TXT_StrSplit(pcIpPrefixString, '/', &stIP, &stPrefix);

    LSTR_Strim(&stIP, TXT_BLANK_CHARS, &stIP);
    LSTR_Strim(&stPrefix, TXT_BLANK_CHARS, &stPrefix);

    if ((stIP.uiLen > 15) || (stPrefix.uiLen > 2)) {
        return BS_BAD_PARA;
    }

    LSTR_Strlcpy(&stIP, sizeof(szTmp), szTmp);
    if (!Socket_IsIPv4(szTmp)){
        return BS_BAD_PARA;
    }
    pstIpPrefix->uiIP = Socket_Ipsz2IpHost(szTmp);
  
    if (stPrefix.uiLen == 0) {
        uiPrefix = 32;
    } else {
        uiPrefix = atoi(stPrefix.pcData);
    }

    if (uiPrefix > 32) {
        return BS_BAD_PARA;
    }
    pstIpPrefix->ucPrefix = (UCHAR)uiPrefix;

    return BS_OK;
}


int IPString_ParseIpPort(CHAR *ip_port_string, OUT IP_PORT_S *out)
{
    LSTR_S stIP;
    LSTR_S stPort;
    char szTmp[32];
    U16 port = 0;
    U32 ip = 0;

    TXT_StrSplit(ip_port_string, ':', &stIP, &stPort);

    LSTR_Strim(&stIP, TXT_BLANK_CHARS, &stIP);
    LSTR_Strim(&stPort, TXT_BLANK_CHARS, &stPort);

    if ((stIP.uiLen > 15) || (stPort.uiLen > 5)) {
        return BS_BAD_PARA;
    }

    if (stIP.uiLen > 0) {
        LSTR_Strlcpy(&stIP, sizeof(szTmp), szTmp);
        if (! Socket_IsIPv4(szTmp)){
            return BS_BAD_PARA;
        }
        ip = Socket_Ipsz2IpHost(szTmp);
    }

    if (stPort.uiLen > 0) {
        port = atoi(stPort.pcData);
    }

    out->ip = ip;
    out->port = port;

    return 0;
}

