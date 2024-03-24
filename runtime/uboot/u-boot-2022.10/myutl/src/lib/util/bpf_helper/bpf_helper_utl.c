/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2017-10-3
* Description: 
*
******************************************************************************/
#include "bs.h"
#include "utl/arch_utl.h"
#include "utl/bpf_helper_utl.h"
#include "rand.h"
static const void ** ulc_get_base_helpers(void);
static const void ** ulc_get_sys_helpers(void);
static const void ** ulc_get_user_helpers(void);

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
    return get_timer_us(0) * 1000;
}

static long __bpftrace_printk(const char *fmt, U32 fmt_size, ...)
{
    va_list args;
	int len;

	va_start(args, fmt_size);
    len = vprintf(fmt, args);
	va_end(args);

	return len;
}

static U32 __bpfget_prandom_u32(void)
{
    return rand();
}

static long __bpfstrtol(const char *buf, size_t buf_len, U64 flags, long *res)
{
    char *end;
    *res = simple_strtol(buf, &end, flags);
    return end - buf;
}

static long __bpfstrtoul(const char *buf, size_t buf_len, U64 flags, unsigned long *res)
{
    char *end;
    *res = strtoul(buf, &end, flags);
    return end - buf;
}

static long __bpf_snprintf(char *str, U32 str_size, const char *fmt, U64 *d, U32 d_len)
{
    switch (d_len) {
        case 0: return snprintf(str,str_size,"%s",fmt);
        case 8: return snprintf(str,str_size,fmt,d[0]);
        case 16: return snprintf(str,str_size,fmt,d[0],d[1]);
        case 24: return snprintf(str,str_size,fmt,d[0],d[1],d[2]);
        case 32: return snprintf(str,str_size,fmt,d[0],d[1],d[2],d[3]);
        case 40: return snprintf(str,str_size,fmt,d[0],d[1],d[2],d[3],d[4]);
        case 48: return snprintf(str,str_size,fmt,d[0],d[1],d[2],d[3],d[4],d[5]);
        case 56: return snprintf(str,str_size,fmt,d[0],d[1],d[2],d[3],d[4],d[5],d[6]);
        case 64: return snprintf(str,str_size,fmt,d[0],d[1],d[2],d[3],d[4],d[5],d[6],d[7]);
        case 72: return snprintf(str,str_size,fmt,d[0],d[1],d[2],d[3],d[4],d[5],d[6],d[7],d[8]);
        case 80: return snprintf(str,str_size,fmt,d[0],d[1],d[2],d[3],d[4],d[5],d[6],d[7],d[8],d[9]);
        default: return -1;
    }
}

static void * ulc_sys_rcu_malloc(int size)
{
    return malloc(size);
}

static void ulc_sys_rcu_free(void *m)
{
    free(m);
}

static int ulc_sys_strlcpy(void *dst, void *src, int size)
{
    return strlcpy(dst, src, size);
}

static int ulc_sys_strnlen(void *a, int max_len)
{
    const char *end = memchr(a, '\0', max_len);
    return end ? end - (char*)a : max_len;
}

static int ulc_sys_fprintf(void *fp, char *fmt, U64 *d, int count)
{
    return -1;
}

static long ulc_sys_ftell(void *fp)
{
    return -1;
}

static int ulc_sys_fseek(void *fp, long int offset, int whence)
{
    return -1;
}

static void * ulc_sys_fopen(const char *filename, const char *mode)
{
    return NULL;
}

static long ulc_sys_fread(void *ptr, long size, long nmemb, void *stream)
{
    return -1;
}

static int ulc_sys_fclose(void *stream)
{
    return -1;
}

static void * g_bpf_helper_trusteeship[16];

static int ulc_set_trusteeship(unsigned int id, void *ptr)
{
    if (id >= ARRAY_SIZE(g_bpf_helper_trusteeship)) {
        return -1;
    }
    g_bpf_helper_trusteeship[id] = ptr;
    return 0;
}

static void * ulc_get_trusteeship(unsigned int id)
{
    if (id >= ARRAY_SIZE(g_bpf_helper_trusteeship)) {
        return NULL;
    }
    return g_bpf_helper_trusteeship[id];
}

static void * ulc_get_helper(unsigned int id, const void **tmp_helpers)
{
    return BpfHelper_GetFuncExt(id, tmp_helpers);
}

static int ulc_get_local_arch(void)
{
    return ARCH_LocalArch();
}

static void * ulc_mmap_map(void *addr, U64 len, U64 flag, int fd, U64 off)
{
    return MEM_Malloc(len);
}

static void ulc_mmap_unmap(void *buf, int size)
{
    free(buf);
}

static int ulc_sys_mprotect(void *buf, int size)
{
    return 0;
}

static const void * g_bpf_base_helpers[BPF_BASE_HELPER_COUNT] = {
    [0] = NULL,
    [4] = __bpfprobe_read,
    [5] = __bpfktime_get_ns,
    [6] = __bpftrace_printk,
    [7] = __bpfget_prandom_u32,
    [105] = __bpfstrtol,
    [106] = __bpfstrtoul,
    [165] = __bpf_snprintf,
};

static const void * g_bpf_sys_helpers[BPF_SYS_HELPER_COUNT] = {
    [0] = NULL, /* 1000000 */
    [1] = calloc,
    [2] = free,
    [3] = ulc_sys_rcu_malloc,
    [4] = ulc_sys_rcu_free,
    [5] = malloc,
    [8] = strncmp,
    [9] = strlen,
    [10] = ulc_sys_strnlen,
    [11] = strcmp,
    [12] = ulc_sys_strlcpy,
    [13] = strdup,
    [14] = strtok,
    [40] = memcpy,
    [41] = memset,
    [42] = memmove,
    [101] = ulc_sys_fprintf,
    [102] = ulc_sys_ftell,
    [103] = ulc_sys_fseek,
    [104] = ulc_sys_fopen,
    [105] = ulc_sys_fread,
    [106] = ulc_sys_fclose,
    [400] = ulc_set_trusteeship,
    [401] = ulc_get_trusteeship,
    [500] = ulc_mmap_map,
    [501] = ulc_mmap_unmap,
    [502] = ulc_sys_mprotect,
    [507] = ulc_get_local_arch,
    [508] = ulc_get_helper,
    [509] = ulc_get_base_helpers,
    [510] = ulc_get_sys_helpers,
    [511] = ulc_get_user_helpers,
};

static const void * g_bpf_user_helpers[BPF_USER_HELPER_COUNT] = {
    [0] = NULL, /* 2000000 */
    [1] = NULL,
};

static const void ** ulc_get_base_helpers(void)
{
    return g_bpf_base_helpers;
}

static const void ** ulc_get_sys_helpers(void)
{
    return g_bpf_sys_helpers;
}

static const void ** ulc_get_user_helpers(void)
{
    return g_bpf_user_helpers;
}

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
        RETURN(BS_BAD_PARA);
    }

    return 0;
}

