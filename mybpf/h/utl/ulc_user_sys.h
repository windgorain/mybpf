/*********************************************************
*   Copyright (C) LiXingang
*   Description: 给ULC用户使用的头文件
*
********************************************************/
#ifndef _ULC_USER_SYS_H
#define _ULC_USER_SYS_H

#include "utl/mybpf_utl.h"
#include "ulc_helper_id.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef IN_ULC_USER

#undef strlcpy

size_t strlcpy(char *__dest, const char *__src, size_t __n);

static void * (*ulc_sys_malloc)(int size) = (void *)ULC_ID_MALLOC;
static void (*ulc_sys_free)(void *m) = (void *)ULC_ID_FREE;
static void * (*ulc_sys_vmalloc)(int size) = (void *)ULC_ID_VMALLOC;
static void * (*ulc_sys_vfree)(void *m) = (void *)ULC_ID_VFREE;
static void * (*ulc_sys_realloc)(void *ptr, size_t size) = (void *)ULC_ID_REALLOC;
static void * (*ulc_sys_module_alloc)(int size) = (void *)ULC_ID_MODULE_ALLOC;
static void * (*ulc_sys_module_free)(void *m) = (void *)ULC_ID_MODULE_FREE;
static unsigned long (*ulc_sys_copy_from_user)(void *to, void *from, unsigned long len) = (void*)ULC_ID_COPY_FROM_USER;
static unsigned long (*ulc_sys_copy_to_user)(void *to, void *from, unsigned long len) = (void*)ULC_ID_COPY_TO_USER;

static int (*ulc_sys_printf)(char *fmt, ...) = (void*)ULC_ID_PRINTF;
static int (*ulc_sys_puts)(const char *str) =  (void*)ULC_ID_PUTS;
static int (*ulc_sys_sprintf)(char *buf, const char *fmt, ...) =  (void*)ULC_ID_SPRINTF;
static int (*ulc_sys_getc)(void *fp) = (void*)ULC_ID_GETC;
static int (*ulc_sys_ungetc)(int c, void *fp) = (void*)ULC_ID_UNGETC;
static char * (*ulc_sys_strerror)(int errnum) = (void*)ULC_ID_STRERROR;
static int (*ulc_sys_access)(const char *pathname, int mode) = (void*)ULC_ID_ACCESS;
static long (*ulc_sys_ftell)(void *fp) = (void*)ULC_ID_FTELL;
static int (*ulc_sys_fseek)(void *fp, long int offset, int whence) = (void*)ULC_ID_FSEEK;
static void * (*ulc_sys_fopen)(const char *filename, const char *mode) = (void*)ULC_ID_FOPEN;
static long (*ulc_sys_fread)(void *ptr, long size, long nmemb, void *fp) = (void*)ULC_ID_FREAD;
static int (*ulc_sys_fclose)(void *fp) = (void*)ULC_ID_FCLOSE;
static char * (*ulc_sys_fgets)(void *str, int n, void *fp) = (void*)ULC_ID_FGETS;
static size_t (*ulc_sys_fwrite)(const void *ptr, size_t size, size_t nmemb, void *fp) = (void*)ULC_ID_FWRITE;
static int (*ulc_sys_stat)(const char *path, void *stat) = (void*)ULC_ID_STAT;
static void * (*ulc_sys_freopen)(const char *filename, const char *mode, void *fp) = (void*)ULC_ID_FREOPEN;
static int (*ulc_sys_ferror)(void *fp) = (void*)ULC_ID_FERROR;
static int (*ulc_sys_feof)(void *fp) = (void*)ULC_ID_FEOF;
static int (*ulc_sys_fputs)(const char *str, void *fp) = (void*)ULC_ID_FPUTS;
static int (*ulc_sys_fputc)(int c, void *fp) = (void*)ULC_ID_FPUTC;
static int (*ulc_sys_fflush)(void *fp) = (void*)ULC_ID_FFLUSH;
static int (*ulc_sys_setvbuf)(void *fp, char *buffer, int mode, size_t size) = (void*)ULC_ID_SETVBUF;
static void (*ulc_sys_clearerr)(void *fp) = (void*)ULC_ID_CLEARERR;
static int (*ulc_sys_fprintf)(void *fp, char *fmt, U64 *d, int count) = (void*)ULC_ID_FPRINTF;
static int (*ulc_sys_fscanf)(void *fp, const char *fmt, U64 *d, int count) = (void*)ULC_ID_FSCANF;
static void * (*ulc_sys_tmpfile)(void) = (void*)ULC_ID_TMPFILE;
static char * (*ulc_sys_tmpnam)(char *str) = (void*)ULC_ID_TMPNAM;

