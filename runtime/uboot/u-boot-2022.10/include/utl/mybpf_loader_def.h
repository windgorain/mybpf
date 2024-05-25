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

#define MYBPF_LOADER_FLAG_AUTO_ATTACH 0x1  /* load时是否进行自动attach */
#define MYBPF_LOADER_FLAG_KEEP_MAP    0x2  /* replace时是否保留原来的map */
#define MYBPF_LOADER_FLAG_JIT         0x4  /* 加载时自动进行jit */

typedef struct {
    char *instance; /* instance name */
    char *filename; /* object file name */
    FILE_MEM_S simple_mem;
    UINT flag;
    const void **tmp_helpers;
}MYBPF_LOADER_PARAM_S;

#define MYBPF_LOADER_MAX_MAPS 32
#define MYBPF_LOADER_MAX_PROGS 32


#ifdef __cplusplus
}
#endif
#endif //MYBPF_LOADER_DEF_H_
