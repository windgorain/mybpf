/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2022-7-16
* Description:
******************************************************************************/
#include "bs.h"
#include "utl/mmap_utl.h"
#include "utl/arch_utl.h"
#include "utl/mybpf_prog_def.h"
#include "utl/bpf_helper_utl.h"
#include "utl/file_utl.h"
#include "utl/mybpf_bare.h"

static void ** _mybpf_bare_malloc_bss(int bss_size)
{
    void **bss = MEM_ZMalloc(16 + bss_size);
    if (! bss) {
        return NULL;
    }

    bss[0] = (char*)bss + 16;

    return bss;
}

static void _mybpf_bare_free_bss(void **bss)
{
    if (bss) {
        MEM_Free(bss);
    }
}

static int _mybpf_bare_check_depends(MYBPF_BARE_HDR_S *hdr, const void **tmp_helpers)
{
    MYBPF_BARE_SUB_HDR_S *shdr = (void*)(hdr + 1);

    int depend_count = ntohl(shdr->depends_count);
    if (depend_count == 0) {
        return 0;
    }

    int *helpers = (void*)(shdr + 1);

    for (int i=0; i<depend_count; i++) {
        int helper_id = ntohl(helpers[i]);
        void *fn = ulc_get_helper(helper_id, tmp_helpers);
        if (! fn) {
            RETURNI(BS_NOT_SUPPORT, "Not support helper %d", helper_id);
        }
    }

    return 0;
}


static int _mybpf_bare_check(MYBPF_BARE_HDR_S *hdr, int mem_len, const void **tmp_helpers)
{
    if (mem_len <= sizeof(*hdr)) {
        
        RETURNI(BS_TOO_SMALL, "Too small");
    }

    if (hdr->magic != htonl(MYBPF_BARE_MAGIC)) {
        
        RETURNI(BS_NOT_MATCHED, "Magic not match");
    }

    int size = ntohl(hdr->total_size);
    if (size > mem_len) {
        
        RETURNI(BS_WRONG_FILE, "File length not valid");
    }

    if ((! hdr->arch) || (hdr->arch != ARCH_LocalArch())) {
        RETURNI(BS_NOT_SUPPORT, "Jit arch not matched");
    }

    return _mybpf_bare_check_depends(hdr, tmp_helpers);
}

static int _mybpf_bare_load(void *data, int len, const void **tmp_helpers, OUT MYBPF_BARE_S *bare)
{
    MYBPF_BARE_HDR_S *hdr = data;
    MYBPF_BARE_SUB_HDR_S *shdr = (void*)((char*)hdr + ntohs(hdr->hdr_size));
    void **bss = NULL;

    int ret = _mybpf_bare_check(hdr, len, tmp_helpers);
    if (ret < 0) {
        return ret;
    }

    int bss_size = ntohl(shdr->bss_size);
    if (bss_size) {
        bss = _mybpf_bare_malloc_bss(bss_size);
        if (! bss) {
            RETURNI(BS_NO_MEMORY, "Can't malloc bss memory");
        }
    }

    int shdr_len = ntohs(shdr->sub_hdr_size) + (sizeof(int) * ntohl(shdr->depends_count));
    void *prog_begin = (char*)shdr + shdr_len;
    int prog_size = ntohl(shdr->sub_size) - shdr_len;

    void *fn = mmap(0, prog_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (fn == MAP_FAILED) {
        _mybpf_bare_free_bss(bss);
        RETURN(BS_ERR);
    }

    memcpy(fn, prog_begin, prog_size);
    mprotect(fn, prog_size, PROT_READ | PROT_EXEC);

    bare->prog = fn;
    bare->prog_len = prog_size;
    bare->ctx.base_helpers = ulc_get_base_helpers();
    bare->ctx.sys_helpers = ulc_get_sys_helpers();
    bare->ctx.user_helpers = ulc_get_user_helpers();
    bare->ctx.global_map_data = bss;
    bare->ctx.tmp_helpers = tmp_helpers;

    return 0;
}

static U64 _mybpf_bare_call(MYBPF_BARE_S *bare, void *func, MYBPF_PARAM_S *p)
{
    U64 (*fn)(U64, U64, U64, U64, U64, void*) = func;
    assert(fn != NULL);
    p->bpf_ret = fn(p->p[0], p->p[1], p->p[2], p->p[3], p->p[4], &bare->ctx);
    return p->bpf_ret;
}

int MYBPF_LoadBare(void *data, int len, const void **tmp_helpers, OUT MYBPF_BARE_S *bare)
{
    return _mybpf_bare_load(data, len, tmp_helpers, bare);
}

void MYBPF_UnloadBare(MYBPF_BARE_S *bare)
{
    if (bare) {
        _mybpf_bare_free_bss(bare->ctx.global_map_data);
        MMAP_Unmap(bare->prog, bare->prog_len);
        memset(bare, 0, sizeof(*bare));
    }
}

U64 MYBPF_RunBareMain(MYBPF_BARE_S *bare, MYBPF_PARAM_S *p)
{
    return _mybpf_bare_call(bare, bare->prog, p);
}

