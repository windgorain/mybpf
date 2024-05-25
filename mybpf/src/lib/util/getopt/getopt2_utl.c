/******************************************************************************
* Copyright (C) LiXingang
* Author:      LiXingang  Version: 1.0  Date: 2012-10-26
* Description: 
* History:     
******************************************************************************/
#include "bs.h"

#include "utl/bit_opt.h"
#include "utl/ctype_utl.h"
#include "utl/ip_string.h"
#include "utl/ip_utl.h"
#include "utl/txt_utl.h"
#include "utl/num_list.h"
#include "utl/getopt2_utl.h"
#include "utl/socket_utl.h"
#include "utl/ip_protocol.h"

typedef int (*PF_GETOPT2_PARSE_VALUE)(GETOPT2_NODE_S *pstNode, CHAR *pcValue);

typedef struct {
    int value_type;
    char *value_type_help;
    PF_GETOPT2_PARSE_VALUE func;
}GETOPT2_VALUE_TYPE_S;


static inline BOOL_T getopt2_is_opt_type(char type)
{
    if ((type == 'o') || (type == 'O')) {
        return TRUE;
    }
    return FALSE;
}

static inline BOOL_T getopt2_is_param_type(char type)
{
    if ((type == 'p') || (type == 'P')) {
        return TRUE;
    }
    return FALSE;
}


static inline BOOL_T getopt2_is_must(GETOPT2_NODE_S *node)
{
    
    if (node->opt_type == 'P') {
        return TRUE;
    }

    
    if (node->opt_type == 'O') {
        return TRUE;
    }

    return FALSE;
}

static int getopt2_parse_value_u32(GETOPT2_NODE_S *pstNode, CHAR *pcValue)
{
	if (FALSE == CTYPE_IsNumString(pcValue)) {
        return BS_ERR;
	}
    *((UINT*)pstNode->value) = strtoul(pcValue, NULL, 10);
    return 0;
}

static int getopt2_parse_value_bool(GETOPT2_NODE_S *pstNode, CHAR *pcValue)
{
    if ((strcmp(pcValue, "1") == 0) || (stricmp(pcValue, "true") == 0) || (stricmp(pcValue, "yes") == 0)) {
        *((BOOL_T*)pstNode->value) = TRUE;
    } else {
        *((BOOL_T*)pstNode->value) = FALSE;
    }

    return 0;
}

static int getopt2_parse_value_string(GETOPT2_NODE_S *pstNode, CHAR *pcValue)
{
    *((CHAR**)pstNode->value) = pcValue;
    return 0;
}


static int getopt2_parse_value_ipv4(GETOPT2_NODE_S *pstNode, CHAR *pcValue)
{
    UINT *p = pstNode->value;

    if (!Socket_IsIPv4(pcValue)){
        return BS_BAD_PARA;
    }

    *p = Socket_Ipsz2IpHost(pcValue);

    return 0;
}


static int getopt2_parse_value_ipv6(GETOPT2_NODE_S *pstNode, CHAR *pcValue)
{
    char tmp[256];
    struct in6_addr addr;
    char *p = pcValue;

    memcpy(tmp, p, strlen(p)+1);
    inet_pton(AF_INET6, tmp, &addr);
    memcpy(pstNode->value, &addr, 16);

    return 0;
}


static int getopt2_parse_value_ipv4_prefix(GETOPT2_NODE_S *pstNode, CHAR *pcValue)
{
    IP_PREFIX_S *p = pstNode->value;
    return IPString_ParseIpPrefix(pcValue, p);
}


static int getopt2_parse_value_ipv4_port(GETOPT2_NODE_S *pstNode, CHAR *pcValue)
{
    IP_PORT_S *p = pstNode->value;
    return IPString_ParseIpPort(pcValue, p);
}


static int getopt2_parse_value_range(GETOPT2_NODE_S *pstNode, CHAR *pcValue)
{
    LSTR_S lstr;
    GETOPT2_NUM_RANGE_S *r = pstNode->value;

    lstr.pcData = pcValue;
    lstr.uiLen = strlen(pcValue);

    return NumList_ParseElement(&lstr, &r->min, &r->max);
}

static int getopt2_parse_value_mac(GETOPT2_NODE_S *pstNode, CHAR *pcValue)
{
    int i = 0;
    
    UCHAR *mac = pstNode->value;

    if (strlen(pcValue) != 17)
       return BS_BAD_PTR;

    for (i=0;i<6;i++) {
        DH_Hex2Data(pcValue + 3*i, 2, &mac[i]);
    }

    return 0;
}

static int getopt2_parse_value_ip_protocol(GETOPT2_NODE_S *pstNode, CHAR *pcValue)
{
    int *protocol = pstNode->value;
    int ret;

    ret = IPProtocol_GetByName(pcValue);
    if (ret < 0) {
        return BS_BAD_PARA;
    }

    *protocol = ret;

    return 0;
}

