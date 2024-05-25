/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2008-4-9
* Description: 
* History:     
******************************************************************************/

#ifndef __PROCESS_UTL_H_
#define __PROCESS_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

#define PROCESS_FLAG_WAIT_FOR_OVER      0x1 
#define PROCESS_FLAG_SUSPEND            0x2 
#define PROCESS_FLAG_HIDE               0x4 

LONG PROCESS_CreateByFile
(
    IN CHAR *pcFilePath,
    IN CHAR *pcParam,
    IN UINT uiFlag
);
BS_STATUS PROCESS_Resume(IN LONG lProcessID);
BS_STATUS PROCESS_SuspendMainThread(IN LONG lProcessID);

HANDLE PROCESS_GetProcess(IN LONG lProcessID);

BOOL_T PROCESS_IsPidExist(IN UINT pid);
BOOL_T PROCESS_IsProcessNameExist(IN char *process);
int PROCESS_RenameSelf(IN char *new_name);
UINT PROCESS_GetPid();
UINT64 PROCESS_GetTid();

#ifdef __cplusplus
    }
#endif 

#endif 


