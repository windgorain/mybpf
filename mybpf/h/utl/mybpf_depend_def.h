/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _MYBPF_DEPEND_DEF_H
#define _MYBPF_DEPEND_DEF_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int count;
    int helpers[1024];
}MYBPF_HELPER_DEPENDS_S;

#ifdef __cplusplus
}
#endif
#endif 
