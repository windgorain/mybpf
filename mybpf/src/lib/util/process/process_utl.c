/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2008-4-9
* Description: 
* History:     
******************************************************************************/
#include "bs.h"
#include "sys/file.h"

#include "process_inc.h"

LONG PROCESS_CreateByFile
(
    IN CHAR *pcFilePath,
    IN CHAR *pcParam,
    IN UINT uiFlag
)
{
    return _OS_PROCESS_CreateByFile(pcFilePath, pcParam, uiFlag);
}

BS_STATUS PROCESS_Resume(IN LONG lProcessID)
{
    if (0 == lProcessID)
    {
        return (BS_BAD_PARA);
    }
    
    return _OS_PROCESS_Resume(lProcessID);
}

BS_STATUS PROCESS_SuspendMainThread(IN LONG lProcessID)
{
    if (0 == lProcessID)
    {
        return (BS_BAD_PARA);
    }
    
    return _OS_PROCESS_SuspendMainThread(lProcessID);
}


HANDLE PROCESS_GetProcess(IN LONG lProcessID)
{
    return _OS_PROCESS_GetProcess(lProcessID);
}


BOOL_T PROCESS_IsPidExist(IN UINT pid)
{
    return _OS_PROCESS_IsPidExist(pid);
}

BOOL_T PROCESS_IsProcessNameExist(IN char *process)
{
    return _OS_PROCESS_IsProcessNameExist(process);
}

int PROCESS_RenameSelf(IN char *new_name)
{
    return _OS_PROCESS_RenameSelf(new_name);
}


UINT PROCESS_GetPid()
{
    return _OS_PROCESS_GetPid();
}


UINT64 PROCESS_GetTid()
{
    return _OS_PROCESS_GetTid();
}


