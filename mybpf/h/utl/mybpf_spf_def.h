/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _MYBPF_SPF_DEF_H_
#define _MYBPF_SPF_DEF_H_

#include "utl/mybpf_loader_def.h"
#include "utl/mybpf_ioctl_def.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int (*finit)(void);
    int (*config_by_file)(char *config_file);
    int (*load_instance)(MYBPF_LOADER_PARAM_S *p);
    int (*unload_instance)(char *instance_name);
    void (*unload_all_instance)(void);
    void * (*get_next_prog)(char *instance, char *sec, void *curr);
    int (*run_prog)(void *prog, MYBPF_PARAM_S *p);
    int (*run_hookpoint)(int type, MYBPF_PARAM_S *p);
    int (*lock_run_hookpoint)(int type, MYBPF_PARAM_S *p);
    int (*run_bytecode)(void *begin, void *end, MYBPF_PARAM_S *p);
    int (*run_idfunc)(U32 id, MYBPF_PARAM_S *p);
    int (*run_namefunc)(char *name, MYBPF_PARAM_S *p);
    void * (*get_namefunc)(char *name);
    int (*evob_notify)(U32 event, MYBPF_PARAM_S *p);
    void (*set_agent)(unsigned int id, void *func);
    int (*loader_ioctl)(int cmd, MYBPF_IOCTL_S *d);
    int (*module_ioctl)(char *instance, int cmd, MYBPF_IOCTL_S *d);
}MYBPF_SPF_S;

#ifdef __cplusplus
}
#endif
#endif 
