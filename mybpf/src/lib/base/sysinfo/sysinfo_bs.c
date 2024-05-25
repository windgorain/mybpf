/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2007-11-3
* Description: 
* History:     
******************************************************************************/
#include "bs.h"

#include "utl/file_utl.h"
#include "utl/txt_utl.h"
#include "utl/exec_utl.h"
#include "utl/local_info.h"
#include "utl/sys_utl.h"
#include "utl/process_utl.h"


#define RETCODE_FILE_NUM RETCODE_FILE_NUM_SYSINFO

static UINT g_uiSysinfoArgc = 0;
static CHAR **g_ppcSysInfoArgv = NULL;
static CHAR g_szSysInfoInitWorkDir[FILE_MAX_PATH_LEN + 1] = ""; 
static CHAR g_szSysInfoConfDir[128] = "conf_dft"; 
static char g_szSysInfoSelfName[128];


CHAR * SYSINFO_GetExePath()
{
    return SYS_GetSelfFilePath();
}


CHAR *SYSINFO_GetInitWorkDir()
{
    return g_szSysInfoInitWorkDir;
}

void SYSINFO_SetInitWorkDir(char* dir)
{
	TXT_Strlcpy(g_szSysInfoInitWorkDir, dir, sizeof(g_szSysInfoInitWorkDir));
}

char* SYSINFO_ExpandWorkDir(OUT char* buf, int buf_size, char* file_path)
{
    scnprintf(buf, buf_size, "%s/%s", g_szSysInfoInitWorkDir, file_path);
	return buf;
}

void SYSINFO_SetConfDir(char *conf_dir)
{
    TXT_Strlcpy(g_szSysInfoConfDir, conf_dir, sizeof(g_szSysInfoConfDir));
}

char * SYSINFO_GetConfDir()
{
    return g_szSysInfoConfDir;
}

char *SYSINFO_ExpandConfPath(OUT char *buf, int buf_size, char *file_path)
{
    scnprintf(buf, buf_size, "%s/%s", g_szSysInfoConfDir, file_path);
    return buf;
}

char * SYSINFO_GetSlefName()
{
    return g_szSysInfoSelfName;
}

BS_STATUS SYSINFO_Show(IN UINT ulArgc, IN UCHAR **argv)
{
    CHAR szWorkDir[200] = "";
    char *v = "Release";

#ifdef IN_DEBUG
    v = "Debug";
#endif

    if (FILE_GET_CURRENT_DIRECTORY(szWorkDir, sizeof(szWorkDir)) == NULL) {
        szWorkDir[0] = '\0';
    }
    
    EXEC_OutInfo(
        "-------------------------------------------------------------\r\n"
        "Info: %s \r\n"
        "Compile Date: %s %s \r\n"
        "Git Head: %s \r\n"
        "Git Fetch Head: %s \r\n"
        "Init Work Directory: %s \r\n"
        "Work Directory: %s \r\n"
        "Exe Directory: %s \r\n"
        "Conf Directory: %s \r\n"
        "Self Name: %s \r\n"
        "PID: %d \r\n"
        "-------------------------------------------------------------\r\n",
        v, __DATE__, __TIME__, GIT_HEAD, GIT_FETCH_HEAD,
        g_szSysInfoInitWorkDir, szWorkDir, SYS_GetSelfFilePath(), g_szSysInfoConfDir,
        g_szSysInfoSelfName, PROCESS_GetPid());

    return BS_OK;
}

BS_STATUS SYSINFO_Init1()
{
    LOCAL_INFO_SetHost("lib/libbs.dll");
    LOCAL_INFO_SetConfPath(".");
    LOCAL_INFO_SetSavePath("./save");
    FUNCTBL_AddFunc(SYSINFO_GetExePath, FUNCTBL_RET_SEQUENCE, "");

    return BS_OK;
}

BS_STATUS SYSINFO_SetArgv(IN UINT uiArgc, IN CHAR **ppcArgv)
{
    g_uiSysinfoArgc = uiArgc;
    g_ppcSysInfoArgv = ppcArgv;

    char * name = FILE_GetFileNameFromPath(ppcArgv[0]);
    strlcpy(g_szSysInfoSelfName, name, sizeof(g_szSysInfoSelfName));

	return BS_OK;
}

static void sysinfo_init()
{
    if (FILE_GET_CURRENT_DIRECTORY(g_szSysInfoInitWorkDir, sizeof(g_szSysInfoInitWorkDir) - 1) == NULL) {
        g_szSysInfoInitWorkDir[0] = '\0';
    }
}

CONSTRUCTOR(init) {
    sysinfo_init();
}
