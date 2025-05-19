/*********************************************************
*   Copyright (C) LiXingang
*   Description: 
*
********************************************************/
#ifndef _BPFASM_UTL_H
#define _BPFASM_UTL_H

#include "utl/mybpf_utl.h"
#include "utl/mybpf_vm.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    char *func_name;
    UINT insn_offset;
}BPFASM_FUNC_S;

typedef struct {
    BPFASM_FUNC_S *funcs;
    void *begin_addr;
    void *end_addr;
}BPFASM_S;

int BPFASM_Run(BPFASM_S *ctrl, const char *func_name, OUT UINT64 *bpf_ret, MYBPF_PARAM_S *p);

#ifdef __cplusplus
}
#endif
#endif 
