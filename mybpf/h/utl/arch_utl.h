/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _ARCH_UTL_H_
#define _ARCH_UTL_H_

#ifdef __cplusplus
extern "C" {
#endif

enum {
    ARCH_TYPE_NONE = 0,
    ARCH_TYPE_ARM64,
    ARCH_TYPE_X86_64,

    ARCH_TYPE_MAX
};

#ifdef IN_ULC_USER
static inline int ARCH_LocalArch(void)
{
    return ulc_get_local_arch();
}
#else

static inline int ARCH_LocalArch(void)
{
#ifdef __aarch64__
    return ARCH_TYPE_ARM64;
#endif

#ifdef __x86_64__
    return ARCH_TYPE_X86_64;
#endif

    return ARCH_TYPE_NONE;
}
#endif

int ARCH_GetTypeByName(char *arch_name);
char * ARCH_GetArchName(int arch_type);

#ifdef __cplusplus
}
#endif
#endif 
