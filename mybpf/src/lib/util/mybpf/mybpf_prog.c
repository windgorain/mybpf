/*================================================================
*   Created by LiXingang, Copyright LiXingang
*   Date: 2017.10.1
*   Description: ebpf prog管理
*
================================================================*/
#include "bs.h"
#include "utl/ufd_utl.h"
#include "utl/umap_utl.h"
#include "utl/bpf_helper_utl.h"
#include "utl/mybpf_loader.h"
#include "utl/mybpf_prog_def.h"
#include "utl/mybpf_prog.h"
#include "utl/mybpf_vm.h"
#include "utl/mybpf_jit.h"
#include "mybpf_prog_func.h"

static void _mybpf_prog_free_prog_rcu(void *pstRcuNode)
{
    MYBPF_PROG_NODE_S *node = container_of(pstRcuNode, MYBPF_PROG_NODE_S, rcu_node);
    MEM_Free(node);
}

static inline void _mybpf_prog_free_prog(void *f)
{
    MYBPF_PROG_NODE_S *node = f;
    RcuEngine_Call(&node->rcu_node, _mybpf_prog_free_prog_rcu);
}

static inline int _mybpf_prog_run_bpf_code(MYBPF_PROG_NODE_S *prog, MYBPF_PARAM_S *p)
{
    MYBPF_CTX_S ctx = {0};
    MYBPF_LOADER_NODE_S *n = prog->loader_node;

    ctx.begin_addr = n->insts;
    ctx.end_addr = (char*)n->insts + n->insts_len;
    ctx.mybpf_prog = prog;
    ctx.insts = prog->insn;
    ctx.auto_stack = 1;
    ctx.helper_fixed = 0;

    int ret = MYBPF_DefultRun(&ctx, p);
    if (ret < 0) {
        return ret;
    }

    p->bpf_ret = ctx.bpf_ret;

    return 0;
}

static inline int _mybpf_prog_run_jitted_code(MYBPF_PROG_NODE_S *prog, MYBPF_PARAM_S *p)
{
    U64 (*func)(U64,U64,U64,U64,U64) = prog->insn;
    p->bpf_ret = func(p->p[0], p->p[1], p->p[2], p->p[3], p->p[4]);
    return 0;
}

static inline int _mybpf_prog_run(MYBPF_PROG_NODE_S *prog, MYBPF_PARAM_S *p)
{
    MYBPF_LOADER_NODE_S *n = prog->loader_node;

    if (n->jitted) {
        return _mybpf_prog_run_jitted_code(prog, p);
    }

    return _mybpf_prog_run_bpf_code(prog, p);
}

void MYBPF_PROG_Free(MYBPF_RUNTIME_S *runtime, MYBPF_PROG_NODE_S *prog)
{
    _mybpf_prog_free_prog(prog);
}

void MYBPF_PROG_ShowProg(MYBPF_RUNTIME_S *runtime, PF_PRINT_FUNC print_func)
{
    MYBPF_PROG_NODE_S *prog;
    void *iter = NULL;
    MYBPF_LOADER_NODE_S *n;
    int i;

    while ((n = MYBPF_LoaderGetNext(runtime, &iter))) {
        for (i=0; i<n->main_prog_count; i++) {
            prog = n->main_progs[i];
            print_func("xlated:%u, sec:%s, name:%s, instance:%s, file:%s \r\n",
                    prog->insn_len, prog->sec_name, prog->prog_name,
                    n->param.instance, n->param.filename);
        }
    }
}


MYBPF_PROG_NODE_S * MYBPF_PROG_GetByFuncName(MYBPF_RUNTIME_S *runtime, char *instance, char *func_name)
{
    int i;
    MYBPF_PROG_NODE_S *prog;

    MYBPF_LOADER_NODE_S *loader = MYBPF_LoaderGet(runtime, instance);
    if (! loader) {
        return NULL;
    }

    for (i=0; i<loader->main_prog_count; i++) {
        prog = loader->main_progs[i];
        BS_DBGASSERT(prog);
        if (strcmp(prog->prog_name, func_name) == 0) {
            return prog;
        }
    }

    return NULL;
}


MYBPF_PROG_NODE_S * MYBPF_PROG_GetBySecName(MYBPF_RUNTIME_S *runtime, char *instance, char *sec_name)
{
    int i;
    MYBPF_PROG_NODE_S *prog;

    MYBPF_LOADER_NODE_S *loader = MYBPF_LoaderGet(runtime, instance);
    if (! loader) {
        return NULL;
    }

    for (i=0; i<loader->main_prog_count; i++) {
        prog = loader->main_progs[i];
        BS_DBGASSERT(prog);
        if (strcmp(prog->sec_name, sec_name) == 0) {
            return prog;
        }
    }

    return NULL;
}

int MYBPF_PROG_Run(MYBPF_PROG_NODE_S *prog, MYBPF_PARAM_S *p)
{
    return _mybpf_prog_run(prog, p);
}


MYBPF_PROG_NODE_S * MYBPF_PROG_GetNext(MYBPF_RUNTIME_S *runtime,
        char *instance, char *sec_name, MYBPF_PROG_NODE_S *current)
{
    if (! current) {
        void *node = MYBPF_LoaderGet(runtime, instance);
        return _MYBPF_PROG_GetFirst(runtime, node, sec_name);
    }

    return _MYBPF_PROG_GetNext(runtime, current->loader_node, sec_name, current);
}

