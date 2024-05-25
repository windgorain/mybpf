/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#include "bs.h"
#include "utl/txt_utl.h"
#include "utl/ctype_utl.h"
#include "utl/ip_protocol.h"

typedef struct {
    UCHAR protocol;
    char *str;
}IP_PROTOCOL_MAP_S;

static IP_PROTOCOL_MAP_S g_ip_protocol_map[] = {
    {.protocol=0, .str="IP"},
    {.protocol=IPPROTO_TCP, .str="TCP"},
    {.protocol=IPPROTO_UDP, .str="UDP"},
    {.protocol=IPPROTO_ICMP, .str="ICMP"},
    {.protocol=IPPROTO_IGMP, .str="IGMP"},
};


CHAR * IPProtocol_GetName(IN UCHAR ucProtocol)
{
    int i;

    for (i=0; i<sizeof(g_ip_protocol_map)/sizeof(IP_PROTOCOL_MAP_S); i++) {
        if (g_ip_protocol_map[i].protocol == ucProtocol) {
            return g_ip_protocol_map[i].str;
        }
    }

    return NULL;
}


CHAR * IPProtocol_GetNameExt(IN UCHAR ucProtocol)
{
    char *p = IPProtocol_GetName(ucProtocol);
    if (! p) {
        p = "";
    }
    return p;
}

int IPProtocol_GetByName(char *protocol_name)
{
    int i;

    if (CTYPE_IsNumString(protocol_name)) {
        return TXT_Str2Ui(protocol_name);
    }

    for (i=0; i<sizeof(g_ip_protocol_map)/sizeof(IP_PROTOCOL_MAP_S); i++) {
        if (stricmp(g_ip_protocol_map[i].str, protocol_name) == 0) {
            return g_ip_protocol_map[i].protocol;
        }
    }

    RETURN(BS_NOT_FOUND);
}


int IPProtocol_NameList2Protocols(INOUT char *protocol_name_list)
{
    int size = strlen(protocol_name_list) + 1;
    int i;
    char numstr[32];

    TXT_Upper(protocol_name_list);

    for (i=0; i<sizeof(g_ip_protocol_map)/sizeof(IP_PROTOCOL_MAP_S); i++) {
        sprintf(numstr, "%u", g_ip_protocol_map[i].protocol);
        TXT_ReplaceSubStr(protocol_name_list, g_ip_protocol_map[i].str, numstr, protocol_name_list, size);
    }

    return 0;
}
