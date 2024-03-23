/*********************************************************
*   Copyright (C) LiXingang
*   Date: 2022.7.11
*   Description: 
*
********************************************************/
#include "bs.h"
#include "utl/mem_inline.h"
#include "utl/mmap_utl.h"
#include "utl/ufd_utl.h"
#include "utl/umap_utl.h"
#include "utl/arch_utl.h"
#include "utl/bpf_helper_utl.h"
#include "utl/mybpf_loader.h"
#include "utl/mybpf_prog.h"
#include "utl/mybpf_vm.h"
#include "utl/mybpf_jit.h"
#include "utl/mybpf_insn.h"
#include "utl/mybpf_file.h"
#include "utl/mybpf_asmdef.h"
#include "utl/mybpf_dbg.h"
#include "mybpf_jit_inner.h"
#include "mybpf_osbase.h"

static MYBPF_JIT_ARCH_S g_mybpf_jit_arch[] = {
    [ARCH_TYPE_ARM64] = {
        .jit_func = MYBPF_JitArm64_Jit,
        .fix_bpf_call = MYBPF_JitArm64_FixBpfCall,
        .fix_load_func_ptr = MYBPF_JitArm64_FixLoadFuncPtr,
    },
    [ARCH_TYPE_X86_64] = {
        .jit_func = MYBPF_JitX64_Jit,
        .fix_bpf_call = MYBPF_JitX64_FixBpfCall,
        .fix_load_func_ptr = MYBPF_JitX64_FixLoadFuncPtr,
    },
};


static MYBPF_JIT_ARCH_S * _mybpf_jit_get_arch(int arch_type)
{
    if ((arch_type <= 0) || (arch_type >= ARCH_TYPE_MAX)) {
        return NULL;
    }

    return &g_mybpf_jit_arch[arch_type];
}

static void _mybpf_jit_fini_res(MYBPF_JIT_RES_S *res)
{
    MEM_SafeFree(res->locs);
    MEM_SafeFree(res->jitted_buf);
    MEM_SafeFree(res->progs);
}

static int _mybpf_jit_init_res(MYBPF_JIT_INSN_S *jit_insn, OUT MYBPF_JIT_RES_S *res)
{
    int num_insts = jit_insn->insts_len / sizeof(MYBPF_INSN_S);
    res->max_jitted_size = jit_insn->insts_len * 8;

    res->locs = MEM_Malloc(sizeof(UINT) * num_insts);
    res->jitted_buf = MEM_Malloc(res->max_jitted_size);
    res->progs = MEM_ZMalloc(sizeof(ELF_PROG_INFO_S) * jit_insn->progs_count);

    if ((! res->locs) || (! res->jitted_buf) || (! res->progs)) {
        _mybpf_jit_fini_res(res);
        RETURNI(BS_NO_MEMORY, "Alloc memory error");
    }

    memcpy(res->progs, jit_insn->progs, sizeof(jit_insn->progs[0]) * jit_insn->progs_count);

    return 0;
}


static void _mybpf_recacl_locs(OUT UINT *locs, int num, int offset)
{
    int i;
    for (i=0; i<num; i++) {
        locs[i] += offset;
    }
}


static int _mybpf_prog_jit_progs(MYBPF_JIT_ARCH_S *arch, MYBPF_JIT_INSN_S *jit_insn,
        MYBPF_JIT_RES_S *res, MYBPF_JIT_CFG_S *cfg)
{
    MYBPF_JIT_CTX_S jit_ctx = {0};
    MYBPF_JIT_VM_S vm = {0};
    int i;
    int ret;
    int total_jitted_size = 0;

    
    if (cfg->helper_mode == MYBPF_JIT_HELPER_MODE_ID) {
        vm.get_helper_by_id = cfg->get_helper_by_id;
        vm.tail_call_func = cfg->tail_call_func;
    }

    jit_ctx.max_jitted_size = res->max_jitted_size;
    jit_ctx.locs = res->locs;

    jit_ctx.jit_cfg = cfg;

    for (i=0; i<jit_insn->progs_count; i++) {
        vm.insts = (void*)((char*)jit_insn->insts + res->progs[i].prog_offset);
        vm.num_insts = res->progs[i].size / sizeof(MYBPF_INSN_S);
        jit_ctx.prog_offset = total_jitted_size;
        jit_ctx.jitted_buf = (char*)res->jitted_buf + total_jitted_size;
        jit_ctx.max_jitted_size = res->max_jitted_size - total_jitted_size;
        jit_ctx.stack_size = MYBPF_INSN_GetStackSize(vm.insts, res->progs[i].size);
        jit_ctx.stack_size = NUM_UP_ALIGN(jit_ctx.stack_size, 16);
        jit_ctx.jitted_size = 0;

        if (strcmp(res->progs[i].sec_name, ".text") == 0) {
            jit_ctx.is_main_prog = 0;
        } else {
            jit_ctx.is_main_prog = 1;
        }

        ret = arch->jit_func(&vm, &jit_ctx);
        
        if (ret < 0) {
            return ret;
        }

        _mybpf_recacl_locs(jit_ctx.locs, vm.num_insts, total_jitted_size);
        res->progs[i].prog_offset = total_jitted_size;
        res->progs[i].size = jit_ctx.jitted_size;
        total_jitted_size += jit_ctx.jitted_size;
        jit_ctx.locs += vm.num_insts;
    }

    return total_jitted_size;
}

