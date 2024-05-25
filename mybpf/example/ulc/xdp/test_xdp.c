/*********************************************************
*   Copyright (C) LiXingang
*   Description: 
*     clang -O2 -target bpf -c test_xdp.c -o test_xdp.o
*
********************************************************/
#include "utl/ulc_user.h"

typedef struct {
    unsigned int count;
    unsigned int bytes;
}MLB_CFG_S;


struct bpf_map_def SEC("maps") g_test_cfg_tbl = {
    .type = BPF_MAP_TYPE_ARRAY,
    .key_size = sizeof(int),
    .value_size = sizeof(MLB_CFG_S),
    .max_entries = 1,
};

static inline int _test_input(struct xdp_md *ctx, char *data, char *data_end)
{
    int id = 0;

    MLB_CFG_S *cfg = bpf_map_lookup_elem(&g_test_cfg_tbl, &id);
    if (! cfg) {
        printf("Cat't lookup elem \n");
        return XDP_PASS;
    }

    cfg->count ++;
    cfg->bytes += (data_end - data);

    printf("Bytes: %u, Count: %u \n", cfg->bytes, cfg->count);


    return XDP_PASS;
}

SEC("xdp/test_xdp_input")
int test_xdp_input(struct xdp_md *ctx)
{
    void *data_end = (void *)(long)ctx->data_end;
    void *data = (void *)(long)ctx->data;

    return _test_input(ctx, data, data_end);
}

char _license[] SEC("license") = "Dual BSD/GPL";


