/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2017-10-3
* Description: 
*
******************************************************************************/
#include "bs.h"
#include "utl/arch_utl.h"
#include "utl/time_utl.h"
#include "utl/rand_utl.h"
#include "utl/process_utl.h"
#include "utl/bpf_helper_utl.h"
#include "utl/umap_utl.h"
#include "utl/mmap_utl.h"
#include "utl/get_cpu.h"
#include "utl/ulc_helper_id.h"

#undef IN_ULC_USER

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
    return TM_NsFromInit();
}

static long __bpftrace_printk(const char *fmt, U32 fmt_size, void *p1, void *p2, void *p3)
{
    (void)fmt_size;
    printf(fmt, p1, p2, p3);
    return 0;
}

static U32 __bpfget_prandom_u32(void)
{
    return RAND_Get();
}

static U32 __bpfget_smp_processor_id(void)
{
    return sched_getcpu();
}

static U64 __bpfget_current_pid_tgid(void)
{
    U64 tgid = PROCESS_GetPid();
    U64 tid = PROCESS_GetTid();
    return (tgid << 32) | tid;
}

static U64 __bpfget_current_uid_gid(void)
{
    U64 gid = getgid();
    U64 uid = getuid();
    return (gid << 32) | uid;
}

static long __bpfget_current_comm(void *buf, U32 size_of_buf)
{
    char *self_name = SYSINFO_GetSlefName();
    if (! self_name) {
        return -1;
    }

    strlcpy(buf, self_name, size_of_buf);

    return 0;
}

static long __bpfstrtol(const char *buf, size_t buf_len, U64 flags, long *res)
{
    char *end;
    (void)buf_len;
    *res = strtol(buf, &end, flags);
    return end - buf;
}

