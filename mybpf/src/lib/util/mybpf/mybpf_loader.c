/*================================================================
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2017-10-1
* Description: 
*
================================================================*/
#include <sys/mman.h>
#include "bs.h"
#include "utl/err_code.h"
#include "utl/mmap_utl.h"
#include "utl/elf_utl.h"
#include "utl/bit_opt.h"
#include "utl/arch_utl.h"
#include "utl/mybpf_utl.h"
#include "utl/mybpf_loader.h"
#include "utl/mybpf_prog.h"
#include "utl/mybpf_vm.h"
#include "utl/mybpf_runtime.h"
#include "utl/mybpf_hookpoint.h"
#include "utl/mybpf_elf.h"
#include "utl/mybpf_relo.h"
#include "utl/mybpf_simple.h"
#include "utl/mybpf_jit.h"
#include "utl/mybpf_insn.h"
#include "utl/mybpf_dbg.h"
#include "utl/map_utl.h"
#include "utl/exec_utl.h"
#include "utl/ufd_utl.h"
#include "utl/umap_utl.h"
#include "utl/file_utl.h"
#include "mybpf_def_inner.h"
#include "mybpf_osbase.h"
#include "mybpf_loader_func.h"

static void * _mybpf_loader_map_fd_2_ptr(int is_direct, int imm, int off, void *ud)
{
    MYBPF_LOADER_NODE_S *node = ud;
    U64 addr = 0;

    if (imm >= node->map_count) {
        return NULL;
    }

    if (! is_direct) {
        return node->maps[imm];
    }

    UMAP_DirectValue(node->maps[imm], &addr, off);

    return (void*)(unsigned long)addr;
}

static int _mybpf_loader_fixup(MYBPF_LOADER_NODE_S *node)
{
    int ret;

    ret = MYBPF_PROG_ReplaceMapFdWithMapPtr(node->insts, node->insts_len, _mybpf_loader_map_fd_2_ptr, node);
    if (ret < 0) {
        return ret;
    }

    


    return 0;
}

static int _mybpf_loader_attach(MYBPF_RUNTIME_S *runtime, int type, MYBPF_PROG_NODE_S *prog)
{
    if (MYBPF_HookPointAttach(runtime, &runtime->hp_list[type], prog) < 0) {
        RETURN(BS_CAN_NOT_CONNECT);
    }

    prog->attached |= (1 << type);

    return 0;
}

static int _mybpf_loader_auto_attach_prog(MYBPF_RUNTIME_S *runtime, MYBPF_LOADER_NODE_S *node)
{
    int i;
    MYBPF_PROG_NODE_S *prog;

    for (i=0; i<node->main_prog_count; i++) {
        prog = node->main_progs[i];

        if (strncmp("tcmd/", prog->sec_name, 5) == 0) {
            _mybpf_loader_attach(runtime, MYBPF_HP_TCMD, prog);
        } else if (strncmp("xdp/", prog->sec_name, 4) == 0) {
            _mybpf_loader_attach(runtime, MYBPF_HP_XDP, prog);
        }
    }

    return 0;
}

static int _mybpf_loader_deattach(MYBPF_RUNTIME_S *runtime, int type, MYBPF_PROG_NODE_S *prog)
{
    MYBPF_HookPointDetach(runtime, &runtime->hp_list[type], prog);
    BIT_CLR(prog->attached, (1 << type));
    return 0;
}

static void _mybpf_detach_all_type(MYBPF_RUNTIME_S *runtime, MYBPF_PROG_NODE_S *prog)
{
    int type;

    for (type=0; type<MYBPF_HP_MAX; type++) {
        if (prog->attached & (1 << type)) {
            _mybpf_loader_deattach(runtime, type, prog);
        }
    }
}

static void _mybpf_loader_deattach_prog_all(MYBPF_RUNTIME_S *runtime, MYBPF_LOADER_NODE_S *node)
{
    MYBPF_PROG_NODE_S *prog;

    for (int i=0; i<node->main_prog_count; i++) {
        prog = node->main_progs[i];
        _mybpf_detach_all_type(runtime, prog);
    }
}