static int _mybpf_fixup_bpf_call(void *insts, int insn_index, void *ud)
{
    MYBPF_INSN_S *insn = insts;

    if (insn[insn_index].src_reg != BPF_PSEUDO_CALL) {
        
        return 0;
    }

    USER_HANDLE_S *uh = ud;
    MYBPF_JIT_ARCH_S *arch = uh->ahUserHandle[0];
    MYBPF_JIT_RES_S *res = uh->ahUserHandle[1];
    MYBPF_JIT_CFG_S *cfg = uh->ahUserHandle[2];
    char *jitted_code = uh->ahUserHandle[3];

    
    int dst_func = insn[insn_index].imm + insn_index + 1;
    int dst = res->locs[dst_func] - res->locs[0];

    
    void * bpf_func = jitted_code + dst;

    
    void * to_fix = jitted_code + res->locs[insn_index];

    MYBPF_DBG_OUTPUT(MYBPF_DBG_ID_JIT, DBG_UTL_FLAG_PROCESS, 
            "Fix bpf call, code_insn:%d, func_insn:%d, func_off:%d, to_fix:%p, bpf_func:%p \n",
            insn_index, dst_func, dst, to_fix, bpf_func);

    arch->fix_bpf_call(cfg, to_fix, bpf_func);
    

    return 0;
}

static int _mybpf_fixup_funcptr_lddw(void *insts, int insn_index, void *ud)
{
    MYBPF_INSN_S *insn = insts;

    if (insn[insn_index].src_reg != BPF_PSEUDO_FUNC_PTR) {
        
        return 0;
    }

    USER_HANDLE_S *uh = ud;
    MYBPF_JIT_ARCH_S *arch = uh->ahUserHandle[0];
    MYBPF_JIT_RES_S *res = uh->ahUserHandle[1];
    MYBPF_JIT_CFG_S *cfg = uh->ahUserHandle[2];
    char *jitted_code = uh->ahUserHandle[3];

    
    int func_idx = insn[insn_index].imm / sizeof(MYBPF_INSN_S);
    UINT64 func = res->locs[func_idx] - res->locs[0];

    func += (long)jitted_code;

    
    void * to_fix = jitted_code + res->locs[insn_index];

    arch->fix_load_func_ptr(cfg, to_fix, func);
    

    return 0;
}

static void _mybpf_jit_fixup(MYBPF_JIT_ARCH_S *arch, MYBPF_JIT_INSN_S *jit_insn,
        MYBPF_JIT_RES_S *res, MYBPF_JIT_CFG_S *cfg, INOUT void *jitted_code)
{
    USER_HANDLE_S uh;

    uh.ahUserHandle[0] = arch;
    uh.ahUserHandle[1] = res;
    uh.ahUserHandle[2] = cfg;
    uh.ahUserHandle[3] = jitted_code;

    MYBPF_INSN_WalkCalls(jit_insn->insts, jit_insn->insts_len, _mybpf_fixup_bpf_call, &uh);
    MYBPF_INSN_WalkLddw(jit_insn->insts, jit_insn->insts_len, _mybpf_fixup_funcptr_lddw, &uh);
}

static int _mybpf_jit_do(MYBPF_JIT_ARCH_S *arch, MYBPF_JIT_INSN_S *jit_insn,
        MYBPF_JIT_RES_S *res, MYBPF_JIT_CFG_S *cfg)
{
    int ret;
    int jitted_size;
    void *jitted_code = NULL;

    ret = jitted_size = _mybpf_prog_jit_progs(arch, jit_insn, res, cfg);
    if (ret < 0) {
        return ret;
    }

    if (cfg->mmap_exe) {
        jitted_code = MMAP_Map(res->jitted_buf, jitted_size, 0);
    } else {
        jitted_code = MEM_Dup(res->jitted_buf, jitted_size);
    }

    if (! jitted_code) {
        RETURNI(BS_ERR, "mmap error");
    }

    _mybpf_jit_fixup(arch, jit_insn, res, cfg, jitted_code);

    if (cfg->mmap_exe) {
        MMAP_Mprotect(jitted_code, jitted_size, PROT_READ | PROT_EXEC);
    }

    jit_insn->insts = jitted_code;
    jit_insn->insts_len = jitted_size;

    
    memcpy(jit_insn->progs, res->progs, sizeof(jit_insn->progs[0]) * jit_insn->progs_count);

    return 0;
}

static int _mybpf_prog_jit(MYBPF_JIT_ARCH_S *arch, MYBPF_JIT_INSN_S *jit_insn, MYBPF_JIT_CFG_S *cfg)
{
    MYBPF_JIT_RES_S res = {0};
    int ret;

    ret = _mybpf_jit_init_res(jit_insn, &res);
    if (ret < 0) {
        return ret;
    }

    ret = _mybpf_jit_do(arch, jit_insn, &res, cfg);

    _mybpf_jit_fini_res(&res);

    return ret;
}

int MYBPF_Jit(MYBPF_JIT_INSN_S *jit_insn, MYBPF_JIT_CFG_S *cfg)
{
    int arch_type = cfg->jit_arch;

    if (arch_type == ARCH_TYPE_NONE) {
        arch_type = ARCH_LocalArch();
    }

    if ((arch_type <= 0) || (arch_type >= ARCH_TYPE_MAX)) {
        RETURN(BS_BAD_PARA);
    }

    MYBPF_JIT_ARCH_S *arch = _mybpf_jit_get_arch(arch_type);
    if (! arch) {
        RETURN(BS_ERR);
    }

    return _mybpf_prog_jit(arch, jit_insn, cfg);
}