static void * (*ulc_sys_localeconv)(void) = (void*)ULC_ID_LOCALECONV;
static char * (*ulc_sys_setlocale)(int category, const char *locale) = (void*)ULC_ID_SETLOCALE;
static int (*ulc_sys_usleep)(U32 usec) = (void*)ULC_ID_USLEEP;
static void (*ulc_sys_exit)(int status) = (void*)ULC_ID_EXIT;
static void (*ulc_sys_abort)(void) = (void*)ULC_ID_ABORT;
static char * (*ulc_sys_getenv)(const char *name) = (void*)ULC_ID_GETENV;
static U64 (*ulc_sys_clock)(void) = (void*)ULC_ID_CLOCK;
static U64 (*ulc_sys_time)(void *t) = (void*)ULC_ID_TIME;
static void * (*ulc_sys_gmtime)(const void *t) = (void*)ULC_ID_GMTIME;
static void * (*ulc_sys_localtime)(const void *t) = (void*)ULC_ID_LOCALTIME;
static U64 (*ulc_sys_strftime)(char *str, unsigned long maxsize, const char *format, const void *t) = (void*)ULC_ID_STRFTIME;
static U64 (*ulc_sys_mktime)(void *timeptr) = (void*)ULC_ID_MKTIME;
static int (*ulc_sys_system)(const char *command) = (void*)ULC_ID_SYSTEM;
static int (*ulc_sys_remove)(const char *filename) = (void*)ULC_ID_REMOVE;
static int (*ulc_sys_rename)(const char *old_filename, const char *new_filename) = (void*)ULC_ID_RENAME;
static void * (*ulc_sys_signal)(int sig, void (*func)(int)) = (void*)ULC_ID_SIGNAL;

static int (*ulc_sys_socket)(int af, int type, int protocol) = (void*)ULC_ID_SOCKET;
static int (*ulc_sys_bind)(int sock, void *addr, int addrlen) = (void*)ULC_ID_BIND;
static int (*ulc_sys_connect)(int sock, void *serv_addr, int addrlen) = (void*)ULC_ID_CONNECT;
static int (*ulc_sys_listen)(int sock, int backlog) = (void*)ULC_ID_LISTEN;
static int (*ulc_sys_accept)(int sock, void *addr, int *addrlen) = (void*)ULC_ID_ACCEPT;
static int (*ulc_sys_recv)(int s, char *buf, int len, int flags) = (void*)ULC_ID_RECV;
static int (*ulc_sys_send)(int s, char *buf, int len, int flags) = (void*)ULC_ID_SEND;
static int (*ulc_sys_close)(int s) = (void*)ULC_ID_CLOSE;
static int (*ulc_sys_setsockopt)(int sockfd, int level, int optname, const void *optval, int optlen) = (void*)ULC_ID_SETSOCKOPT;

static int (*ulc_pthread_create)(void *thread, const void *attr, void *fn, void *arg) = (void*)ULC_ID_THREAD_CREATE;

