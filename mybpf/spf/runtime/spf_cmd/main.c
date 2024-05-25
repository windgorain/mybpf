/*********************************************************
*   Copyright (C), Xingang.Li
*   Author:      Xingang.Li  Version: 1.0
*   Description: 
*
********************************************************/
#include "bs.h"
#include "utl/args_utl.h"
#include "utl/arch_utl.h"
#include "utl/file_func.h"
#include "utl/subcmd_utl.h"
#include "utl/getopt2_utl.h"
#include "utl/mybpf_bare.h"
#include "utl/mybpf_loader_def.h"
#include "utl/mybpf_hookpoint_def.h"
#include "utl/mybpf_spf_def.h"

static MYBPF_BARE_S g_mybpf_bare;
static MYBPF_SPF_S *g_mybpf_spf_ctrl;

static int _load_spf(char *file)
{
    MYBPF_LOADER_PARAM_S p = {0};

    p.filename = file;
    p.instance = ".spfrun";
    p.flag = MYBPF_LOADER_FLAG_AUTO_ATTACH | MYBPF_LOADER_FLAG_JIT;

    int ret = g_mybpf_spf_ctrl->load_instance(&p);
    if (ret < 0) {
        printf("Load %s failed \n", file);
    }

    return ret;
}

static int _run_spf_run_cmd(int argc, char **argv)
{
    MYBPF_PARAM_S p = {0};
    void *prog = NULL;

    p.p[0] = argc;
    p.p[1] = (long)argv;

    while ((prog = g_mybpf_spf_ctrl->get_next_prog(".spfrun", ".spf.cmd/", prog))) {
        g_mybpf_spf_ctrl->run_prog(prog, &p);
    }
 
    return 0;
}

static int _run_spf(char *file, int argc, char **argv)
{
    int ret = _load_spf(file);
    if (ret < 0) {
        return ret;
    }

    return _run_spf_run_cmd(argc, argv);
}

static int _load_spf_loader(FILE_MEM_S *m)
{
    int ret;
    MYBPF_PARAM_S p = {0};

    ret = MYBPF_LoadBare(m->data, m->len, NULL, &g_mybpf_bare);
    if (ret < 0) {
        printf("Load spfloader failed \n");
        return ret;
    }

    p.p[0] = (long)m->data;
    MYBPF_RunBareMain(&g_mybpf_bare, &p);

    g_mybpf_spf_ctrl = (void*)(long)p.bpf_ret;
    if (! g_mybpf_spf_ctrl) {
        printf("Can't get spf ctrl \n");
        return -1;
    }

    g_mybpf_spf_ctrl->config_by_file("spf_load.cfg");

    return 0;
}

static int _load_spf_loader_file()
{
    FILE_MEM_S m = {0};
    char *spf_loader;

    if (ARCH_LocalArch() == ARCH_TYPE_ARM64) {
        spf_loader = "spf_loader.arm64.bare";
    } else if (ARCH_LocalArch() == ARCH_TYPE_X86_64) {
        spf_loader = "spf_loader.x64.bare";
    } else {
        fprintf(stderr, "Can't support this arch \n");
        return -1;
    }

    int ret = FILE_Mem(spf_loader, &m);
    if (ret < 0) {
        printf("Can't open file %s", spf_loader);
        return -1;
    }

    ret = _load_spf_loader(&m);

    FILE_FreeMem(&m);

    return ret;
}


int main(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage: %s spf-file [args] \n", argv[0]);
        return -1;
    }

    ErrCode_EnablePrint(1);

    if (_load_spf_loader_file() < 0) {
        return -1;
    }

    return _run_spf(argv[1], argc-1, argv+1);
}

