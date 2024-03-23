/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _MYBPF_SPF_DEF_H_
#define _MYBPF_SPF_DEF_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int (*init)(void);
    int (*finit)(void);
    int (*config_by_file)(char *config_file);
    int (*load_instance)(MYBPF_LOADER_PARAM_S *p);
    int (*unload_instance)(char *instance_name);
    void (*unload_all_instance)(void);
    int (*run)(int type, MYBPF_PARAM_S *p);
    void (*show_all_instance)(void);
}MYBPF_SPF_S;

#ifdef __cplusplus
}
#endif
#endif //MYBPF_SPF_DEF_H_
