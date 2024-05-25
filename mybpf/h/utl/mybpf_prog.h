/*================================================================
*   Created by LiXingang, Copyright LiXingang
*   Description: 
*
================================================================*/
#ifndef _MYBPF_PROG_H
#define _MYBPF_PROG_H

#include "utl/mybpf_utl.h"
#include "utl/mybpf_runtime.h"
#include "utl/mybpf_vm.h"
#include "utl/mybpf_prog_def.h"

#ifdef __cplusplus
extern "C"
{
#endif

void MYBPF_PROG_Free(MYBPF_RUNTIME_S *runtime, MYBPF_PROG_NODE_S *prog);
void MYBPF_PROG_ShowProg(MYBPF_RUNTIME_S *runtime, PF_PRINT_FUNC print_func);
MYBPF_PROG_NODE_S * MYBPF_PROG_GetByFuncName(MYBPF_RUNTIME_S *runtime, char *instance, char *func_name);
MYBPF_PROG_NODE_S * MYBPF_PROG_GetBySecName(MYBPF_RUNTIME_S *runtime, char *instance, char *sec_name);
MYBPF_PROG_NODE_S * MYBPF_PROG_GetNext(MYBPF_RUNTIME_S *runtime, char *instance, char *sec_name,
        MYBPF_PROG_NODE_S *current);
int MYBPF_PROG_Run(MYBPF_PROG_NODE_S *prog, MYBPF_PARAM_S *p);

int MYBPF_PROG_FixupExtCalls(void *insts, int len, const void **tmp_helpers);
void MYBPF_FixBaseAddr(void);

#ifdef __cplusplus
}
#endif
#endif 
