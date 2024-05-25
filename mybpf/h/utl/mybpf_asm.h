/*********************************************************
*   Copyright (C) LiXingang
*   Description: 
*
********************************************************/
#ifndef _MYBPF_ASM_H
#define _MYBPF_ASM_H
#ifdef __cplusplus
extern "C"
{
#endif

#define MYBPF_DUMP_FLAG_ASM 0x1
#define MYBPF_DUMP_FLAG_EXP 0x2
#define MYBPF_DUMP_FLAG_RAW 0x4
#define MYBPF_DUMP_FLAG_LINE 0x8

int MYBPF_ASM_Insn2Asm(MYBPF_INSN_S *insn, int insn_count, int insn_idx, OUT char *buf, int size);
int MYBPF_ASM_Insn2Exp(MYBPF_INSN_S *insn, int insn_count, int insn_idx, OUT char *buf, int size);
void MYBPF_ASM_DumpAsm(void *data, int len, UINT flag);

#ifdef __cplusplus
}
#endif
#endif 
