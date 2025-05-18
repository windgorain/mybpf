/*================================================================
*   Created by LiXingang, Copyright LiXingang
*   Description: 
*
================================================================*/
#ifndef _MYBPF_VM_H
#define _MYBPF_VM_H

#include "utl/mybpf_utl.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    int tail_call_index;
    PF_PRINT_FUNC print_func;
}MYBPF_VM_S;

typedef struct {
    U64 bpf_ret;
    void *begin_addr; 
    void *end_addr; 
    void *mybpf_prog; 
    void *insts; 
    void *mem;        
    U32 mem_len; 
    U32 mem_check: 1; 
    U32 auto_stack: 1; 
    U32 helper_fixed: 1; 
    int stack_size;
    char *stack;
    const void **tmp_helpers;
}MYBPF_CTX_S;

int MYBPF_SetTailCallIndex(MYBPF_VM_S *vm, unsigned int idx);
BOOL_T MYBPF_Validate(MYBPF_VM_S *vm, void *insn, UINT num_insts, const void **tmp_helpers);
int MYBPF_VmRun(MYBPF_VM_S *vm, MYBPF_CTX_S *ctx, MYBPF_PARAM_S *p);


int MYBPF_DefultRun(MYBPF_CTX_S *ctx, MYBPF_PARAM_S *p);

#ifdef __cplusplus
}
#endif
#endif 
