/*********************************************************
*   Copyright (C) LiXingang
*   Description: 
*
********************************************************/
#include "bs.h"
#include "utl/file_utl.h"
#include "utl/mybpf_vm.h"
#include "utl/bpf_helper_utl.h"
#include "utl/mybpf_runtime.h"
#include "utl/mybpf_loader.h"
#include "utl/mybpf_prog.h"
#include "utl/mybpf_hookpoint.h"
#include "utl/mybpf_elf.h"
#include "utl/mybpf_file.h"
#include "utl/mybpf_insn.h"
#include "utl/mybpf_simple.h"

static inline int _mybpf_run_file(MYBPF_RUNTIME_S *runtime, MYBPF_FILE_CTX_S *ctx, MYBPF_PARAM_S *p)
{
    int ret;
    MYBPF_LOADER_PARAM_S d = {0};

    d.instance = "runfile";
    d.filename = ctx->file;

    if (ctx->jit) {
        d.flag |= MYBPF_LOADER_FLAG_JIT;
    }

    if ((ret = MYBPF_LoaderLoad(runtime, &d)) < 0) {
        return ret;
    }

    MYBPF_PROG_NODE_S *prog = NULL;
    while ((prog = MYBPF_PROG_GetNext(runtime, "runfile", ctx->sec_name, prog))) {
        ret = MYBPF_PROG_Run(prog, p);
        if (ret == MYBPF_HP_RET_STOP) {
            break;
        }
    }

    return ret;
}

int MYBPF_RunFileExt(MYBPF_FILE_CTX_S *ctx, MYBPF_PARAM_S *p)
{
    MYBPF_RUNTIME_S runtime;
    int ret;

    if ((ret = MYBPF_RuntimeInit(&runtime)) < 0) {
        return ret;
    }

    ret = _mybpf_run_file(&runtime, ctx, p);

    MYBPF_RuntimeFini(&runtime);

    return ret;
}

int MYBPF_RunFile(char *filename, char *sec_name, MYBPF_PARAM_S *p)
{
    MYBPF_FILE_CTX_S ctx = {0};

    ctx.file = filename;
    ctx.sec_name = sec_name;

    return MYBPF_RunFileExt(&ctx, p);
}

int MYBPF_ShowPcAccessGlobal(char *filename)
{
    FILE_MEM_S m;

    int ret = MYBPF_SIMPLE_OpenFileRaw(filename, &m);
    if (ret < 0) {
        return ret;
    }

    void *insn = MYBPF_SIMPLE_GetProgs(&m);
    int insn_len = MYBPF_SIMPLE_GetProgsSize(&m);

    MYBPF_INSN_ShowPcAccessGlobal(insn, insn_len);

    FILE_FreeMem(&m);

    return 0;
}
