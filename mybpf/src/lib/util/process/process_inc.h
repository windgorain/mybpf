/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2014-3-26
* Description: 
* History:     
******************************************************************************/

#ifndef __PROCESS_INC_H_
#define __PROCESS_INC_H_

#ifdef __cplusplus
    extern "C" {
#endif 

LONG _OS_PROCESS_CreateByFile
(
    IN CHAR *pcFilePath,
    IN CHAR *pcParam,
    IN UINT uiFlag
);

BS_STATUS _OS_PROCESS_Resume(IN LONG lID);
BS_STATUS _OS_PROCESS_SuspendMainThread(IN LONG lID);
HANDLE _OS_PROCESS_GetProcess(IN LONG lId);
BOOL_T _OS_PROCESS_IsPidExist(IN UINT pid);
BOOL_T _OS_PROCESS_IsProcessNameExist(IN char *process);
int _OS_PROCESS_RenameSelf(IN char *new_name);
UINT _OS_PROCESS_GetPid();
UINT64 _OS_PROCESS_GetTid();

#ifdef __cplusplus
    }
#endif 

#endif 


