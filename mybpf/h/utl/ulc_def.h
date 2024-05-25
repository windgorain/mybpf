/*********************************************************
*   Copyright (C) LiXingang
*   Description: 
*
********************************************************/
#ifndef _ULC_DEF_H
#define _ULC_DEF_H
#ifdef __cplusplus
extern "C"
{
#endif

enum bpf_map_type {
	BPF_MAP_TYPE_UNSPEC,
	BPF_MAP_TYPE_HASH,
	BPF_MAP_TYPE_ARRAY,
	BPF_MAP_TYPE_PROG_ARRAY,
	BPF_MAP_TYPE_PERF_EVENT_ARRAY,
	BPF_MAP_TYPE_PERCPU_HASH,
	BPF_MAP_TYPE_PERCPU_ARRAY,

    BPF_MAP_TYPE_MAX
};

enum xdp_action {
	XDP_ABORTED = 0,
	XDP_DROP,
	XDP_PASS,
	XDP_TX,
	XDP_REDIRECT,
};

struct xdp_md {
	unsigned int data;
	unsigned int data_end;
	unsigned int data_meta;
	
	unsigned int ingress_ifindex; 
	unsigned int rx_queue_index;  
	unsigned int egress_ifindex;  
};

#ifdef __cplusplus
}
#endif
#endif 
