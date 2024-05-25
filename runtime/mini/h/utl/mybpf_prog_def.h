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
    const void **base_helpers;
    const void **sys_helpers;
    const void **user_helpers;
    const void **tmp_helpers;
    void *maps; 
    void **global_map_data;
    void *loader_node;
}MYBPF_AOT_PROG_CTX_S;

#ifdef __cplusplus
}
#endif
#endif 