static void (*ulc_sys_rcu_call)(void *rcu, void *func) = (void*)ULC_ID_RCU_CALL;
static int (*ulc_sys_rcu_lock)(void) = (void*)ULC_ID_RCU_LOCK;
static void (*ulc_sys_rcu_unlock)(int state) = (void*)ULC_ID_RCU_UNLOCK;
static void (*ulc_sys_rcu_sync)(void) = (void*)ULC_ID_RCU_SYNC;
static void (*ulc_sys_rcu_barrier)(void) = (void*)ULC_ID_RCU_BARRIER;

static int (*ulc_sys_get_errno)(void) = (void*)ULC_ID_ERRNO;
static int (*ulc_sys_setjmp)(void *env) = (void*)ULC_ID_SETJMP;
static void (*ulc_sys_longjmp)(void *env, int val) = (void*)ULC_ID_LONGJMP;

static int (*ulc_init_timer)(void *timer_node, void *timeout_func, int node_size) = (void*)ULC_ID_INIT_TIMER;
static int (*ulc_add_timer)(void *timer_node, U32 ms) = (void*)ULC_ID_ADD_TIMER;
static void (*ulc_del_timer)(void *timer_node) = (void*)ULC_ID_DEL_TIMER;

static void * (*ulc_mmap_map)(void *addr, U64 len, U64 flag, int fd, U64 off) = (void*)ULC_ID_MMAP_MAP;
static void (*ulc_mmap_unmap)(void *buf, int size) = (void*)ULC_ID_MMAP_UNMAP;
static int (*ulc_mmap_mprotect)(void *buf, int size, U32 flag) = (void*)ULC_ID_MMAP_MPROTECT;

static void * (*ulc_sys_get_sym)(char *sym_name) = (void*)ULC_ID_GET_SYM;

static int (*ulc_set_trusteeship)(unsigned int id, void *ptr) = (void*)ULC_ID_SET_TRUSTEESHIP;
static void * (*ulc_get_trusteeship)(unsigned int id) = (void*)ULC_ID_GET_TRUSTEESHIP;
static void (*ulc_do_nothing)(void) = (void*)ULC_ID_DO_NOTHING;
static int (*ulc_get_local_arch)(void) = (void*)ULC_ID_LOCAL_ARCH;
static int (*ulc_set_helper)(U32 id, void *func) = (void*)ULC_ID_SET_HELPER;
static void * (*ulc_get_helper)(unsigned int id, const void **tmp_helpers) = (void*)ULC_ID_GET_HELPER;
static const void ** (*ulc_get_base_helpers)(void) = (void*)ULC_ID_GET_BASE_HELPER;
static const void ** (*ulc_get_sys_helpers)(void) = (void*)ULC_ID_GET_SYS_HELPER;
static const void ** (*ulc_get_user_helpers)(void) = (void*)ULC_ID_GET_USER_HELPER;
static char * (*ulc_sys_env_name)(void) = (void*)ULC_ID_ENV_NAME;

static void * (*ulc_map_lookup_elem)(void *map, void *key) = (void*)ULC_ID_MAP_LOOKUP_ELEM;
static long (*ulc_map_update_elem)(void *map, void *key, void *value, U32 flag) = (void*)ULC_ID_MAP_UPDATE_ELEM;
static long (*ulc_map_delete_elem)(void *map, void *key) = (void*)ULC_ID_MAP_DELETE_ELEM;
static void * (*ulc_map_get_next_key)(void *map, void *curr_key, void *next_key) = (void*)ULC_ID_MAP_GET_NEXT_KEY;

static void * (*ulc_raw_map_lookup_elem)(void *map, void *key) = (void*)ULC_ID_RAW_MAP_LOOKUP_ELEM;
static long (*ulc_raw_map_update_elem)(void *map, void *key, void *value, U32 flag) = (void*)ULC_ID_RAW_MAP_UPDATE_ELEM;
static long (*ulc_raw_map_delete_elem)(void *map, void *key) = (void*)ULC_ID_RAW_MAP_DELETE_ELEM;

