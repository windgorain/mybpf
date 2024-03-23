/*********************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2017-10-1
* Description: 
*
********************************************************/
#include "bs.h"
#include "utl/mybpf_loader.h"
#include "utl/mybpf_prog.h"
#include "utl/mybpf_hookpoint.h"
#include "utl/ufd_utl.h"
#include "mybpf_def_inner.h"
#include "mybpf_osbase.h"

static MYBPF_HOOKPOINT_NODE_S * _mybpf_hookpoint_detach(DLL_HEAD_S *list, void *prog)
{
    MYBPF_HOOKPOINT_NODE_S *node;

    DLL_SCAN(list, node) {
        if (node->prog == prog) {
            DLL_DEL(list, &node->link_node);
            return node;
        }
    }

    return NULL;
}

int MYBPF_HookPointAttach(MYBPF_RUNTIME_S *runtime, DLL_HEAD_S *list, MYBPF_PROG_NODE_S *prog)
{
    MYBPF_HOOKPOINT_NODE_S *node;

    node = MEM_RcuZMalloc(sizeof(MYBPF_HOOKPOINT_NODE_S));
    if (! node) {
        RETURN(BS_NO_MEMORY);
    }

    node->prog = prog;

    DLL_ADD_RCU(list, &node->link_node);

    return 0;
}

void MYBPF_HookPointDetach(MYBPF_RUNTIME_S *runtime, DLL_HEAD_S *list, MYBPF_PROG_NODE_S *prog)
{
    MYBPF_HOOKPOINT_NODE_S * node = _mybpf_hookpoint_detach(list, prog);
    if (node) {
        MEM_RcuFree(node);
    }
}

int MYBPF_HookPointCall(MYBPF_RUNTIME_S *runtime, int type, MYBPF_PARAM_S *p)
{
    MYBPF_HOOKPOINT_NODE_S *node;
    DLL_HEAD_S *list = &runtime->hp_list[type];
    U64 ret = 0;

    int state = RcuEngine_Lock();

    DLL_SCAN(list, node) {
        ret = MYBPF_PROG_Run(node->prog, p);
        if (ret == MYBPF_HP_RET_STOP) {
            break;
        }
    }

    RcuEngine_UnLock(state);

    return ret;
}

