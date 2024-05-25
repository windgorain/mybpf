/*********************************************************
*   Copyright (C) LiXingang
*   Description: 
*
********************************************************/
#ifndef _MYBPF_INSN_H
#define _MYBPF_INSN_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    UINT is_bpf_2_bpf: 1;
    UINT insn_index: 31;
    int imm;
}MYBPF_INSN_CALLS_S;

typedef int (*PF_MYBPF_INSN_WALK)(void *insts, int insn_idx, void *ud);
typedef void * PF_MYBPF_INSN_MAP_FD_2_PTR(int is_direct, int imm, int off, void *ud);
typedef int (*PF_MYBPF_INSN_EXTCALL_CONVERT)(int imm, void *ud);

int MYBPF_INSN_GetCallsCount(void *insts, int insts_len);
int MYBPF_INSN_GetCallsInfo(void *insts, int insts_len, MYBPF_INSN_CALLS_S *calls, int max_count);
int MYBPF_INSN_WalkCalls(void *insts, int insts_len, PF_MYBPF_INSN_WALK walk_func, void *ud);
int MYBPF_INSN_WalkExternCalls(void *insts, int insts_len, PF_MYBPF_INSN_WALK walk_func, void *ud);
int MYBPF_INSN_WalkLddw(void *insts, int insts_len, PF_MYBPF_INSN_WALK walk_func, void *ud);
int MYBPF_INSN_FixupExtCalls(void *insts, int len, PF_MYBPF_INSN_EXTCALL_CONVERT convert_imm, void *ud);
int MYBPF_INSN_GetStackSize(void *insts, int insts_len );
int MYBPF_INSN_GetStackSizeUntilExit(void *insts);
int MYBPF_INSN_ShowPcAccessGlobal(void *insts, int insts_len);
void MYBPF_INSN_ModifyTextOff(void *insts, int insts_len, int text_off );

int MYBPF_PROG_ReplaceMapFdWithMapPtr(void *insts, int insts_len,
        PF_MYBPF_INSN_MAP_FD_2_PTR map_2_ptr_fn, void *ud);

#ifdef __cplusplus
}
#endif
#endif 
