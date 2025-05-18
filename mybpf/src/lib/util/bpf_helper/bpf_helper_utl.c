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
#include <setjmp.h>
#include <locale.h>

#undef IN_ULC_USER

static long _ulc_ret_0(void)
{
    return 0;
}

static long _ulc_ret_1(void)
{
    return 1;
}

static long _ulc_ret_negative_1(void)
{
    return -1;
}

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

static long __bpf_sprintf(char *str, U32 str_size, const char *fmt, U64 *d, U32 d_len)
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

static unsigned long ulc_sys_copy_from_user(OUT void *to, void *from, unsigned long len)
{
    memcpy(to, from ,len);
    return 0;
}

static unsigned long ulc_sys_copy_to_user(OUT void *to, void *from, unsigned long len)
{
    memcpy(to, from ,len);
    return 0;
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


static int _sprintf(OUT char *buf, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int ret = vsnprintf(buf, 4096, fmt, args);
    va_end(args);
    return ret;
}

static int ulc_sys_fflush(void *fp)
{
    fp = _ulc_fp_2_process(fp);
    return fflush(fp);
}

static int ulc_sys_setvbuf(void *fp, char *buffer, int mode, size_t size)
{
    fp = _ulc_fp_2_process(fp);
    return setvbuf(fp, buffer, mode ,size);
}

static void ulc_sys_clearerr(void *fp)
{
    fp = _ulc_fp_2_process(fp);
    return clearerr(fp);
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

static int ulc_sys_fscanf(void *fp, char *fmt, U64 *d, int count)
{
    fp = _ulc_fp_2_process(fp);

    switch (count) {
        case 0: return fscanf(fp,"%s",fmt);
        case 1: return fscanf(fp,fmt,d[0]);
        case 2: return fscanf(fp,fmt,d[0],d[1]);
        case 3: return fscanf(fp,fmt,d[0],d[1],d[2]);
        case 4: return fscanf(fp,fmt,d[0],d[1],d[2],d[3]);
        case 5: return fscanf(fp,fmt,d[0],d[1],d[2],d[3],d[4]);
        case 6: return fscanf(fp,fmt,d[0],d[1],d[2],d[3],d[4],d[5]);
        case 7: return fscanf(fp,fmt,d[0],d[1],d[2],d[3],d[4],d[5],d[6]);
        case 8: return fscanf(fp,fmt,d[0],d[1],d[2],d[3],d[4],d[5],d[6],d[7]);
        case 9: return fscanf(fp,fmt,d[0],d[1],d[2],d[3],d[4],d[5],d[6],d[7],d[8]);
        case 10: return fscanf(fp,fmt,d[0],d[1],d[2],d[3],d[4],d[5],d[6],d[7],d[8],d[9]);
        default: return -1;
    }
}

static int ulc_sys_getc(void *fp)
{
    return getc(fp);
}

static int ulc_sys_ungetc(int c, void *fp)
{
    return ungetc(c, fp);
}

static char * ulc_sys_strerror(int errnum)
{
    return strerror(errnum);
}

static int ulc_sys_fclose(void *fp)
{
    fp = _ulc_fp_2_process(fp);
    return fclose(fp);
}

static long ulc_sys_ftell(void *fp)
{
    fp = _ulc_fp_2_process(fp);
    return ftell(fp);
}

static int ulc_sys_fseek(void *fp, long int offset, int whence)
{
    fp = _ulc_fp_2_process(fp);
    return fseek(fp, offset, whence);
}

static long ulc_sys_fread(void *ptr, long size, long nmemb, void *fp)
{
    fp = _ulc_fp_2_process(fp);
    return fread(ptr, size, nmemb, fp);
}

static char * ulc_sys_fgets(char *s, int size, void *fp)
{
    fp = _ulc_fp_2_process(fp);
    return fgets(s, size, fp);
}

static size_t ulc_sys_fwrite(const void *ptr, size_t size, size_t nmemb, void *fp)
{
    fp = _ulc_fp_2_process(fp);
    return fwrite(ptr, size, nmemb, fp);
}

int ulc_sys_fputs(const char *str, void *fp)
{
    fp = _ulc_fp_2_process(fp);
    return fputs(str, fp);
}

static int ulc_sys_fputc(int c, void *fp)
{
    fp = _ulc_fp_2_process(fp);
    return fputc(c, fp);
}

static int ulc_sys_ferror(void *fp)
{
    fp = _ulc_fp_2_process(fp);
    return ferror(fp);
}

static int ulc_sys_feof(void *fp)
{
    fp = _ulc_fp_2_process(fp);
    return feof(fp);
}

static char * ulc_sys_tmpnam(char *str)
{
	static char internal[L_tmpnam];
    char s[]="/tmp/cmguiXXXXXX";
    int temp_fd;

    temp_fd=mkstemp(s);
    if (temp_fd < 0) {
        return NULL;
    }

    
    close(temp_fd);

    
    return strcpy(str ? str : internal, s);
}

static void * g_bpf_runtime_ctrl = NULL; 

static void ulc_set_runtime(void *ptr)
{
    g_bpf_runtime_ctrl = ptr;
}

static void * ulc_get_runtime(void)
{
    return g_bpf_runtime_ctrl;
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

static void * ulc_sys_get_sym(char *sym_name)
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

static int ulc_sys_setjmp(void *env)
{
    return setjmp(env);
}

static void ulc_sys_longjmp(void *env, int val)
{
    longjmp(env, val);
}

static int ulc_init_timer(void *timer_node, void *timeout_func, int node_size)
{
    if (node_size < sizeof(MTIMER_S)) {
        return -1;
    }

    MTIMER_S *t = timer_node;
    t->vclock.pfFunc = timeout_func;

    return 0;
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
    [165] = __bpf_sprintf,
};

#undef _
#define _(x) ((x) - 1000000)

static const void * g_bpf_sys_helpers[BPF_SYS_HELPER_COUNT] = {
    [0] = NULL, 
    [_(ULC_ID_MALLOC)] = malloc,
    [_(ULC_ID_FREE)] = free,
    [_(ULC_ID_VMALLOC)] = malloc,
    [_(ULC_ID_VFREE)] = free,
    [_(ULC_ID_REALLOC)] = realloc,
    [_(ULC_ID_MODULE_ALLOC)] = malloc,
    [_(ULC_ID_MODULE_FREE)] = free,
    [_(ULC_ID_COPY_FROM_USER)] = ulc_sys_copy_from_user,
    [_(ULC_ID_COPY_TO_USER)] = ulc_sys_copy_to_user,

    [_(ULC_ID_PRINTF)] = printf,
    [_(ULC_ID_PUTS)] = ulc_sys_puts,
    [_(ULC_ID_SPRINTF)] = _sprintf,
    [_(ULC_ID_GETC)] = ulc_sys_getc,
    [_(ULC_ID_UNGETC)] = ulc_sys_ungetc,
    [_(ULC_ID_STRERROR)] = ulc_sys_strerror,
    [_(ULC_ID_FTELL)] = ulc_sys_ftell,
    [_(ULC_ID_FSEEK)] = ulc_sys_fseek,
    [_(ULC_ID_FOPEN)] = fopen,
    [_(ULC_ID_FREAD)] = ulc_sys_fread,
    [_(ULC_ID_FCLOSE)] = ulc_sys_fclose,
    [_(ULC_ID_FGETS)] = ulc_sys_fgets,
    [_(ULC_ID_FWRITE)] = ulc_sys_fwrite,
    [_(ULC_ID_FREOPEN)] = freopen,
    [_(ULC_ID_FERROR)] = ulc_sys_ferror,
    [_(ULC_ID_FEOF)] = ulc_sys_feof,
    [_(ULC_ID_FPUTS)] = ulc_sys_fputs,
    [_(ULC_ID_FPUTC)] = ulc_sys_fputc,
    [_(ULC_ID_FFLUSH)] = ulc_sys_fflush,
    [_(ULC_ID_SETVBUF)] = ulc_sys_setvbuf,
    [_(ULC_ID_CLEARERR)] = ulc_sys_clearerr,
    [_(ULC_ID_FPRINTF)] = ulc_sys_fprintf,
    [_(ULC_ID_FSCANF)] = ulc_sys_fscanf,
    [_(ULC_ID_TMPFILE)] = tmpfile,
    [_(ULC_ID_TMPNAM)] = ulc_sys_tmpnam,

    [_(ULC_ID_STAT)] = stat,
    [_(ULC_ID_ACCESS)] = access,
    [_(ULC_ID_TIME)] = time,
    [_(ULC_ID_LOCALECONV)] = localeconv,
    [_(ULC_ID_SETLOCALE)] = setlocale,
    [_(ULC_ID_USLEEP)] = ulc_sys_usleep,
    [_(ULC_ID_EXIT)] = exit,
    [_(ULC_ID_GETENV)] = getenv,
    [_(ULC_ID_CLOCK)] = clock,
    [_(ULC_ID_GMTIME)] = gmtime,
    [_(ULC_ID_LOCALTIME)] = localtime,
    [_(ULC_ID_STRFTIME)] = strftime,
    [_(ULC_ID_MKTIME)] = mktime,
    [_(ULC_ID_SYSTEM)] = system,
    [_(ULC_ID_REMOVE)] = remove,
    [_(ULC_ID_RENAME)] = rename,
    [_(ULC_ID_SIGNAL)] = signal,
    [_(ULC_ID_ABORT)] = abort,

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
    [_(ULC_ID_RCU_BARRIER)] = RcuEngine_Barrier,

    [_(ULC_ID_ERRNO)] = ulc_sys_get_errno,
    [_(ULC_ID_SETJMP)] = ulc_sys_setjmp,
    [_(ULC_ID_LONGJMP)] = ulc_sys_longjmp,

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
    [_(ULC_ID_ENV_NAME)] = ulc_sys_env_name,

    [_(ULC_ID_MAP_LOOKUP_ELEM)] = UMAP_LookupElem,
    [_(ULC_ID_MAP_UPDATE_ELEM)] = UMAP_UpdateElem,
    [_(ULC_ID_MAP_DELETE_ELEM)] = UMAP_DeleteElem,
    [_(ULC_ID_MAP_GET_NEXT_KEY)] = UMAP_GetNextKey,

    
    [_(ULC_ID_RAW_MAP_LOOKUP_ELEM)] = UMAP_LookupElem,
    [_(ULC_ID_RAW_MAP_UPDATE_ELEM)] = UMAP_UpdateElem,
    [_(ULC_ID_RAW_MAP_DELETE_ELEM)] = UMAP_DeleteElem,

    [_(ULC_ID_SET_RUNTIME)] = ulc_set_runtime,
    [_(ULC_ID_GET_RUNTIME)] = ulc_get_runtime,
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
    if (func == NULL) {
        func = _ulc_ret_0;
    } else if (func == (void*)(long)1) {
        func = _ulc_ret_1;
    } else if (func == (void*)(long)-1) {
        func = _ulc_ret_negative_1;
    }

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

