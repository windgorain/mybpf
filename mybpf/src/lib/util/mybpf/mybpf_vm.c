/*================================================================
*   Created by LiXingang, Copyright LiXingang
*   Date: 2017.10.1
*   Description: 
*
================================================================*/
#include "bs.h"
#include "utl/mybpf_vm.h"
#include "utl/mybpf_insn.h"
#include "utl/mybpf_loader.h"
#include "utl/mybpf_prog.h"
#include "utl/mybpf_utl.h"
#include "utl/mybpf_asmdef.h"
#include "utl/mybpf_dbg.h"
#include "utl/ubpf/ebpf.h"
#include "utl/endian_utl.h"
#include "mybpf_osbase.h"

#define MYBPF_STACK_SIZE 512
#define MYBPF_MAX_INSTS 1000000

static int _mybpf_run(MYBPF_VM_S *vm, MYBPF_CTX_S *ctx, MYBPF_PARAM_S *p);
static inline bool _mybpf_bounds_check(
        MYBPF_VM_S *vm,
        MYBPF_CTX_S *ctx,
        void *addr,
        int size,
        const char *type,
        uint16_t cur_pc,
        void *mem,
        size_t mem_len);

int MYBPF_SetTailCallIndex(MYBPF_VM_S *vm, unsigned int id)
{
    if (vm->tail_call_index != 0) {
        return -1;
    }

    vm->tail_call_index = id;
    return 0;
}

static inline void * _mybpf_get_helper(MYBPF_VM_S *vm, MYBPF_CTX_S *ctx, int imm, const void **tmp_helpers)
{
    if (imm == -1) {
        return NULL;
    }

    if ((ctx) && (ctx->helper_fixed)) {
        return BpfHelper_BaseHelper + imm;
    }

    if (imm >= BPF_TMP_HELPER_START) {
        return (void*)ctx->tmp_helpers[imm - BPF_TMP_HELPER_START];
    }

    return BpfHelper_GetFuncExt(imm, tmp_helpers);
}

static inline UINT64 _mybpf_call(MYBPF_VM_S *vm, MYBPF_CTX_S *ctx, int imm,
        UINT64 p1, UINT64 p2, UINT64 p3, UINT64 p4, UINT64 p5)
{
    PF_BPF_HELPER_FUNC func = _mybpf_get_helper(vm, ctx, imm, ctx->tmp_helpers);

    if (! func) {
        return -1;
    }

    return func(p1, p2, p3, p4, p5);
}

static ELF_PROG_INFO_S * _mybpf_vm_get_sub_prog(MYBPF_CTX_S *ctx, void *insn)
{
    MYBPF_PROG_NODE_S *prog = ctx->mybpf_prog;
    if (! prog) {
        return NULL;
    }

    MYBPF_LOADER_NODE_S *n = prog->loader_node;
    if (! n) {
        return NULL;
    }

    for (int i=0; i<n->progs_count; i++) {
        if (((char*)n->insts + n->progs[i].prog_offset) == (char*)insn) {
            return &n->progs[i];
        }
    }

    return NULL;
}

#define u32 _ubpf_to_u32
#define i32 _ubpf_to_i32

static uint32_t 
u32(uint64_t x)
{
    return x;
}

static int32_t
i32(uint64_t x)
{
    return x;
}

#define IS_ALIGNED(x, a) (((uintptr_t)(x) & ((a)-1)) == 0)

inline static uint64_t
ubpf_mem_load(uint64_t address, size_t size)
{
    if (!IS_ALIGNED(address, size)) {
        
        uint64_t value = 0;
        memcpy(&value, (void*)address, size);
        return value;
    }

    switch (size) {
    case 1:
        return *(uint8_t*)address;
    case 2:
        return *(uint16_t*)address;
    case 4:
        return *(uint32_t*)address;
    case 8:
        return *(uint64_t*)address;
    default:
        abort();
    }
}

inline static void
ubpf_mem_store(uint64_t address, uint64_t value, size_t size)
{
    if (!IS_ALIGNED(address, size)) {
        memcpy((void*)address, &value, size);
        return;
    }

    switch (size) {
    case 1:
        *(uint8_t*)address = value;
        break;
    case 2:
        *(uint16_t*)address = value;
        break;
    case 4:
        *(uint32_t*)address = value;
        break;
    case 8:
        *(uint64_t*)address = value;
        break;
    default:
        abort();
    }
}

static inline UINT64 _mybpf_call_sub_prog(MYBPF_VM_S *vm, MYBPF_CTX_S *ctx, 
        void *insn, UINT64 p1, UINT64 p2, UINT64 p3, UINT64 p4, UINT64 p5)
{
    MYBPF_PARAM_S p;

    p.p[0] = p1;
    p.p[1] = p2;
    p.p[2] = p3;
    p.p[3] = p4;
    p.p[4] = p5;

    ctx->insts = insn;

    _mybpf_run(vm, ctx, &p);

    return ctx->bpf_ret;
}

static inline UINT64 _mybpf_call_ext_func_ptr(void *ptr, UINT64 p1, UINT64 p2, UINT64 p3, UINT64 p4, UINT64 p5)
{
    PF_BPF_HELPER_FUNC func = ptr;

    if (! func) {
        return -1;
    }

    return func(p1, p2, p3, p4, p5);
}

