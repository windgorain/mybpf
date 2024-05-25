/******************************************************************************
* Copyright (C), 2000-2006,  Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2016-9-21
* Description: 
* History:     
******************************************************************************/

#ifndef __DBG_UTL_H_
#define __DBG_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 


#define DBG_UTL_FLAG_PROCESS (1UL << 31)

#define DBG_UTL_HEADER_FMT "[%s:%s:%s] "

typedef struct
{
    const char *pcModuleName;
    const char *pcDbgFlagName;
    UINT uiDbgID;
    UINT uiDbgFlag;
}DBG_UTL_DEF_S;

typedef struct {
    const char *product_name; 
    UINT *debug_flags;
    DBG_UTL_DEF_S *debug_defs;
    int max_module_id;
}DBG_UTL_CTRL_S;

#define DBG_INIT_VALUE(_name, _flags, _defs, _max_id) \
    {.product_name=(_name), .debug_flags=(_flags), .debug_defs=(_defs), .max_module_id = (_max_id)}

#define DBG_UTL_IS_SWITCH_ON(_pstCtrl, _DbgID, _DbgFlag) ((_pstCtrl)->debug_flags[_DbgID] & (_DbgFlag))
    
#define DBG_UTL_OUTPUT(_pstCtrl, _DbgID, _DbgFlag, _fmt, ...) do {\
        if ((_pstCtrl)->debug_flags[_DbgID] & (_DbgFlag))  { \
            DBG_UTL_OutputHeader(_pstCtrl, _DbgID, _DbgFlag);   \
            IC_Info(_fmt, ##__VA_ARGS__) ;	\
        }   \
    }while(0)


void DBG_UTL_Init(DBG_UTL_CTRL_S *ctrl, const char *name, UINT *dbg_flags, DBG_UTL_DEF_S *dbg_defs, int max_module_id);
extern void DBG_UTL_SetDebugFlag(IN DBG_UTL_CTRL_S *pstCtrl, IN UINT uiModuleID, IN UINT uiFlag);
extern void DBG_UTL_ClrDebugFlag(IN DBG_UTL_CTRL_S *pstCtrl, IN UINT uiModuleID, IN UINT uiFlag);
void DBG_UTL_SetAllDebugFlags(DBG_UTL_CTRL_S *ctrl);
void DBG_UTL_ClrAllDebugFlags(DBG_UTL_CTRL_S *ctrl);
extern void DBG_UTL_DebugCmd(IN DBG_UTL_CTRL_S *pstCtrl, IN CHAR *pcModuleName, IN CHAR *pcDbgName);
extern void DBG_UTL_NoDebugCmd(IN DBG_UTL_CTRL_S *pstCtrl, IN CHAR *pcModuleName, IN CHAR *pcDbgName);
extern void DBG_UTL_OutputHeader(IN DBG_UTL_CTRL_S *pstCtrl, IN UINT uiDbgID, IN UINT uiFlag);

#ifdef __cplusplus
    }
#endif 

#endif 


