/*********************************************************
*   Copyright (C) LiXingang
*   Description: 
*
********************************************************/
#ifndef _MYBPF_LOADER_DEF_H
#define _MYBPF_LOADER_DEF_H
#ifdef __cplusplus
extern "C"
{
#endif

#define MYBPF_LOADER_FLAG_AUTO_ATTACH 0x1  
#define MYBPF_LOADER_FLAG_REPLACE     0x2  
#define MYBPF_LOADER_FLAG_KEEP_MAP    0x4  
#define MYBPF_LOADER_FLAG_JIT         0x8  
#define MYBPF_LOADER_FLAG_SKIP_JIT_HELPER_CHECK   0x10  

typedef struct {
    char *instance; 
    char *filename; 
    FILE_MEM_S *m;
    UINT flag;
    const void **tmp_helpers;
}MYBPF_LOADER_PARAM_S;

#define MYBPF_LOADER_MAX_MAPS 32
#define MYBPF_LOADER_MAX_PROGS 32

#ifdef __cplusplus
}
#endif
#endif 
