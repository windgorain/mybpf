/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      lixingang  Version: 1.0  Date: 2007-7-21
* Description: 
* History:     
******************************************************************************/
#ifndef _EXEC_UTL_H
#define _EXEC_UTL_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef VOID (*PF_EXEC_OUT_STRING_FUNC)(HANDLE hExec, CHAR *pszInfo);
typedef UCHAR (*PF_EXEC_GET_CHAR_FUNC)(HANDLE hExec);

HANDLE EXEC_Create(PF_EXEC_OUT_STRING_FUNC pfOutStringFunc,
        PF_EXEC_GET_CHAR_FUNC pfGetCharFunc);
BS_STATUS EXEC_Delete(IN HANDLE hExecHandle);
void EXEC_SetUD(HANDLE hExec, int index, HANDLE ud);
HANDLE EXEC_GetUD(HANDLE hExec, int index);
BS_STATUS EXEC_OutInfo(const char *fmt, ...);
BS_STATUS EXEC_OutString(IN CHAR *pszInfo);
void EXEC_OutDataHex(UCHAR *pucMem, int len);
void EXEC_OutErrCodeInfo();
VOID EXEC_Flush();
UCHAR EXEC_GetChar();

BS_STATUS EXEC_TM(IN UINT uiArgc, IN CHAR **pcArgv);
BS_STATUS EXEC_NoTM(IN UINT uiArgc, IN CHAR **pcArgv);

#ifdef __cplusplus
}
#endif
#endif 