static inline UINT64 _mybpf_call_func_ptr(MYBPF_VM_S *vm, MYBPF_CTX_S *ctx, 
        void *insn, UINT64 p1, UINT64 p2, UINT64 p3, UINT64 p4, UINT64 p5)
{
    if (((char*)insn >= (char*)ctx->begin_addr) && ((char*)insn <= (char*)ctx->end_addr)) {
        return _mybpf_call_sub_prog(vm, ctx, insn, p1, p2, p3, p4, p5);
    } else {
        return _mybpf_call_ext_func_ptr(insn, p1, p2, p3, p4, p5);
    }
}

static int _mybpf_run_atomic32(MYBPF_INSN_S *insn, U64 *regs)
{
    int imm = insn->imm;
    int src = insn->src_reg;
    int dst = insn->dst_reg;
    int off = insn->off;

    int *ptr = (void*)(long)(regs[dst] + off);
    int val = regs[src];

	if (imm == BPF_ADD) {
        __sync_fetch_and_add(ptr, val);
    } else if (imm == BPF_AND) {
        __sync_fetch_and_and(ptr, val);
    } else if (imm == BPF_OR) {
        __sync_fetch_and_or(ptr, val);
    } else if (imm == BPF_XOR) {
        __sync_fetch_and_xor(ptr, val);
    } else if (imm == (BPF_ADD | BPF_FETCH)) {
        regs[src] = __sync_fetch_and_add(ptr, val);
    } else if (imm == (BPF_AND | BPF_FETCH)) {
        regs[src] = __sync_fetch_and_and(ptr, val);
    } else if (imm == (BPF_OR | BPF_FETCH)) {
        regs[src] = __sync_fetch_and_or(ptr, val);
    } else if (imm == (BPF_XOR | BPF_FETCH)) {
        regs[src] = __sync_fetch_and_xor(ptr, val);
    } else if (imm == BPF_XCHG) {
        regs[src] = atomic_xchg((void*)ptr, val);
    } else if (imm == BPF_CMPXCHG) {
        regs[0] = atomic_cmpxchg((void*)ptr, regs[0], val);
    } else {
        return -1;
    }

    return 0;
}

static int _mybpf_run_atomic64(MYBPF_INSN_S *insn, U64 *regs)
{
    int imm = insn->imm;
    int src = insn->src_reg;
    int dst = insn->dst_reg;
    int off = insn->off;

    S64 *ptr = (void*)(long)(regs[dst] + off);
    S64 val = regs[src];

	if (imm == BPF_ADD) {
        __sync_fetch_and_add(ptr, val);
    } else if (imm == BPF_AND) {
        __sync_fetch_and_and(ptr, val);
    } else if (imm == BPF_OR) {
        __sync_fetch_and_or(ptr, val);
    } else if (imm == BPF_XOR) {
        __sync_fetch_and_xor(ptr, val);
    } else if (imm == (BPF_ADD | BPF_FETCH)) {
        regs[src] = __sync_fetch_and_add(ptr, val);
    } else if (imm == (BPF_AND | BPF_FETCH)) {
        regs[src] = __sync_fetch_and_and(ptr, val);
    } else if (imm == (BPF_OR | BPF_FETCH)) {
        regs[src] = __sync_fetch_and_or(ptr, val);
    } else if (imm == (BPF_XOR | BPF_FETCH)) {
        regs[src] = __sync_fetch_and_xor(ptr, val);
    } else if (imm == BPF_XCHG) {
        regs[src] = atomic_xchg((void*)ptr, val);
    } else if (imm == BPF_CMPXCHG) {
        regs[0] = atomic_cmpxchg((void*)ptr, regs[0], val);
    } else {
        return -1;
    }

    return 0;
}

