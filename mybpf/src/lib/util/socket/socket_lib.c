/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#include "bs.h"
#include "utl/num_utl.h"
#include "utl/bit_opt.h"
#include "utl/txt_utl.h"
#include "utl/mem_utl.h"
#include "utl/socket_utl.h"

BOOL_T Socket_IsIPv4(IN CHAR *pcIpOrName)
{
    CHAR *pcTmp = pcIpOrName;
    CHAR *pcSplit;
    UINT ulLen, i, j;

    for (j=0; j<4; j++) {
        
        if (j < 3) {
            pcSplit = strchr(pcTmp, '.');
            if (pcSplit == NULL) {
                return FALSE;
            }
        } else {
            pcSplit = pcTmp + strlen(pcTmp);
        }
        
        ulLen = (UINT)(pcSplit - pcTmp);
        if (ulLen > 3) {     
            return FALSE;
        }
        for (i=0; i<ulLen; i++) {     
            if (!NUM_IN_RANGE((INT)pcTmp[i], (INT)'0', (INT)'9')) {
                return FALSE;
            }
        }
        if (ulLen == 3) {    
            if (pcTmp[0] > '2') {
                return FALSE;
            }

            if (pcTmp[0] == '2') {
                if (pcTmp[1] > '5') {
                    return FALSE;
                }

                if (pcTmp[1] == '5') {
                    if (pcTmp[2] > '5') {
                        return FALSE;
                    }
                }
            }

        }
        pcTmp = pcSplit + 1;
    }

    return TRUE;
}


UINT Socket_Ipsz2IpNet(char *pcIP)
{
    return inet_addr(pcIP);
}

UINT Socket_Ipsz2IpHost(IN CHAR *pcIP)
{
    UINT uiIp;

    uiIp = Socket_Ipsz2IpNet(pcIP);

    return ntohl(uiIp);
}