static GETOPT2_VALUE_TYPE_S g_getopt2_value_types[] = {
    {.value_type = GETOPT2_V_STRING, .value_type_help="String", .func = getopt2_parse_value_string},
    {.value_type = GETOPT2_V_U32, .value_type_help="U32", .func = getopt2_parse_value_u32},
    {.value_type = GETOPT2_V_BOOL, .value_type_help="Bool", .func = getopt2_parse_value_bool},
    {.value_type = GETOPT2_V_RANGE, .value_type_help="Range", .func = getopt2_parse_value_range},
    {.value_type = GETOPT2_V_MAC, .value_type_help="MAC", .func = getopt2_parse_value_mac},
    {.value_type = GETOPT2_V_IP, .value_type_help="IPv4", .func = getopt2_parse_value_ipv4},
    {.value_type = GETOPT2_V_IP6, .value_type_help="IPv6", .func = getopt2_parse_value_ipv6},
    {.value_type = GETOPT2_V_IP_PREFIX, .value_type_help="IP/Prefix", .func = getopt2_parse_value_ipv4_prefix},
    {.value_type = GETOPT2_V_IP_PORT, .value_type_help="IP:Port", .func = getopt2_parse_value_ipv4_port},
    {.value_type = GETOPT2_V_IP_PROTOCOL, .value_type_help="IPProtocol", .func = getopt2_parse_value_ip_protocol},
};

static GETOPT2_VALUE_TYPE_S * getopt2_find_value_type(int value_type)
{
    int i;

    if (! value_type) {
        return NULL;
    }

    for (i=0; i<ARRAY_SIZE(g_getopt2_value_types); i++) {
        if (value_type == g_getopt2_value_types[i].value_type) {
            return &g_getopt2_value_types[i];
        }
    }

    return NULL;
}

static char * getopt2_opt_type_2_string(int value_type)
{
    GETOPT2_VALUE_TYPE_S *type = getopt2_find_value_type(value_type);

    if (! type) {
        return "NULL";
    }

    return type->value_type_help;
}

static int getopt2_ParseValue(GETOPT2_NODE_S *pstNode, CHAR *pcValue)
{
    GETOPT2_VALUE_TYPE_S *type = getopt2_find_value_type(pstNode->value_type);
    
    if (! type) {
        RETURN(BS_NOT_SUPPORT);
    }

    return type->func(pstNode, pcValue);
}

static GETOPT2_NODE_S * getopt2_find_by_long_opt(GETOPT2_NODE_S *pstNodes, char *opt)
{
    GETOPT2_NODE_S *pstNode;

    for (pstNode=pstNodes; pstNode->opt_type != 0; pstNode++) {
        if ((getopt2_is_opt_type(pstNode->opt_type)) && (0 == strcmp(opt, pstNode->opt_long_name))) {
            return pstNode;
        }
    }

    return NULL;
}


static int getopt2_parse_long_opt(UINT uiArgc, CHAR **ppcArgv, GETOPT2_NODE_S *pstNodes, UINT uiOptIndex)
{
    GETOPT2_NODE_S *pstNode;
    char *pcOptName = &ppcArgv[uiOptIndex][2];
    char *pcSplit;
    char *pcValue = NULL;

    pcSplit = strchr(pcOptName, '=');
    if (NULL != pcSplit) {
        *pcSplit = '\0';
        pcValue = pcSplit + 1;
    }

    pstNode = getopt2_find_by_long_opt(pstNodes, pcOptName);
    if (! pstNode) {
        RETURNI(BS_NOT_FOUND, "Unknown option \'--%s\'", pcOptName);
    }

    BIT_SET(pstNode->flag, GETOPT2_OUT_FLAG_ISSET);

    if (! pstNode->value_type) {
        return 0;
    }

    if (pcValue == NULL) {
        if (uiOptIndex + 1 >= uiArgc) {
            RETURNI(BS_NOT_FOUND, "option \'--%s\' should give value", pcOptName);
        }
        pcValue = ppcArgv[uiOptIndex + 1];
    }

    if (BS_OK != getopt2_ParseValue(pstNode, pcValue)) {
        RETURNI(BS_BAD_PARA, "\'--%s %s\' parse error", pcOptName, pcValue);
    }

    return 1;
}

static GETOPT2_NODE_S * getopt2_find_by_short_opt(GETOPT2_NODE_S *pstNodes, char opt)
{
    GETOPT2_NODE_S *pstNode;

    for (pstNode=pstNodes; pstNode->opt_type != 0; pstNode++) {
        if ((getopt2_is_opt_type(pstNode->opt_type)) && (opt == pstNode->opt_short_name)) {
            return pstNode;
        }
    }

    return NULL;
}