static int _mybpf_run_bpf(MYBPF_VM_S *vm, MYBPF_CTX_S *ctx, MYBPF_PARAM_S *p)
{
    int pc = 0;
    int pc_off = ((char*)ctx->insts - (char*)ctx->begin_addr) / 8;
    MYBPF_INSN_S *insts = ctx->insts;
    U64 reg[16];

    if (!insts) {
        
        RETURN(BS_ERR);
    }

    reg[1] = p->p[0];
    reg[2] = p->p[1];
    reg[3] = p->p[2];
    reg[4] = p->p[3];
    reg[5] = p->p[4];
    reg[10] = (unsigned long)(ctx->stack + ctx->stack_size);

    while (1) {
        const int cur_pc = pc;
        MYBPF_INSN_S inst = insts[pc++];
        U8 *c = (void*)&inst;

        switch (inst.opcode) {
        case EBPF_OP_ADD_IMM:
            reg[inst.dst_reg] += inst.imm;
            reg[inst.dst_reg] &= UINT32_MAX;
            break;
        case EBPF_OP_ADD_REG:
            reg[inst.dst_reg] += reg[inst.src_reg];
            reg[inst.dst_reg] &= UINT32_MAX;
            break;
        case EBPF_OP_SUB_IMM:
            reg[inst.dst_reg] -= inst.imm;
            reg[inst.dst_reg] &= UINT32_MAX;
            break;
        case EBPF_OP_SUB_REG:
            reg[inst.dst_reg] -= reg[inst.src_reg];
            reg[inst.dst_reg] &= UINT32_MAX;
            break;
        case EBPF_OP_MUL_IMM:
            reg[inst.dst_reg] *= inst.imm;
            reg[inst.dst_reg] &= UINT32_MAX;
            break;
        case EBPF_OP_MUL_REG:
            reg[inst.dst_reg] *= reg[inst.src_reg];
            reg[inst.dst_reg] &= UINT32_MAX;
            break;
        case EBPF_OP_DIV_IMM:
            reg[inst.dst_reg] = u32(inst.imm) ? u32(reg[inst.dst_reg]) / u32(inst.imm) : 0;
            reg[inst.dst_reg] &= UINT32_MAX;
            break;
        case EBPF_OP_DIV_REG:
            reg[inst.dst_reg] = reg[inst.src_reg] ? u32(reg[inst.dst_reg]) / u32(reg[inst.src_reg]) : 0;
            reg[inst.dst_reg] &= UINT32_MAX;
            break;
        case EBPF_OP_OR_IMM:
            reg[inst.dst_reg] |= inst.imm;
            reg[inst.dst_reg] &= UINT32_MAX;
            break;
        case EBPF_OP_OR_REG:
            reg[inst.dst_reg] |= reg[inst.src_reg];
            reg[inst.dst_reg] &= UINT32_MAX;
            break;
        case EBPF_OP_AND_IMM:
            reg[inst.dst_reg] &= inst.imm;
            reg[inst.dst_reg] &= UINT32_MAX;
            break;
        case EBPF_OP_AND_REG:
            reg[inst.dst_reg] &= reg[inst.src_reg];
            reg[inst.dst_reg] &= UINT32_MAX;
            break;
        case EBPF_OP_LSH_IMM:
            reg[inst.dst_reg] <<= inst.imm;
            reg[inst.dst_reg] &= UINT32_MAX;
            break;
        case EBPF_OP_LSH_REG:
            reg[inst.dst_reg] <<= reg[inst.src_reg];
            reg[inst.dst_reg] &= UINT32_MAX;
            break;
        case EBPF_OP_RSH_IMM:
            reg[inst.dst_reg] = u32(reg[inst.dst_reg]) >> inst.imm;
            reg[inst.dst_reg] &= UINT32_MAX;
            break;
        case EBPF_OP_RSH_REG:
            reg[inst.dst_reg] = u32(reg[inst.dst_reg]) >> reg[inst.src_reg];
            reg[inst.dst_reg] &= UINT32_MAX;
            break;
        case EBPF_OP_NEG:
            reg[inst.dst_reg] = -(int64_t)reg[inst.dst_reg];
            reg[inst.dst_reg] &= UINT32_MAX;
            break;
        case EBPF_OP_MOD_IMM:
            reg[inst.dst_reg] = u32(inst.imm) ? u32(reg[inst.dst_reg]) % u32(inst.imm) : u32(reg[inst.dst_reg]);
            reg[inst.dst_reg] &= UINT32_MAX;
            break;
        case EBPF_OP_MOD_REG:
            reg[inst.dst_reg] = u32(reg[inst.src_reg]) ? u32(reg[inst.dst_reg]) % u32(reg[inst.src_reg]) : u32(reg[inst.dst_reg]);
            break;
        case EBPF_OP_XOR_IMM:
            reg[inst.dst_reg] ^= inst.imm;
            reg[inst.dst_reg] &= UINT32_MAX;
            break;
        case EBPF_OP_XOR_REG:
            reg[inst.dst_reg] ^= reg[inst.src_reg];
            reg[inst.dst_reg] &= UINT32_MAX;
            break;
        case EBPF_OP_MOV_IMM:
            reg[inst.dst_reg] = inst.imm;
            reg[inst.dst_reg] &= UINT32_MAX;
            break;
        case EBPF_OP_MOV_REG:
            reg[inst.dst_reg] = reg[inst.src_reg];
            reg[inst.dst_reg] &= UINT32_MAX;
            break;
        case EBPF_OP_ARSH_IMM:
            reg[inst.dst_reg] = (int32_t)reg[inst.dst_reg] >> inst.imm;
            reg[inst.dst_reg] &= UINT32_MAX;
            break;
        case EBPF_OP_ARSH_REG:
            reg[inst.dst_reg] = (int32_t)reg[inst.dst_reg] >> u32(reg[inst.src_reg]);
            reg[inst.dst_reg] &= UINT32_MAX;
            break;

        case EBPF_OP_LE:
            if (inst.imm == 16) {
                reg[inst.dst_reg] = htole16(reg[inst.dst_reg]);
            } else if (inst.imm == 32) {
                reg[inst.dst_reg] = htole32(reg[inst.dst_reg]);
            } else if (inst.imm == 64) {
                reg[inst.dst_reg] = htole64(reg[inst.dst_reg]);
            }
            break;
        case EBPF_OP_BE:
            if (inst.imm == 16) {
                reg[inst.dst_reg] = htobe16(reg[inst.dst_reg]);
            } else if (inst.imm == 32) {
                reg[inst.dst_reg] = htobe32(reg[inst.dst_reg]);
            } else if (inst.imm == 64) {
                reg[inst.dst_reg] = htobe64(reg[inst.dst_reg]);
            }
            break;
        case EBPF_OP_ADD64_IMM:
            reg[inst.dst_reg] += inst.imm;
            break;
        case EBPF_OP_ADD64_REG:
            reg[inst.dst_reg] += reg[inst.src_reg];
            break;
        case EBPF_OP_SUB64_IMM:
            reg[inst.dst_reg] -= inst.imm;
            break;
        case EBPF_OP_SUB64_REG:
            reg[inst.dst_reg] -= reg[inst.src_reg];
            break;
        case EBPF_OP_MUL64_IMM:
            reg[inst.dst_reg] *= inst.imm;
            break;
        case EBPF_OP_MUL64_REG:
            reg[inst.dst_reg] *= reg[inst.src_reg];
            break;
        case EBPF_OP_DIV64_IMM:
            reg[inst.dst_reg] = inst.imm ? reg[inst.dst_reg] / inst.imm : 0;
            break;
        case EBPF_OP_DIV64_REG:
            reg[inst.dst_reg] = reg[inst.src_reg] ? reg[inst.dst_reg] / reg[inst.src_reg] : 0;
            break;
        case EBPF_OP_OR64_IMM:
            reg[inst.dst_reg] |= inst.imm;
            break;
        case EBPF_OP_OR64_REG:
            reg[inst.dst_reg] |= reg[inst.src_reg];
            break;
        case EBPF_OP_AND64_IMM:
            reg[inst.dst_reg] &= inst.imm;
            break;
        case EBPF_OP_AND64_REG:
            reg[inst.dst_reg] &= reg[inst.src_reg];
            break;
        case EBPF_OP_LSH64_IMM:
            reg[inst.dst_reg] <<= inst.imm;
            break;
        case EBPF_OP_LSH64_REG:
            reg[inst.dst_reg] <<= reg[inst.src_reg];
            break;
        case EBPF_OP_RSH64_IMM:
            reg[inst.dst_reg] >>= inst.imm;
            break;
        case EBPF_OP_RSH64_REG:
            reg[inst.dst_reg] >>= reg[inst.src_reg];
            break;
        case EBPF_OP_NEG64:
            reg[inst.dst_reg] = -reg[inst.dst_reg];
            break;
        case EBPF_OP_MOD64_IMM:
            reg[inst.dst_reg] = inst.imm ? reg[inst.dst_reg] % inst.imm : reg[inst.dst_reg];
            break;
        case EBPF_OP_MOD64_REG:
            reg[inst.dst_reg] = reg[inst.src_reg] ? reg[inst.dst_reg] % reg[inst.src_reg] : reg[inst.dst_reg];
            break;
        case EBPF_OP_XOR64_IMM:
            reg[inst.dst_reg] ^= inst.imm;
            break;
        case EBPF_OP_XOR64_REG:
            reg[inst.dst_reg] ^= reg[inst.src_reg];
            break;
        case EBPF_OP_MOV64_IMM:
            reg[inst.dst_reg] = inst.imm;
            break;
        case EBPF_OP_MOV64_REG:
            reg[inst.dst_reg] = reg[inst.src_reg];
            break;
        case EBPF_OP_ARSH64_IMM:
            reg[inst.dst_reg] = (int64_t)reg[inst.dst_reg] >> inst.imm;
            break;
        case EBPF_OP_ARSH64_REG:
            reg[inst.dst_reg] = (int64_t)reg[inst.dst_reg] >> reg[inst.src_reg];
            break;

#define BOUNDS_CHECK_LOAD(size) \
    do { \
        if (!_mybpf_bounds_check(vm, ctx, (char *)reg[inst.src_reg] + inst.off, size, "load", cur_pc, ctx->mem, ctx->mem_len)) { \
            return -1; \
        } \
    } while (0)

#define BOUNDS_CHECK_STORE(size) \
    do { \
        if (!_mybpf_bounds_check(vm, ctx, (char *)reg[inst.dst_reg] + inst.off, size, "store", cur_pc, ctx->mem, ctx->mem_len)) { \
            return -1; \
        } \
    } while (0)

        case EBPF_OP_LDXW:
            BOUNDS_CHECK_LOAD(4);
            reg[inst.dst_reg] = ubpf_mem_load(reg[inst.src_reg] + inst.off, 4);
            break;
        case EBPF_OP_LDXH:
            BOUNDS_CHECK_LOAD(2);
            reg[inst.dst_reg] = ubpf_mem_load(reg[inst.src_reg] + inst.off, 2);
            break;
        case EBPF_OP_LDXB:
            BOUNDS_CHECK_LOAD(1);
            reg[inst.dst_reg] = ubpf_mem_load(reg[inst.src_reg] + inst.off, 1);
            break;
        case EBPF_OP_LDXDW:
            BOUNDS_CHECK_LOAD(8);
            reg[inst.dst_reg] = ubpf_mem_load(reg[inst.src_reg] + inst.off, 8);
            break;

        case EBPF_OP_STW:
            BOUNDS_CHECK_STORE(4);
            ubpf_mem_store(reg[inst.dst_reg] + inst.off, inst.imm, 4);
            break;
        case EBPF_OP_STH:
            BOUNDS_CHECK_STORE(2);
            ubpf_mem_store(reg[inst.dst_reg] + inst.off, inst.imm, 2);
            break;
        case EBPF_OP_STB:
            BOUNDS_CHECK_STORE(1);
            ubpf_mem_store(reg[inst.dst_reg] + inst.off, inst.imm, 1);
            break;
        case EBPF_OP_STDW:
            BOUNDS_CHECK_STORE(8);
            ubpf_mem_store(reg[inst.dst_reg] + inst.off, inst.imm, 8);
            break;

        case EBPF_OP_STXW:
            BOUNDS_CHECK_STORE(4);
            ubpf_mem_store(reg[inst.dst_reg] + inst.off, reg[inst.src_reg], 4);
            break;
        case EBPF_OP_STXH:
            BOUNDS_CHECK_STORE(2);
            ubpf_mem_store(reg[inst.dst_reg] + inst.off, reg[inst.src_reg], 2);
            break;
        case EBPF_OP_STXB:
            BOUNDS_CHECK_STORE(1);
            ubpf_mem_store(reg[inst.dst_reg] + inst.off, reg[inst.src_reg], 1);
            break;
        case EBPF_OP_STXDW:
            BOUNDS_CHECK_STORE(8);
            ubpf_mem_store(reg[inst.dst_reg] + inst.off, reg[inst.src_reg], 8);
            break;

        case EBPF_OP_LDDW:
            if (inst.src_reg == BPF_PSEUDO_FUNC_PTR) {
                unsigned char *ptr = ctx->begin_addr;
                reg[inst.dst_reg] = (unsigned long)(ptr + inst.imm);
                pc ++;
            } else {
                reg[inst.dst_reg] = u32(inst.imm) | ((uint64_t)insts[pc++].imm << 32);
            }
            break;

        case EBPF_OP_JA:
            pc += inst.off;
            break;
        case EBPF_OP_JEQ_IMM:
            if (reg[inst.dst_reg] == inst.imm) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JEQ_REG:
            if (reg[inst.dst_reg] == reg[inst.src_reg]) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JEQ32_IMM:
            if (u32(reg[inst.dst_reg]) == u32(inst.imm)) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JEQ32_REG:
            if (u32(reg[inst.dst_reg]) == reg[inst.src_reg]) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JGT_IMM:
            if (reg[inst.dst_reg] > u32(inst.imm)) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JGT_REG:
            if (reg[inst.dst_reg] > reg[inst.src_reg]) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JGT32_IMM:
            if (u32(reg[inst.dst_reg]) > u32(inst.imm)) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JGT32_REG:
            if (u32(reg[inst.dst_reg]) > u32(reg[inst.src_reg])) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JGE_IMM:
            if (reg[inst.dst_reg] >= u32(inst.imm)) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JGE_REG:
            if (reg[inst.dst_reg] >= reg[inst.src_reg]) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JGE32_IMM:
            if (u32(reg[inst.dst_reg]) >= u32(inst.imm)) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JGE32_REG:
            if (u32(reg[inst.dst_reg]) >= u32(reg[inst.src_reg])) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JLT_IMM:
            if (reg[inst.dst_reg] < u32(inst.imm)) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JLT_REG:
            if (reg[inst.dst_reg] < reg[inst.src_reg]) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JLT32_IMM:
            if (u32(reg[inst.dst_reg]) < u32(inst.imm)) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JLT32_REG:
            if (u32(reg[inst.dst_reg]) < u32(reg[inst.src_reg])) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JLE_IMM:
            if (reg[inst.dst_reg] <= u32(inst.imm)) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JLE_REG:
            if (reg[inst.dst_reg] <= reg[inst.src_reg]) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JLE32_IMM:
            if (u32(reg[inst.dst_reg]) <= u32(inst.imm)) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JLE32_REG:
            if (u32(reg[inst.dst_reg]) <= u32(reg[inst.src_reg])) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JSET_IMM:
            if (reg[inst.dst_reg] & inst.imm) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JSET_REG:
            if (reg[inst.dst_reg] & reg[inst.src_reg]) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JSET32_IMM:
            if (u32(reg[inst.dst_reg]) & u32(inst.imm)) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JSET32_REG:
            if (u32(reg[inst.dst_reg]) & u32(reg[inst.src_reg])) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JNE_IMM:
            if (reg[inst.dst_reg] != inst.imm) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JNE_REG:
            if (reg[inst.dst_reg] != reg[inst.src_reg]) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JNE32_IMM:
            if (u32(reg[inst.dst_reg]) != u32(inst.imm)) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JNE32_REG:
            if (u32(reg[inst.dst_reg]) != u32(reg[inst.src_reg])) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JSGT_IMM:
            if ((int64_t)reg[inst.dst_reg] > inst.imm) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JSGT_REG:
            if ((int64_t)reg[inst.dst_reg] > (int64_t)reg[inst.src_reg]) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JSGT32_IMM:
            if (i32(reg[inst.dst_reg]) > i32(inst.imm)) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JSGT32_REG:
            if (i32(reg[inst.dst_reg]) > i32(reg[inst.src_reg])) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JSGE_IMM:
            if ((int64_t)reg[inst.dst_reg] >= inst.imm) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JSGE_REG:
            if ((int64_t)reg[inst.dst_reg] >= (int64_t)reg[inst.src_reg]) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JSGE32_IMM:
            if (i32(reg[inst.dst_reg]) >= i32(inst.imm)) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JSGE32_REG:
            if (i32(reg[inst.dst_reg]) >= i32(reg[inst.src_reg])) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JSLT_IMM:
            if ((int64_t)reg[inst.dst_reg] < inst.imm) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JSLT_REG:
            if ((int64_t)reg[inst.dst_reg] < (int64_t)reg[inst.src_reg]) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JSLT32_IMM:
            if (i32(reg[inst.dst_reg]) < i32(inst.imm)) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JSLT32_REG:
            if (i32(reg[inst.dst_reg]) < i32(reg[inst.src_reg])) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JSLE_IMM:
            if ((int64_t)reg[inst.dst_reg] <= inst.imm) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JSLE_REG:
            if ((int64_t)reg[inst.dst_reg] <= (int64_t)reg[inst.src_reg]) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JSLE32_IMM:
            if (i32(reg[inst.dst_reg]) <= i32(inst.imm)) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_JSLE32_REG:
            if (i32(reg[inst.dst_reg]) <= i32(reg[inst.src_reg])) {
                pc += inst.off;
            }
            break;
        case EBPF_OP_EXIT:
            ctx->bpf_ret = reg[0];
            return 0;
        case EBPF_OP_CALL:
            if (inst.src_reg == BPF_PSEUDO_CALL) {
                MYBPF_INSN_S *n = &insts[pc];
                reg[0] = _mybpf_call_sub_prog(vm, ctx, n + inst.imm, reg[1], reg[2], reg[3], reg[4], reg[5]);
            } else {
                reg[0] = _mybpf_call(vm, ctx, inst.imm, reg[1], reg[2], reg[3], reg[4], reg[5]);
                if ((inst.imm == vm->tail_call_index) && (reg[0] == 0)) {
                    ctx->bpf_ret = reg[0];
                    return 0;
                }
            }
            break;
        case EBPF_OP_CALLX:
            reg[0] = _mybpf_call_func_ptr(vm, ctx, (void*)reg[inst.imm], reg[1], reg[2], reg[3], reg[4], reg[5]);
            break;
        case EPBF_OP_LOCK_STXW:
            _mybpf_run_atomic32(&inst, reg);
            break;
        case EPBF_OP_LOCK_STXDW:
            _mybpf_run_atomic64(&inst, reg);
            break;
        default: 
            {
                PRINTFLM_RED("Not support insn %d, insn=%02x %02x %02x %02x %02x %02x %02x %02x",
                        pc_off + cur_pc, c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7]);
            }
            break;
        }
    }
}

