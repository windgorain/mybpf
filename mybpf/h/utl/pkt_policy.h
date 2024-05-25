/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _PKT_POLICY_H
#define _PKT_POLICY_H

#include "utl/list_stq.h"
#include "utl/ip_mask_tbl.h"
#include "utl/dnsname_trie.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define PKT_POLICY_RULE_MAX (10*1024)

typedef enum {
    PKT_POLICY_ACTION_PERMIT = 0,
    PKT_POLICY_ACTION_DENY,
    PKT_POLICY_ACTION_BYPASS
}PKT_POLICY_ACTION_E;

typedef struct {
    LSTR_S user;
    LSTR_S host;
    UCHAR protocol;
    UINT sip; 
    UINT dip; 
    USHORT sport; 
    USHORT dport; 
}PKT_POLICY_PKTINFO_S;

typedef struct {
    char *action;           

    char *host;             
    char *protocol;         
    char *sip;              
    char *dip;
    char *sport;            
    char *dport;
}PKT_POLICY_RULE_S;

typedef struct {
    RULE_LIST_S rule_list;
}PKT_POLICY_S;

#define PKT_POLICY_DIM_MASK_PROTOCOL 0x1
#define PKT_POLICY_DIM_MASK_SIP      0x2
#define PKT_POLICY_DIM_MASK_DIP      0x4
#define PKT_POLICY_DIM_MASK_SPORT    0x8
#define PKT_POLICY_DIM_MASK_DPORT    0x10
#define PKT_POLICY_DIM_MASK_USER     0x20
#define PKT_POLICY_DIM_MASK_HOST     0x40

typedef struct {
    RULE_NODE_S rule_node; 
    PKT_POLICY_RULE_S policy_rule;
    UINT dim_mask; 
}PKT_POLICY_NODE_S;

typedef struct {
    STQ_HEAD_S port_dim[65536];
}PKT_POLICY_PORT_DIM_S;

typedef struct {
    STQ_HEAD_S protocol_dim[256];
}PKT_POLICY_PROTOCOL_DIM_S;

typedef struct {
    IPMASKTBL_S ipmasktbl;
    IPMASKTBL_BF_S bf;
}PKT_POLICY_IP_DIM_S;

typedef struct {
    UINT rule_dim_mask;
    int action;
}PKT_POLICY_RULE_DESC_S;

typedef struct {
    PKT_POLICY_RULE_DESC_S rule_desc[PKT_POLICY_RULE_MAX];
    TRIE_HANDLE trie;
    PKT_POLICY_PROTOCOL_DIM_S protocol_dim;
    PKT_POLICY_PORT_DIM_S sport_dim;
    PKT_POLICY_PORT_DIM_S dport_dim;
    PKT_POLICY_IP_DIM_S sip_dim;
    PKT_POLICY_IP_DIM_S dip_dim;
    int default_action; 
}PKT_POLICY_ENGINE_S;

int PKTPolicy_Init(PKT_POLICY_S *pkt_policy);
void PKTPolicy_Finit(PKT_POLICY_S *pkt_policy);
int PKTPolicy_AddRule(PKT_POLICY_S *pkt_policy, UINT rule_id, PKT_POLICY_RULE_S *rule);
void PKTPolicy_DelRule(PKT_POLICY_S *pkt_policy, UINT rule_id);
void PKTPolicy_DelRuleBatch(PKT_POLICY_S *pkt_policy, UINT *rule_ids, UINT count);
PKT_POLICY_RULE_S * PKTPolicy_GetByID(PKT_POLICY_S *pkt_policy, UINT id);

int PKTPolicy_EngineInit(PKT_POLICY_ENGINE_S *engine);
void PKTPolicy_EngineSetDefaultAction(PKT_POLICY_ENGINE_S *engine, int action);
int PKTPolicy_EngineCompile(PKT_POLICY_ENGINE_S *engine, PKT_POLICY_S *pkt_policy);
int PKTPolicy_EngineMatch(PKT_POLICY_ENGINE_S *engine, PKT_POLICY_PKTINFO_S *pktinfo);

#ifdef __cplusplus
}
#endif
#endif 
