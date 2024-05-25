/*
* Copyright (C), Xingang.Li
*
* File name     : ic.c
* Date          : 2005年3月27日
* Description   : 
*/

#include "bs.h"

#include "utl/sprintf_utl.h"
#include "utl/txt_utl.h"
#include "utl/log_file.h"
#include "utl/log_file.h"
#include "utl/mutex_utl.h"
#include "utl/atomic_utl.h"

#define BS_PRINTF_MAX_LEN    2048
#define _IC_MAX_OB 1


typedef struct {
    UINT used:1;
    UINT events;
    PF_IC_PRINT_FUNC pfFunc;
    USER_HANDLE_S stUserHandle;
}_IC_REG_NODE_S;


static DLL_HEAD_S  g_stIcRegList = DLL_HEAD_INIT_VALUE(&g_stIcRegList);
static MUTEX_S g_stIcMutex;
static _IC_REG_NODE_S g_ic_obs[_IC_MAX_OB];

void IC_OutString(UINT event, char *msg)
{
    _IC_REG_NODE_S *pstNode;
    int i;

    for (i=0; i<_IC_MAX_OB; i++) {
        pstNode = &g_ic_obs[i];
        if (pstNode->used && pstNode->pfFunc && (event & pstNode->events)) {
            pstNode->pfFunc(msg, &pstNode->stUserHandle);
        }
    }

#ifndef USE_BS
    
    PRINT_COLOR(SHELL_FONT_COLOR_GREEN, "%s", msg);
#endif

    return;
}

static _IC_REG_NODE_S * _ic_alloc_regnode()
{
    int i;
    _IC_REG_NODE_S *found = NULL;

    for (i=0; i<_IC_MAX_OB; i++) {
        if (! g_ic_obs[i].used) {
            found = &g_ic_obs[i];
            break;
        }
    }


    return found;
}

IC_HANDLE IC_Reg(PF_IC_PRINT_FUNC pfFunc, USER_HANDLE_S *pstUserHandle, UINT events)
{
    _IC_REG_NODE_S *pstNode;

    BS_DBGASSERT(NULL != pfFunc);

    MUTEX_P(&g_stIcMutex);

    pstNode = _ic_alloc_regnode();
    if (pstNode) {
        pstNode->pfFunc = pfFunc;
        pstNode->events = events;
        if (NULL != pstUserHandle) {
            pstNode->stUserHandle = *pstUserHandle;
        }
        ATOM_BARRIER();
        pstNode->used = 1;
    }

    MUTEX_V(&g_stIcMutex);

    return pstNode;
}

VOID IC_UnReg(IN IC_HANDLE hIcHandle)
{
    _IC_REG_NODE_S *pstNode = hIcHandle;

    if (pstNode) {
        pstNode->used = 0;
    }

    return;
}

void IC_SetEvents(IC_HANDLE hIcHandle, UINT events)
{
    _IC_REG_NODE_S *pstNode = hIcHandle;
    pstNode->events = events;
}

void IC_Print(UINT uiLevel, char *fmt, ...)
{
    char buf[BS_PRINTF_MAX_LEN];
    TXT_ARGS_BUILD(buf, sizeof(buf));
    IC_OutString(uiLevel, buf);
}

int IC_GetLogEvent(char *event) 
{
    if (!event) return 0;
    
    if (stricmp(event, "fatal") == 0) return IC_LEVEL_FATAL;
    if (stricmp(event, "err") == 0) return IC_LEVEL_ERR;
    if (stricmp(event, "warn") == 0) return IC_LEVEL_WARNING;
    if (stricmp(event, "fail") == 0) return IC_LEVEL_FAIL;
    if (stricmp(event, "info") == 0) return IC_LEVEL_INFO;
    if (stricmp(event, "debug") == 0) return IC_LEVEL_DBG;
    if (stricmp(event, "all") == 0) return 0xffffffff;

    return 0;
}

int IC_GetLogEventStr(UINT events, OUT char **strs)
{
    int i=0;

    if (events & IC_LEVEL_FATAL) {
        strs[i] = "fatal";
        i++;
    }
    if (events & IC_LEVEL_ERR) {
        strs[i] = "err";
        i++;
    }
    if (events & IC_LEVEL_WARNING) {
        strs[i] = "warn";
        i++;
    }
    if (events & IC_LEVEL_FAIL) {
        strs[i] = "fail";
        i++;
    }
    if (events & IC_LEVEL_INFO) {
        strs[i] = "info";
        i++;
    }
    if (events & IC_LEVEL_DBG) {
        strs[i] = "debug";
        i++;
    }

    return i;
}

static void icutl_Init()
{
    MUTEX_Init(&g_stIcMutex);
}

CONSTRUCTOR(init) {
    icutl_Init();
}

