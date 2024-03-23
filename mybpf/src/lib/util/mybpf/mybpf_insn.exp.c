#include "bs.h" 
#include "utl/mybpf_regs.h" 
#include "utl/mybpf_asmdef.h" 
#include "utl/bpfasm_utl.h" 

static MYBPF_INSN_S g_bpfasm_insts[] = { 

    BPF_MOV64_IMM(BPF_R0, 0), 
    BPF_ALU64_IMM(BPF_LSH, BPF_R2, 32), 
    BPF_MOV64_REG(BPF_R3, BPF_R2), 
    BPF_ALU64_IMM(BPF_ARSH, BPF_R3, 32), 
    BPF_MOV64_IMM(BPF_R4, 8), 
    BPF_JMP_REG(BPF_JSGT, BPF_R4, BPF_R3, 12), 
    BPF_ALU64_IMM(BPF_ARSH, BPF_R2, 35), 
    BPF_MOV64_IMM(BPF_R0, 0), 
    BPF_LD_IMM64_RAW(BPF_R3, BPF_R0, 0x100000000LL), 
    BPF_MOV64_IMM(BPF_R4, 0), 
    BPF_JMP_A(7), 
    BPF_ALU64_REG(BPF_ADD, BPF_R0, BPF_R5), 
    BPF_ALU64_IMM(BPF_LSH, BPF_R4, 32), 
    BPF_ALU64_REG(BPF_ADD, BPF_R4, BPF_R3), 
    BPF_ALU64_IMM(BPF_ADD, BPF_R1, 8), 
    BPF_ALU64_IMM(BPF_ARSH, BPF_R4, 32), 
    BPF_JMP_REG(BPF_JSGT, BPF_R2, BPF_R4, 1), 
    BPF_EXIT_INSN(), 
    BPF_LDX_MEM(BPF_B, BPF_R6, BPF_R1, 0), 
    BPF_MOV64_IMM(BPF_R5, 1), 
    BPF_JMP_IMM(BPF_JEQ, BPF_R6, 133, -10), 
    BPF_MOV64_IMM(BPF_R5, 0), 
    BPF_JMP_A(-12), 

    BPF_STX_MEM(BPF_DW, BPF_R10, BPF_R3, -8), 
    BPF_MOV64_IMM(BPF_R0, 0), 
    BPF_ALU64_IMM(BPF_LSH, BPF_R2, 32), 
    BPF_MOV64_REG(BPF_R5, BPF_R2), 
    BPF_ALU64_IMM(BPF_ARSH, BPF_R5, 32), 
    BPF_MOV64_IMM(BPF_R6, 8), 
    BPF_JMP_REG(BPF_JSGT, BPF_R6, BPF_R5, 30), 
    BPF_ALU64_IMM(BPF_ARSH, BPF_R2, 35), 
    BPF_MOV64_IMM(BPF_R5, 0), 
    BPF_ALU64_IMM(BPF_LSH, BPF_R4, 32), 
    BPF_ALU64_IMM(BPF_ARSH, BPF_R4, 32), 
    BPF_LD_IMM64_RAW(BPF_R6, BPF_R0, 0x100000000LL), 
    BPF_MOV64_IMM(BPF_R7, 0), 
    BPF_MOV64_IMM(BPF_R8, 0), 
    BPF_JMP_A(22), 
    BPF_MOV64_REG(BPF_R9, BPF_R8), 
    BPF_ALU64_IMM(BPF_LSH, BPF_R9, 32), 
    BPF_ALU64_IMM(BPF_ARSH, BPF_R9, 32), 
    BPF_ALU64_IMM(BPF_LSH, BPF_R9, 3), 
    BPF_LDX_MEM(BPF_DW, BPF_R3, BPF_R10, -8), 
    BPF_ALU64_REG(BPF_ADD, BPF_R3, BPF_R9), 
    BPF_LDX_MEM(BPF_W, BPF_R9, BPF_R3, 0), 
    BPF_ALU64_IMM(BPF_AND, BPF_R9, 1), 
    BPF_ALU64_REG(BPF_OR, BPF_R9, BPF_R0), 
    BPF_ALU64_REG(BPF_OR, BPF_R9, BPF_R5), 
    BPF_STX_MEM(BPF_W, BPF_R3, BPF_R9, 0), 
    BPF_LDX_MEM(BPF_W, BPF_R0, BPF_R1, 4), 
    BPF_STX_MEM(BPF_W, BPF_R3, BPF_R0, 4), 
    BPF_ALU64_IMM(BPF_ADD, BPF_R8, 1), 
    BPF_ALU64_IMM(BPF_LSH, BPF_R7, 32), 
    BPF_ALU64_REG(BPF_ADD, BPF_R7, BPF_R6), 
    BPF_ALU64_IMM(BPF_ADD, BPF_R5, 2), 
    BPF_ALU64_IMM(BPF_ADD, BPF_R1, 8), 
    BPF_ALU64_IMM(BPF_ARSH, BPF_R7, 32), 
    BPF_MOV64_REG(BPF_R0, BPF_R8), 
    BPF_JMP_REG(BPF_JSGT, BPF_R2, BPF_R7, 1), 
    BPF_EXIT_INSN(), 
    BPF_LDX_MEM(BPF_B, BPF_R0, BPF_R1, 0), 
    BPF_JMP_IMM(BPF_JNE, BPF_R0, 133, -10), 
    BPF_LD_IMM64_RAW(BPF_R0, BPF_R0, 0xfffffff8LL), 
    BPF_MOV64_REG(BPF_R9, BPF_R8), 
    BPF_ALU64_IMM(BPF_LSH, BPF_R9, 32), 
    BPF_ALU64_IMM(BPF_ARSH, BPF_R9, 32), 
    BPF_JMP_REG(BPF_JSGE, BPF_R9, BPF_R4, -9), 
    BPF_LDX_MEM(BPF_B, BPF_R9, BPF_R1, 1), 
    BPF_ALU64_IMM(BPF_AND, BPF_R9, 240), 
    BPF_MOV64_IMM(BPF_R0, 1), 
    BPF_JMP_IMM(BPF_JEQ, BPF_R9, 16, -34), 
    BPF_MOV64_IMM(BPF_R0, 0), 
    BPF_JMP_A(-36), 

    BPF_STX_MEM(BPF_DW, BPF_R10, BPF_R4, -8), 
    BPF_STX_MEM(BPF_DW, BPF_R10, BPF_R3, -16), 
    BPF_MOV64_REG(BPF_R8, BPF_R2), 
    BPF_MOV64_REG(BPF_R7, BPF_R1), 
    BPF_MOV64_IMM(BPF_R0, 0), 
    BPF_ALU64_IMM(BPF_LSH, BPF_R8, 32), 
    BPF_MOV64_REG(BPF_R1, BPF_R8), 
    BPF_ALU64_IMM(BPF_ARSH, BPF_R1, 32), 
    BPF_MOV64_IMM(BPF_R2, 8), 
    BPF_JMP_REG(BPF_JSGT, BPF_R2, BPF_R1, 24), 
    BPF_ALU64_IMM(BPF_ARSH, BPF_R8, 35), 
    BPF_LD_IMM64_RAW(BPF_R2, BPF_R0, 0x100000000LL), 
    BPF_MOV64_IMM(BPF_R6, 0), 
    BPF_MOV64_REG(BPF_R9, BPF_R7), 
    BPF_LDX_MEM(BPF_B, BPF_R1, BPF_R9, 0), 
    BPF_JMP_IMM(BPF_JNE, BPF_R1, 133, 11), 
    BPF_MOV64_REG(BPF_R1, BPF_R7), 
    BPF_MOV64_REG(BPF_R2, BPF_R6), 
    BPF_LDX_MEM(BPF_DW, BPF_R3, BPF_R10, -8), 
    BPF_LDX_MEM(BPF_DW, BPF_R4, BPF_R10, -16), 
    BPF_RAW_INSN(BPF_JMP | BPF_CALL | BPF_X, 0, 0, 0, 4), 
    BPF_LD_IMM64_RAW(BPF_R2, BPF_R0, 0x100000000LL), 
    BPF_ALU64_IMM(BPF_LSH, BPF_R0, 32), 
    BPF_ALU64_IMM(BPF_ARSH, BPF_R0, 32), 
    BPF_MOV64_IMM(BPF_R1, 0), 
    BPF_JMP_REG(BPF_JSGT, BPF_R1, BPF_R0, 6), 
    BPF_ALU64_IMM(BPF_LSH, BPF_R6, 32), 
    BPF_ALU64_REG(BPF_ADD, BPF_R6, BPF_R2), 
    BPF_MOV64_IMM(BPF_R0, 0), 
    BPF_ALU64_IMM(BPF_ADD, BPF_R9, 8), 
    BPF_ALU64_IMM(BPF_ARSH, BPF_R6, 32), 
    BPF_JMP_REG(BPF_JSGT, BPF_R8, BPF_R6, -19), 
    BPF_EXIT_INSN(), 

    BPF_STX_MEM(BPF_DW, BPF_R10, BPF_R4, -16), 
    BPF_STX_MEM(BPF_DW, BPF_R10, BPF_R3, -24), 
    BPF_MOV64_REG(BPF_R8, BPF_R2), 
    BPF_STX_MEM(BPF_DW, BPF_R10, BPF_R1, -8), 
    BPF_MOV64_IMM(BPF_R0, 0), 
    BPF_ALU64_IMM(BPF_LSH, BPF_R8, 32), 
    BPF_MOV64_REG(BPF_R1, BPF_R8), 
    BPF_ALU64_IMM(BPF_ARSH, BPF_R1, 32), 
    BPF_MOV64_IMM(BPF_R2, 8), 
    BPF_JMP_REG(BPF_JSGT, BPF_R2, BPF_R1, 26), 
    BPF_ALU64_IMM(BPF_ARSH, BPF_R8, 35), 
    BPF_LDX_MEM(BPF_DW, BPF_R9, BPF_R10, -8), 
    BPF_ALU64_IMM(BPF_ADD, BPF_R9, 1), 
    BPF_LD_IMM64_RAW(BPF_R7, BPF_R0, 0x100000000LL), 
    BPF_MOV64_IMM(BPF_R6, 0), 
    BPF_LDX_MEM(BPF_B, BPF_R1, BPF_R9, -1), 
    BPF_JMP_IMM(BPF_JNE, BPF_R1, 133, 12), 
    BPF_LDX_MEM(BPF_B, BPF_R1, BPF_R9, 0), 
    BPF_ALU64_IMM(BPF_AND, BPF_R1, 240), 
    BPF_JMP_IMM(BPF_JEQ, BPF_R1, 16, 9), 
    BPF_LDX_MEM(BPF_DW, BPF_R1, BPF_R10, -8), 
    BPF_MOV64_REG(BPF_R2, BPF_R6), 
    BPF_LDX_MEM(BPF_DW, BPF_R3, BPF_R10, -16), 
    BPF_LDX_MEM(BPF_DW, BPF_R4, BPF_R10, -24), 
    BPF_RAW_INSN(BPF_JMP | BPF_CALL | BPF_X, 0, 0, 0, 4), 
    BPF_ALU64_IMM(BPF_LSH, BPF_R0, 32), 
    BPF_ALU64_IMM(BPF_ARSH, BPF_R0, 32), 
    BPF_MOV64_IMM(BPF_R1, 0), 
    BPF_JMP_REG(BPF_JSGT, BPF_R1, BPF_R0, 6), 
    BPF_ALU64_IMM(BPF_LSH, BPF_R6, 32), 
    BPF_ALU64_REG(BPF_ADD, BPF_R6, BPF_R7), 
    BPF_MOV64_IMM(BPF_R0, 0), 
    BPF_ALU64_IMM(BPF_ADD, BPF_R9, 8), 
    BPF_ALU64_IMM(BPF_ARSH, BPF_R6, 32), 
    BPF_JMP_REG(BPF_JSGT, BPF_R8, BPF_R6, -20), 
    BPF_EXIT_INSN(), 

    BPF_STX_MEM(BPF_DW, BPF_R10, BPF_R4, -8), 
    BPF_STX_MEM(BPF_DW, BPF_R10, BPF_R3, -16), 
    BPF_MOV64_REG(BPF_R8, BPF_R2), 
    BPF_MOV64_REG(BPF_R7, BPF_R1), 
    BPF_MOV64_IMM(BPF_R0, 0), 
    BPF_ALU64_IMM(BPF_LSH, BPF_R8, 32), 
    BPF_MOV64_REG(BPF_R1, BPF_R8), 
    BPF_ALU64_IMM(BPF_ARSH, BPF_R1, 32), 
    BPF_MOV64_IMM(BPF_R2, 8), 
    BPF_JMP_REG(BPF_JSGT, BPF_R2, BPF_R1, 26), 
    BPF_ALU64_IMM(BPF_ARSH, BPF_R8, 35), 
    BPF_LD_IMM64_RAW(BPF_R2, BPF_R0, 0x100000000LL), 
    BPF_MOV64_IMM(BPF_R6, 0), 
    BPF_MOV64_REG(BPF_R9, BPF_R7), 
    BPF_LDX_MEM(BPF_B, BPF_R1, BPF_R9, 0), 
    BPF_JMP_IMM(BPF_JNE, BPF_R1, 24, 13), 
    BPF_MOV64_REG(BPF_R1, BPF_R7), 
    BPF_MOV64_REG(BPF_R2, BPF_R6), 
    BPF_LDX_MEM(BPF_DW, BPF_R3, BPF_R10, -8), 
    BPF_LDX_MEM(BPF_DW, BPF_R4, BPF_R10, -16), 
    BPF_RAW_INSN(BPF_JMP | BPF_CALL | BPF_X, 0, 0, 0, 4), 
    BPF_LD_IMM64_RAW(BPF_R2, BPF_R0, 0x100000000LL), 
    BPF_ALU64_IMM(BPF_LSH, BPF_R0, 32), 
    BPF_ALU64_IMM(BPF_ARSH, BPF_R0, 32), 
    BPF_MOV64_IMM(BPF_R1, 0), 
    BPF_JMP_REG(BPF_JSGT, BPF_R1, BPF_R0, 8), 
    BPF_ALU64_IMM(BPF_ADD, BPF_R9, 8), 
    BPF_ALU64_IMM(BPF_ADD, BPF_R6, 1), 
    BPF_ALU64_IMM(BPF_LSH, BPF_R6, 32), 
    BPF_ALU64_REG(BPF_ADD, BPF_R6, BPF_R2), 
    BPF_MOV64_IMM(BPF_R0, 0), 
    BPF_ALU64_IMM(BPF_ADD, BPF_R9, 8), 
    BPF_ALU64_IMM(BPF_ARSH, BPF_R6, 32), 
    BPF_JMP_REG(BPF_JSGT, BPF_R8, BPF_R6, -21), 
    BPF_EXIT_INSN(), 

    BPF_STX_MEM(BPF_DW, BPF_R10, BPF_R4, -32), 
    BPF_STX_MEM(BPF_DW, BPF_R10, BPF_R3, -40), 
    BPF_MOV64_REG(BPF_R9, BPF_R2), 
    BPF_MOV64_REG(BPF_R6, BPF_R1), 
    BPF_MOV64_IMM(BPF_R0, 0), 
    BPF_ALU64_IMM(BPF_LSH, BPF_R9, 32), 
    BPF_MOV64_REG(BPF_R1, BPF_R9), 
    BPF_ALU64_IMM(BPF_ARSH, BPF_R1, 32), 
    BPF_MOV64_IMM(BPF_R2, 8), 
    BPF_JMP_REG(BPF_JSGT, BPF_R2, BPF_R1, 45), 
    BPF_ALU64_IMM(BPF_ARSH, BPF_R9, 35), 
    BPF_MOV64_IMM(BPF_R7, 0), 
    BPF_ALU64_IMM(BPF_ADD, BPF_R6, 4), 
    BPF_LD_IMM64_RAW(BPF_R8, BPF_R0, 0x100000000LL), 
    BPF_LDX_MEM(BPF_B, BPF_R1, BPF_R6, -4), 
    BPF_JMP_IMM(BPF_JNE, BPF_R1, 133, 32), 
    BPF_LDX_MEM(BPF_B, BPF_R1, BPF_R6, -3), 
    BPF_ALU64_IMM(BPF_AND, BPF_R1, 240), 
    BPF_JMP_IMM(BPF_JEQ, BPF_R1, 16, 29), 
    BPF_LDX_MEM(BPF_W, BPF_R1, BPF_R6, 0), 
    BPF_LDX_MEM(BPF_DW, BPF_R2, BPF_R10, -32), 
    BPF_LDX_MEM(BPF_DW, BPF_R3, BPF_R10, -40), 
    BPF_RAW_INSN(BPF_JMP | BPF_CALL | BPF_X, 0, 0, 0, 3), 
    BPF_MOV64_REG(BPF_R1, BPF_R0), 
    BPF_ALU64_IMM(BPF_LSH, BPF_R1, 32), 
    BPF_ALU64_IMM(BPF_RSH, BPF_R1, 32), 
    BPF_JMP_IMM(BPF_JNE, BPF_R1, 0, 20), 
    BPF_MOV64_IMM(BPF_R1, 0), 
    BPF_STX_MEM(BPF_B, BPF_R10, BPF_R1, -2), 
    BPF_MOV64_IMM(BPF_R1, 25637), 
    BPF_STX_MEM(BPF_H, BPF_R10, BPF_R1, -4), 
    BPF_MOV64_IMM(BPF_R1, 543451424), 
    BPF_STX_MEM(BPF_W, BPF_R10, BPF_R1, -8), 
    BPF_LD_IMM64_RAW(BPF_R1, BPF_R0, 0x636e75662074726fLL), 
    BPF_STX_MEM(BPF_DW, BPF_R10, BPF_R1, -16), 
    BPF_LD_IMM64_RAW(BPF_R1, BPF_R0, 0x7070757320746f4eLL), 
    BPF_STX_MEM(BPF_DW, BPF_R10, BPF_R1, -24), 
    BPF_LDX_MEM(BPF_W, BPF_R3, BPF_R6, 0), 
    BPF_MOV64_REG(BPF_R1, BPF_R10), 
    BPF_ALU64_IMM(BPF_ADD, BPF_R1, -24), 
    BPF_MOV64_IMM(BPF_R2, 23), 
    BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0, 6), 
    BPF_LD_IMM64_RAW(BPF_R0, BPF_R0, 0xfffffff9LL), 
    BPF_JMP_A(7), 
    BPF_STX_MEM(BPF_W, BPF_R6, BPF_R0, 0), 
    BPF_ALU64_IMM(BPF_LSH, BPF_R7, 32), 
    BPF_ALU64_REG(BPF_ADD, BPF_R7, BPF_R8), 
    BPF_MOV64_IMM(BPF_R0, 0), 
    BPF_ALU64_IMM(BPF_ADD, BPF_R6, 8), 
    BPF_ALU64_IMM(BPF_ARSH, BPF_R7, 32), 
    BPF_JMP_REG(BPF_JSGT, BPF_R9, BPF_R7, -40), 
    BPF_EXIT_INSN(), 

    BPF_MOV64_IMM(BPF_R0, 0), 
    BPF_ALU64_IMM(BPF_LSH, BPF_R2, 32), 
    BPF_ALU64_IMM(BPF_ARSH, BPF_R2, 32), 
    BPF_ALU64_IMM(BPF_AND, BPF_R2, -8), 
    BPF_MOV64_REG(BPF_R3, BPF_R1), 
    BPF_ALU64_REG(BPF_ADD, BPF_R3, BPF_R2), 
    BPF_JMP_REG(BPF_JGE, BPF_R1, BPF_R3, 23), 
    BPF_MOV64_IMM(BPF_R0, 0), 
    BPF_ALU64_IMM(BPF_ADD, BPF_R1, 8), 
    BPF_JMP_A(21), 
    BPF_JMP_IMM(BPF_JEQ, BPF_R1, 1, 1), 
    BPF_JMP_IMM(BPF_JNE, BPF_R1, 0, 15), 
    BPF_LDX_MEM(BPF_B, BPF_R1, BPF_R2, -7), 
    BPF_ALU64_IMM(BPF_AND, BPF_R1, 240), 
    BPF_JMP_IMM(BPF_JNE, BPF_R1, 160, 12), 
    BPF_LDX_MEM(BPF_H, BPF_R1, BPF_R2, -6), 
    BPF_ALU64_IMM(BPF_LSH, BPF_R1, 48), 
    BPF_ALU64_IMM(BPF_ARSH, BPF_R1, 48), 
    BPF_ALU64_IMM(BPF_NEG, BPF_R1, 0), 
    BPF_MOV64_REG(BPF_R4, BPF_R1), 
    BPF_ALU64_IMM(BPF_LSH, BPF_R4, 32), 
    BPF_ALU64_IMM(BPF_ARSH, BPF_R4, 32), 
    BPF_MOV64_REG(BPF_R5, BPF_R0), 
    BPF_ALU64_IMM(BPF_LSH, BPF_R5, 32), 
    BPF_ALU64_IMM(BPF_ARSH, BPF_R5, 32), 
    BPF_JMP_REG(BPF_JSGE, BPF_R5, BPF_R4, 1), 
    BPF_MOV64_REG(BPF_R0, BPF_R1), 
    BPF_MOV64_REG(BPF_R1, BPF_R2), 
    BPF_ALU64_IMM(BPF_ADD, BPF_R1, 8), 
    BPF_JMP_REG(BPF_JGT, BPF_R3, BPF_R2, 1), 
    BPF_EXIT_INSN(), 
    BPF_MOV64_REG(BPF_R2, BPF_R1), 
    BPF_LDX_MEM(BPF_B, BPF_R1, BPF_R2, -8), 
    BPF_JMP_IMM(BPF_JNE, BPF_R1, 191, 14), 
    BPF_LDX_MEM(BPF_B, BPF_R4, BPF_R2, -7), 
    BPF_MOV64_REG(BPF_R5, BPF_R4), 
    BPF_ALU64_IMM(BPF_AND, BPF_R5, 240), 
    BPF_JMP_IMM(BPF_JNE, BPF_R5, 160, 10), 
    BPF_JMP_REG(BPF_JGE, BPF_R2, BPF_R3, 9), 
    BPF_LDX_MEM(BPF_B, BPF_R1, BPF_R2, 0), 
    BPF_JMP_IMM(BPF_JEQ, BPF_R1, 23, 14), 
    BPF_JMP_IMM(BPF_JNE, BPF_R1, 7, -15), 
    BPF_LDX_MEM(BPF_B, BPF_R1, BPF_R2, 1), 
    BPF_ALU64_REG(BPF_XOR, BPF_R1, BPF_R4), 
    BPF_ALU64_IMM(BPF_AND, BPF_R1, 15), 
    BPF_JMP_IMM(BPF_JNE, BPF_R1, 0, -19), 
    BPF_LDX_MEM(BPF_W, BPF_R1, BPF_R2, 4), 
    BPF_JMP_A(-30), 
    BPF_ALU64_IMM(BPF_AND, BPF_R1, 7), 
    BPF_JMP_IMM(BPF_JEQ, BPF_R1, 3, 1), 
    BPF_JMP_IMM(BPF_JNE, BPF_R1, 2, -41), 
    BPF_LDX_MEM(BPF_B, BPF_R4, BPF_R2, -7), 
    BPF_ALU64_IMM(BPF_AND, BPF_R4, 15), 
    BPF_JMP_IMM(BPF_JNE, BPF_R4, 10, -44), 
    BPF_JMP_A(-40), 
    BPF_LDX_MEM(BPF_B, BPF_R1, BPF_R2, 1), 
    BPF_ALU64_REG(BPF_XOR, BPF_R1, BPF_R4), 
    BPF_ALU64_IMM(BPF_AND, BPF_R1, 15), 
    BPF_JMP_IMM(BPF_JNE, BPF_R1, 0, -32), 
    BPF_LDX_MEM(BPF_W, BPF_R1, BPF_R2, 4), 
    BPF_JMP_A(-42), 

    BPF_MOV64_IMM(BPF_R0, 0), 
    BPF_LDX_MEM(BPF_B, BPF_R2, BPF_R1, 0), 
    BPF_JMP_IMM(BPF_JEQ, BPF_R2, 149, 6), 
    BPF_MOV64_IMM(BPF_R0, 0), 
    BPF_ALU64_IMM(BPF_ADD, BPF_R1, 8), 
    BPF_JMP_A(4), 
    BPF_LDX_MEM(BPF_B, BPF_R2, BPF_R1, 0), 
    BPF_ALU64_IMM(BPF_ADD, BPF_R1, 8), 
    BPF_JMP_IMM(BPF_JNE, BPF_R2, 149, 1), 
    BPF_EXIT_INSN(), 
    BPF_ALU64_IMM(BPF_AND, BPF_R2, 4), 
    BPF_JMP_IMM(BPF_JNE, BPF_R2, 0, -6), 
    BPF_LDX_MEM(BPF_B, BPF_R2, BPF_R1, -7), 
    BPF_ALU64_IMM(BPF_AND, BPF_R2, 15), 
    BPF_JMP_IMM(BPF_JNE, BPF_R2, 10, -9), 
    BPF_LDX_MEM(BPF_H, BPF_R2, BPF_R1, -6), 
    BPF_ALU64_IMM(BPF_LSH, BPF_R2, 48), 
    BPF_ALU64_IMM(BPF_ARSH, BPF_R2, 48), 
    BPF_ALU64_IMM(BPF_NEG, BPF_R2, 0), 
    BPF_MOV64_REG(BPF_R3, BPF_R0), 
    BPF_ALU64_IMM(BPF_LSH, BPF_R3, 32), 
    BPF_ALU64_IMM(BPF_ARSH, BPF_R3, 32), 
    BPF_JMP_REG(BPF_JSGE, BPF_R3, BPF_R2, -17), 
    BPF_MOV64_REG(BPF_R0, BPF_R2), 
    BPF_JMP_A(-19), 

    BPF_MOV64_REG(BPF_R8, BPF_R2), 
    BPF_MOV64_REG(BPF_R6, BPF_R1), 
    BPF_MOV64_IMM(BPF_R0, 0), 
    BPF_ALU64_IMM(BPF_LSH, BPF_R8, 32), 
    BPF_MOV64_REG(BPF_R1, BPF_R8), 
    BPF_ALU64_IMM(BPF_ARSH, BPF_R1, 32), 
    BPF_MOV64_IMM(BPF_R2, 8), 
    BPF_JMP_REG(BPF_JSGT, BPF_R2, BPF_R1, 40), 
    BPF_ALU64_IMM(BPF_ARSH, BPF_R8, 35), 
    BPF_MOV64_IMM(BPF_R7, 0), 
    BPF_MOV64_REG(BPF_R9, BPF_R8), 
    BPF_ALU64_IMM(BPF_ADD, BPF_R9, -1), 
    BPF_MOV64_IMM(BPF_R4, 16), 
    BPF_LD_IMM64_RAW(BPF_R5, BPF_R0, 0x100000000LL), 
    BPF_JMP_A(33), 
    BPF_LDX_MEM(BPF_W, BPF_R5, BPF_R6, 12), 
    BPF_MOV64_IMM(BPF_R1, 10), 
    BPF_STX_MEM(BPF_H, BPF_R10, BPF_R1, -8), 
    BPF_LD_IMM64_RAW(BPF_R1, BPF_R0, 0x2064252066666f20LL), 
    BPF_STX_MEM(BPF_DW, BPF_R10, BPF_R1, -16), 
    BPF_LD_IMM64_RAW(BPF_R1, BPF_R0, 0x6425206469207061LL), 
    BPF_STX_MEM(BPF_DW, BPF_R10, BPF_R1, -24), 
    BPF_LD_IMM64_RAW(BPF_R1, BPF_R0, 0x6d20737365636361LL), 
    BPF_STX_MEM(BPF_DW, BPF_R10, BPF_R1, -32), 
    BPF_LD_IMM64_RAW(BPF_R1, BPF_R0, 0x206425206e736e69LL), 
    BPF_STX_MEM(BPF_DW, BPF_R10, BPF_R1, -40), 
    BPF_LDX_MEM(BPF_W, BPF_R4, BPF_R6, 4), 
    BPF_MOV64_REG(BPF_R1, BPF_R10), 
    BPF_ALU64_IMM(BPF_ADD, BPF_R1, -40), 
    BPF_MOV64_IMM(BPF_R2, 34), 
    BPF_MOV64_REG(BPF_R3, BPF_R7), 
    BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0, 6), 
    BPF_LD_IMM64_RAW(BPF_R5, BPF_R0, 0x100000000LL), 
    BPF_MOV64_IMM(BPF_R4, 16), 
    BPF_ALU64_IMM(BPF_ADD, BPF_R7, 1), 
    BPF_ALU64_IMM(BPF_ADD, BPF_R6, 8), 
    BPF_ALU64_IMM(BPF_LSH, BPF_R7, 32), 
    BPF_ALU64_REG(BPF_ADD, BPF_R7, BPF_R5), 
    BPF_MOV64_IMM(BPF_R0, 0), 
    BPF_ALU64_IMM(BPF_ADD, BPF_R6, 8), 
    BPF_ALU64_IMM(BPF_ARSH, BPF_R7, 32), 
    BPF_JMP_REG(BPF_JSGT, BPF_R8, BPF_R7, 1), 
    BPF_EXIT_INSN(), 
    BPF_LDX_MEM(BPF_B, BPF_R1, BPF_R6, 0), 
    BPF_MOV64_REG(BPF_R2, BPF_R1), 
    BPF_ALU64_IMM(BPF_AND, BPF_R2, 7), 
    BPF_JMP_IMM(BPF_JEQ, BPF_R2, 3, 25), 
    BPF_JMP_IMM(BPF_JNE, BPF_R2, 1, 31), 
    BPF_MOV64_REG(BPF_R2, BPF_R1), 
    BPF_ALU64_IMM(BPF_AND, BPF_R2, 224), 
    BPF_JMP_IMM(BPF_JNE, BPF_R2, 96, 2), 
    BPF_LDX_MEM(BPF_W, BPF_R2, BPF_R6, 4), 
    BPF_JMP_IMM(BPF_JEQ, BPF_R2, 0, 26), 
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
    BPF_LDX_MEM(BPF_W, BPF_R5, BPF_R6, 4), 
    BPF_LDX_MEM(BPF_B, BPF_R4, BPF_R6, 0), 
    BPF_MOV64_REG(BPF_R1, BPF_R10), 
    BPF_ALU64_IMM(BPF_ADD, BPF_R1, -40), 
    BPF_MOV64_IMM(BPF_R2, 27), 
    BPF_JMP_A(75), 
    BPF_MOV64_REG(BPF_R2, BPF_R1), 
    BPF_ALU64_IMM(BPF_AND, BPF_R2, 224), 
    BPF_JMP_IMM(BPF_JNE, BPF_R2, 96, 2), 
    BPF_LDX_MEM(BPF_W, BPF_R3, BPF_R6, 4), 
    BPF_JMP_IMM(BPF_JEQ, BPF_R3, 0, 2), 
    BPF_JMP_IMM(BPF_JEQ, BPF_R2, 192, -42), 
    BPF_JMP_A(-26), 
    BPF_JMP_IMM(BPF_JNE, BPF_R1, 24, -44), 
    BPF_MOV64_REG(BPF_R1, BPF_R9), 
    BPF_ALU64_IMM(BPF_LSH, BPF_R1, 32), 
    BPF_ALU64_IMM(BPF_RSH, BPF_R1, 32), 
    BPF_MOV64_REG(BPF_R2, BPF_R7), 
    BPF_ALU64_IMM(BPF_LSH, BPF_R2, 32), 
    BPF_ALU64_IMM(BPF_RSH, BPF_R2, 32), 
    BPF_MOV64_REG(BPF_R3, BPF_R9), 
    BPF_JMP_REG(BPF_JEQ, BPF_R2, BPF_R1, 9), 
    BPF_LDX_MEM(BPF_B, BPF_R1, BPF_R6, 8), 
    BPF_MOV64_REG(BPF_R3, BPF_R7), 
    BPF_JMP_IMM(BPF_JNE, BPF_R1, 0, 6), 
    BPF_LDX_MEM(BPF_B, BPF_R1, BPF_R6, 9), 
    BPF_MOV64_REG(BPF_R3, BPF_R7), 
    BPF_JMP_IMM(BPF_JNE, BPF_R1, 0, 3), 
    BPF_LDX_MEM(BPF_H, BPF_R1, BPF_R6, 10), 
    BPF_MOV64_REG(BPF_R3, BPF_R7), 
    BPF_JMP_IMM(BPF_JEQ, BPF_R1, 0, 18), 
    BPF_MOV64_IMM(BPF_R1, 0), 
    BPF_STX_MEM(BPF_B, BPF_R10, BPF_R1, -22), 
    BPF_MOV64_IMM(BPF_R1, 30757), 
    BPF_STX_MEM(BPF_H, BPF_R10, BPF_R1, -24), 
    BPF_LD_IMM64_RAW(BPF_R1, BPF_R0, 0x78303d65646f6370LL), 
    BPF_STX_MEM(BPF_DW, BPF_R10, BPF_R1, -32), 
    BPF_LD_IMM64_RAW(BPF_R1, BPF_R0, 0x6f202c64253d6370LL), 
    BPF_STX_MEM(BPF_DW, BPF_R10, BPF_R1, -40), 
    BPF_LDX_MEM(BPF_B, BPF_R4, BPF_R6, 0), 
    BPF_MOV64_REG(BPF_R1, BPF_R10), 
    BPF_ALU64_IMM(BPF_ADD, BPF_R1, -40), 
    BPF_MOV64_IMM(BPF_R2, 19), 
    BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0, 6), 
    BPF_LD_IMM64_RAW(BPF_R0, BPF_R0, 0xffffffffLL), 
    BPF_JMP_A(-73), 
    BPF_LDX_MEM(BPF_B, BPF_R1, BPF_R6, 1), 
    BPF_JMP_REG(BPF_JGT, BPF_R4, BPF_R1, -83), 
    BPF_MOV64_REG(BPF_R2, BPF_R1), 
    BPF_ALU64_IMM(BPF_RSH, BPF_R2, 4), 
    BPF_ALU64_IMM(BPF_ADD, BPF_R2, -1), 
    BPF_JMP_IMM(BPF_JGT, BPF_R2, 1, -87), 
    BPF_ALU64_IMM(BPF_AND, BPF_R1, 240), 
    BPF_JMP_IMM(BPF_JEQ, BPF_R1, 32, -113), 
    BPF_JMP_IMM(BPF_JNE, BPF_R1, 16, -90), 
    BPF_LDX_MEM(BPF_W, BPF_R1, BPF_R6, 12), 
    BPF_JMP_IMM(BPF_JEQ, BPF_R1, 0, -92), 
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
    BPF_LDX_MEM(BPF_W, BPF_R5, BPF_R6, 12), 
    BPF_LDX_MEM(BPF_B, BPF_R4, BPF_R6, 0), 
    BPF_MOV64_REG(BPF_R1, BPF_R10), 
    BPF_ALU64_IMM(BPF_ADD, BPF_R1, -40), 
    BPF_MOV64_IMM(BPF_R2, 35), 
    BPF_MOV64_REG(BPF_R3, BPF_R7), 
    BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0, 6), 
    BPF_LD_IMM64_RAW(BPF_R0, BPF_R0, 0xffffffffLL), 
    BPF_JMP_A(-110), 

    BPF_ALU64_IMM(BPF_LSH, BPF_R2, 32), 
    BPF_MOV64_REG(BPF_R4, BPF_R2), 
    BPF_ALU64_IMM(BPF_ARSH, BPF_R4, 32), 
    BPF_MOV64_IMM(BPF_R5, 8), 
    BPF_JMP_REG(BPF_JSGT, BPF_R5, BPF_R4, 18), 
    BPF_ALU64_IMM(BPF_ARSH, BPF_R2, 35), 
    BPF_MOV64_IMM(BPF_R4, 0), 
    BPF_ALU64_IMM(BPF_ADD, BPF_R1, 4), 
    BPF_MOV64_REG(BPF_R5, BPF_R3), 
    BPF_ALU64_IMM(BPF_LSH, BPF_R5, 32), 
    BPF_ALU64_IMM(BPF_ARSH, BPF_R5, 35), 
    BPF_JMP_A(12), 
    BPF_LDX_MEM(BPF_B, BPF_R0, BPF_R1, -3), 
    BPF_ALU64_IMM(BPF_AND, BPF_R0, 240), 
    BPF_JMP_IMM(BPF_JNE, BPF_R0, 16, 5), 
    BPF_MOV64_REG(BPF_R0, BPF_R2), 
    BPF_JMP_REG(BPF_JSGE, BPF_R4, BPF_R5, 3), 
    BPF_LDX_MEM(BPF_W, BPF_R6, BPF_R1, 0), 
    BPF_ALU64_REG(BPF_ADD, BPF_R6, BPF_R0), 
    BPF_STX_MEM(BPF_W, BPF_R1, BPF_R6, 0), 
    BPF_ALU64_IMM(BPF_ADD, BPF_R1, 8), 
    BPF_ALU64_IMM(BPF_ADD, BPF_R4, 1), 
    BPF_JMP_REG(BPF_JSGT, BPF_R2, BPF_R4, 1), 
    BPF_EXIT_INSN(), 
    BPF_LDX_MEM(BPF_B, BPF_R0, BPF_R1, -4), 
    BPF_JMP_IMM(BPF_JEQ, BPF_R0, 133, -14), 
    BPF_JMP_IMM(BPF_JNE, BPF_R0, 24, -7), 
    BPF_LDX_MEM(BPF_B, BPF_R6, BPF_R1, -3), 
    BPF_ALU64_IMM(BPF_AND, BPF_R6, 240), 
    BPF_MOV64_REG(BPF_R0, BPF_R3), 
    BPF_JMP_IMM(BPF_JEQ, BPF_R6, 64, -14), 
    BPF_JMP_A(-12), 
}; 