static void _mybpf_clear_progs_info(ELF_PROG_INFO_S *progs, int prog_count)
{
    for (int i=0; i<prog_count; i++) {
        MEM_SafeFree(progs[i].func_name);
        MEM_SafeFree(progs[i].sec_name);
    }
}

static void _mybpf_loader_free_node(MYBPF_RUNTIME_S *runtime, MYBPF_LOADER_NODE_S *node)
{
    int i;

    for (i=0; i<node->main_prog_count; i++) {
        MYBPF_PROG_Free(runtime, node->main_progs[i]);
    }

    for (i=0; i<node->map_count; i++) {
        UMAP_Close(node->maps[i]);
    }

    if (node->param.filename) {
        MEM_Free(node->param.filename);
    }

    if (node->param.instance) {
        MEM_Free(node->param.instance);
    }

    if (node->insts_mem) {
        if (node->jitted) {
            MMAP_Unmap(node->insts_mem, node->insts_mem_len);
        } else {
            MEM_Free(node->insts_mem);
        }
    }

    if (node->progs) {
        _mybpf_clear_progs_info(node->progs, node->progs_count);
        MEM_Free(node->progs);
    }

    MEM_Free(node);
}

static void _mybpf_laoder_free_node_rcu(void *rcu_node)
{
    MYBPF_LOADER_NODE_S *node = container_of(rcu_node, MYBPF_LOADER_NODE_S, rcu);
    _mybpf_loader_free_node(node->runtime, node);
}

static MYBPF_PROG_NODE_S * _mybpf_loader_find_sec(MYBPF_LOADER_NODE_S *node, char *sec)
{
    MYBPF_PROG_NODE_S *prog;
    for (int i=0; i<node->main_prog_count; i++) {
        prog = node->main_progs[i];
        if (strcmp(sec, prog->sec_name) == 0) {
            return prog;
        }
    }
    return NULL;
}
static int _mybpf_loader_do_module_init(MYBPF_LOADER_NODE_S *node)
{
    MYBPF_PROG_NODE_S *prog;
    MYBPF_PARAM_S p = {0};
    char tmp[] = ".spf/init";
    prog = _mybpf_loader_find_sec(node, tmp);
    if (! prog) {
        return 0;
    }
    MYBPF_PROG_Run(prog, &p);
    return p.bpf_ret;
}
static int _mybpf_loader_notify_module_finit(MYBPF_LOADER_NODE_S *node)
{
    MYBPF_PROG_NODE_S *prog;
    MYBPF_PARAM_S p = {0};
    char tmp[] = ".spf/finit";
    prog = _mybpf_loader_find_sec(node, tmp);
    if (! prog) {
        return 0;
    }

    MYBPF_PROG_Run(prog, &p);

    return p.bpf_ret;
}

static int _mybpf_loader_unload_node(MYBPF_RUNTIME_S *runtime, MYBPF_LOADER_NODE_S *node)
{
    _mybpf_loader_deattach_prog_all(runtime, node);

    if (_mybpf_loader_notify_module_finit(node) < 0) {
        return BS_NOT_COMPLETE;
    }

    MAP_Del(runtime->loader_map, node->param.instance, strlen(node->param.instance));
    RcuEngine_Call(&node->rcu, _mybpf_laoder_free_node_rcu);

    return 0;
}

static void _mybpf_load_and_jit(MYBPF_LOADER_NODE_S *node)
{
    MYBPF_JIT_INSN_S jit_insn = {0};
    MYBPF_JIT_CFG_S cfg = {0};

    jit_insn.insts = node->insts;
    jit_insn.insts_len = node->insts_len;
    jit_insn.progs = node->progs;
    jit_insn.progs_count = node->progs_count;

    MYBPF_DBG_OUTPUT(MYBPF_DBG_ID_LOADER, DBG_UTL_FLAG_PROCESS, "jit prog to local arch\n");

    cfg.mmap_exe = 1;
    cfg.helper_mode = MYBPF_JIT_HELPER_MODE_ID;
    cfg.get_helper_by_id = BpfHelper_GetFunc;
    cfg.tail_call_func = 12;

    if (MYBPF_Jit(&jit_insn, &cfg) < 0) {
        ErrCode_Print();
        PRINTLN_HYELLOW("Jit failed, to use raw code");
        return;
    }

    MEM_Free(node->insts_mem);
    node->insts_mem = node->insts = jit_insn.insts;
    node->insts_mem_len = node->insts_len = jit_insn.insts_len;
    node->jitted = 1;
}

