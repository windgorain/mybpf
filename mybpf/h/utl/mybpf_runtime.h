/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _MYBPF_RUNTIME_H
#define _MYBPF_RUNTIME_H

#include "utl/ufd_utl.h"
#include "utl/map_utl.h"
#include "utl/mybpf_hookpoint_def.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    DLL_HEAD_S list; 
    void *namefunc_tbl;
    void *idfunc_tbl;
    void *evob_tbl;
    DLL_HEAD_S hp_list[MYBPF_HP_MAX];
}MYBPF_RUNTIME_S;

int MYBPF_RuntimeInit(OUT MYBPF_RUNTIME_S *runtime);
void MYBPF_RuntimeFini(OUT MYBPF_RUNTIME_S *runtime);
BOOL_T MYBPF_RuntimeIsInited(MYBPF_RUNTIME_S *runtime);

MYBPF_RUNTIME_S * MYBPF_GetDftRuntime(void);

#ifdef __cplusplus
}
#endif
#endif 