U64 MYBPF_INSN_GetCallsCount(U64 p1, U64 p2, U64 p3, U64 p4, U64 p5) 
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

U64 MYBPF_INSN_GetCallsInfo(U64 p1, U64 p2, U64 p3, U64 p4, U64 p5) 
{ 
    MYBPF_PARAM_S p; 
    MYBPF_CTX_S ctx = {0}; 

    ctx.begin_addr = g_bpfasm_insts; 
    ctx.end_addr = (char*)(void*)g_bpfasm_insts + sizeof(g_bpfasm_insts); 
    ctx.insts = (char*)g_bpfasm_insts + 192; 
    p.p[0]=p1; p.p[1]=p2; p.p[2]=p3; p.p[3]=p4; p.p[4]=p5; 
    int ret = MYBPF_DefultRun(&ctx, &p); 
    if (ret < 0) return ret; 
    return ctx.bpf_ret; 
} 

U64 MYBPF_INSN_WalkCalls(U64 p1, U64 p2, U64 p3, U64 p4, U64 p5) 
{ 
    MYBPF_PARAM_S p; 
    MYBPF_CTX_S ctx = {0}; 

    ctx.begin_addr = g_bpfasm_insts; 
    ctx.end_addr = (char*)(void*)g_bpfasm_insts + sizeof(g_bpfasm_insts); 
    ctx.insts = (char*)g_bpfasm_insts + 608; 
    p.p[0]=p1; p.p[1]=p2; p.p[2]=p3; p.p[3]=p4; p.p[4]=p5; 
    int ret = MYBPF_DefultRun(&ctx, &p); 
    if (ret < 0) return ret; 
    return ctx.bpf_ret; 
} 

