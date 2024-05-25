/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _MYBPF_BARE_H
#define _MYBPF_BARE_H

#include "utl/mybpf_utl.h"
#include "utl/mybpf_bare_def.h"

#ifdef __cplusplus
extern "C" {
#endif


BOOL_T MYBPF_IsBareFile(char *filename);

int MYBPF_LoadBare(void *data, int len, const void **tmp_helpers, OUT MYBPF_BARE_S *bare);
void MYBPF_UnloadBare(MYBPF_BARE_S *bare);

U64 MYBPF_RunBareMain(MYBPF_BARE_S *bare, MYBPF_PARAM_S *p);
U64 MYBPF_RunBareFile(char *file, const void **tmp_helpers, MYBPF_PARAM_S *p);

#ifdef __cplusplus
}
#endif
#endif 
