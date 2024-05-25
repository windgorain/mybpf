/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2009-1-6
* Description: 
* History:     
******************************************************************************/
#include "bs.h"

#include "utl/file_utl.h"
#include "utl/txt_utl.h"

static CHAR g_szLocalPath[FILE_MAX_PATH_LEN + 1] = ""; 

static CHAR g_szLocalFileName[FILE_MAX_PATH_LEN + 1] = "";

static CHAR g_szLocalFilePath[FILE_MAX_PATH_LEN + 1] = "";

static CHAR g_szLocalConfPath[FILE_MAX_PATH_LEN + 1] = "";

static CHAR g_szLocalSavePath[FILE_MAX_PATH_LEN + 1] = "";

PLUG_HIDE VOID LOCAL_INFO_SetConfPath(char *conf_path)
{
    if (TRUE == FILE_IsAbsolutePath(conf_path)) {
        TXT_Strlcpy(g_szLocalConfPath, conf_path, sizeof(g_szLocalConfPath));
    } else {
        char *pszSysPath = SYSINFO_GetExePath();
        if (NULL == pszSysPath) {
            return;
        }
        if (strlen(conf_path) + strlen(pszSysPath) + 1 > FILE_MAX_PATH_LEN) {
            BS_DBGASSERT(0);
            return;
        }
        sprintf(g_szLocalConfPath, "%s/%s", pszSysPath, conf_path);

    }
}

PLUG_HIDE VOID LOCAL_INFO_SetSavePath(char *save_path)
{
    if (TRUE == FILE_IsAbsolutePath(save_path)) {
        TXT_Strlcpy(g_szLocalSavePath, save_path, sizeof(g_szLocalSavePath));
    } else {
        char *pszSysPath = SYSINFO_GetExePath();
        if (NULL == pszSysPath) {
            return;
        }
        if (strlen(save_path) + strlen(pszSysPath) + 1 > FILE_MAX_PATH_LEN) {
            BS_DBGASSERT(0);
            return;
        }
        sprintf(g_szLocalSavePath, "%s/%s", pszSysPath, save_path);

    }
}


PLUG_HIDE VOID LOCAL_INFO_SetHost(char *file)
{
    CHAR *pszSysPath;
    CHAR *pszFileName;
    UINT ulLen;
    
    if (strlen(file) > FILE_MAX_PATH_LEN) {
        BS_DBGASSERT(0);
        return;
    }
 
    if (TRUE == FILE_IsAbsolutePath(file)) {
        TXT_Strlcpy(g_szLocalFilePath, file, sizeof(g_szLocalFilePath));
    } else {
        pszSysPath = SYSINFO_GetExePath();
        if (NULL == pszSysPath) {
            return;
        }
        if (strlen(file) + strlen(pszSysPath) + 1 > FILE_MAX_PATH_LEN) {
            BS_DBGASSERT(0);
            return;
        }
        sprintf(g_szLocalFilePath, "%s/%s", pszSysPath, file);
    }

    pszFileName = FILE_GetFileNameFromPath(g_szLocalFilePath);
    if (NULL != pszFileName) {
        TXT_Strlcpy(g_szLocalFileName, pszFileName, sizeof(g_szLocalFileName));
    }

    FILE_GetPathFromFilePath(g_szLocalFilePath, g_szLocalPath);

    
    ulLen = strlen(g_szLocalPath);
    if (ulLen > 0) {
        g_szLocalPath[ulLen - 1] = '\0';
    }

    FILE_PATH_TO_HOST(g_szLocalPath);
    FILE_PATH_TO_HOST(g_szLocalFilePath);

    return;
}


PLUG_HIDE CHAR * LOCAL_INFO_GetHostPath()
{
    return g_szLocalPath;
}


PLUG_HIDE CHAR * LOCAL_INFO_GetHostFileName()
{
    return g_szLocalFileName;
}


PLUG_HIDE CHAR * LOCAL_INFO_GetHostFilePath()
{
    return g_szLocalFilePath;
}

PLUG_HIDE CHAR * LOCAL_INFO_GetConfPath()
{
    return g_szLocalConfPath;
}

PLUG_HIDE CHAR * LOCAL_INFO_GetSavePath()
{
    return g_szLocalSavePath;
}

PLUG_HIDE BS_STATUS LOCAL_INFO_ExpandToHostPath(CHAR *pszPath ,
        OUT CHAR szLocalPath[FILE_MAX_PATH_LEN + 1])
{
    CHAR *pszHostPath;

    if (NULL == pszPath) {
        return BS_NULL_PARA;
    }

    pszHostPath = LOCAL_INFO_GetHostPath();
    if (strlen(pszHostPath) + strlen(pszPath) + 1 > FILE_MAX_PATH_LEN) {
        BS_DBGASSERT(0);
        return BS_ERR;
    }

    if (SNPRINTF(szLocalPath, FILE_MAX_PATH_LEN + 1, "%s/%s", pszHostPath, pszPath) < 0) {
        RETURN(BS_ERR);
    }

    return BS_OK;
}

PLUG_HIDE BS_STATUS LOCAL_INFO_ExpandToConfPath(CHAR *pszPath ,
        OUT CHAR szPath[FILE_MAX_PATH_LEN + 1])
{
    CHAR *pCfgPath;

    if (NULL == pszPath) {
        return BS_NULL_PARA;
    }

    pCfgPath = LOCAL_INFO_GetConfPath();
    if (strlen(pszPath) + strlen(pCfgPath) + 1 > FILE_MAX_PATH_LEN) {
        BS_DBGASSERT(0);
        return BS_ERR;
    }

    if (SNPRINTF(szPath, FILE_MAX_PATH_LEN + 1, "%s/%s", pCfgPath, pszPath) < 0) {
        RETURN(BS_ERR);
    }

    return BS_OK;
}

PLUG_HIDE BS_STATUS LOCAL_INFO_ExpandToSavePath(CHAR *pszPath ,
        OUT CHAR szPath[FILE_MAX_PATH_LEN + 1])
{
    CHAR *path;

    if (NULL == pszPath) {
        return BS_NULL_PARA;
    }

    path = LOCAL_INFO_GetSavePath();
    if (strlen(pszPath) + strlen(path) + 1 > FILE_MAX_PATH_LEN) {
        BS_DBGASSERT(0);
        return BS_ERR;
    }

    if (SNPRINTF(szPath, FILE_MAX_PATH_LEN+1, "%s/%s", path, pszPath) < 0) {
        RETURN(BS_ERR);
    }

    return BS_OK;
}