U64 MYBPF_INSN_WalkExternCalls(U64 p1, U64 p2, U64 p3, U64 p4, U64 p5) 
{ 
    MYBPF_PARAM_S p; 
    MYBPF_CTX_S ctx = {0}; 

    ctx.begin_addr = g_bpfasm_insts; 
    ctx.end_addr = (char*)(void*)g_bpfasm_insts + sizeof(g_bpfasm_insts); 
    ctx.insts = (char*)g_bpfasm_insts + 888; 
    p.p[0]=p1; p.p[1]=p2; p.p[2]=p3; p.p[3]=p4; p.p[4]=p5; 
    int ret = MYBPF_DefultRun(&ctx, &p); 
    if (ret < 0) return ret; 
    return ctx.bpf_ret; 
} 

U64 MYBPF_INSN_WalkLddw(U64 p1, U64 p2, U64 p3, U64 p4, U64 p5) 
{ 
    MYBPF_PARAM_S p; 
    MYBPF_CTX_S ctx = {0}; 

    ctx.begin_addr = g_bpfasm_insts; 
    ctx.end_addr = (char*)(void*)g_bpfasm_insts + sizeof(g_bpfasm_insts); 
    ctx.insts = (char*)g_bpfasm_insts + 1184; 
    p.p[0]=p1; p.p[1]=p2; p.p[2]=p3; p.p[3]=p4; p.p[4]=p5; 
    int ret = MYBPF_DefultRun(&ctx, &p); 
    if (ret < 0) return ret; 
    return ctx.bpf_ret; 
} 

