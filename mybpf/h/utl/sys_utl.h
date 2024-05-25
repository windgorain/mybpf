/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2008-10-23
* Description: 
* History:     
******************************************************************************/

#ifndef __SYS_UTL_H_
#define __SYS_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

typedef enum
{
    SYS_OS_VER_OTHER,
    SYS_OS_VER_WIN_OLD,  
    SYS_OS_VER_WIN2000,
    SYS_OS_VER_WINXP,
    SYS_OS_VER_WIN_SERVER2003,
    SYS_OS_VER_WIN_VISTA,
    SYS_OS_VER_WIN7,
    SYS_OS_VER_WIN8,
    SYS_OS_VER_WIN8_1,

    SYS_OS_VER_WIN_LATTER 
}SYS_OS_VER_E;

typedef enum
{
    SYS_OS_BIT_32,  
    SYS_OS_BIT_64,  

    SYS_OS_BIT_UNKNOWN,  
}SYS_OS_BIT_E;

BOOL_T SYS_IsInstanceExist(IN VOID *pszName);
SYS_OS_VER_E SYS_GetOsVer(void);
SYS_OS_BIT_E SYS_GetOsBit(void);
CHAR * SYS_GetSelfFileName(void);

CHAR * SYS_GetSelfFilePath(void);

BS_STATUS SYS_SetSelfStart(IN CHAR *pcRegName, IN BOOL_T bSelfStart, IN char *arg);
void ShowCmdWin(int show);

#ifdef __cplusplus
    }
#endif 

#endif 


