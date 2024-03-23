/*********************************************************
*   Copyright (C), Xingang.Li
*   Author:      Xingang.Li  Version: 1.0
*   Description: 
*
********************************************************/
#include <assert.h>
#include "bs.h"
#include "utl/arch_utl.h"
#include "utl/mybpf_utl.h"
#include "utl/mybpf_loader_def.h"
#include "utl/mybpf_spf_def.h"
#include "utl/mybpf_bare.h"
#include "utl/mybpf_main.h"

static MYBPF_BARE_S g_mybpf_bare;
static MYBPF_SPF_S *g_mybpf_spf_ctrl;

static int _load_spf_loader(char *spf_loader)
{
    MYBPF_PARAM_S p = {0};

    int ret = MYBPF_LoadBareFile(spf_loader, NULL, &g_mybpf_bare);
    if (ret < 0) {
        printf("Load loader failed \n");
        return ret;
    }

    MYBPF_RunBareMain(&g_mybpf_bare, &p);

    g_mybpf_spf_ctrl = (void*)(long)p.bpf_ret;
    if (! g_mybpf_spf_ctrl) {
        printf("Can't get spf ctrl \n");
        return -1;
    }

    ret = g_mybpf_spf_ctrl->init();
    if (ret < 0) {
        printf("SPF init failed \n");
        return -1;
    }

    g_mybpf_spf_ctrl->config_by_file("/usr/local/mybpf/mybpf.cfg");

    return 0;
}

/* bare_spf [spfloader] */
__attribute__((constructor)) int MYBPF_init(void)
{
    char *file = NULL;

    if (ARCH_LocalArch() == ARCH_TYPE_ARM64) {
        file = "/usr/local/mybpf/spf_loader.arm64.bare";
    } else if (ARCH_LocalArch() == ARCH_TYPE_X86_64) {
        file = "/usr/local/mybpf/spf_loader.x64.bare";
    } else {
        fprintf(stderr, "Can't support this arch \n");
        return -1;
    }

    return _load_spf_loader(file);
}

U64 MYBPF_Notify(int event_type, MYBPF_PARAM_S *p)
{
    return g_mybpf_spf_ctrl->run(event_type, p);
}


