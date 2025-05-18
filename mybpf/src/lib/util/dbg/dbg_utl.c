/******************************************************************************
* Copyright (C), 2000-2006,  LiXingang
* Author:      LiXingang  Version: 1.0  Date: 2016-9-21
* Description: 
* History:     
******************************************************************************/
#include "bs.h"

#include "utl/dbg_utl.h"

static DBG_UTL_DEF_S * _dbg_utl_GetDef(IN DBG_UTL_CTRL_S *pstCtrl, IN UINT uiDbgID, IN UINT uiFlag)
{
    DBG_UTL_DEF_S *def = pstCtrl->debug_defs;

    for (; def->pcModuleName; def++) {
        if ((def->uiDbgID == uiDbgID) && (def->uiDbgFlag == uiFlag)) {
            return def;
        }
    }

    return NULL;
}

void DBG_UTL_SetDebugFlag(IN DBG_UTL_CTRL_S *pstCtrl, IN UINT uiModuleID, IN UINT uiFlag)
{
    pstCtrl->debug_flags[pstCtrl->debug_defs[uiModuleID].uiDbgID] |= uiFlag;
}

void DBG_UTL_ClrDebugFlag(IN DBG_UTL_CTRL_S *pstCtrl, IN UINT uiModuleID, IN UINT uiFlag)
{
    pstCtrl->debug_flags[pstCtrl->debug_defs[uiModuleID].uiDbgID] &= ~uiFlag;
}

void DBG_UTL_SetAllDebugFlags(DBG_UTL_CTRL_S *ctrl)
{
    for (int i=0; i<ctrl->max_module_id; i++) {
        ctrl->debug_flags[i] = 0xffffffff;
    }
}

void DBG_UTL_ClrAllDebugFlags(DBG_UTL_CTRL_S *ctrl)
{
    for (int i=0; i<ctrl->max_module_id; i++) {
        ctrl->debug_flags[i] = 0;
    }
}

void DBG_UTL_DebugCmd(IN DBG_UTL_CTRL_S *pstCtrl, IN CHAR *pcModuleName, IN CHAR *pcDbgName)
{
    BOOL_T bModuleWild = FALSE; 
    BOOL_T bFlagWild = FALSE;   
    DBG_UTL_DEF_S *def = pstCtrl->debug_defs;

    if (strcmp(pcModuleName, "all") == 0) {
        bModuleWild = TRUE;
    }

    if (strcmp(pcDbgName, "all") == 0) {
        bFlagWild = TRUE;
    }

    for (; def->pcModuleName; def++) {
        if ((! bModuleWild) && (strcmp(def->pcModuleName, pcModuleName) != 0)) {
            continue;
        }
        if ((! bFlagWild) && (strcmp(def->pcDbgFlagName, pcDbgName) != 0)) {
            continue;
        }
        pstCtrl->debug_flags[def->uiDbgID] |= def->uiDbgFlag;
    }
}

void DBG_UTL_NoDebugCmd(IN DBG_UTL_CTRL_S *pstCtrl, IN CHAR *pcModuleName, IN CHAR *pcDbgName)
{
    BOOL_T bModuleWild = FALSE; 
    BOOL_T bFlagWild = FALSE;   
    DBG_UTL_DEF_S *def = pstCtrl->debug_defs;

    if (strcmp(pcModuleName, "all") == 0) {
        bModuleWild = TRUE;
    }

    if (strcmp(pcDbgName, "all") == 0) {
        bFlagWild = TRUE;
    }

    for (; def->pcModuleName; def++) {
        if ((! bModuleWild) && (strcmp(def->pcModuleName, pcModuleName) != 0)) {
            continue;
        }

        if ((! bFlagWild) && (strcmp(def->pcDbgFlagName, pcDbgName) != 0)) {
            continue;
        }

        pstCtrl->debug_flags[def->uiDbgID] &= (~(def->uiDbgFlag));
    }
}

void DBG_UTL_OutputHeader(IN DBG_UTL_CTRL_S *pstCtrl, IN UINT uiDbgID, IN UINT uiFlag)
{
    DBG_UTL_DEF_S *pstDef;

    pstDef = _dbg_utl_GetDef(pstCtrl, uiDbgID, uiFlag);
    if (NULL == pstDef) {
        BS_DBGASSERT(0);
        return;
    }

    IC_Info(DBG_UTL_HEADER_FMT, pstCtrl->product_name, pstDef->pcModuleName, pstDef->pcDbgFlagName);
}