static int _mybpf_dup_progs_info_names(ELF_PROG_INFO_S *progs, int prog_count)
{
    int ret = 0;

    for (int i=0; i<prog_count; i++) {
        if (progs[i].func_name) {
            progs[i].func_name = TXT_Strdup(progs[i].func_name);
        }
        if (progs[i].sec_name) {
            progs[i].sec_name = TXT_Strdup(progs[i].sec_name);
            if (! progs[i].sec_name) {
                ret = BS_NO_MEMORY;
            }
        }
    }

    return ret;
}

static int _mybpf_loader_load_ok(MYBPF_RUNTIME_S *runtime, MYBPF_LOADER_NODE_S *node, MYBPF_LOADER_NODE_S *old)
{
    if (old) {
        _mybpf_loader_unload_node(runtime, old);
    }

    char *instance = node->param.instance;

    int ret = MAP_AddNode(runtime->loader_map, instance, strlen(instance), node, &node->link_node, 0);
    if (ret < 0) {
        return ret;
    }

    if (node->param.flag & MYBPF_LOADER_FLAG_AUTO_ATTACH) {
        _mybpf_loader_auto_attach_prog(runtime, node);
    }

    return 0;
}


static BOOL_T _mybpf_loader_check_may_keep_map_elf(MYBPF_RUNTIME_S *runtime,
        ELF_S *new_elf, MYBPF_LOADER_NODE_S *old_node)
{
    MYBPF_MAPS_SEC_S map_sec;

    if (MYBPF_ELF_GetMapsSection(new_elf, &map_sec) < 0) {
        return FALSE;
    }

    return _MYBPF_LOADER_CheckMayKeepMap(runtime, &map_sec, old_node);
}


static int _mybpf_loader_check_may_replace_elf(MYBPF_RUNTIME_S *runtime,
        MYBPF_LOADER_PARAM_S *p, MYBPF_LOADER_NODE_S *old_node)
{
    ELF_S elf = {0};
    int ret;
    BOOL_T check = TRUE;

    
    ret = ELF_Open(p->filename, &elf);
    if (ret < 0) {
        RETURNI(BS_CAN_NOT_OPEN, "Can't open file %s", p->filename);
    }

    if (p->flag & MYBPF_LOADER_FLAG_KEEP_MAP) {
        
        check = _mybpf_loader_check_may_keep_map_elf(runtime, &elf, old_node);
    }

    ELF_Close(&elf);

    if (! check) {
        RETURNI(BS_NOT_MATCHED, "Map not match");
    }

    return 0;
}

static int _mybpf_loader_load_simple_maps(MYBPF_LOADER_NODE_S *node, MYBPF_MAPS_SEC_S *map_sec, MYBPF_LOADER_PARAM_S *p)
{
    int i;
    void *hdr;
    UMAP_ELF_MAP_S *map;

    map = map_sec->maps;
    node->map_def_size = map_sec->map_def_size;
    node->map_count = 0;

    for (i=0; i<map_sec->map_count; i++) {
        char *map_name = MYBPF_SIMPLE_GetMapName(&p->simple_mem, i);

        if (! map_name) {
            map_name = "";
        }

        MYBPF_DBG_OUTPUT(MYBPF_DBG_ID_LOADER, DBG_UTL_FLAG_PROCESS,
                "Open map:%s key_size:%d value_size:%d max_elem:%d flags:0x%x \n",
                map_name, map->size_key, map->size_value, map->max_elem, map->flags);

        hdr = UMAP_Open(map, map_name);
        if (! hdr) {
            RETURN(BS_CAN_NOT_OPEN);
        }
        node->maps[i] = hdr;
        node->map_count ++;
        map = (void*)((char*)map + map_sec->map_def_size);
    }

    return 0;
}

