/******************************************************************************
* Copyright (C),    LiXingang
* Author:      LiXingang  Version: 1.0  Date: 2013-6-4
* Description: 
* History:     
******************************************************************************/
#include "bs.h"

#include "utl/sys_utl.h"
#include "utl/file_utl.h"
#include "utl/txt_utl.h"

#include "sys_os_utl.h"


#ifdef IN_LINUX

static char * _sys_os_get_self_file_path(OUT char *path, int size)
{
    int n;
    char *pcSplit;

    path[0] = '\0';

    n = readlink("/proc/self/exe", path, size);
    if (n < 0) {
        return NULL;
    }

    path[n] = '\0';

    pcSplit = strrchr(path, '/');
    if (NULL == pcSplit) {
        return NULL;
    }

    *pcSplit = '\0';
    
    return path;
}


char * _SYS_OS_GetSelfFileName(void)
{
    static CHAR szFileName[FILE_MAX_PATH_LEN + 1] = "";
    static BOOL_T bExist = FALSE;
    INT n;

    if (bExist == TRUE)
    {
        return szFileName;
    }

    n = readlink("/proc/self/exe", szFileName, FILE_MAX_PATH_LEN);
    if (n < 0)
    {
        return NULL;
    }

    szFileName[n] = '\0';
    bExist = TRUE;
    
    return szFileName;
}


CHAR * _SYS_OS_GetSelfFilePath(void)
{
    static CHAR szFilePath[FILE_MAX_PATH_LEN + 1] = "";
    static BOOL_T bExist = FALSE;

    if (bExist == TRUE) {
        return szFilePath;
    }

    _sys_os_get_self_file_path(szFilePath, sizeof(szFilePath));
    bExist = TRUE;
    
    return szFilePath;
}
#endif

#ifdef IN_MAC
#include <mach-o/dyld.h>


char * _SYS_OS_GetSelfFileName(void)
{
    static CHAR szFileName[FILE_MAX_PATH_LEN + 1] = "";
    static BOOL_T bExist = FALSE;
    UINT n = FILE_MAX_PATH_LEN;

    if (bExist == TRUE) {
        return szFileName;
    }

    _NSGetExecutablePath(szFileName, &n);
    
    return szFileName;
}


CHAR * _SYS_OS_GetSelfFilePath(void)
{
    static CHAR szFilePath[FILE_MAX_PATH_LEN + 1] = "";
    static BOOL_T bExist = FALSE;
    UINT n = FILE_MAX_PATH_LEN;
    CHAR *pcSplit;

    if (bExist == TRUE)
    {
        return szFilePath;
    }

    _NSGetExecutablePath(szFilePath, &n);

    pcSplit = strrchr(szFilePath, '/');
    if (NULL == pcSplit)
    {
        szFilePath[0] = '\0';
        return "";
    }

    *pcSplit = '\0';
    bExist = TRUE;
    
    return szFilePath;
}
#endif