BOOL_T MYBPF_Validate(MYBPF_VM_S *vm, void *insn, UINT num_insts, const void **tmp_helpers)
{
    MYBPF_INSN_S *insts = insn;

    if (num_insts > MYBPF_MAX_INSTS) {
        vm->print_func("too many instructions (max %u)", MYBPF_MAX_INSTS);
        return false;
    }

    int i;
    for (i = 0; i < num_insts; i++) {
        MYBPF_INSN_S inst = insts[i];
        bool store = false;

        switch (inst.opcode) {
        case EBPF_OP_ADD_IMM:
        case EBPF_OP_ADD_REG:
        case EBPF_OP_SUB_IMM:
        case EBPF_OP_SUB_REG:
        case EBPF_OP_MUL_IMM:
        case EBPF_OP_MUL_REG:
        case EBPF_OP_DIV_REG:
        case EBPF_OP_OR_IMM:
        case EBPF_OP_OR_REG:
        case EBPF_OP_AND_IMM:
        case EBPF_OP_AND_REG:
        case EBPF_OP_LSH_IMM:
        case EBPF_OP_LSH_REG:
        case EBPF_OP_RSH_IMM:
        case EBPF_OP_RSH_REG:
        case EBPF_OP_NEG:
        case EBPF_OP_MOD_REG:
        case EBPF_OP_XOR_IMM:
        case EBPF_OP_XOR_REG:
        case EBPF_OP_MOV_IMM:
        case EBPF_OP_MOV_REG:
        case EBPF_OP_ARSH_IMM:
        case EBPF_OP_ARSH_REG:
            break;

        case EBPF_OP_LE:
        case EBPF_OP_BE:
            if (inst.imm != 16 && inst.imm != 32 && inst.imm != 64) {
                vm->print_func("invalid endian immediate at PC %d", i);
                return false;
            }
            break;

        case EBPF_OP_ADD64_IMM:
        case EBPF_OP_ADD64_REG:
        case EBPF_OP_SUB64_IMM:
        case EBPF_OP_SUB64_REG:
        case EBPF_OP_MUL64_IMM:
        case EBPF_OP_MUL64_REG:
        case EBPF_OP_DIV64_REG:
        case EBPF_OP_OR64_IMM:
        case EBPF_OP_OR64_REG:
        case EBPF_OP_AND64_IMM:
        case EBPF_OP_AND64_REG:
        case EBPF_OP_LSH64_IMM:
        case EBPF_OP_LSH64_REG:
        case EBPF_OP_RSH64_IMM:
        case EBPF_OP_RSH64_REG:
        case EBPF_OP_NEG64:
        case EBPF_OP_MOD64_REG:
        case EBPF_OP_XOR64_IMM:
        case EBPF_OP_XOR64_REG:
        case EBPF_OP_MOV64_IMM:
        case EBPF_OP_MOV64_REG:
        case EBPF_OP_ARSH64_IMM:
        case EBPF_OP_ARSH64_REG:
            break;

        case EBPF_OP_LDXW:
        case EBPF_OP_LDXH:
        case EBPF_OP_LDXB:
        case EBPF_OP_LDXDW:
            break;

        case EBPF_OP_STW:
        case EBPF_OP_STH:
        case EBPF_OP_STB:
        case EBPF_OP_STDW:
        case EBPF_OP_STXW:
        case EBPF_OP_STXH:
        case EBPF_OP_STXB:
        case EBPF_OP_STXDW:
            store = true;
            break;

        case EBPF_OP_LDDW:
            if (inst.src_reg != 0) {
                vm->print_func("invalid source register for LDDW at PC %d", i);
                return false;
            }
            if (i + 1 >= num_insts || insts[i+1].opcode != 0) {
                vm->print_func("incomplete lddw at PC %d", i);
                return false;
            }
            i++; 
            break;

        case EBPF_OP_JA:
        case EBPF_OP_JEQ_REG:
        case EBPF_OP_JEQ_IMM:
        case EBPF_OP_JGT_REG:
        case EBPF_OP_JGT_IMM:
        case EBPF_OP_JGE_REG:
        case EBPF_OP_JGE_IMM:
        case EBPF_OP_JLT_REG:
        case EBPF_OP_JLT_IMM:
        case EBPF_OP_JLE_REG:
        case EBPF_OP_JLE_IMM:
        case EBPF_OP_JSET_REG:
        case EBPF_OP_JSET_IMM:
        case EBPF_OP_JNE_REG:
        case EBPF_OP_JNE_IMM:
        case EBPF_OP_JSGT_IMM:
        case EBPF_OP_JSGT_REG:
        case EBPF_OP_JSGE_IMM:
        case EBPF_OP_JSGE_REG:
        case EBPF_OP_JSLT_IMM:
        case EBPF_OP_JSLT_REG:
        case EBPF_OP_JSLE_IMM:
        case EBPF_OP_JSLE_REG:
        case EBPF_OP_JEQ32_IMM:
        case EBPF_OP_JEQ32_REG:
        case EBPF_OP_JGT32_IMM:
        case EBPF_OP_JGT32_REG:
        case EBPF_OP_JGE32_IMM:
        case EBPF_OP_JGE32_REG:
        case EBPF_OP_JSET32_REG:
        case EBPF_OP_JSET32_IMM:
        case EBPF_OP_JNE32_IMM:
        case EBPF_OP_JNE32_REG:
        case EBPF_OP_JSGT32_IMM:
        case EBPF_OP_JSGT32_REG:
        case EBPF_OP_JSGE32_IMM:
        case EBPF_OP_JSGE32_REG:
        case EBPF_OP_JLT32_IMM:
        case EBPF_OP_JLT32_REG:
        case EBPF_OP_JLE32_IMM:
        case EBPF_OP_JLE32_REG:
        case EBPF_OP_JSLT32_IMM:
        case EBPF_OP_JSLT32_REG:
        case EBPF_OP_JSLE32_IMM:
        case EBPF_OP_JSLE32_REG:
            if (inst.off == -1) {
                vm->print_func("infinite loop at PC %d", i);
                return false;
            }
            int new_pc = i + 1 + inst.off;
            if (new_pc < 0 || new_pc >= num_insts) {
                vm->print_func("jump out of bounds at PC %d", i);
                return false;
            } else if (insts[new_pc].opcode == 0) {
                vm->print_func("jump to middle of lddw at PC %d", i);
                return false;
            }
            break;

        case EBPF_OP_CALL:
            if (NULL == _mybpf_get_helper(vm, NULL, inst.imm, tmp_helpers)) {
                vm->print_func("invalid call immediate at PC %d", i);
                return false;
            }
            break;

        case EBPF_OP_EXIT:
            break;

        case EBPF_OP_DIV_IMM:
        case EBPF_OP_MOD_IMM:
        case EBPF_OP_DIV64_IMM:
        case EBPF_OP_MOD64_IMM:
            if (inst.imm == 0) {
                vm->print_func("division by zero at PC %d", i);
                return false;
            }
            break;

        default:
            vm->print_func("unknown opcode 0x%02x at PC %d", inst.opcode, i);
            return false;
        }

        if (inst.src_reg > 10) {
            vm->print_func("invalid source register at PC %d", i);
            return false;
        }

        if (inst.dst_reg > 9 && !(store && inst.dst_reg == 10)) {
            vm->print_func("invalid destination register at PC %d", i);
            return false;
        }
    }

    return true;
}

