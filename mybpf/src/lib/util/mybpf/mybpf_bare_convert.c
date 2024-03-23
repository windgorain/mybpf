/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2022-7-16
* Description:
******************************************************************************/
#include "bs.h"
#include "utl/args_utl.h"
#include "utl/time_utl.h"
#include "utl/subcmd_utl.h"
#include "utl/getopt2_utl.h"
#include "utl/mmap_utl.h"
#include "utl/mybpf_loader.h"
#include "utl/mybpf_prog.h"
#include "utl/mybpf_vm.h"
#include "utl/mybpf_elf.h"
#include "utl/mybpf_relo.h"
#include "utl/mybpf_file.h"
#include "utl/mybpf_jit.h"
#include "utl/mybpf_simple.h"
#include "utl/mybpf_dbg.h"
#include "utl/mybpf_asm.h"
#include "utl/mybpf_asmexp.h"
#include "utl/ubpf_utl.h"
#include "utl/file_utl.h"
#include "utl/mybpf_merge.h"
#include "utl/mybpf_bare.h"
#include "utl/umap_def.h"
#include "mybpf_loader_func.h"

static int _mybpf_bare_get_bss_size(FILE_MEM_S *m, MYBPF_SIMPLE_CONVERT_PARAM_S *p)
{
    U32 flags;
    UMAP_ELF_MAP_S *map;

    int map_count = MYBPF_SIMPLE_GetMapCount(m);
    if (map_count == 0) {
        return 0;
    }

    
    if (! p->jit_arch) {
        RETURNI(BS_NOT_SUPPORT, "Not support map");
    }

    if (map_count > 1) {
        RETURNI(BS_NOT_SUPPORT, "Not support many map");
    }

    
    flags = MYBPF_SIMPLE_GetMapFlags(m);
    if ((flags & MYBPF_SIMPLE_MAP_FLAG_BSS) == 0) {
        RETURNI(BS_NOT_SUPPORT, "Not support !bss map");
    }

    map = MYBPF_SIMPLE_GetMap(m, 0);
    if (! map) {
        RETURNI(BS_ERR, "Can't get bss map");
    }

    return (map->size_value + (MYBPF_BARE_BSS_BLOCK_SIZE - 1)) / MYBPF_BARE_BSS_BLOCK_SIZE;
}

static int _mybpf_bare_write_hdr(VBUF_S *vbuf, MYBPF_SIMPLE_CONVERT_PARAM_S *p,
        int bss_size, int depend_count)
{
    MYBPF_BARE_HDR_S hdr = {0};
    MYBPF_BARE_SUB_HDR_S shdr = {0};

    int slen = VBUF_GetDataLength(vbuf) + sizeof(shdr); 
    int tlen = slen + sizeof(hdr); 

    hdr.magic = htonl(MYBPF_BARE_MAGIC);
    hdr.total_size = htonl(tlen);
    hdr.jit_arch = p->jit_arch;

    shdr.sub_size = htonl(slen);
    shdr.bss_size = htons(bss_size);
    shdr.app_ver = htons(p->app_ver);
    shdr.depends_count = htons(depend_count);
    shdr.utc_sec = TM_SecondsFromUTC();
    shdr.utc_sec = htonl(shdr.utc_sec);

    int ret = VBUF_AddHeadBuf(vbuf, &shdr, sizeof(shdr));
    ret |= VBUF_AddHeadBuf(vbuf, &hdr, sizeof(hdr));

    return ret;
}

static int _mybpf_bare_write_depends(VBUF_S *vbuf, MYBPF_HELPER_DEPENDS_S *depends)
{
    int ret = VBUF_AddHeadBuf(vbuf, depends->helpers, sizeof(int) * depends->count);
    if (ret < 0) {
        RETURNI(BS_ERR, "Expand vbuf error");
    }

    return 0;
}


static int _mybpf_bare_drop_no_used(VBUF_S *vbuf, FILE_MEM_S *m)
{
    int len;

    void *progs = MYBPF_SIMPLE_GetProgs(m);
    int progs_size = MYBPF_SIMPLE_GetProgsSize(m);

    if (progs_size <= 0) {
        RETURNI(BS_OUT_OF_RANGE, "Can't get progs");
    }

    
    len = (char*)progs - (char*)m->data;

    VBUF_EarseHead(vbuf, len); 
    VBUF_CutTail(vbuf, (m->len - len) - progs_size); 

    return 0;
}

static int _mybpf_bare_convert_vbuf(MYBPF_SIMPLE_CONVERT_PARAM_S *p, INOUT VBUF_S *vbuf)
{
    int ret;
    FILE_MEM_S m;
    MYBPF_HELPER_DEPENDS_S depends;

    m.data = VBUF_GetData(vbuf);
    m.len = VBUF_GetDataLength(vbuf);

    int bss_size = _mybpf_bare_get_bss_size(&m, p);
    if (bss_size < 0) {
        return bss_size;
    }

    ret = MYBPF_SIMPLE_CopyDepends(&m, &depends, ARRAY_SIZE(depends.helpers));
    if (ret < 0) {
        RETURNI(BS_OUT_OF_RANGE, "Can't get depends");
    }

    ret = _mybpf_bare_drop_no_used(vbuf, &m);

    if (! p->raw_bare) {
        ret |= _mybpf_bare_write_depends(vbuf, &depends);
        ret |= _mybpf_bare_write_hdr(vbuf, p, bss_size, depends.count);
    }

    return ret;
}

static int _mybpf_bare_convert_file(char *src_filename, char *dst_filename, MYBPF_SIMPLE_CONVERT_PARAM_S *p, OUT VBUF_S *vbuf)
{
    int ret;

    ret = MYBPF_SIMPLE_Convert2Buf(src_filename, p, vbuf);
    if (ret < 0) {
        return ret;
    }

    ret = _mybpf_bare_convert_vbuf(p, vbuf);
    if (ret < 0) {
        return ret;
    }

    ret = VBUF_WriteFile(dst_filename, vbuf);
    if (ret < 0) {
        RETURNI(BS_OUT_OF_RANGE, "Can't write file");
    }

    return 0;
}



int MYBPF_BARE_Convert2File(char *src_filename, char *dst_filename, MYBPF_SIMPLE_CONVERT_PARAM_S *p)
{
    VBUF_S vbuf;

    VBUF_Init(&vbuf);

    int ret = _mybpf_bare_convert_file(src_filename, dst_filename, p, &vbuf);

    VBUF_Finit(&vbuf);

    return ret;
}

