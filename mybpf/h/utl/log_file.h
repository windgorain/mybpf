/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2008-4-15
* Description: 
* History:     
******************************************************************************/

#ifndef __LogFile_UTL_H_
#define __LogFile_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

HANDLE LogFile_Open(IN CHAR *pszFileName);
VOID LogFile_Close(IN HANDLE hLogHandle);
void LogFile_SetCapacity(HANDLE hLogHandle, UINT64 capacity);
VOID LogFile_OutString(IN HANDLE hLogHandle, IN CHAR *pszLogFmt, ...);
VOID LogFile_OutStringByValist(IN HANDLE hLogHandle, IN CHAR *pszLogFmt, IN va_list args);

#ifdef __cplusplus
    }
#endif 

#endif 


