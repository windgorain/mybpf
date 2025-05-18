/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2007-11-3
* Description: 
* History:     
******************************************************************************/

#ifndef __SYSINFO_BS_H_
#define __SYSINFO_BS_H_

#ifdef __cplusplus
    extern "C" {
#endif 

PLUG_API CHAR * SYSINFO_GetExePath();

PLUG_API BS_STATUS SYSINFO_Show(IN UINT ulArgc, IN UCHAR **argv);

BS_STATUS SYSINFO_Init1();
PLUG_API BS_STATUS SYSINFO_SetArgv(IN UINT uiArgc, IN CHAR **ppcArgv);
PLUG_API void SYSINFO_SetConfDir(char *conf_dir);
PLUG_API void SYSINFO_SetInitWorkDir(char* dir);
PLUG_API char* SYSINFO_ExpandWorkDir(OUT char* buf, int buf_size, char* file_path);
PLUG_API char * SYSINFO_GetConfDir();
PLUG_API char *SYSINFO_ExpandConfPath(OUT char *buf, int buf_size, char *file_path);
PLUG_API char * SYSINFO_GetSlefName();

#ifdef __cplusplus
    }
#endif 

#endif 


