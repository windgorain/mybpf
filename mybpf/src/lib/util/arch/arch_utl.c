/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#include "bs.h"
#include "utl/arch_utl.h"

static char * g_arch_name[] = {
    [ARCH_TYPE_NONE] = "none",
    [ARCH_TYPE_ARM64] = "arm64",
    [ARCH_TYPE_X86_64] = "x64",
};


int ARCH_GetTypeByName(char *arch_name)
{
    int i;

    if (! arch_name) {
        return ARCH_TYPE_NONE;
    }

    for (i=0; i< ARCH_TYPE_MAX; i++) {
        if (strcmp(arch_name, g_arch_name[i]) == 0) {
            return i;
        }
    }

    return ARCH_TYPE_NONE;
}

char * ARCH_GetArchName(int arch_type)
{
    if ((arch_type <= 0) || (arch_type >= ARCH_TYPE_MAX)) {
        return g_arch_name[ARCH_TYPE_NONE];
    }
    return g_arch_name[arch_type];
}

