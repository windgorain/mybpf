/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _MYBPF_BARE_DEF_H_
#define _MYBPF_BARE_DEF_H_

#include "utl/mybpf_prog_def.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MYBPF_BARE_MAGIC 0x7781648d
#define MYBPF_BARE_BSS_BLOCK_SIZE 16

typedef struct {
    U32 sub_size; 
    U32 utc_sec;

    U16 bss_size; 
    U16 app_ver;
    U16 depends_count;
    U16 reserved;
}MYBPF_BARE_SUB_HDR_S;

typedef struct {
    U32 magic;
    U32 total_size; 

    U8 ver;
    U8 jit_arch;
    U8 resereved[6];
}MYBPF_BARE_HDR_S;

typedef struct {
    int prog_len;
    void *prog;
    MYBPF_AOT_PROG_CTX_S ctx;
}MYBPF_BARE_S;

#ifdef __cplusplus
}
#endif
#endif 