static int _mybpf_loader_load_globle_data(MYBPF_LOADER_NODE_S *node, MYBPF_LOADER_PARAM_S *p)
{
    int i;
    int map_data_count;

    
    map_data_count = MYBPF_SIMPLE_GetTypeSecCount(&p->simple_mem, MYBPF_SIMPLE_SEC_TYPE_GLOBAL_DATA);

    for (i=0; i<map_data_count; i++) {
        MYBPF_SIMPLE_MAP_DATA_S *sec = MYBPF_SIMPLE_GetSec(&p->simple_mem, MYBPF_SIMPLE_SEC_TYPE_GLOBAL_DATA, i);
        if (sec)  {
            int key = 0;
            void *data = MYBPF_SIMPLE_GetSecData(sec);
            if (data) {
                UMAP_UpdateElem(node->maps[sec->map_index], &key, data, 0);
            }
            node->global_data[node->global_data_count] = UMAP_LookupElem(node->maps[sec->map_index], &key);
            node->global_data_count ++;
        }
    }

    return 0;
}

static int _mybpf_loader_open_simple_maps(MYBPF_RUNTIME_S *runtime, MYBPF_LOADER_NODE_S *node, MYBPF_LOADER_PARAM_S *p)
{
    int ret;
    MYBPF_MAPS_SEC_S map_sec;

    ret = MYBPF_SIMPLE_GetMapsSection(&p->simple_mem, &map_sec);
    if (ret < 0) {
        return ret;
    }

    if (map_sec.map_count == 0) {
        return 0;
    }

    if (map_sec.map_def_size < sizeof(UMAP_ELF_MAP_S)) {
        RETURN(BS_ERR);
    }

    if (map_sec.map_count > MYBPF_LOADER_MAX_MAPS) {
        BS_DBGASSERT(0);
        RETURN(BS_OUT_OF_RANGE);
    }

    if ((ret = _mybpf_loader_load_simple_maps(node, &map_sec, p)) < 0) {
        return ret;
    }

    if ((ret = _mybpf_loader_load_globle_data(node, p)) < 0) {
        return ret;
    }

    return 0;
}

static int _mybpf_loader_check_helpers_exist(FILE_MEM_S *f, const void **tmp_helpers)
{
    int i;

    void *sec = MYBPF_SIMPLE_GetSec(f, MYBPF_SIMPLE_SEC_TYPE_HELPER_DEPENDS, 0);
    if (! sec) {
        return 0;
    }

    int *helpers = MYBPF_SIMPLE_GetSecData(sec);
    int count = MYBPF_SIMPLE_GetSecDataSize(sec) / sizeof(int);

    for (i=0; i<count; i++) {
        int helper_id = ntohl(helpers[i]);
        if (! BpfHelper_GetFuncExt(helper_id, tmp_helpers)) {
            RETURNI(BS_NOT_SUPPORT, "Helper %d is not support", helper_id);
        }
    }

    return 0;
}

static int _mybpf_loader_simple_prog_load_jitted(MYBPF_RUNTIME_S *runtime,
        FILE_MEM_S *f, MYBPF_LOADER_NODE_S *node, int jit_arch)
{
    void *prog;

    MYBPF_DBG_OUTPUT(MYBPF_DBG_ID_LOADER, DBG_UTL_FLAG_PROCESS,
            "Prog has been jitted, arch=%s\n", ARCH_GetArchName(jit_arch));

    if (jit_arch != ARCH_LocalArch()) {
        RETURNI(BS_NOT_SUPPORT, "Can't support jitted arch %s", ARCH_GetArchName(jit_arch));
    }

    prog = MYBPF_SIMPLE_GetProgs(f);

    return _MYBPF_LOADER_MakeExe(node, prog, node->insts_len);
}