static inline bool _mybpf_bounds_check(
        MYBPF_VM_S *vm,
        MYBPF_CTX_S *ctx,
        void *addr,
        int size,
        const char *type,
        uint16_t cur_pc,
        void *mem,
        size_t mem_len)
{
    if (! ctx->mem_check)
        return true;

    if (mem && (addr >= mem && ((char*)addr + size) <= ((char*)mem + mem_len))) {
        return true; 
    } else if ((char*)addr >= ctx->stack && ((char*)addr + size) <= (ctx->stack + ctx->stack_size)) {
        return true; 
    } else {
        vm->print_func("uBPF error: out of bounds memory %s at PC %u, addr %p, size %d\nmem %p/%d stack %p/%d\n",
                type, cur_pc, addr, size, mem, mem_len, ctx->stack, ctx->stack_size);
        return false;
    }
}

static int _mybpf_run_stack8(MYBPF_VM_S *vm, MYBPF_CTX_S *ctx, MYBPF_PARAM_S *p)
{
    char stack[8];
    ctx->stack_size = 8;
    ctx->stack = stack;
    return _mybpf_run_bpf(vm, ctx, p);
}

static int _mybpf_run_stack16(MYBPF_VM_S *vm, MYBPF_CTX_S *ctx, MYBPF_PARAM_S *p)
{
    char stack[16];
    ctx->stack_size = 16;
    ctx->stack = stack;
    return _mybpf_run_bpf(vm, ctx, p);
}

