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
    MYBPF_HP_TCMD = 0, /* trigger cmd */
    MYBPF_HP_XDP,

    MYBPF_HP_MAX
};

#ifdef __cplusplus
}
#endif
#endif //MYBPF_HOOKPOINT_DEF_H_