static int _mybpf_loader_simple_prog_load_raw(MYBPF_RUNTIME_S *runtime, FILE_MEM_S *f, MYBPF_LOADER_NODE_S *node)
{
    node->insts_mem =  node->insts = MYBPF_SIMPLE_DupProgs(f);
    if (! node->insts) {
        RETURNI(BS_ERR, "Can't alloc memory for prog");
    }

    node->insts_mem_len = node->insts_len;

    int ret = _mybpf_loader_fixup(node);
    if (ret < 0) {
        return ret;
    }

    if (node->param.flag & MYBPF_LOADER_FLAG_JIT) {
        _mybpf_load_and_jit(node);
    }

    return 0;
}

static int _mybpf_loader_simple_prog_load(MYBPF_RUNTIME_S *runtime, FILE_MEM_S *f, MYBPF_LOADER_NODE_S *node)
{
    int jit_arch = MYBPF_SIMPLE_GetJitArch(f);
    int ret;

    if (jit_arch) {
        ret = _mybpf_loader_simple_prog_load_jitted(runtime, f, node, jit_arch);
    } else {
        ret = _mybpf_loader_simple_prog_load_raw(runtime, f, node);
    }

    return ret;
}

static int _mybpf_loader_load_simple_progs(MYBPF_RUNTIME_S *runtime, MYBPF_LOADER_NODE_S *node,
        MYBPF_LOADER_PARAM_S *p)
{
    int ret = 0;
    FILE_MEM_S *f = &p->simple_mem;
    int size = MYBPF_SIMPLE_GetProgsSize(f);

    node->insts_len = size;

    node->progs_count = MYBPF_SIMPLE_GetProgsCount(f);
    if (node->progs_count == 0) {
        RETURNI(BS_ERR, "Prog count is 0");
    }

    node->progs = MEM_ZMalloc(sizeof(ELF_PROG_INFO_S) * node->progs_count);
    if (! node->progs) {
        RETURNI(BS_ERR, "Can't alloc memory for progs");
    }
    MYBPF_SIMPLE_GetProgsInfo(f, node->progs, node->progs_count);
    if (_mybpf_dup_progs_info_names(node->progs,node->progs_count) < 0) {
        RETURN(BS_NO_MEMORY);
    }

    ret = _mybpf_loader_simple_prog_load(runtime, f, node);
    if (ret < 0) {
        return ret;
    }

    if (MYBPF_DBG_IS_SWITCH_ON(MYBPF_DBG_ID_LOADER, MYBPF_DBG_FLAG_LOADER_PROGS)) {
        for (int i=0; i<node->progs_count; i++) {
            ELF_PROG_INFO_S *p = &node->progs[i];
            MYBPF_DBG_OUTPUT(MYBPF_DBG_ID_LOADER, MYBPF_DBG_FLAG_LOADER_PROGS,
                    "sec_name:%s funcname:%s offset:%d addr:0x%p \n",
                    p->sec_name, p->func_name, p->prog_offset, (char*)node->insts + p->prog_offset);
        }
    }

    return _MYBPF_LOADER_LoadProgs(runtime, node);
}

static int _mybpf_loader_load_simple(MYBPF_RUNTIME_S *runtime, MYBPF_LOADER_NODE_S *node,
        MYBPF_LOADER_NODE_S *old, MYBPF_LOADER_PARAM_S *p)
{
    int ret;

    if (old && (p->flag & MYBPF_LOADER_FLAG_KEEP_MAP)) {
        _MYBPF_LOADER_CopyMapsFd(node, old);
    } else {
        ret = _mybpf_loader_open_simple_maps(runtime, node, p);
        if (ret < 0) {
            return ret;
        }
    }

    ret = _mybpf_loader_load_simple_progs(runtime, node, p);
    if (ret < 0) {
        return ret;
    }

    ret = _mybpf_loader_do_module_init(node);
    if (ret < 0) {
        return ret;
    }
    return _mybpf_loader_load_ok(runtime, node, old);
}


static BOOL_T _mybpf_loader_check_may_keep_map_simple(MYBPF_RUNTIME_S *runtime, FILE_MEM_S *f, MYBPF_LOADER_NODE_S *old)
{
    MYBPF_MAPS_SEC_S map_sec;

    if (MYBPF_SIMPLE_GetMapsSection(f, &map_sec) < 0) {
        return FALSE;
    }

    return _MYBPF_LOADER_CheckMayKeepMap(runtime, &map_sec, old);
}

