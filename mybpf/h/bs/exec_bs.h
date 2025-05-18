/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      lixingang  Version: 1.0  Date: 2007-7-21
* Description: 
* History:     
******************************************************************************/
#ifndef _EXEC_BS_H
#define _EXEC_BS_H
#ifdef __cplusplus
extern "C"
{
#endif

PLUG_API void EXEC_Attach(HANDLE hExec);
PLUG_API void EXEC_Detach(HANDLE hExec);
PLUG_API HANDLE EXEC_GetExec();

#ifdef __cplusplus
}
#endif
#endif 