static int _mybpf_run_stack32(MYBPF_VM_S *vm, MYBPF_CTX_S *ctx, MYBPF_PARAM_S *p)
{
    char stack[32];
    ctx->stack_size = 32;
    ctx->stack = stack;
    return _mybpf_run_bpf(vm, ctx, p);
}

static int _mybpf_run_stack64(MYBPF_VM_S *vm, MYBPF_CTX_S *ctx, MYBPF_PARAM_S *p)
{
    char stack[64];
    ctx->stack_size = 64;
    ctx->stack = stack;
    return _mybpf_run_bpf(vm, ctx, p);
}

static int _mybpf_run_stack128(MYBPF_VM_S *vm, MYBPF_CTX_S *ctx, MYBPF_PARAM_S *p)
{
    char stack[128];
    ctx->stack_size = 128;
    ctx->stack = stack;
    return _mybpf_run_bpf(vm, ctx, p);
}

static noinline int _mybpf_run_stack256(MYBPF_VM_S *vm, MYBPF_CTX_S *ctx, MYBPF_PARAM_S *p)
{
    char stack[256];
    ctx->stack_size = 256;
    ctx->stack = stack;
    return _mybpf_run_bpf(vm, ctx, p);
}

static noinline int _mybpf_run_stack512(MYBPF_VM_S *vm, MYBPF_CTX_S *ctx, MYBPF_PARAM_S *p)
{
    char stack[512];
    ctx->stack_size = 512;
    ctx->stack = stack;
    return _mybpf_run_bpf(vm, ctx, p);
}

