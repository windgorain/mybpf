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

#define BPFASM_RUN(_ctrl, _name, _bpf_ret, ...) \
        BPFASM_Run(_ctrl, _name, _bpf_ret, \
                (long)BS_ARG_GET(1, ##__VA_ARGS__), (long)BS_ARG_GET(2, ##__VA_ARGS__), \
                (long)BS_ARG_GET(3, ##__VA_ARGS__), (long)BS_ARG_GET(4, ##__VA_ARGS__), \
                (long)BS_ARG_GET(5, ##__VA_ARGS__))


#ifdef __cplusplus
}
#endif
#endif 
