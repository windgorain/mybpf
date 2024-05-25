/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _MYBPF_BARE_DEF_H_
#define _MYBPF_BARE_DEF_H_

#include "utl/mybpf_aot_def.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MYBPF_BARE_MAGIC 0x7781648d
#define MYBPF_BARE_BLOCK_SIZE 16

typedef struct {
    U32 sub_size; 
    U32 utc_sec;  
    U32 depends_count; 
    U32 bss_size; 
    U16 app_ver;
    U16 sub_hdr_size;
    U8  reserved[12];
}MYBPF_BARE_SUB_HDR_S;

typedef struct {
    U32 magic;
    U32 total_size; 
    U8 ver;
    U8 arch;
    U16 hdr_size; 
    U8 resereved[4];
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
