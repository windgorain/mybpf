/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _MYBPF_LOADER_H
#define _MYBPF_LOADER_H

#include "utl/elf_utl.h"
#include "utl/umap_utl.h"
#include "utl/mybpf_runtime.h"
#include "utl/mybpf_loader_def.h"
#include "utl/mybpf_aot_def.h"
#include "utl/mybpf_ioctl_def.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    DLL_NODE_S link_node;
    MYBPF_LOADER_PARAM_S param;
    RCU_NODE_S rcu;
    void *runtime;
    UINT jitted: 1;
    UINT reserved: 7;
    UINT main_prog_count: 8;
    UINT map_count: 8;
    int map_def_size;
    UMAP_HEADER_S *maps[MYBPF_LOADER_MAX_MAPS];
    int global_data_count; 
    void *global_data[ELF_MAX_GLOBAL_DATA_SEC_NUM]; 
    void *main_progs[MYBPF_LOADER_MAX_PROGS];
    int func_count;
    int insts_mem_len; 
    int insts_len; 
    ELF_PROG_INFO_S *funcs; 
    void *insts_mem; 
    void *insts; 
    MYBPF_AOT_PROG_CTX_S aot_ctx; 
}MYBPF_LOADER_NODE_S;

int MYBPF_LoaderLoad(MYBPF_RUNTIME_S *runtime, MYBPF_LOADER_PARAM_S *p);
int MYBPF_AttachAuto(MYBPF_RUNTIME_S *runtime, char *instance);
int MYBPF_LoaderUnload(MYBPF_RUNTIME_S *runtime, char *instance);
void MYBPF_LoaderUnloadAll(MYBPF_RUNTIME_S *runtime);
MYBPF_LOADER_NODE_S * MYBPF_LoaderGet(MYBPF_RUNTIME_S *runtime, char *instance);
MYBPF_LOADER_NODE_S * MYBPF_LoaderGetNext(MYBPF_RUNTIME_S *runtime, MYBPF_LOADER_NODE_S *cur);
void MYBPF_LoaderShowMaps(MYBPF_RUNTIME_S *r, PF_PRINT_FUNC print_func);
int MYBPF_ModuleIoctl(MYBPF_RUNTIME_S *r, char *instance, int cmd, MYBPF_IOCTL_S *d);

#ifdef __cplusplus
}
#endif
#endif 
