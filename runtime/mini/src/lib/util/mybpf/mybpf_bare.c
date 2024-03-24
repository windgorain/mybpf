/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#include <sys/mman.h>
#include "bs.h"
#include "utl/bpf_helper_utl.h"
#include "utl/arch_utl.h"
#include "utl/mybpf_bare.h"
#include "utl/mybpf_prog_def.h"
#include "utl/mmap_utl.h"

static int _runbpf_run_bare(void *data, int len, void **tmp_helpers, MYBPF_PARAM_S *p)
{
    MYBPF_AOT_PROG_CTX_S ctx = {0};
    int (*fn)(U64,U64,U64,U64,U64,void*);

    ctx.base_helpers = BpfHelper_BaseHelper();
    ctx.sys_helpers = BpfHelper_SysHelper();
    ctx.user_helpers = BpfHelper_UserHelper();
    ctx.tmp_helpers = (const void **)tmp_helpers;

    fn = mmap(0, len, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (! fn) {
        RETURNI(BS_ERR, "Can't map prog");
    }

    memcpy(fn, data, len);
    mprotect(fn, len, PROT_READ | PROT_EXEC);

    int ret = fn(p->p[0], p->p[1], p->p[2], p->p[3], p->p[4], &ctx);

    munmap(fn, len);

    return ret;
}

static int _mybpf_bare_check(MYBPF_BARE_HDR_S *hdr)
{
    if (hdr->magic != htonl(MYBPF_BARE_MAGIC)) {
        /* 魔数不对 */
        RETURNI(BS_NOT_MATCHED, "Magic not match");
    }

    if ((! hdr->jit_arch) || (hdr->jit_arch != ARCH_LocalArch())) {
        RETURNI(BS_NOT_SUPPORT, "Jit arch not matched");
    }

    return 0;
}

int MYBPF_RunBareMain(void *mem, MYBPF_PARAM_S *p)
{
    MYBPF_BARE_HDR_S *hdr = mem;
    MYBPF_BARE_SUB_HDR_S *shdr = (void*)(hdr + 1);

    if (_mybpf_bare_check(hdr) < 0) {
        return -1;
    }

    int head_size = sizeof(*shdr) + sizeof(int) * ntohs(shdr->depends_count);

    return _runbpf_run_bare((char*)shdr + head_size, ntohl(shdr->sub_size) - head_size, NULL, p);
}

