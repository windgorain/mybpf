/*================================================================
*   Created by LiXingang
*   Description: 大部分情况下, 采用默认内置的vm环境即可
*                这样就省去了自己构建vm环境
*                default vm是禁止动态修改的, 它是const的
*
================================================================*/
#include "bs.h"
#include "utl/mybpf_vm.h"
#include "utl/bpf_helper_utl.h"

static int _mybpf_default_vm_print(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    return 0;
}

static const MYBPF_VM_S g_mybpf_default_vm = {
    .print_func = _mybpf_default_vm_print
};


int MYBPF_DefultRun(MYBPF_CTX_S *ctx, MYBPF_PARAM_S *p)
{
    return MYBPF_Run((void*)&g_mybpf_default_vm, ctx, p);
}


int MYBPF_DefultRunCode(void *begin_addr, void *end_addr, void *entry, OUT UINT64 *bpf_ret, const void **tmp_helpers, MYBPF_PARAM_S *p)
{
    MYBPF_CTX_S ctx = {0};

    ctx.begin_addr = begin_addr;
    ctx.end_addr = end_addr;
    ctx.insts = entry;
    ctx.tmp_helpers = tmp_helpers;

    int ret = MYBPF_Run((void*)&g_mybpf_default_vm, &ctx, p);
    if (ret < 0) {
        return ret;
    }

    if (bpf_ret) {
        *bpf_ret = ctx.bpf_ret;
    }

    return 0;
}

