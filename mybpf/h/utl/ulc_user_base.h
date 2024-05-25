/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _ULC_USER_BASE_H_
#define _ULC_USER_BASE_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef IN_ULC_USER

#define BPF_Print(_fmt, ...) ({ \
    char _msg[] = _fmt; \
    bpf_trace_printk(_msg, sizeof(_msg), ##__VA_ARGS__); \
})

#define ulc_sys_snprintf(buf,size,_fmt, ...) ({ \
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
    if (_count <= 10) { _ret = bpf_snprintf(buf,size,_fmt,_d,_count*8);} \
    _ret; \
})

#define ulc_bpf_snprintf(buf,size,fmt, ...) ({ \
    char _fmt[] = fmt; \
    long _ret = _ulc_sys_snprintf(buf,size,_fmt,##__VA_ARGS__); \
    _ret; \
})

#define snprintf(buf,size,fmt, ...) ulc_sys_snprintf(buf,size,fmt,##__VA_ARGS__)

struct bpf_map_def {
	unsigned int type;
	unsigned int key_size;
	unsigned int value_size;
	unsigned int max_entries;
	unsigned int map_flags;
};

static void *(*bpf_map_lookup_elem)(void *map, const void *key) = (void *) 1;
static long (*bpf_map_update_elem)(void *map, const void *key, const void *value, unsigned long long flags) = (void *) 2;
static long (*bpf_map_delete_elem)(void *map, const void *key) = (void *) 3;
static long (*bpf_probe_read)(void *dst, unsigned int size, const void *unsafe_ptr) = (void *) 4;
static unsigned long long (*bpf_ktime_get_ns)(void) = (void *) 5;
static long (*bpf_trace_printk)(const char *fmt, unsigned int fmt_size, ...) = (void *) 6;
static unsigned int (*bpf_get_prandom_u32)(void) = (void *) 7;
static unsigned int  (*bpf_get_smp_processor_id)(void) = (void *) 8;

static unsigned long long (*bpf_get_current_pid_tgid)(void) = (void *) 14;
static unsigned long long (*bpf_get_current_uid_gid)(void) = (void *) 15;
static long (*bpf_get_current_comm)(void *buf, unsigned int size_of_buf) = (void *) 16;
static long (*bpf_strtol)(const char *buf, int buf_len, unsigned long long flags, long *res) = (void *) 105;
static long (*bpf_strtoul)(const char *buf, int buf_len, unsigned long long flags, unsigned long *res) = (void *) 106;
static long (*bpf_snprintf)(char *str, U32 str_size, const char *fmt, U64 *data, U32 data_len) = (void*)165;

#endif

#ifdef __cplusplus
}
#endif
#endif 
