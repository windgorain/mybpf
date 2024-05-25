/*********************************************************
*   Copyright (C) LiXingang
*   Description: 给ULC用户使用的头文件
*
********************************************************/
#ifndef _ULC_USER_SYS_H
#define _ULC_USER_SYS_H

#include "utl/args_def.h"
#include "ulc_helper_id.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef IN_ULC_USER

#undef strlcpy
int strlcpy(char *dst, char *src, int size);

static void * (*ulc_sys_malloc)(int size) = (void *)ULC_ID_MALLOC;
static void (*ulc_sys_free)(void *m) = (void *)ULC_ID_FREE;
static void * (*ulc_sys_kalloc)(int size) = (void *)ULC_ID_KALLOC;
static void * (*ulc_sys_kfree)(void *m) = (void *)ULC_ID_KFREE;
static void * (*ulc_sys_realloc)(void *ptr, size_t size) = (void *)ULC_ID_REALLOC;

static int (*ulc_sys_usleep)(U32 usec) = (void*)ULC_ID_USLEEP;

static int (*ulc_sys_printf)(char *fmt, ...) = (void*)ULC_ID_PRINTF;
static int (*ulc_sys_puts)(const char *str) =  (void*)ULC_ID_PUTS;
static int (*ulc_sys_sprintf)(char *buf, const char *fmt, ...) =  (void*)ULC_ID_SPRINTF;
static int (*ulc_sys_fprintf)(void *fp, char *fmt, U64 *d, int count) = (void*)ULC_ID_FPRINTF;

static int (*ulc_sys_access)(const char *pathname, int mode) = (void*)ULC_ID_ACCESS;
static long (*ulc_sys_ftell)(void *fp) = (void*)ULC_ID_FTELL;
static int (*ulc_sys_fseek)(void *fp, long int offset, int whence) = (void*)ULC_ID_FSEEK;
static void * (*ulc_sys_fopen)(const char *filename, const char *mode) = (void*)ULC_ID_FOPEN;
static long (*ulc_sys_fread)(void *ptr, long size, long nmemb, void *stream) = (void*)ULC_ID_FREAD;
static int (*ulc_sys_fclose)(void *stream) = (void*)ULC_ID_FCLOSE;
static char * (*ulc_sys_fgets)(void *str, int n, void *fp) = (void*)ULC_ID_FGETS;
static size_t (*ulc_sys_fwrite)(const void *ptr, size_t size, size_t nmemb, void *fp) = (void*)ULC_ID_FWRITE;
static int (*ulc_sys_stat)(const char *path, void *stat) = (void*)ULC_ID_STAT;

static U64 (*ulc_sys_time)(U64 *seconds) = (void*)ULC_ID_TIME;

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

static int (*ulc_sys_get_errno)(void) = (void*)ULC_ID_ERRNO;

static void (*ulc_init_timer)(void *timer_node, void *timeout_func) = (void*)ULC_ID_INIT_TIMER;
static int (*ulc_add_timer)(void *timer_node, U32 ms) = (void*)ULC_ID_ADD_TIMER;
static void (*ulc_del_timer)(void *timer_node) = (void*)ULC_ID_DEL_TIMER;

static void * (*ulc_mmap_map)(void *addr, U64 len, U64 flag, int fd, U64 off) = (void*)ULC_ID_MMAP_MAP;
static void (*ulc_mmap_unmap)(void *buf, int size) = (void*)ULC_ID_MMAP_UNMAP;
static int (*ulc_mmap_mprotect)(void *buf, int size, U32 flag) = (void*)ULC_ID_MMAP_MPROTECT;

static void * (*ulc_sys_get_sym)(char *sym_name) = (void*)ULC_ID_GET_SYM;

enum {
    ULC_TRUSTEESHIP_MYBPF_FUNCS = 0,
};
static int (*ulc_set_trusteeship)(unsigned int id, void *ptr) = (void*)ULC_ID_SET_TRUSTEESHIP;
static void * (*ulc_get_trusteeship)(unsigned int id) = (void*)ULC_ID_GET_TRUSTEESHIP;
static void (*ulc_do_nothing)(void) = (void*)ULC_ID_DO_NOTHING;
static int (*ulc_get_local_arch)(void) = (void*)ULC_ID_LOCAL_ARCH;
static int (*ulc_set_helper)(U32 id, void *func) = (void*)ULC_ID_SET_HELPER;
static void * (*ulc_get_helper)(unsigned int id, const void **tmp_helpers) = (void*)ULC_ID_GET_HELPER;
static const void ** (*ulc_get_base_helpers)(void) = (void*)ULC_ID_GET_BASE_HELPER;
static const void ** (*ulc_get_sys_helpers)(void) = (void*)ULC_ID_GET_SYS_HELPER;
static const void ** (*ulc_get_user_helpers)(void) = (void*)ULC_ID_GET_USER_HELPER;
static void * (*ulc_map_get_next_key)(void *map, void *curr_key, void *next_key) = (void*)ULC_ID_MAP_GET_NEXT_KEY;
static char * (*ulc_sys_env_name)(void) = (void*)ULC_ID_ENV_NAME;

#define ulc_call_sym(_err_ret, _name, ...) ({ \
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

#endif

#ifdef __cplusplus
}
#endif
#endif
