/*********************************************************
*   Copyright (C) LiXingang
*
********************************************************/
#include "utl/ulc_user.h"

typedef struct {
    unsigned int count;
}CFG_S;


struct bpf_map_def SEC("maps") g_test_cfg_tbl = {
    .type = BPF_MAP_TYPE_ARRAY,
    .key_size = sizeof(int),
    .value_size = sizeof(CFG_S),
    .max_entries = 1,
};

SEC(".spf.cmd/main")
int test_map()
{
    int id = 0;

    CFG_S *cfg = bpf_map_lookup_elem(&g_test_cfg_tbl, &id);
    if (! cfg) {
        printf("Cat't lookup elem \n");
        return 0;
    }

    cfg->count ++;

    printf("Hello %u \n", cfg->count);

    return 0;
}