static long __bpfstrtoul(const char *buf, size_t buf_len, U64 flags, unsigned long *res)
{
    char *end;
    (void)buf_len;
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

static void ulc_sys_usleep(U64 us)
{
    usleep(us);
}

static int ulc_sys_puts(const char *str)
{
    return printf("%s\n", str);
}

static inline void * _ulc_fp_2_process(void *fp)
{
    if (fp == (void*)0)
        return stdin;
    if (fp == (void*)1)
        return stdout;
    if (fp == (void*)2)
        return stderr;
    return fp;
}

static int ulc_sys_fprintf(void *fp, char *fmt, U64 *d, int count)
{
    fp = _ulc_fp_2_process(fp);

    switch (count) {
        case 0: return fprintf(fp,"%s",fmt);
        case 1: return fprintf(fp,fmt,d[0]);
        case 2: return fprintf(fp,fmt,d[0],d[1]);
        case 3: return fprintf(fp,fmt,d[0],d[1],d[2]);
        case 4: return fprintf(fp,fmt,d[0],d[1],d[2],d[3]);
        case 5: return fprintf(fp,fmt,d[0],d[1],d[2],d[3],d[4]);
        case 6: return fprintf(fp,fmt,d[0],d[1],d[2],d[3],d[4],d[5]);
        case 7: return fprintf(fp,fmt,d[0],d[1],d[2],d[3],d[4],d[5],d[6]);
        case 8: return fprintf(fp,fmt,d[0],d[1],d[2],d[3],d[4],d[5],d[6],d[7]);
        case 9: return fprintf(fp,fmt,d[0],d[1],d[2],d[3],d[4],d[5],d[6],d[7],d[8]);
        case 10: return fprintf(fp,fmt,d[0],d[1],d[2],d[3],d[4],d[5],d[6],d[7],d[8],d[9]);
        default: return -1;
    }
}

char * ulc_sys_fgets(char *s, int size, void *fp)
{
    fp = _ulc_fp_2_process(fp);
    return fgets(s, size, fp);
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

static int ulc_get_local_arch(void)
{
    return ARCH_LocalArch();
}

char * ulc_sys_env_name(void)
{
    return "user-space";
}

static void ulc_do_nothing()
{
}

void * ulc_sys_get_sym(char *sym_name)
{
    return dlsym(RTLD_DEFAULT, sym_name);
}

static void * ulc_mmap_map(void *addr, U64 len, U64 flag, int fd, U64 off)
{
    int prot = flag >> 32;
    int flags = flag;
#ifdef IN_MAC
    if (flags & 0x20) {
        flags &= ~(0x20);
        flags |= MAP_ANONYMOUS;
    }
#endif
    return mmap(addr, len, prot, flags, fd, off);
}

static int ulc_sys_get_errno()
{
    return errno;
}

static void ulc_init_timer(void *timer_node, void *timeout_func)
{
    MTIMER_S *t = timer_node;
    t->vclock.pfFunc = timeout_func;
    return;
}

static int ulc_add_timer(void *timer_node, U32 ms)
{
    MTIMER_S *t = timer_node;
    if (MTimer_IsRunning(t)) {
        return MTimer_RestartWithTime(t, ms);
    } else {
        return MTimer_Add(t, ms, 0, t->vclock.pfFunc, NULL);
    }
}

static void ulc_del_timer(void *timer_node)
{
    MTimer_Del(timer_node);
}


static const void * g_bpf_base_helpers[BPF_BASE_HELPER_COUNT] = {
    [0] = NULL,
    [1] = UMAP_LookupElem,
    [2] = UMAP_UpdateElem,
    [3] = UMAP_DeleteElem,
    [4] = __bpfprobe_read,
    [5] = __bpfktime_get_ns,
    [6] = __bpftrace_printk,
    [7] = __bpfget_prandom_u32,
    [8] = __bpfget_smp_processor_id,
    [14] = __bpfget_current_pid_tgid,
    [15] = __bpfget_current_uid_gid,
    [16] = __bpfget_current_comm,
    [93] = SpinLock_Lock,
    [94] = SpinLock_UnLock,
    [105] = __bpfstrtol,
    [106] = __bpfstrtoul,
    [165] = __bpf_snprintf,
};

#undef _
#define _(x) ((x) - 1000000)

static const void * g_bpf_sys_helpers[BPF_SYS_HELPER_COUNT] = {
    [0] = NULL, 
    [_(ULC_ID_MALLOC)] = malloc,
    [_(ULC_ID_FREE)] = free,
    [_(ULC_ID_KALLOC)] = malloc,
    [_(ULC_ID_KFREE)] = free,
    [_(ULC_ID_REALLOC)] = realloc,

    [_(ULC_ID_USLEEP)] = ulc_sys_usleep,

    [_(ULC_ID_PRINTF)] = printf,
    [_(ULC_ID_PUTS)] = ulc_sys_puts,
    [_(ULC_ID_SPRINTF)] = sprintf,
    [_(ULC_ID_FPRINTF)] = ulc_sys_fprintf,

    [_(ULC_ID_ACCESS)] = access,
    [_(ULC_ID_FTELL)] = ftell,
    [_(ULC_ID_FSEEK)] = fseek,
    [_(ULC_ID_FOPEN)] = fopen,
    [_(ULC_ID_FREAD)] = fread,
    [_(ULC_ID_FCLOSE)] = fclose,
    [_(ULC_ID_FGETS)] = ulc_sys_fgets,
    [_(ULC_ID_FWRITE)] = fwrite,
    [_(ULC_ID_STAT)] = stat,

    [_(ULC_ID_TIME)] = time,

    [_(ULC_ID_SOCKET)] = socket,
    [_(ULC_ID_BIND)] = bind,
    [_(ULC_ID_CONNECT)] = connect,
    [_(ULC_ID_LISTEN)] = listen,
    [_(ULC_ID_ACCEPT)] = accept,
    [_(ULC_ID_RECV)] = recv,
    [_(ULC_ID_SEND)] = send,
    [_(ULC_ID_CLOSE)] = close,
    [_(ULC_ID_SETSOCKOPT)] = setsockopt,

    [_(ULC_ID_THREAD_CREATE)] = pthread_create,

    [_(ULC_ID_RCU_CALL)] = RcuEngine_Call,
    [_(ULC_ID_RCU_LOCK)] = RcuEngine_Lock,
    [_(ULC_ID_RCU_UNLOCK)] = RcuEngine_UnLock,
    [_(ULC_ID_RCU_SYNC)] = RcuEngine_Sync,

    [_(ULC_ID_ERRNO)] = ulc_sys_get_errno,

    [_(ULC_ID_INIT_TIMER)] = ulc_init_timer,
    [_(ULC_ID_ADD_TIMER)] = ulc_add_timer,
    [_(ULC_ID_DEL_TIMER)] = ulc_del_timer,

    [_(ULC_ID_MMAP_MAP)] = ulc_mmap_map,
    [_(ULC_ID_MMAP_UNMAP)] = munmap,
    [_(ULC_ID_MMAP_MPROTECT)] = mprotect,

    [_(ULC_ID_GET_SYM)] = ulc_sys_get_sym,

    [_(ULC_ID_SET_TRUSTEESHIP)] = ulc_set_trusteeship,
    [_(ULC_ID_GET_TRUSTEESHIP)] = ulc_get_trusteeship,

    [_(ULC_ID_DO_NOTHING)] = ulc_do_nothing,
    [_(ULC_ID_LOCAL_ARCH)] = ulc_get_local_arch,
    [_(ULC_ID_SET_HELPER)] = ulc_set_helper,
    [_(ULC_ID_GET_HELPER)] = ulc_get_helper,
    [_(ULC_ID_GET_BASE_HELPER)] = ulc_get_base_helpers,
    [_(ULC_ID_GET_SYS_HELPER)] = ulc_get_sys_helpers,
    [_(ULC_ID_GET_USER_HELPER)] = ulc_get_user_helpers,
    [_(ULC_ID_MAP_GET_NEXT_KEY)] = UMAP_GetNextKey,
    [_(ULC_ID_ENV_NAME)] = ulc_sys_env_name,
};


static const void * g_bpf_user_helpers[BPF_USER_HELPER_COUNT] = {
    [0] = NULL, 
};


void * ulc_get_helper(unsigned int id, const void **tmp_helpers)
{
    if (id < BPF_BASE_HELPER_END) {
        return (void*)g_bpf_base_helpers[id];
    } else if ((id >= BPF_SYS_HELPER_START) && (id < BPF_SYS_HELPER_END)) {
        return (void*)g_bpf_sys_helpers[id - BPF_SYS_HELPER_START];
    } else if ((id >= BPF_USER_HELPER_START) && (id < BPF_USER_HELPER_END)) {
        return (void*)g_bpf_user_helpers[id - BPF_USER_HELPER_START];
    } else if ((id >= BPF_TMP_HELPER_START) && (id < BPF_TMP_HELPER_END) && (tmp_helpers)) {
        int idx = id - BPF_TMP_HELPER_START;
        if ((idx <= 0) || (idx >= *(U32*)tmp_helpers)) { 
            return NULL;
        }
        return (void*)tmp_helpers[idx];
    }

    return NULL;
}

const void ** ulc_get_base_helpers(void)
{
    return g_bpf_base_helpers;
}

const void ** ulc_get_sys_helpers(void)
{
    return g_bpf_sys_helpers;
}

const void ** ulc_get_user_helpers(void)
{
    return g_bpf_user_helpers;
}

int ulc_set_helper(U32 id, void *func)
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