static noinline int _mybpf_run_stack1024(MYBPF_VM_S *vm, MYBPF_CTX_S *ctx, MYBPF_PARAM_S *p)
{
    char stack[1024];
    ctx->stack_size = 1024;
    ctx->stack = stack;
    return _mybpf_run_bpf(vm, ctx, p);
}

static noinline int _mybpf_run_stack2048(MYBPF_VM_S *vm, MYBPF_CTX_S *ctx, MYBPF_PARAM_S *p)
{
    char stack[2048];
    ctx->stack_size = 2048;
    ctx->stack = stack;
    return _mybpf_run_bpf(vm, ctx, p);
}

static noinline int _mybpf_run_stack4096(MYBPF_VM_S *vm, MYBPF_CTX_S *ctx, MYBPF_PARAM_S *p)
{
    char stack[4096];
    ctx->stack_size = 4096;
    ctx->stack = stack;
    return _mybpf_run_bpf(vm, ctx, p);
}

static int _mybpf_run(MYBPF_VM_S *vm, MYBPF_CTX_S *ctx, MYBPF_PARAM_S *p)
{
    int stack_size = 512;
    ELF_PROG_INFO_S *info = _mybpf_vm_get_sub_prog(ctx, ctx->insts);

    if (info) {
        MYBPF_DBG_OUTPUT(MYBPF_DBG_ID_VM, DBG_UTL_FLAG_PROCESS,
                "call %s:%s, prog_offset:%d, prog_addr:0x%x \n",
                info->sec_name, info->func_name, info->prog_offset, ctx->insts);
        if (ctx->auto_stack) {
            stack_size = MYBPF_INSN_GetStackSize(ctx->insts, info->size);
        }
    } else {
        MYBPF_DBG_OUTPUT(MYBPF_DBG_ID_VM, DBG_UTL_FLAG_PROCESS,
                "call insn_idx:%d \n", ((char *)ctx->insts - (char*)ctx->begin_addr) / 8);
    }

    if (stack_size <= 8) {
        return _mybpf_run_stack8(vm, ctx, p);
    } else if (stack_size <= 16) {
        return _mybpf_run_stack16(vm, ctx, p);
    } else if (stack_size <= 32) {
        return _mybpf_run_stack32(vm, ctx, p);
    } else if (stack_size <= 64) {
        return _mybpf_run_stack64(vm, ctx, p);
    } else if (stack_size <= 128) {
        return _mybpf_run_stack128(vm, ctx, p);
    } else if (stack_size <= 256) {
        return _mybpf_run_stack256(vm, ctx, p);
    } else if (stack_size <= 512) {
        return _mybpf_run_stack512(vm, ctx, p);
    } else if (stack_size <= 1024) {
        return _mybpf_run_stack1024(vm, ctx, p);
    } else if (stack_size <= 2048) {
        return _mybpf_run_stack2048(vm, ctx, p);
    } else if (stack_size <= 4096) {
        return _mybpf_run_stack4096(vm, ctx, p);
    } else {
        BS_DBGASSERT(0);
        RETURN(BS_OUT_OF_RANGE);
    }
}

int MYBPF_Run(MYBPF_VM_S *vm, MYBPF_CTX_S *ctx, MYBPF_PARAM_S *p)
{
    
    return _mybpf_run(vm, ctx, p);
}

