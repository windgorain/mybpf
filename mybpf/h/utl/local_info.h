/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2009-1-7
* Description: 
* History:     
******************************************************************************/

#ifndef __LOCAL_INFO_H_
#define __LOCAL_INFO_H_

#include "utl/file_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 


PLUG_HIDE VOID LOCAL_INFO_SetHost(char *file);
PLUG_HIDE VOID LOCAL_INFO_SetConfPath(char *conf_path);
PLUG_HIDE VOID LOCAL_INFO_SetSavePath(char *save_path);
PLUG_HIDE CHAR * LOCAL_INFO_GetHostPath();
PLUG_HIDE CHAR * LOCAL_INFO_GetHostFileName();
PLUG_HIDE CHAR * LOCAL_INFO_GetHostFilePath();
PLUG_HIDE CHAR * LOCAL_INFO_GetConfPath();
PLUG_HIDE CHAR * LOCAL_INFO_GetSavePath();
PLUG_HIDE BS_STATUS LOCAL_INFO_ExpandToHostPath(CHAR *pszPath ,
    OUT CHAR szLocalPath[FILE_MAX_PATH_LEN + 1]);
PLUG_HIDE BS_STATUS LOCAL_INFO_ExpandToConfPath(CHAR *pszPath ,
        OUT CHAR szPath[FILE_MAX_PATH_LEN + 1]);
PLUG_HIDE BS_STATUS LOCAL_INFO_ExpandToSavePath(CHAR *pszPath ,
        OUT CHAR szPath[FILE_MAX_PATH_LEN + 1]);

#ifdef __cplusplus
    }
#endif 

#endif 


