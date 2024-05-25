/*================================================================
*   Created by LiXingang
*   Description: IDS RULE PARSER UTL
*
================================================================*/
#ifndef _IRP_UTL_H
#define _IRP_UTL_H
#include "utl/ac_smx.h"
#include "utl/ip_utl.h"
#include "utl/tcp_utl.h"
#include "utl/udp_utl.h"
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    DLL_HEAD_S rtn_list;
    ACSMX_S *ac;
}IRP_CTRL_S;

typedef enum {
    IRP_RULE_ACTION_ALERT = 0,
    IRP_RULE_ACTION_LOG,
    IRP_RULE_ACTION_PASS,

}IRP_RULE_ACTION_E;

typedef struct {
    DLL_NODE_S link_node;

    UINT protocol:8;
    UINT dir_out:1;
    UINT dir_in:1;

    char *sip_pool;
    char *dip_pool;
    char *sport_pool;
    char *dport_pool;

    IP_MASK_S sip;
    IP_MASK_S dip;
    USHORT sport;
    USHORT dport;

    DLL_HEAD_S otn_list;
}IRP_RTN_S;

typedef struct {
    UINT enabled:1;
    UINT type_threshold: 1;
    UINT type_limit: 1;
    UINT track_by_src: 1;
    UINT track_by_dst: 1;
    UINT count;
    UINT seconds;
}IRP_THRESHOLD_OPT_S;

typedef struct {
    UINT flags:8;
    UINT to_client:1;
    UINT to_server:1;
    UINT from_client:1;
    UINT from_server:1;
    UINT established:1;
    UINT stateless:1;
    UINT no_stream:1;
    UINT only_stream:1;
    UINT enabled:1;
}IRP_TCP_OPT_S;

enum {
    IRP_CONTENT_TYPE_NORMAL = 0,
    IRP_CONTENT_TYPE_URI_CONTENT,
    IRP_CONTENT_TYPE_RAWBYTES,
    IRP_CONTENT_TYPE_HTTP_CLIENT_BODY,
    IRP_CONTENT_TYPE_HTTP_COOKIE,
    IRP_CONTENT_TYPE_HTTP_RAW_COOKIE,
    IRP_CONTENT_TYPE_HTTP_HEADER,
    IRP_CONTENT_TYPE_HTTP_RAW_HEADER,
    IRP_CONTENT_TYPE_HTTP_URI,
    IRP_CONTENT_TYPE_HTTP_RAW_URI,
    IRP_CONTENT_TYPE_HTTP_STATE_CODE,
    IRP_CONTENT_TYPE_HTTP_STATE_MSG,
};

typedef struct {
    DLL_NODE_S link_node;
    LDATA_S content;
    UINT content_type:4;
    UINT nocase:1;
    UINT fast_pattern:1;
    int depth;
    int offset;
    int distance;
    int within;
}IRP_CONTENT_OPT_S;

typedef struct {
    void *pcre;
}IRP_PCRE_S;

typedef struct {
    DLL_NODE_S link_node;
    IRP_RTN_S *rtn; 
    IRP_CONTENT_OPT_S *fast_opt;
    DLL_HEAD_S content_opt_list;
    IRP_TCP_OPT_S tcp_opt;
    IRP_THRESHOLD_OPT_S threshold_opt;
    IRP_PCRE_S pcre_opt;
    UINT action:8; 
    int gid;
    int sid;
    char *msg;
}IRP_OTN_S;

typedef struct {
    int protocol;
    UINT sip;
    UINT dip;
    USHORT sport;
    USHORT dport;
}IRP_IP_S;

typedef struct {
    void *l4_payload;
    int l4_payload_len;
    IRP_IP_S *addr;
    IP_HEAD_S *ip_header;
    TCP_HEAD_S *tcp_header;
    UDP_HEAD_S *udp_header;
}IRP_PKT_INFO_S;


typedef int (*PF_IRP_MATCH_FUNC)(IRP_OTN_S *otn, void *ud);

int IRP_Init(IRP_CTRL_S *ctrl);
void IRP_Fini(IRP_CTRL_S *ctrl);
int IRP_AddRule(IRP_CTRL_S *ctrl, char *rule);
int IRP_LoadRuleByFile(IRP_CTRL_S *ctrl, char *filename);
int IRP_Compile(IRP_CTRL_S *ctrl);
int IRP_Match(IRP_CTRL_S *ctrl, IRP_PKT_INFO_S *info, PF_IRP_MATCH_FUNC func, void *ud);

#ifdef __cplusplus
}
#endif
#endif 
