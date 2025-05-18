/*********************************************************
*   Copyright (C), Xingang.Li
*   Author:      Xingang.Li  Version: 1.0
*   Description: 
*
********************************************************/
#include "bs.h"
#include "utl/int_types.h"
#include "utl/args_utl.h"
#include "utl/arch_utl.h"
#include "utl/file_func.h"
#include "utl/subcmd_utl.h"
#include "utl/getopt2_utl.h"
#include "utl/mybpf_bare.h"
#include "utl/mybpf_loader_def.h"
#include "utl/mybpf_hookpoint_def.h"
#include "utl/mybpf_spf_def.h"

#include "./bare_builder_def.h"

static MYBPF_BARE_S g_mybpf_bare;
static MYBPF_SPF_S *g_mybpf_spf_ctrl;

static int _load_spf(char *instance, void *data, U32 data_size)
{
    MYBPF_LOADER_PARAM_S p = {0};
    FILE_MEM_S m;

    m.data = data;
    m.len = data_size;

    p.m = &m;
    p.instance = instance;
    p.flag = MYBPF_LOADER_FLAG_AUTO_ATTACH | MYBPF_LOADER_FLAG_SKIP_JIT_HELPER_CHECK;

    int ret = g_mybpf_spf_ctrl->load_instance(&p);
    if (ret < 0) {
        printf("Load %s failed \n", instance);
    }

    return ret;
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

    return 0;
}

static int _run_spf(int argc, char **argv)
{
    MYBPF_PARAM_S p = {0};

    p.p[0] = argc;
    p.p[1] = (long)argv;

    int ret =g_mybpf_spf_ctrl->run_hookpoint(MYBPF_HP_TCMD, &p);
    if (ret < 0) {
        return ret;
    }

    return p.bpf_ret;
}


int main(int argc, char **argv)
{
    FILE_MEM_S m;

    m.data = g_spf_loader;
    m.len = sizeof(g_spf_loader);

    if (_load_spf_loader(&m) < 0) {
        return -1;
    }

    if (_load_spf("bpfvm", g_bpfvm, sizeof(g_bpfvm)) < 0) {
        return -1;
    }

    if (_load_spf("runbare", g_runbare, sizeof(g_runbare)) < 0) {
        return -1;
    }

    return _run_spf(argc, argv);
}

