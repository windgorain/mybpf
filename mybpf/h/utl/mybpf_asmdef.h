/*********************************************************
*   Copyright (C) LiXingang
*   Description: 
*
********************************************************/
#ifndef _MYBPF_ASMDEF_H
#define _MYBPF_ASMDEF_H
#ifdef __cplusplus
extern "C"
{
#endif


#define BPF_CLASS(code) ((code) & 0x07)
#define		BPF_LD		0x00
#define		BPF_LDX		0x01
#define		BPF_ST		0x02
#define		BPF_STX		0x03
#define		BPF_ALU		0x04
#define		BPF_JMP		0x05
#define		BPF_RET		0x06
#define		BPF_MISC    0x07
#define     BPF_JMP32	0x06	
#define     BPF_ALU64	0x07	


#define BPF_SIZE(code)  ((code) & 0x18)
#define		BPF_W		0x00 
#define		BPF_H		0x08 
#define		BPF_B		0x10 

#define BPF_MODE(code)  ((code) & 0xe0)
#define		BPF_IMM		0x00
#define		BPF_ABS		0x20
#define		BPF_IND		0x40
#define		BPF_MEM		0x60
#define		BPF_LEN		0x80
#define		BPF_MSH		0xa0


#define BPF_OP(code)    ((code) & 0xf0)
#define		BPF_ADD		0x00
#define		BPF_SUB		0x10
#define		BPF_MUL		0x20
#define		BPF_DIV		0x30
#define		BPF_OR		0x40
#define		BPF_AND		0x50
#define		BPF_LSH		0x60
#define		BPF_RSH		0x70
#define		BPF_NEG		0x80
#define		BPF_MOD		0x90
#define		BPF_XOR		0xa0

#define		BPF_JA		0x00
#define		BPF_JEQ		0x10
#define		BPF_JGT		0x20
#define		BPF_JGE		0x30
#define		BPF_JSET        0x40
#define BPF_SRC(code)   ((code) & 0x08)
#define		BPF_K		0x00
#define		BPF_X		0x08


#define BPF_DW		0x18	
#define BPF_ATOMIC	0xc0	
#define BPF_XADD	0xc0	


#define BPF_MOV		0xb0	
#define BPF_ARSH	0xc0	


#define BPF_END		0xd0	
#define BPF_TO_LE	0x00	
#define BPF_TO_BE	0x08	
#define BPF_FROM_LE	BPF_TO_LE
#define BPF_FROM_BE	BPF_TO_BE


#define BPF_JNE		0x50	
#define BPF_JLT		0xa0	
#define BPF_JLE		0xb0	
#define BPF_JSGT	0x60	
#define BPF_JSGE	0x70	
#define BPF_JSLT	0xc0	
#define BPF_JSLE	0xd0	
#define BPF_CALL	0x80	
#define BPF_EXIT	0x90	


#define BPF_FETCH	0x01	
#define BPF_XCHG	(0xe0 | BPF_FETCH)	
#define BPF_CMPXCHG	(0xf0 | BPF_FETCH)	

#define BPF_PSEUDO_MAP_FD   	1
#define BPF_PSEUDO_MAP_VALUE	2
#define BPF_PSEUDO_FUNC_PTR 	4

#define BPF_MAX_VAR_OFF	(1 << 29)


#define BPF_ALU64_REG(OP, DST, SRC)				\
	((MYBPF_INSN_S) {					\
		.opcode  = BPF_ALU64 | BPF_OP(OP) | BPF_X,	\
		.dst_reg = DST,					\
		.src_reg = SRC,					\
		.off   = 0,					\
		.imm   = 0 })

#define BPF_ALU32_REG(OP, DST, SRC)				\
	((MYBPF_INSN_S) {					\
		.opcode  = BPF_ALU | BPF_OP(OP) | BPF_X,		\
		.dst_reg = DST,					\
		.src_reg = SRC,					\
		.off   = 0,					\
		.imm   = 0 })



#define BPF_ALU64_IMM(OP, DST, IMM)				\
	((MYBPF_INSN_S) {					\
		.opcode  = BPF_ALU64 | BPF_OP(OP) | BPF_K,	\
		.dst_reg = DST,					\
		.src_reg = 0,					\
		.off   = 0,					\
		.imm   = IMM })

#define BPF_ALU32_IMM(OP, DST, IMM)				\
	((MYBPF_INSN_S) {					\
		.opcode  = BPF_ALU | BPF_OP(OP) | BPF_K,		\
		.dst_reg = DST,					\
		.src_reg = 0,					\
		.off   = 0,					\
		.imm   = IMM })



#define BPF_ENDIAN(TYPE, DST, LEN)				\
	((MYBPF_INSN_S) {					\
		.opcode  = BPF_ALU | BPF_END | BPF_SRC(TYPE),	\
		.dst_reg = DST,					\
		.src_reg = 0,					\
		.off   = 0,					\
		.imm   = LEN })



#define BPF_MOV64_REG(DST, SRC)					\
	((MYBPF_INSN_S) {					\
		.opcode  = BPF_ALU64 | BPF_MOV | BPF_X,		\
		.dst_reg = DST,					\
		.src_reg = SRC,					\
		.off   = 0,					\
		.imm   = 0 })

#define BPF_MOV32_REG(DST, SRC)					\
	((MYBPF_INSN_S) {					\
		.opcode  = BPF_ALU | BPF_MOV | BPF_X,		\
		.dst_reg = DST,					\
		.src_reg = SRC,					\
		.off   = 0,					\
		.imm   = 0 })



#define BPF_MOV64_IMM(DST, IMM)					\
	((MYBPF_INSN_S) {					\
		.opcode  = BPF_ALU64 | BPF_MOV | BPF_K,		\
		.dst_reg = DST,					\
		.src_reg = 0,					\
		.off   = 0,					\
		.imm   = IMM })

#define BPF_MOV32_IMM(DST, IMM)					\
	((MYBPF_INSN_S) {					\
		.opcode  = BPF_ALU | BPF_MOV | BPF_K,		\
		.dst_reg = DST,					\
		.src_reg = 0,					\
		.off   = 0,					\
		.imm   = IMM })


#define BPF_ZEXT_REG(DST)					\
	((MYBPF_INSN_S) {					\
		.opcode  = BPF_ALU | BPF_MOV | BPF_X,		\
		.dst_reg = DST,					\
		.src_reg = DST,					\
		.off   = 0,					\
		.imm   = 1 })



#define BPF_LD_IMM64(DST, IMM)					\
	BPF_LD_IMM64_RAW(DST, 0, IMM)

#define BPF_LD_IMM64_RAW(DST, SRC, IMM)				\
	((MYBPF_INSN_S) {					\
		.opcode  = BPF_LD | BPF_DW | BPF_IMM,		\
		.dst_reg = DST,					\
		.src_reg = SRC,					\
		.off   = 0,					\
		.imm   = (unsigned int) (IMM) }),			\
	((MYBPF_INSN_S) {					\
		.opcode  = 0, 	\
		.dst_reg = 0,					\
		.src_reg = 0,					\
		.off   = 0,					\
		.imm   = ((unsigned long long) (IMM)) >> 32 })


#define BPF_LD_MAP_FD(DST, MAP_FD)				\
	BPF_LD_IMM64_RAW(DST, BPF_PSEUDO_MAP_FD, MAP_FD)



#define BPF_MOV64_RAW(TYPE, DST, SRC, IMM)			\
	((MYBPF_INSN_S) {					\
		.opcode  = BPF_ALU64 | BPF_MOV | BPF_SRC(TYPE),	\
		.dst_reg = DST,					\
		.src_reg = SRC,					\
		.off   = 0,					\
		.imm   = IMM })

#define BPF_MOV32_RAW(TYPE, DST, SRC, IMM)			\
	((MYBPF_INSN_S) {					\
		.opcode  = BPF_ALU | BPF_MOV | BPF_SRC(TYPE),	\
		.dst_reg = DST,					\
		.src_reg = SRC,					\
		.off   = 0,					\
		.imm   = IMM })



#define BPF_LD_ABS(SIZE, IMM)					\
	((MYBPF_INSN_S) {					\
		.opcode  = BPF_LD | BPF_SIZE(SIZE) | BPF_ABS,	\
		.dst_reg = 0,					\
		.src_reg = 0,					\
		.off   = 0,					\
		.imm   = IMM })



#define BPF_LD_IND(SIZE, SRC, IMM)				\
	((MYBPF_INSN_S) {					\
		.opcode  = BPF_LD | BPF_SIZE(SIZE) | BPF_IND,	\
		.dst_reg = 0,					\
		.src_reg = SRC,					\
		.off   = 0,					\
		.imm   = IMM })



#define BPF_LDX_MEM(SIZE, DST, SRC, OFF)			\
	((MYBPF_INSN_S) {					\
		.opcode  = BPF_LDX | BPF_SIZE(SIZE) | BPF_MEM,	\
		.dst_reg = DST,					\
		.src_reg = SRC,					\
		.off   = OFF,					\
		.imm   = 0 })



#define BPF_STX_MEM(SIZE, DST, SRC, OFF)			\
	((MYBPF_INSN_S) {					\
		.opcode  = BPF_STX | BPF_SIZE(SIZE) | BPF_MEM,	\
		.dst_reg = DST,					\
		.src_reg = SRC,					\
		.off   = OFF,					\
		.imm   = 0 })




#define BPF_ATOMIC_OP(SIZE, OP, DST, SRC, OFF)			\
	((MYBPF_INSN_S) {					\
		.opcode  = BPF_STX | BPF_SIZE(SIZE) | BPF_ATOMIC,	\
		.dst_reg = DST,					\
		.src_reg = SRC,					\
		.off   = OFF,					\
		.imm   = OP })


#define BPF_STX_XADD(SIZE, DST, SRC, OFF) BPF_ATOMIC_OP(SIZE, BPF_ADD, DST, SRC, OFF)



#define BPF_ST_MEM(SIZE, DST, OFF, IMM)				\
	((MYBPF_INSN_S) {					\
		.opcode  = BPF_ST | BPF_SIZE(SIZE) | BPF_MEM,	\
		.dst_reg = DST,					\
		.src_reg = 0,					\
		.off   = OFF,					\
		.imm   = IMM })



#define BPF_JMP_REG(OP, DST, SRC, OFF)				\
	((MYBPF_INSN_S) {					\
		.opcode  = BPF_JMP | BPF_OP(OP) | BPF_X,		\
		.dst_reg = DST,					\
		.src_reg = SRC,					\
		.off   = OFF,					\
		.imm   = 0 })



#define BPF_JMP_IMM(OP, DST, IMM, OFF)				\
	((MYBPF_INSN_S) {					\
		.opcode  = BPF_JMP | BPF_OP(OP) | BPF_K,		\
		.dst_reg = DST,					\
		.src_reg = 0,					\
		.off   = OFF,					\
		.imm   = IMM })



#define BPF_JMP32_REG(OP, DST, SRC, OFF)			\
	((MYBPF_INSN_S) {					\
		.opcode  = BPF_JMP32 | BPF_OP(OP) | BPF_X,	\
		.dst_reg = DST,					\
		.src_reg = SRC,					\
		.off   = OFF,					\
		.imm   = 0 })



#define BPF_JMP32_IMM(OP, DST, IMM, OFF)			\
	((MYBPF_INSN_S) {					\
		.opcode  = BPF_JMP32 | BPF_OP(OP) | BPF_K,	\
		.dst_reg = DST,					\
		.src_reg = 0,					\
		.off   = OFF,					\
		.imm   = IMM })



#define BPF_JMP_A(OFF)						\
	((MYBPF_INSN_S) {					\
		.opcode  = BPF_JMP | BPF_JA,			\
		.dst_reg = 0,					\
		.src_reg = 0,					\
		.off   = OFF,					\
		.imm   = 0 })



#define BPF_CALL_REL(TGT)					\
	((MYBPF_INSN_S) {					\
		.opcode  = BPF_JMP | BPF_CALL,			\
		.dst_reg = 0,					\
		.src_reg = BPF_PSEUDO_CALL,			\
		.off   = 0,					\
		.imm   = TGT })



#define BPF_CAST_CALL(x)					\
		((u64 (*)(u64, u64, u64, u64, u64))(x))

#define BPF_EMIT_CALL(FUNC)					\
	((MYBPF_INSN_S) {					\
		.opcode  = BPF_JMP | BPF_CALL,			\
		.dst_reg = 0,					\
		.src_reg = 0,					\
		.off   = 0,					\
		.imm   = ((FUNC) - __bpf_call_base) })



#define BPF_RAW_INSN(CODE, DST, SRC, OFF, IMM)			\
	((MYBPF_INSN_S) {					\
		.opcode  = CODE,					\
		.dst_reg = DST,					\
		.src_reg = SRC,					\
		.off   = OFF,					\
		.imm   = IMM })



#define BPF_EXIT_INSN()						\
	((MYBPF_INSN_S) {					\
		.opcode  = BPF_JMP | BPF_EXIT,			\
		.dst_reg = 0,					\
		.src_reg = 0,					\
		.off   = 0,					\
		.imm   = 0 })



#define BPF_ST_NOSPEC()						\
	((MYBPF_INSN_S) {					\
		.opcode  = BPF_ST | BPF_NOSPEC,			\
		.dst_reg = 0,					\
		.src_reg = 0,					\
		.off   = 0,					\
		.imm   = 0 })

#ifdef __cplusplus
}
#endif
#endif 
