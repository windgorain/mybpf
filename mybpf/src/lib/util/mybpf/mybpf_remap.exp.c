#include "bs.h" 
#include "utl/mybpf_regs.h" 
#include "utl/mybpf_asmdef.h" 
#include "utl/bpfasm_utl.h" 

static MYBPF_INSN_S g_bpfasm_insts[] = { 

    BPF_MOV64_REG(BPF_R5, BPF_R3), 
    BPF_MOV64_REG(BPF_R7, BPF_R1), 
    BPF_MOV64_IMM(BPF_R0, 0), 
    BPF_ALU64_IMM(BPF_LSH, BPF_R2, 32), 
    BPF_MOV64_REG(BPF_R1, BPF_R2), 
    BPF_ALU64_IMM(BPF_ARSH, BPF_R1, 32), 
    BPF_MOV64_IMM(BPF_R3, 8), 
    BPF_JMP_REG(BPF_JSGT, BPF_R3, BPF_R1, 165), 
    BPF_ALU64_IMM(BPF_ARSH, BPF_R2, 35), 
    BPF_MOV64_IMM(BPF_R6, 0), 
    BPF_MOV64_REG(BPF_R8, BPF_R2), 
    BPF_ALU64_IMM(BPF_ADD, BPF_R8, -1), 
    BPF_MOV64_IMM(BPF_R9, 16), 
    BPF_STX_MEM(BPF_DW, BPF_R10, BPF_R2, -48), 
    BPF_STX_MEM(BPF_DW, BPF_R10, BPF_R8, -56), 
    BPF_JMP_A(7), 
    BPF_MOV64_REG(BPF_R0, BPF_R1), 
    BPF_ALU64_IMM(BPF_AND, BPF_R0, 224), 
    BPF_JMP_IMM(BPF_JNE, BPF_R0, 96, 2), 
    BPF_LDX_MEM(BPF_W, BPF_R3, BPF_R7, 4), 
    BPF_JMP_IMM(BPF_JEQ, BPF_R3, 0, 28), 
    BPF_JMP_IMM(BPF_JEQ, BPF_R0, 192, 143), 
    BPF_JMP_A(10), 
    BPF_LDX_MEM(BPF_B, BPF_R1, BPF_R7, 0), 
    BPF_MOV64_REG(BPF_R3, BPF_R1), 
    BPF_ALU64_IMM(BPF_AND, BPF_R3, 7), 
    BPF_JMP_IMM(BPF_JEQ, BPF_R3, 3, -11), 
    BPF_JMP_IMM(BPF_JNE, BPF_R3, 1, 21), 
    BPF_MOV64_REG(BPF_R3, BPF_R1), 
    BPF_ALU64_IMM(BPF_AND, BPF_R3, 224), 
    BPF_JMP_IMM(BPF_JNE, BPF_R3, 96, 2), 
    BPF_LDX_MEM(BPF_W, BPF_R3, BPF_R7, 4), 
    BPF_JMP_IMM(BPF_JEQ, BPF_R3, 0, 16), 
    BPF_MOV64_IMM(BPF_R1, 0), 
    BPF_STX_MEM(BPF_B, BPF_R10, BPF_R1, -14), 
    BPF_MOV64_IMM(BPF_R1, 29989), 
    BPF_STX_MEM(BPF_H, BPF_R10, BPF_R1, -16), 
    BPF_LD_IMM64_RAW(BPF_R1, BPF_R0, 0x3d6d6d69202c7825LL), 
    BPF_STX_MEM(BPF_DW, BPF_R10, BPF_R1, -24), 
    BPF_LD_IMM64_RAW(BPF_R1, BPF_R0, 0x78303d65646f6370LL), 
    BPF_STX_MEM(BPF_DW, BPF_R10, BPF_R1, -32), 
    BPF_LD_IMM64_RAW(BPF_R1, BPF_R0, 0x6f202c64253d6370LL), 
    BPF_STX_MEM(BPF_DW, BPF_R10, BPF_R1, -40), 
    BPF_LDX_MEM(BPF_W, BPF_R5, BPF_R7, 4), 
    BPF_LDX_MEM(BPF_B, BPF_R4, BPF_R7, 0), 
    BPF_JMP_A(125), 
    BPF_JMP_IMM(BPF_JNE, BPF_R1, 24, 115), 
    BPF_MOV64_REG(BPF_R1, BPF_R8), 
    BPF_ALU64_IMM(BPF_LSH, BPF_R1, 32), 
    BPF_ALU64_IMM(BPF_RSH, BPF_R1, 32), 
    BPF_MOV64_REG(BPF_R0, BPF_R6), 
    BPF_ALU64_IMM(BPF_LSH, BPF_R0, 32), 
    BPF_ALU64_IMM(BPF_RSH, BPF_R0, 32), 
    BPF_MOV64_REG(BPF_R3, BPF_R8), 
    BPF_JMP_REG(BPF_JEQ, BPF_R0, BPF_R1, 9), 
    BPF_LDX_MEM(BPF_B, BPF_R1, BPF_R7, 8), 
    BPF_MOV64_REG(BPF_R3, BPF_R6), 
    BPF_JMP_IMM(BPF_JNE, BPF_R1, 0, 6), 
    BPF_LDX_MEM(BPF_B, BPF_R1, BPF_R7, 9), 
    BPF_MOV64_REG(BPF_R3, BPF_R6), 
    BPF_JMP_IMM(BPF_JNE, BPF_R1, 0, 3), 
    BPF_LDX_MEM(BPF_H, BPF_R1, BPF_R7, 10), 
    BPF_MOV64_REG(BPF_R3, BPF_R6), 
    BPF_JMP_IMM(BPF_JEQ, BPF_R1, 0, 18), 
    BPF_MOV64_IMM(BPF_R1, 0), 
    BPF_STX_MEM(BPF_B, BPF_R10, BPF_R1, -22), 
    BPF_MOV64_IMM(BPF_R1, 30757), 
    BPF_STX_MEM(BPF_H, BPF_R10, BPF_R1, -24), 
    BPF_LD_IMM64_RAW(BPF_R1, BPF_R0, 0x78303d65646f6370LL), 
    BPF_STX_MEM(BPF_DW, BPF_R10, BPF_R1, -32), 
    BPF_LD_IMM64_RAW(BPF_R1, BPF_R0, 0x6f202c64253d6370LL), 
    BPF_STX_MEM(BPF_DW, BPF_R10, BPF_R1, -40), 
    BPF_LDX_MEM(BPF_B, BPF_R4, BPF_R7, 0), 
    BPF_MOV64_REG(BPF_R1, BPF_R10), 
    BPF_ALU64_IMM(BPF_ADD, BPF_R1, -40), 
    BPF_MOV64_IMM(BPF_R2, 19), 
    BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0, 6), 
    BPF_LD_IMM64_RAW(BPF_R0, BPF_R0, 0xffffffffLL), 
    BPF_JMP_A(88), 
    BPF_LDX_MEM(BPF_B, BPF_R1, BPF_R7, 1), 
    BPF_JMP_REG(BPF_JGT, BPF_R9, BPF_R1, 76), 
    BPF_MOV64_REG(BPF_R3, BPF_R1), 
    BPF_ALU64_IMM(BPF_RSH, BPF_R3, 4), 
    BPF_ALU64_IMM(BPF_ADD, BPF_R3, -1), 
    BPF_JMP_IMM(BPF_JGT, BPF_R3, 1, 72), 
    BPF_LDX_MEM(BPF_W, BPF_R3, BPF_R7, 12), 
    BPF_ALU64_IMM(BPF_AND, BPF_R1, 240), 
    BPF_JMP_IMM(BPF_JNE, BPF_R1, 16, 23), 
    BPF_JMP_IMM(BPF_JEQ, BPF_R3, 0, 42), 
    BPF_MOV64_IMM(BPF_R1, 0), 
    BPF_STX_MEM(BPF_B, BPF_R10, BPF_R1, -6), 
    BPF_MOV64_IMM(BPF_R1, 29989), 
    BPF_STX_MEM(BPF_H, BPF_R10, BPF_R1, -8), 
    BPF_LD_IMM64_RAW(BPF_R1, BPF_R0, 0x3d6d6d692e5d315bLL), 
    BPF_STX_MEM(BPF_DW, BPF_R10, BPF_R1, -16), 
    BPF_LD_IMM64_RAW(BPF_R1, BPF_R0, 0x6e736e69202c7825LL), 
    BPF_STX_MEM(BPF_DW, BPF_R10, BPF_R1, -24), 
    BPF_LD_IMM64_RAW(BPF_R1, BPF_R0, 0x78303d65646f6370LL), 
    BPF_STX_MEM(BPF_DW, BPF_R10, BPF_R1, -32), 
    BPF_LD_IMM64_RAW(BPF_R1, BPF_R0, 0x6f202c64253d6370LL), 
    BPF_STX_MEM(BPF_DW, BPF_R10, BPF_R1, -40), 
    BPF_LDX_MEM(BPF_W, BPF_R5, BPF_R7, 12), 
    BPF_LDX_MEM(BPF_B, BPF_R4, BPF_R7, 0), 
    BPF_MOV64_REG(BPF_R1, BPF_R10), 
    BPF_ALU64_IMM(BPF_ADD, BPF_R1, -40), 
    BPF_MOV64_IMM(BPF_R2, 35), 
    BPF_JMP_A(60), 
    BPF_MOV64_IMM(BPF_R1, 536870912), 
    BPF_JMP_REG(BPF_JGT, BPF_R1, BPF_R3, 26), 
    BPF_MOV64_IMM(BPF_R1, 0), 
    BPF_STX_MEM(BPF_B, BPF_R10, BPF_R1, -14), 
    BPF_MOV64_IMM(BPF_R1, 29989), 
    BPF_STX_MEM(BPF_H, BPF_R10, BPF_R1, -16), 
    BPF_LD_IMM64_RAW(BPF_R1, BPF_R0, 0x3d66666f202c7825LL), 
    BPF_STX_MEM(BPF_DW, BPF_R10, BPF_R1, -24), 
    BPF_LD_IMM64_RAW(BPF_R1, BPF_R0, 0x78303d65646f6370LL), 
    BPF_STX_MEM(BPF_DW, BPF_R10, BPF_R1, -32), 
    BPF_LD_IMM64_RAW(BPF_R1, BPF_R0, 0x6f202c64253d6370LL), 
    BPF_STX_MEM(BPF_DW, BPF_R10, BPF_R1, -40), 
    BPF_LDX_MEM(BPF_B, BPF_R4, BPF_R7, 0), 
    BPF_LDX_MEM(BPF_H, BPF_R5, BPF_R7, 2), 
    BPF_ALU64_IMM(BPF_LSH, BPF_R5, 48), 
    BPF_ALU64_IMM(BPF_ARSH, BPF_R5, 48), 
    BPF_JMP_A(37), 
    BPF_LDX_MEM(BPF_W, BPF_R2, BPF_R7, 4), 
    BPF_MOV64_IMM(BPF_R1, 0), 
    BPF_MOV64_IMM(BPF_R3, 0), 
    BPF_MOV64_REG(BPF_R9, BPF_R4), 
    BPF_MOV64_REG(BPF_R8, BPF_R5), 
    BPF_RAW_INSN(BPF_JMP | BPF_CALL | BPF_X, 0, 0, 0, 5), 
    BPF_JMP_IMM(BPF_JNE, BPF_R0, 0, 11), 
    BPF_JMP_A(-112), 
    BPF_LDX_MEM(BPF_W, BPF_R2, BPF_R7, 4), 
    BPF_MOV64_IMM(BPF_R1, 1), 
    BPF_STX_MEM(BPF_DW, BPF_R10, BPF_R3, -64), 
    BPF_MOV64_REG(BPF_R9, BPF_R4), 
    BPF_MOV64_REG(BPF_R8, BPF_R5), 
    BPF_RAW_INSN(BPF_JMP | BPF_CALL | BPF_X, 0, 0, 0, 5), 
    BPF_JMP_IMM(BPF_JNE, BPF_R0, 0, 1), 
    BPF_JMP_A(-120), 
    BPF_LDX_MEM(BPF_DW, BPF_R1, BPF_R10, -64), 
    BPF_ALU64_REG(BPF_ADD, BPF_R0, BPF_R1), 
    BPF_STX_MEM(BPF_W, BPF_R7, BPF_R0, 4), 
    BPF_ALU64_IMM(BPF_RSH, BPF_R0, 32), 
    BPF_STX_MEM(BPF_W, BPF_R7, BPF_R0, 12), 
    BPF_MOV64_REG(BPF_R4, BPF_R9), 
    BPF_MOV64_REG(BPF_R5, BPF_R8), 
    BPF_LDX_MEM(BPF_DW, BPF_R2, BPF_R10, -48), 
    BPF_LDX_MEM(BPF_DW, BPF_R8, BPF_R10, -56), 
    BPF_MOV64_IMM(BPF_R9, 16), 
    BPF_ALU64_IMM(BPF_ADD, BPF_R6, 1), 
    BPF_ALU64_IMM(BPF_ADD, BPF_R7, 8), 
    BPF_ALU64_IMM(BPF_LSH, BPF_R6, 32), 
    BPF_LD_IMM64_RAW(BPF_R1, BPF_R0, 0x100000000LL), 
    BPF_ALU64_REG(BPF_ADD, BPF_R6, BPF_R1), 
    BPF_MOV64_IMM(BPF_R0, 0), 
    BPF_ALU64_IMM(BPF_ADD, BPF_R7, 8), 
    BPF_ALU64_IMM(BPF_ARSH, BPF_R6, 32), 
    BPF_JMP_REG(BPF_JSGT, BPF_R2, BPF_R6, -150), 
    BPF_EXIT_INSN(), 
    BPF_MOV64_REG(BPF_R1, BPF_R10), 
    BPF_ALU64_IMM(BPF_ADD, BPF_R1, -40), 
    BPF_MOV64_IMM(BPF_R2, 27), 
    BPF_MOV64_REG(BPF_R3, BPF_R6), 
    BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0, 6), 
    BPF_LD_IMM64_RAW(BPF_R0, BPF_R0, 0xffffffffLL), 
    BPF_JMP_A(-9), 
}; 

U64 MYBPF_PROG_ReplaceMapFdWithMapPtr(U64 p1, U64 p2, U64 p3, U64 p4, U64 p5) 
{ 
    MYBPF_PARAM_S p; 
    MYBPF_CTX_S ctx = {0}; 

    ctx.begin_addr = g_bpfasm_insts; 
    ctx.end_addr = (char*)(void*)g_bpfasm_insts + sizeof(g_bpfasm_insts); 
    ctx.insts = (char*)g_bpfasm_insts; 
    p.p[0]=p1; p.p[1]=p2; p.p[2]=p3; p.p[3]=p4; p.p[4]=p5; 
    int ret = MYBPF_DefultRun(&ctx, &p); 
    if (ret < 0) return ret; 
    return ctx.bpf_ret; 
} 

