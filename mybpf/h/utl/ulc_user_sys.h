/*********************************************************
*   Copyright (C) LiXingang
*   Description: 给ULC用户使用的头文件
*
********************************************************/
#ifndef _ULC_USER_SYS_H
#define _ULC_USER_SYS_H

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef IN_ULC_USER

#undef PRINTFL
#define PRINTFL() do { \
    char _file[] = __FILE__; \
    printf("%s(%d) \n", _file, __LINE__); \
}while(0)

#define access(a,b) ulc_sys_access(a,b)
#define ftell(a) ulc_sys_ftell(a)
#define fseek(a,b,c) ulc_sys_fseek(a,b,c)
#define fopen(a,b) ulc_sys_fopen(a,b)
#define fread(a,b,c,d) ulc_sys_fread(a,b,c,d)
#define fclose(a) ulc_sys_fclose(a)
#define fgets(a,b,c) ulc_sys_fgets(a,b,c)

#define fprintf(fp,fmt, ...) ({ \
    char _fmt[] = fmt; \
    int _count = BS_ARG_COUNT(__VA_ARGS__); \
    U64 _d[10]; \
    long _ret = -1; \
    switch (_count) { \
        case 10: _d[9]=(unsigned long long)BS_ARG_GET(10, ##__VA_ARGS__); \
        case 9: _d[8]=(unsigned long long)BS_ARG_GET(9, ##__VA_ARGS__); \
        case 8: _d[7]=(unsigned long long)BS_ARG_GET(8, ##__VA_ARGS__); \
        case 7: _d[6]=(unsigned long long)BS_ARG_GET(7, ##__VA_ARGS__); \
        case 6: _d[5]=(unsigned long long)BS_ARG_GET(6, ##__VA_ARGS__); \
        case 5: _d[4]=(unsigned long long)BS_ARG_GET(5, ##__VA_ARGS__); \
        case 4: _d[3]=(unsigned long long)BS_ARG_GET(4, ##__VA_ARGS__); \
        case 3: _d[2]=(unsigned long long)BS_ARG_GET(3, ##__VA_ARGS__); \
        case 2: _d[1]=(unsigned long long)BS_ARG_GET(2, ##__VA_ARGS__); \
        case 1: _d[0]=(unsigned long long)BS_ARG_GET(1, ##__VA_ARGS__); \
        case 0: break; \
    } \
    if (_count <= 10) { _ret = ulc_sys_fprintf(fp,fmt,_d,_count);} \
    _ret; \
})

#define time(a) ulc_sys_time(a)

#undef assert
#define assert(x) 

#define flush(x) 

#undef BS_WARNNING
#define BS_WARNNING(X) BPF_Print X

#undef MEM_ZMalloc
#define MEM_ZMalloc(x) ({void *p = ulc_sys_malloc(x); if (p) {ulc_sys_memset(p, 0, (x));} p;})
#undef MEM_Malloc
#define MEM_Malloc(x) ulc_sys_malloc(x)
#undef MEM_Free
#define MEM_Free(x) ulc_sys_free(x)
#define malloc(a) ulc_sys_malloc(a)
#define calloc(a,b) ulc_sys_calloc(a,b)
#define free(a) ulc_sys_free(a)
static void * (*ulc_sys_calloc)(int nitems, int size) = (void *)1000001;
static void (*ulc_sys_free)(void *m) = (void *)1000002;
static void * (*ulc_sys_rcu_malloc)(int size) = (void *)1000003;
static void (*ulc_sys_rcu_free)(void *m) = (void *)1000004;
static void * (*ulc_sys_malloc)(int size) = (void *)1000005;

#undef strlcpy
#define strlcpy(a,b,c) ulc_sys_strlcpy(a,b,c)
#define TXT_Strdup(a) ulc_sys_strdup(a)
#define strdup(a) ulc_sys_strdup(a)
#define strtok_r(a,b,c) ulc_sys_strtok_r(a,b,c)
#define strcmp(a,b) ulc_sys_strcmp(a,b)
#define strnlen(a,b) ulc_sys_strnlen(a,b)
#define strlen(a) ulc_sys_strlen(a)
#define strncmp(a,b,c) ulc_sys_strncmp(a,b,c)
static int (*ulc_sys_strncmp)(void *a, void *b, int len) = (void*)1000008;
static int (*ulc_sys_strlen)(void *a) = (void*)1000009;
static int (*ulc_sys_strnlen)(void *a, int max_len) = (void*)1000010;
static int (*ulc_sys_strcmp)(void *a, void *b) = (void*)1000011;
static int (*ulc_sys_strlcpy)(void *dst, void *src, int size) = (void*)1000012;
static char * (*ulc_sys_strdup)(void *s) = (void*)1000013;
static char * (*ulc_sys_strtok_r)(char *str, const char *sep, char **ctx) = (void*)1000014;


#define memcpy(a,b,c) ulc_sys_memcpy(a,b,c)
#define memmove(a,b,c) ulc_sys_memmove(a,b,c)
static void (*ulc_sys_memcpy)(void *d, const void *s, int len) = (void*)1000040;
static void (*ulc_sys_memset)(void *d, int c, int len) = (void*)1000041;
static void * (*ulc_sys_memmove)(void *str1, const void *str2, int n) = (void*)1000042;

static int (*ulc_sys_access)(const char *pathname, int mode) = (void*)1000100;
static int (*ulc_sys_fprintf)(void *fp, char *fmt, U64 *d, int count) = (void*)1000101;
static long (*ulc_sys_ftell)(void *fp) = (void*)1000102;
static int (*ulc_sys_fseek)(void *fp, long int offset, int whence) = (void*)1000103;
static void * (*ulc_sys_fopen)(const char *filename, const char *mode) = (void*)1000104;
static long (*ulc_sys_fread)(void *ptr, long size, long nmemb, void *stream) = (void*)1000105;
static int (*ulc_sys_fclose)(void *stream) = (void*)1000106;
static char * (*ulc_sys_fgets)(void *str, int n, void *fp) = (void*)1000107;

static U64 (*ulc_sys_time)(U64 *seconds) = (void*)1000130;

#define socket(a,b,c) ulc_sys_socket(a,b,c)
#define bind(a,b,c) ulc_sys_bind(a,b,c)
#define connect(a,b,c) ulc_sys_connect(a,b,c)
#define listen(a,b) ulc_sys_listen(a,b)
#define accept(a,b,c) ulc_sys_accept(a,b,c)
#define recv(a,b,c,d) ulc_sys_recv(a,b,c,d)
#define send(a,b,c,d) ulc_sys_send(a,b,c,d)
#define close(a) ulc_sys_close(a)
#define setsockopt(a,b,c,d,e) ulc_sys_setsockopt(a,b,c,d,e)
static int (*ulc_sys_socket)(int af, int type, int protocol) = (void*)1000200;
static int (*ulc_sys_bind)(int sock, void *addr, int addrlen) = (void*)1000201;
static int (*ulc_sys_connect)(int sock, void *serv_addr, int addrlen) = (void*)1000202;
static int (*ulc_sys_listen)(int sock, int backlog) = (void*)1000203;
static int (*ulc_sys_accept)(int sock, void *addr, int *addrlen) = (void*)1000204;
static int (*ulc_sys_recv)(int s, char *buf, int len, int flags) = (void*)1000205;
static int (*ulc_sys_send)(int s, char *buf, int len, int flags) = (void*)1000206;
static int (*ulc_sys_close)(int s) = (void*)1000207;
static int (*ulc_sys_setsockopt)(int sockfd, int level, int optname, const void *optval, int optlen) = (void*)1000208;
#define pthread_create(a,b,c,d) ulc_pthread_create(a,b,c,d)
static int (*ulc_pthread_create)(void *thread, const void *attr, void *fn, void *arg) = (void*)1000300;
static int (*ulc_set_trusteeship)(unsigned int id, void *ptr) = (void*)1000400;
static void * (*ulc_get_trusteeship)(unsigned int id) = (void*)1000401;

#undef errno
#define errno ulc_sys_get_errno()
static int (*ulc_sys_get_errno)(void) = (void*)1000402;
#define mmap(a,b,c,d,e,f) ulc_mmap_map(a,b,(((U64)(c))<<32)|(d),e,f)
#define munmap(a,b) ulc_mmap_unmap(a,b)
#define mprotect(a,b,c) ulc_mmap_mprotect(a,b,c)
static void * (*ulc_mmap_map)(void *addr, U64 len, U64 flag, int fd, U64 off) = (void*)1000500;
static void (*ulc_mmap_unmap)(void *buf, int size) = (void*)1000501;
static int (*ulc_mmap_mprotect)(void *buf, int size, U32 flag) = (void*)1000502;

static int (*ulc_get_local_arch)(void) = (void*)1000507;
static void * (*ulc_get_helper)(unsigned int id, const void **tmp_helpers) = (void*)1000508;
static const void ** (*ulc_get_base_helpers)(void) = (void*)1000509;
static const void ** (*ulc_get_sys_helpers)(void) = (void*)1000510;
static const void ** (*ulc_get_user_helpers)(void) = (void*)1000511;

#endif

#ifdef __cplusplus
}
#endif
#endif