static int getopt2_parse_short_opts(UINT uiArgc, CHAR **ppcArgv, GETOPT2_NODE_S *pstNodes, UINT uiOptIndex)
{
    GETOPT2_NODE_S *pstNode;
    char *pcValue = NULL;
    char value_processed = 0;
    char *opt = &ppcArgv[uiOptIndex][1];
    int consume_count = 0; 
    int ret;

    for (; *opt != 0; opt++) {
        pstNode = getopt2_find_by_short_opt(pstNodes, *opt);
        if (! pstNode) {
            RETURNI(BS_NOT_FOUND, "Unknown option \'-%c\'", *opt);
        }

        BIT_SET(pstNode->flag, GETOPT2_OUT_FLAG_ISSET);

        if (pstNode->value_type == 0) {
            continue;
        }

        if (value_processed) {
            
            RETURNI(BS_REACH_MAX, "Only support one opt with value \'-%s\'", ppcArgv[uiOptIndex]);
        }

        value_processed = 1;

        if (uiOptIndex + 1 >= uiArgc) {
            RETURNI(BS_NOT_FOUND, "option \'-%c\' should give value", *opt);
        }

        pcValue = ppcArgv[uiOptIndex + 1];

        if ((ret = getopt2_ParseValue(pstNode, pcValue)) < 0) {
            RETURNI(BS_BAD_PARA, "\'-%c %s\' parse error", *opt, pcValue);
        }

        consume_count ++;
    }

    return consume_count;
}

static int getopt2_ParseParam(CHAR *pcParam, GETOPT2_NODE_S *pstNodes)
{
    GETOPT2_NODE_S *pstNode;

    for (pstNode=pstNodes; pstNode->opt_type!=0; pstNode++) {
        if (! getopt2_is_param_type(pstNode->opt_type)) {
            continue;
        }

        if (pstNode->flag & GETOPT2_OUT_FLAG_ISSET) {
            continue;
        }

        if (BS_OK != getopt2_ParseValue(pstNode, pcParam)) {
            RETURN(BS_BAD_PARA);
        }

        BIT_SET(pstNode->flag, GETOPT2_OUT_FLAG_ISSET);

        return 0;
    }

    RETURN(BS_NOT_FOUND);
}


static void getopt2_init(GETOPT2_NODE_S *pstNodes)
{
    GETOPT2_NODE_S *node;

    for (node=pstNodes; node->opt_type!=0; node++) {
        node->flag = 0;
    }
}

static BOOL_T _getopt2_is_long_opt(char *opt)
{
    if ((opt[0] != '-') || (opt[1] != '-')){
        return FALSE;
    }

    opt += 2;
    if (*opt == 0) {
        
        return FALSE;
    }

    return TRUE;
}


int GETOPT2_ParseFromArgv0(UINT uiArgc, CHAR **ppcArgv, INOUT GETOPT2_NODE_S *opts)
{
    int i;
    int consume_count;

    getopt2_init(opts);

    for (i=0; i<uiArgc; i++) {
        
        if (ppcArgv[i][0] == '-') {
            
            if (_getopt2_is_long_opt(ppcArgv[i])) {
                consume_count = getopt2_parse_long_opt(uiArgc, ppcArgv, opts, i);
            } else {
                consume_count = getopt2_parse_short_opts(uiArgc, ppcArgv, opts, i);
            }
        } else {
            consume_count = getopt2_ParseParam(ppcArgv[i], opts);
        }

        if (consume_count < 0) {
            return consume_count;
        }

        i += consume_count;
    }

    GETOPT2_NODE_S *err_node = GETOPT2_IsMustErr(opts);
    if (err_node) {
        if (err_node->opt_type == 'P') {
            return BS_NOT_COMPLETE;
        }
        if (err_node->opt_short_name) {
            return BS_NOT_COMPLETE;
        }
        return BS_NOT_COMPLETE;
    }

    return 0;
}

int GETOPT2_Parse(UINT uiArgc, CHAR **ppcArgv, INOUT GETOPT2_NODE_S *opts)
{
    
    return GETOPT2_ParseFromArgv0(uiArgc - 1, ppcArgv + 1, opts);
}

#if 0
static BOOL_T getopt2_is_have_param(GETOPT2_NODE_S *nodes)
{
    GETOPT2_NODE_S *node;

    for (node=nodes; node->opt_type!=0; node++) {
        if (getopt2_is_param_type(node->opt_type)) {
            return TRUE;
        }
    }

    return FALSE;
}
#endif

static BOOL_T getopt2_is_have_opt(GETOPT2_NODE_S *nodes)
{
    GETOPT2_NODE_S *node;

    for (node=nodes; node->opt_type!=0; node++) {
        if (getopt2_is_opt_type(node->opt_type)) {
            return TRUE;
        }
    }

    return FALSE;
}

