/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2007-12-8
* Description: 
* History:     
******************************************************************************/

#ifndef __FILE_LOCK_H_
#define __FILE_LOCK_H_

#ifdef __cplusplus
    extern "C" {
#endif 

BS_STATUS FM_Init();


PLUG_API BS_STATUS FM_LockPath(IN CHAR *pszFilePath, IN BOOL_T bIsDir, OUT HANDLE *phFmLockId);
PLUG_API BS_STATUS FM_UnLockPath(IN HANDLE hFmLockId);


#ifdef __cplusplus
    }
#endif 

#endif


