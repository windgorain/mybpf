/*********************************************************
*   Copyright (C) LiXingang
*   Description: 
*
********************************************************/
#ifndef _MYBPF_FILE_H
#define _MYBPF_FILE_H
#include "utl/elf_utl.h"
#include "utl/mybpf_elf.h"
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    void *data;
    int len;
}MBPF_FILE_PROG_S;

typedef struct {
    char *file; 
    char *sec_name; 
    int jit;  
}MYBPF_FILE_CTX_S;

int MYBPF_RunFileExt(MYBPF_FILE_CTX_S *ctx, MYBPF_PARAM_S *p);
int MYBPF_RunFile(char *filename, char *sec_name, MYBPF_PARAM_S *p);
int MYBPF_ShowPcAccessGlobal(char *filename);

#ifdef __cplusplus
}
#endif
#endif 
