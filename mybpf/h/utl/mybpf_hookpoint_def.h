/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _MYBPF_HOOKPOINT_DEF_H_
#define _MYBPF_HOOKPOINT_DEF_H_

#ifdef __cplusplus
extern "C" {
#endif

enum {
    MYBPF_HP_TCMD = 0, 
    MYBPF_HP_XDP,

    MYBPF_HP_MAX
};

enum {
    MYBPF_HP_RET_CONTINUE = 0,
    MYBPF_HP_RET_STOP
};

#ifdef __cplusplus
}
#endif
#endif 
