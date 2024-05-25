/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _MYBPF_MAIN_H_
#define _MYBPF_MAIN_H_

#include "bs.h"
#include "utl/mybpf_utl.h"
#include "utl/mybpf_hookpoint_def.h"

#ifdef __cplusplus
extern "C" {
#endif

int MYBPF_init(void);
U64 MYBPF_Notify(int event_type, MYBPF_PARAM_S *p);

#ifdef __cplusplus
}
#endif
#endif //MYBPF_MAIN_H_
