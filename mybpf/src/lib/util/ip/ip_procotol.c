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
    char *strl; 
}IP_PROTOCOL_MAP_S;

static IP_PROTOCOL_MAP_S g_ip_protocol_map[] = {
    {.protocol=0, .str="IP", .strl="ip"},
    {.protocol=IPPROTO_TCP, .str="TCP", .strl="tcp"},
    {.protocol=IPPROTO_UDP, .str="UDP", .strl="udp"},
    {.protocol=IPPROTO_ICMP, .str="ICMP", .strl="icmp"},
    {.protocol=IPPROTO_IGMP, .str="IGMP", .strl="igmp"},
};

static IP_PROTOCOL_MAP_S * _ip_protocol_get_map_by_proto(U8 proto)
{
    int i;

    for (i=0; i<ARRAY_SIZE(g_ip_protocol_map); i++) {
        if (g_ip_protocol_map[i].protocol == proto) {
            return &g_ip_protocol_map[i];
        }
    }

    return NULL;

}


CHAR * IPProtocol_GetName(IN UCHAR ucProtocol)
{
    IP_PROTOCOL_MAP_S *map = _ip_protocol_get_map_by_proto(ucProtocol);
    if (! map) {
        return NULL;
    }
    return map->str;
}


CHAR * IPProtocol_GetNameExt(IN UCHAR ucProtocol)
{
    char *p = IPProtocol_GetName(ucProtocol);
    if (! p) {
        p = "";
    }
    return p;
}


CHAR * IPProtocol_GetLName(IN UCHAR ucProtocol)
{
    IP_PROTOCOL_MAP_S *map = _ip_protocol_get_map_by_proto(ucProtocol);
    if (! map) {
        return NULL;
    }
    return map->strl;
}


CHAR * IPProtocol_GetLNameExt(IN UCHAR ucProtocol)
{
    char *p = IPProtocol_GetLName(ucProtocol);
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