static int getopt2_build_param_help(GETOPT2_NODE_S *nodes, OUT char *buf, int buf_size)
{
    GETOPT2_NODE_S *node;
    int len = 0;

    for (node=nodes; node->opt_type!=0; node++) {
        if (! getopt2_is_param_type(node->opt_type)) {
            continue;
        }

        if (node->opt_long_name != NULL) {
            if (getopt2_is_must(node)) { 
                len += snprintf(buf+len, buf_size-len, " %s", node->opt_long_name);
            } else { 
                len += snprintf(buf+len, buf_size-len, " [%s]", node->opt_long_name);
            }
        }
    }

    len += snprintf(buf+len, buf_size-len, "\r\n");

    for (node=nodes; node->opt_type!=0; node++) {
        if (! getopt2_is_param_type(node->opt_type)) {
            continue;
        }

        if (node->opt_long_name != NULL) {
            len += snprintf(buf+len, buf_size-len, " %s: %s\r\n", 
                    node->opt_long_name,
                    node->help_info == NULL ? "": node->help_info);
        }
    }

    return len;
}

static void getopt2_build_opt_one(GETOPT2_NODE_S *node, OUT char *buf, int buf_size)
{
    int len = 0;

    if (node->opt_short_name) {
        len += snprintf(buf+len, buf_size-len, "-%c ", node->opt_short_name);
    } else {
        len += snprintf(buf+len, buf_size-len, "   ");
    }

    if (node->opt_long_name) {
        len += snprintf(buf+len, buf_size-len, "--%s ", node->opt_long_name);
    }

    if (node->value_type) {
        len += snprintf(buf+len, buf_size-len, "%s ", getopt2_opt_type_2_string(node->value_type));
    }
}

static int getopt2_build_opt_help(GETOPT2_NODE_S *nodes, OUT char *buf, int buf_size)
{
    GETOPT2_NODE_S *node;
    int len = 0;
    int must;
    char optbuf[128];

    if (getopt2_is_have_opt(nodes)) {
        len += snprintf(buf+len, buf_size-len, "Options:\r\n");
    }

    for (node=nodes; node->opt_type!=0; node++) {
        if (! getopt2_is_opt_type(node->opt_type)) {
            continue;
        }

        if (node->flag & GETOPT2_FLAG_HIDE) {
            continue;
        }

        len += snprintf(buf+len, buf_size-len, " ");

        must = getopt2_is_must(node);

        if (must) {
            len += snprintf(buf+len, buf_size-len, "* ");
        } else {
            len += snprintf(buf+len, buf_size-len, "  ");
        }

        getopt2_build_opt_one(node, optbuf, sizeof(optbuf));

        len += snprintf(buf+len, buf_size-len, "%-32s %s\r\n", optbuf, node->help_info == NULL ? "": node->help_info);
    }

    return len;
}

char * GETOPT2_BuildHelpinfo(GETOPT2_NODE_S *nodes, OUT char *buf, int buf_size)
{
    int len = 0;

    buf[0] = '\0';

    if (getopt2_is_have_opt(nodes)) {
        len += snprintf(buf+len, buf_size-len, "Command: [Options]");
    } else {
        len += snprintf(buf+len, buf_size-len, "Command: ");
    }

    len += getopt2_build_param_help(nodes, buf+len, buf_size-len);

    len += getopt2_build_opt_help(nodes, buf+len, buf_size-len);

    return buf;
}

int GETOPT2_IsOptSetted(GETOPT2_NODE_S *nodes, char short_opt_name, char *long_opt_name)
{
    GETOPT2_NODE_S *node;

    for (node=nodes; node->opt_type!=0; node++) {
        if ((short_opt_name != 0) && (short_opt_name == node->opt_short_name)) {
            return (node->flag & GETOPT2_OUT_FLAG_ISSET);
        } else if ((NULL != long_opt_name) && (NULL != node->opt_long_name)
                && (strcmp(long_opt_name, node->opt_long_name) == 0)) {
            return (node->flag & GETOPT2_OUT_FLAG_ISSET);
        }
    }

    return 0;
}


GETOPT2_NODE_S * GETOPT2_IsMustErr(GETOPT2_NODE_S *opts)
{
    GETOPT2_NODE_S *node;

    for (node=opts; node->opt_type!=0; node++) {
        
        if (getopt2_is_must(node)) {
            if (! (node->flag & GETOPT2_OUT_FLAG_ISSET)) {
                return node;
            }
        }
    }

    return NULL;
}

void GETOPT2_PrintHelp(GETOPT2_NODE_S *opts)
{
    char buf[1024];
    printf("%s\r\n", GETOPT2_BuildHelpinfo(opts, buf, sizeof(buf)));
}

