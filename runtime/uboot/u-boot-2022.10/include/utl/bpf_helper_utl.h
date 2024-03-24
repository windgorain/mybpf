/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2017-10-3
* Description: 
******************************************************************************/
#ifndef _BPF_HELPER_UTL_H
#define _BPF_HELPER_UTL_H
#ifdef __cplusplus
extern "C"
{
#endif

#define BPF_BASE_HELPER_COUNT 1024
#define BPF_BASE_HELPER_END (BPF_BASE_HELPER_COUNT)

#define BPF_SYS_HELPER_START 1000000
#define BPF_SYS_HELPER_COUNT 1024
#define BPF_SYS_HELPER_END (BPF_SYS_HELPER_START + BPF_SYS_HELPER_COUNT)

#define BPF_USER_HELPER_START 2000000
#define BPF_USER_HELPER_COUNT 256
#define BPF_USER_HELPER_END (BPF_USER_HELPER_START + BPF_USER_HELPER_COUNT)

#define BPF_TMP_HELPER_START 3000000
#define BPF_TMP_HELPER_END   4000000

typedef U64 (*PF_BPF_HELPER_FUNC)(U64 p1, U64 p2, U64 p3, U64 p4, U64 p5);

void * BpfHelper_GetFuncExt(unsigned int id, const void **tmp_helpers);

int BpfHelper_RegFunc(U32 id, void *func);
const void ** BpfHelper_BaseHelper(void);
const void ** BpfHelper_SysHelper(void);
const void ** BpfHelper_UserHelper(void);

#define BpfHelper_CallFunc(err_ret, func_id, p1, p2, p3, p4, p5) ({ \
        PF_BPF_HELPER_FUNC fn = bpf_helper_get_func(func_id); \
        if (! fn) { return err_ret; } \
        return fn(p1, p2, p3, p4, p5); \
        })

#ifdef __cplusplus
}
#endif
#endif //BPF_HELPER_UTL_H_