U64 MYBPF_INSN_FixupExtCalls(U64 p1, U64 p2, U64 p3, U64 p4, U64 p5) 
{ 
    MYBPF_PARAM_S p; 
    MYBPF_CTX_S ctx = {0}; 

    ctx.begin_addr = g_bpfasm_insts; 
    ctx.end_addr = (char*)(void*)g_bpfasm_insts + sizeof(g_bpfasm_insts); 
    ctx.insts = (char*)g_bpfasm_insts + 1480; 
    p.p[0]=p1; p.p[1]=p2; p.p[2]=p3; p.p[3]=p4; p.p[4]=p5; 
    int ret = MYBPF_DefultRun(&ctx, &p); 
    if (ret < 0) return ret; 
    return ctx.bpf_ret; 
} 

U64 MYBPF_INSN_GetStackSize(U64 p1, U64 p2, U64 p3, U64 p4, U64 p5) 
{ 
    MYBPF_PARAM_S p; 
    MYBPF_CTX_S ctx = {0}; 

    ctx.begin_addr = g_bpfasm_insts; 
    ctx.end_addr = (char*)(void*)g_bpfasm_insts + sizeof(g_bpfasm_insts); 
    ctx.insts = (char*)g_bpfasm_insts + 1928; 
    p.p[0]=p1; p.p[1]=p2; p.p[2]=p3; p.p[3]=p4; p.p[4]=p5; 
    int ret = MYBPF_DefultRun(&ctx, &p); 
    if (ret < 0) return ret; 
    return ctx.bpf_ret; 
} 

