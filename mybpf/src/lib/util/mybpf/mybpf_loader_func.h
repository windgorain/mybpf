/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _MYBPF_LOADER_FUNC_H_
#define _MYBPF_LOADER_FUNC_H_

#ifdef __cplusplus
extern "C" {
#endif

void _MYBPF_LOADER_CopyMapsFd(MYBPF_LOADER_NODE_S *node, MYBPF_LOADER_NODE_S *old);
int _MYBPF_LOADER_LoadProgs(MYBPF_RUNTIME_S *runtime, MYBPF_LOADER_NODE_S *node);
BOOL_T _MYBPF_LOADER_CheckMayKeepMap(MYBPF_RUNTIME_S *runtime, MYBPF_MAPS_SEC_S *map_sec, MYBPF_LOADER_NODE_S *old_node);
int _MYBPF_LOADER_MakeExe(MYBPF_LOADER_NODE_S *node, void *prog, int prog_size);

#ifdef __cplusplus
}
#endif
#endif 
