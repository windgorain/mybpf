/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2007-7-21
* Description: 
* History:     
******************************************************************************/
#include "bs.h"

#include "utl/exec_utl.h"
#include "utl/txt_utl.h"
#include "utl/buffer_utl.h"
#include "utl/thread_utl.h"
#include "utl/thread_named.h"

#define EXEC_UD_MAX 4


typedef struct
{
    
    PF_EXEC_OUT_STRING_FUNC pfOutFunc;  
    PF_EXEC_GET_CHAR_FUNC pfGetCharFunc;
    HANDLE uds[EXEC_UD_MAX];

    
    IC_HANDLE     hIcHandle;
    BUFFER_S buffer;
}EXEC_S;

static EXEC_S *g_exec_local = NULL;

static BS_STATUS exec_sendData(EXEC_S *exec, CHAR *pszInfo)
{
    if ((exec) && (exec->pfOutFunc)) {
        exec->pfOutFunc(exec, pszInfo);
    }

	return BS_OK;
}

static VOID exec_BufferWrite(void *pcData, UINT uiLen, USER_HANDLE_S *ud)
{
    char *str = pcData;

    str[uiLen] = '\0';
    exec_sendData(ud->ahUserHandle[0], str);
}

static void exec_Output(EXEC_S *exec, char *info, int len)
{
    BUFFER_Write(&exec->buffer, info, len);
}

static void exec_Flush(EXEC_S *exec)
{
    BUFFER_Flush(&exec->buffer);
}

static void exec_OutString(char *info, void *user_data)
{
    EXEC_OutString(info);
}

static VOID exec_Tm(IN CHAR *pcMsg, IN USER_HANDLE_S *ud)
{
    EXEC_S *exec = ud->ahUserHandle[0];
    exec_Output(exec, pcMsg, strlen(pcMsg));
    exec_Flush(exec);
}

HANDLE EXEC_Create(PF_EXEC_OUT_STRING_FUNC pfOutStringFunc,
        PF_EXEC_GET_CHAR_FUNC pfGetCharFunc)
{
    EXEC_S *exec = NULL;
    USER_HANDLE_S ud;

    exec = MEM_ZMalloc(sizeof(EXEC_S));
    if (! exec) {
        return NULL;
    }

    ud.ahUserHandle[0] = exec;
    BUFFER_Init(&exec->buffer);
    BUFFER_SetOutputFunc(&exec->buffer, exec_BufferWrite, &ud);

    if (0 != BUFFER_AllocBuf(&exec->buffer, 511)) {
        MEM_Free(exec);
        return NULL;
    }

    exec->pfOutFunc = pfOutStringFunc;
    exec->pfGetCharFunc = pfGetCharFunc;

    return exec;
}

BS_STATUS EXEC_Delete(HANDLE hExec)
{
    EXEC_S *exec = hExec;
    
    if (! exec) {
        RETURN(BS_NULL_PARA);
    }

    if (g_exec_local == hExec) {
        g_exec_local = NULL;
    }

    if (hExec == EXEC_GetExec()) {
        EXEC_Attach(NULL);
    }

    if (exec->hIcHandle) {
        IC_UnReg(exec->hIcHandle);
        exec->hIcHandle = NULL;
    }

    BUFFER_Fini(&exec->buffer);
    MEM_Free(exec);

    return BS_OK;
}

void EXEC_SetUD(HANDLE hExec, int index, HANDLE ud)
{
    EXEC_S *exec = hExec;

    if (index >= EXEC_UD_MAX) {
        BS_WARNNING(("Out of range"));
        return;
    }
    exec->uds[index] = ud;
}

HANDLE EXEC_GetUD(HANDLE hExec, int index)
{
    EXEC_S *exec = hExec;

    if (index >= EXEC_UD_MAX) {
        BS_WARNNING(("Out of range"));
        return NULL;
    }
    return exec->uds[index];
}

BS_STATUS EXEC_OutString(IN CHAR *pszInfo)
{
    EXEC_S *exec = EXEC_GetExec();
    
    if (! exec) {
        
        exec_sendData(NULL, pszInfo);
        return BS_OK;
    }

    exec_Output(exec, pszInfo, strlen(pszInfo));

    return BS_OK;
}

BS_STATUS EXEC_OutInfo(const char *fmt, ...)
{
    TXT_ARGS_PRINT(exec_OutString, NULL);
    return BS_OK;
}

VOID EXEC_Flush()
{
    EXEC_S *exec = EXEC_GetExec();

    if (exec) {
        exec_Flush(exec);
    }
}

UCHAR EXEC_GetChar()
{
    EXEC_S *exec = EXEC_GetExec();
    
    if ((NULL != exec) && (NULL != exec->pfGetCharFunc)) {
        return exec->pfGetCharFunc(exec);
    }

    return 0;
}


BS_STATUS EXEC_TM(IN UINT uiArgc, IN CHAR **pcArgv)
{
    EXEC_S *exec;
    USER_HANDLE_S ud;

    exec = EXEC_GetExec();
    if (! exec) {
        RETURN(BS_ERR);
    }

    ud.ahUserHandle[0] = exec;
    exec->hIcHandle = IC_Reg(exec_Tm, &ud, 0xffffffff);
    if (! exec->hIcHandle) {
        RETURN(BS_NO_MEMORY);
    }

    return BS_OK;
}


BS_STATUS EXEC_NoTM(IN UINT uiArgc, IN CHAR **pcArgv)
{
    EXEC_S *exec;

    exec = EXEC_GetExec();
    if (NULL == exec) {
        RETURN(BS_ERR);
    }

    if (exec->hIcHandle != NULL) {
        IC_UnReg(exec->hIcHandle);
        exec->hIcHandle = NULL;
    }

    return BS_OK;
}

static void exec_print_data(const char *str, ...)
{
    EXEC_OutString((void*)str);
}

void EXEC_OutDataHex(UCHAR *pucMem, int len)
{
    MEM_Print(pucMem, len, exec_print_data);
}


void EXEC_OutErrCodeInfo()
{
    char info[1024];
    EXEC_OutInfo("%s \r\n", ErrCode_Build(info, sizeof(info)));
}

