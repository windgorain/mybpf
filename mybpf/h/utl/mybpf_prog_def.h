/*********************************************************
*   Copyright (C) LiXingang
*   Description: 
*
********************************************************/
#ifndef _MYBPF_PROG_DEF_H
#define _MYBPF_PROG_DEF_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    RCU_NODE_S rcu_node;
    char sec_name[128];
    char prog_name[64];
    void *loader_node;
    UINT attached; 
    int insn_len; 
    void *insn; 
}MYBPF_PROG_NODE_S;

typedef struct xdp_buff {
	void *data;
	void *data_end;
	void *data_meta;
	
	UINT ingress_ifindex; 
	UINT rx_queue_index; 
}MYBPF_XDP_BUFF_S;

#ifdef __cplusplus
}
#endif
#endif 
