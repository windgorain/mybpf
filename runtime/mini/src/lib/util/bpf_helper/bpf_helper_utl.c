/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2017-10-3
* Description: 
*
******************************************************************************/
#include "bs.h"
#include "utl/bpf_helper_utl.h"

static long __bpfprobe_read(void *dst, U32 size, const void *unsafe_ptr)
{
    if ((! dst) || (! unsafe_ptr)) {
        return -1;
    }

    memcpy(dst, unsafe_ptr, size);

    return 0;
}

static U64 __bpfktime_get_ns(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    return ((U64)ts.tv_sec * 1000000000 + (U64)ts.tv_nsec);
}

static long __bpftrace_printk(const char *fmt, U32 fmt_size, void *p1, void *p2, void *p3)
{
    (void)fmt_size;
    printf(fmt, p1, p2, p3);
    return 0;
}

static U32 __bpfget_prandom_u32(void)
{
    return rand();
}

static long __bpfstrtol(const char *buf, size_t buf_len, U64 flags, long *res)
{
    char *end;
    *res = strtol(buf, &end, flags);
    return end - buf;
}

long __bpfstrtoul(const char *buf, size_t buf_len, U64 flags, unsigned long *res)
{
    char *end;
    *res = strtoul(buf, &end, flags);
    return end - buf;
}



static int ulc_sys_strlcpy(char *dst, char *src, int size)
{
    unsigned long n;
    char *p;

    for (p = dst, n = 0; n + 1 < size && *src!= '\0';  ++p, ++src, ++n) {
        *p = *src;
    }
    *p = '\0';
    if(*src == '\0') {
        return n;
    } else {
        return n + strlen(src);
    }
}

static int ulc_sys_strnlen(void *a, int max_len)
{
    const char *end = memchr(a, '\0', max_len);
    return end ? end - (char*)a : max_len;
}

/* base helper. 和linux内置定义helper一一对应,请不要注册和linux不一致的helper */
static const void * g_bpf_base_helpers[BPF_BASE_HELPER_COUNT] = {
    [4] = __bpfprobe_read,
    [5] = __bpfktime_get_ns,
    [6] = __bpftrace_printk,
    [7] = __bpfget_prandom_u32,
    [105] = __bpfstrtol,
    [106] = __bpfstrtoul,
};
/* sys helper. linux系统定义之外的统一定义, 请不要随意定义 */
static const void * g_bpf_sys_helpers[BPF_SYS_HELPER_COUNT] = {
    [0] = NULL, /* 1000000 */
    [1] = calloc,
    [2] = free,
    [5] = malloc,
    [8] = strncmp,
    [9] = strlen,
    [10] = ulc_sys_strnlen,
    [11] = strcmp,
    [12] = ulc_sys_strlcpy,
    [13] = strdup,
    [14] = strtok_r,
    [40] = memcpy,
    [41] = memset,
    [42] = memmove,
    [102] = ftell,
    [103] = fseek,
    [104] = fopen,
    [105] = fread,
    [106] = fclose,
    [107] = fgets,
    [130] = time,
};
/* user helper. 没有任何预规定，用户可以定义 */
static const void * g_bpf_user_helpers[BPF_USER_HELPER_COUNT];

const void ** BpfHelper_BaseHelper(void)
{
    return g_bpf_base_helpers;
}

const void ** BpfHelper_SysHelper(void)
{
    return g_bpf_sys_helpers;
}

const void ** BpfHelper_UserHelper(void)
{
    return (const void **)g_bpf_user_helpers;
}

/* 根据id获取helper函数指针 */
void * BpfHelper_GetFuncExt(unsigned int id, const void **tmp_helpers)
{
    if (id < BPF_BASE_HELPER_END) {
        return (void*)g_bpf_base_helpers[id];
    } else if ((id >= BPF_SYS_HELPER_START) && (id < BPF_SYS_HELPER_END)) {
        return (void*)g_bpf_sys_helpers[id - BPF_SYS_HELPER_START];
    } else if ((id >= BPF_USER_HELPER_START) && (id < BPF_USER_HELPER_END)) {
        return (void*)g_bpf_user_helpers[id - BPF_USER_HELPER_START];
    } else if ((id >= BPF_TMP_HELPER_START) && (id < BPF_TMP_HELPER_END) && (tmp_helpers)) {
        int idx = id - BPF_TMP_HELPER_START;
        if ((idx <= 0) || (idx >= *(U32*)tmp_helpers)) { /* tmp_helpers的开始位置放的是tmp_helpers数组大小 */
            return NULL;
        }
        return (void*)tmp_helpers[idx];
    }

    return NULL;
}

int BpfHelper_RegFunc(U32 id, void *func)
{
    if (id < BPF_BASE_HELPER_END) {
        g_bpf_base_helpers[id] = func;
    } else if ((id >= BPF_SYS_HELPER_START) && (id < BPF_SYS_HELPER_END)) {
        g_bpf_sys_helpers[id - BPF_SYS_HELPER_START] = func;
    } else if ((BPF_USER_HELPER_START <= id) && (id < BPF_USER_HELPER_END)) {
        g_bpf_user_helpers[id - BPF_USER_HELPER_START] = func;
    } else {
        return -1;
    }

    return 0;
}