static int _mybpf_loader_check_may_replace_simple(MYBPF_RUNTIME_S *r, MYBPF_LOADER_PARAM_S *p, MYBPF_LOADER_NODE_S *old)
{
    BOOL_T check = TRUE;

    FILE_MEM_S *f = &p->simple_mem;

    BS_DBGASSERT(f);

    if (p->flag & MYBPF_LOADER_FLAG_KEEP_MAP) {
        
        check = _mybpf_loader_check_may_keep_map_simple(r, f, old);
    }

    if (! check) {
        RETURNI(BS_NOT_MATCHED, "Map not match");
    }

    return 0;
}

static int _mybpf_loader_check_may_replace(MYBPF_RUNTIME_S *runtime, MYBPF_LOADER_PARAM_S *p, MYBPF_LOADER_NODE_S *old)
{
    if (p->simple_mem.data) {
        return _mybpf_loader_check_may_replace_simple(runtime, p, old);
    }

    return _mybpf_loader_check_may_replace_elf(runtime, p, old);
}

static MYBPF_LOADER_NODE_S * _mybpf_loader_get_node(MYBPF_RUNTIME_S *runtime, char *instance)
{
    return MAP_Get(runtime->loader_map, instance, strlen(instance));
}

static MYBPF_LOADER_NODE_S * _mybpf_loader_get_next_node(MYBPF_RUNTIME_S *runtime, void **iter)
{
    MAP_ELE_S *ele = *iter;

    ele = MAP_GetNextEle(runtime->loader_map, ele);
    *iter = ele;

    if (! ele) {
        return NULL;
    }

    return ele->pData;
}

static MYBPF_LOADER_NODE_S * _mybpf_loader_create_node(MYBPF_RUNTIME_S *runtime, MYBPF_LOADER_PARAM_S *p)
{
    MYBPF_LOADER_NODE_S *node;

    node = MEM_ZMalloc(sizeof(MYBPF_LOADER_NODE_S));
    if (! node) {
        ERR_VSet(BS_NO_MEMORY, "Can't alloc memory");
        return NULL;
    }

    node->param.flag = p->flag;

    node->param.simple_mem = p->simple_mem;

    if (p->filename) {
        node->param.filename = TXT_Strdup(p->filename);
        if (node->param.filename == NULL) {
            ERR_VSet(BS_NO_MEMORY, "Can't alloc memory");
            _mybpf_loader_free_node(runtime, node);
            return NULL;
        }
    }

    node->param.instance = TXT_Strdup(p->instance);
    if (node->param.instance == NULL) {
        ERR_VSet(BS_NO_MEMORY, "Can't alloc memory");
        _mybpf_loader_free_node(runtime, node);
        return NULL;
    }

    node->aot_ctx.base_helpers = BpfHelper_BaseHelper();
    node->aot_ctx.sys_helpers = BpfHelper_SysHelper();
    node->aot_ctx.user_helpers = BpfHelper_UserHelper();
    node->aot_ctx.tmp_helpers = p->tmp_helpers;
    node->aot_ctx.maps = node->maps;
    node->aot_ctx.global_map_data = node->global_data;
    node->aot_ctx.loader_node = node;

    node->runtime = runtime;

    return node;
}

static int _mybpf_loader_load_to_runtime(MYBPF_RUNTIME_S *r, MYBPF_LOADER_PARAM_S *p, MYBPF_LOADER_NODE_S *old)
{
    MYBPF_LOADER_NODE_S *new_node;
    int ret;

    new_node = _mybpf_loader_create_node(r, p);
    if (! new_node) {
        RETURN(BS_ERR);
    }

    ret = _mybpf_loader_load_simple(r, new_node, old, p);
    if (ret < 0) {
        _mybpf_loader_free_node(r, new_node);
        return ret;
    }

    return 0;
}