U64 MYBPF_INSN_GetStackSizeUntilExit(U64 p1, U64 p2, U64 p3, U64 p4, U64 p5) 
{ 
    MYBPF_PARAM_S p; 
    MYBPF_CTX_S ctx = {0}; 

    ctx.begin_addr = g_bpfasm_insts; 
    ctx.end_addr = (char*)(void*)g_bpfasm_insts + sizeof(g_bpfasm_insts); 
    ctx.insts = (char*)g_bpfasm_insts + 2416; 
    p.p[0]=p1; p.p[1]=p2; p.p[2]=p3; p.p[3]=p4; p.p[4]=p5; 
    int ret = MYBPF_DefultRun(&ctx, &p); 
    if (ret < 0) return ret; 
    return ctx.bpf_ret; 
} 

U64 MYBPF_INSN_ShowPcAccessGlobal(U64 p1, U64 p2, U64 p3, U64 p4, U64 p5) 
{ 
    MYBPF_PARAM_S p; 
    MYBPF_CTX_S ctx = {0}; 

    ctx.begin_addr = g_bpfasm_insts; 
    ctx.end_addr = (char*)(void*)g_bpfasm_insts + sizeof(g_bpfasm_insts); 
    ctx.insts = (char*)g_bpfasm_insts + 2616; 
    p.p[0]=p1; p.p[1]=p2; p.p[2]=p3; p.p[3]=p4; p.p[4]=p5; 
    int ret = MYBPF_DefultRun(&ctx, &p); 
    if (ret < 0) return ret; 
    return ctx.bpf_ret; 
} 

U64 MYBPF_INSN_ModifyTextOff(U64 p1, U64 p2, U64 p3, U64 p4, U64 p5) 
{ 
    MYBPF_PARAM_S p; 
    MYBPF_CTX_S ctx = {0}; 

    ctx.begin_addr = g_bpfasm_insts; 
    ctx.end_addr = (char*)(void*)g_bpfasm_insts + sizeof(g_bpfasm_insts); 
    ctx.insts = (char*)g_bpfasm_insts + 3880; 
    p.p[0]=p1; p.p[1]=p2; p.p[2]=p3; p.p[3]=p4; p.p[4]=p5; 
    int ret = MYBPF_DefultRun(&ctx, &p); 
    if (ret < 0) return ret; 
    return ctx.bpf_ret; 
} 

