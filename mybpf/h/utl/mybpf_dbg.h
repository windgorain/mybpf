/*********************************************************
*   Copyright (C) LiXingang
*   Description: 
*
********************************************************/
#ifndef _MYBPF_DBG_H
#define _MYBPF_DBG_H
#include "utl/dbg_utl.h"
#ifdef __cplusplus
extern "C"
{
#endif

enum {
    MYBPF_DBG_ID_LOADER = 0,
    MYBPF_DBG_ID_ELF,
    MYBPF_DBG_ID_VM,
    MYBPF_DBG_ID_RELO,
    MYBPF_DBG_ID_JIT,

    MYBPF_DBG_ID_MAX
};

#define MYBPF_DBG_FLAG_LOADER_PROGS   0x1

extern DBG_UTL_CTRL_S g_mybpf_runtime_dbg;

static inline void MYBPF_DBG_SetDebugFlag(UINT module_id, UINT flags)
{
    DBG_UTL_SetDebugFlag(&g_mybpf_runtime_dbg, module_id, flags);
}

static inline void MYBPF_DBG_ClrDebugFlag(UINT module_id, UINT flags)
{
    DBG_UTL_ClrDebugFlag(&g_mybpf_runtime_dbg, module_id, flags);
}

static inline void MYBPF_DBG_SetAllDebugFlags()
{
    DBG_UTL_SetAllDebugFlags(&g_mybpf_runtime_dbg);
}

static inline void MYBPF_DBG_ClrAllDebugFlags()
{
    DBG_UTL_ClrAllDebugFlags(&g_mybpf_runtime_dbg);
}

#ifdef IN_ULC_USER
#define MYBPF_DBG_IS_SWITCH_ON(...) 0
#define MYBPF_DBG_OUTPUT(...)
#else
#define MYBPF_DBG_IS_SWITCH_ON(_dbgid, _dbgflag) DBG_UTL_IS_SWITCH_ON(&g_mybpf_runtime_dbg, _dbgid, _dbgflag)
#define MYBPF_DBG_OUTPUT(_dbgid, _dbgflag, _fmt, ...) DBG_UTL_OUTPUT(&g_mybpf_runtime_dbg, _dbgid, _dbgflag, _fmt, ##__VA_ARGS__)
#endif

#ifdef __cplusplus
}
#endif
#endif 