static void * (*ulc_get_map_by_path)(char *path) = (void*)ULC_ID_GET_MAP_BY_PATH;
static void (*ulc_bpf_map_put)(void *map) = (void*)ULC_ID_BPF_MAP_PUT;

static int (*ulc_set_runtime)(void *ptr) = (void*)ULC_ID_SET_RUNTIME;
static void * (*ulc_get_runtime)(void) = (void*)ULC_ID_GET_RUNTIME;
static char * (*ulc_get_self_name)(void *self) = (void*)ULC_ID_GET_SELF_NAME;
static int (*_ulc_idfunc_run)(U32 id, MYBPF_PARAM_S *p) = (void*)ULC_ID_IDFUNC_RUN;
static int (*_ulc_namefunc_run)(char *name, MYBPF_PARAM_S *p) = (void*)ULC_ID_NAMEFUNC_RUN;
static int (*_ulc_evob_notify)(U32 event, MYBPF_PARAM_S *p) = (void*)ULC_ID_EVOB_NOTIFY;

#define ulc_idfunc_run(_id, ...) ({ \
    MYBPF_PARAM_S _p; \
    _p.p[0] = (long)BS_ARG_GET(1, ##__VA_ARGS__); \
    _p.p[1] = (long)BS_ARG_GET(2, ##__VA_ARGS__); \
    _p.p[2] = (long)BS_ARG_GET(3, ##__VA_ARGS__); \
    _p.p[3] = (long)BS_ARG_GET(4, ##__VA_ARGS__); \
    _p.p[4] = (long)BS_ARG_GET(5, ##__VA_ARGS__); \
    _ulc_idfunc_run((_id), &_p); \
    _p.bpf_ret; \
    })

#define ulc_namefunc_run(_name, ...) ({ \
    MYBPF_PARAM_S _p; \
    _p.p[0] = (long)BS_ARG_GET(1, ##__VA_ARGS__); \
    _p.p[1] = (long)BS_ARG_GET(2, ##__VA_ARGS__); \
    _p.p[2] = (long)BS_ARG_GET(3, ##__VA_ARGS__); \
    _p.p[3] = (long)BS_ARG_GET(4, ##__VA_ARGS__); \
    _p.p[4] = (long)BS_ARG_GET(5, ##__VA_ARGS__); \
    _ulc_namefunc_run((_name), &_p); \
    _p.bpf_ret; \
    })

#define ulc_evob_notify(_event, ...) ({ \
    MYBPF_PARAM_S _p; \
    _p.p[0] = (long)BS_ARG_GET(1, ##__VA_ARGS__); \
    _p.p[1] = (long)BS_ARG_GET(2, ##__VA_ARGS__); \
    _p.p[2] = (long)BS_ARG_GET(3, ##__VA_ARGS__); \
    _p.p[3] = (long)BS_ARG_GET(4, ##__VA_ARGS__); \
    _p.p[4] = (long)BS_ARG_GET(5, ##__VA_ARGS__); \
    _ulc_evob_notify((_event), &_p); \
    _p.bpf_ret; \
    })

#define ulc_call_sym_name(_err_ret, _name, ...) ({ \
        U64 _ret = (_err_ret); \
        U64 (*_func)(U64,U64,U64,U64,U64) = ulc_sys_get_sym(_name); \
        if (_func) { \
                _ret = _func((long)BS_ARG_GET(1, ##__VA_ARGS__),\
                        (long)BS_ARG_GET(2, ##__VA_ARGS__), \
                        (long)BS_ARG_GET(3, ##__VA_ARGS__), \
                        (long)BS_ARG_GET(4, ##__VA_ARGS__), \
                        (long)BS_ARG_GET(5, ##__VA_ARGS__)); \
        } \
        _ret; })

#define ulc_call_sym(_err_ret, _name, ...) ulc_call_sym_name(_err_ret, #_name, ##__VA_ARGS__)

#endif

#ifdef __cplusplus
}
#endif
#endif