static int _mybpf_loader_load(MYBPF_RUNTIME_S *runtime, MYBPF_LOADER_PARAM_S *p)
{
    int ret;
    MYBPF_LOADER_NODE_S *old_node;

    ret = _mybpf_loader_check_helpers_exist(&p->simple_mem, p->tmp_helpers);
    if (ret < 0) {
        return ret;
    }

    old_node = _mybpf_loader_get_node(runtime, p->instance);
    if (old_node) {
        ret = _mybpf_loader_check_may_replace(runtime, p, old_node);
        if (ret < 0) {
            return ret;
        }
    }

    return _mybpf_loader_load_to_runtime(runtime, p, old_node);
}

static int _mybpf_loader_load_file(MYBPF_RUNTIME_S *runtime, MYBPF_LOADER_PARAM_S *p)
{
    int ret;

    ret = MYBPF_SIMPLE_OpenFile(p->filename, &p->simple_mem);
    if (ret < 0) {
        return ret;
    }

    ret = _mybpf_loader_load(runtime, p);

    MYBPF_SIMPLE_Close(&p->simple_mem);

    return ret;
}

int MYBPF_LoaderLoad(MYBPF_RUNTIME_S *runtime, MYBPF_LOADER_PARAM_S *p)
{
    if (! p->filename) {
        p->filename = "";
    }

    if (p->simple_mem.data) {
        return _mybpf_loader_load(runtime, p);
    } else {
        return _mybpf_loader_load_file(runtime, p);
    }
}

int MYBPF_DetachAll(MYBPF_RUNTIME_S *runtime, char *instance)
{
    MYBPF_LOADER_NODE_S *node;

    node = _mybpf_loader_get_node(runtime, instance);
    if (! node) {
        RETURN(BS_NO_SUCH);
    }

    _mybpf_loader_deattach_prog_all(runtime, node);

    return 0;
}

int MYBPF_AttachAuto(MYBPF_RUNTIME_S *runtime, char *instance)
{
    MYBPF_LOADER_NODE_S *node;

    node = _mybpf_loader_get_node(runtime, instance);
    if (! node) {
        RETURN(BS_NO_SUCH);
    }

    _mybpf_loader_auto_attach_prog(runtime, node);
    node->param.flag |= MYBPF_LOADER_FLAG_AUTO_ATTACH;

    return 0;
}

int MYBPF_LoaderUnload(MYBPF_RUNTIME_S *runtime, char *instance)
{
    MYBPF_LOADER_NODE_S *node;

    node = _mybpf_loader_get_node(runtime, instance);
    if (! node) {
        RETURN(BS_NOT_FOUND);
    }

    return _mybpf_loader_unload_node(runtime, node);
}

void MYBPF_LoaderUnloadAll(MYBPF_RUNTIME_S *runtime)
{
    MYBPF_LOADER_NODE_S *node;
    void *iter = NULL;

    while ((node = _mybpf_loader_get_next_node(runtime, &iter))) {
        _mybpf_loader_unload_node(runtime, node);
    }
}

MYBPF_LOADER_NODE_S * MYBPF_LoaderGet(MYBPF_RUNTIME_S *runtime, char *instance)
{
    return _mybpf_loader_get_node(runtime, instance);
}


MYBPF_LOADER_NODE_S * MYBPF_LoaderGetNext(MYBPF_RUNTIME_S *runtime, INOUT void **iter)
{
    MAP_ELE_S *ele;
    MYBPF_LOADER_NODE_S *node;

    ele = MAP_GetNextEle(runtime->loader_map, *iter);
    *iter = ele;
    if (! ele) {
        return NULL;
    }

    node = ele->pData;

    return node;
}

void MYBPF_LoaderShowMaps(MYBPF_RUNTIME_S *r, PF_PRINT_FUNC print_func)
{
    void *iter = NULL;
    MYBPF_LOADER_NODE_S *n;
    UMAP_HEADER_S *map;
    int i;

    while ((n = MYBPF_LoaderGetNext(r, &iter))) {

        for (i=0; i<n->map_count; i++) {
            map = n->maps[i];
            print_func("type:%s, flags:0x%x, key:%u, value:%u, max:%u, name:%s \r\n",
                    UMAP_TypeName(map->type), map->flags, map->size_key,
                    map->size_value, map->max_elem, map->map_name);
        }
    }
}

