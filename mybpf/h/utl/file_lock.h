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

#if 1

BS_STATUS FM_Init();
BS_STATUS FM_LockPath(IN CHAR *pszFilePath, IN BOOL_T bIsDir, OUT HANDLE *phFmLockId);
BS_STATUS FM_UnLockPath(IN HANDLE hFmLockId);
#endif

#if 1

#ifdef IN_WINDOWS
#define open(a,b,c) _open(a,b,c)
#define O_CREAT _O_CREAT
#define O_RDWR _O_RDWR
#define LOCK_EX 1
#define LOCK_NB 2
#endif

int FILE_LockFD(int fd, int operation);
int FILE_UnlockFD(int fd);
int FILE_LockFile(char *filename);

#endif


#ifdef __cplusplus
    }
#endif 

#endif


