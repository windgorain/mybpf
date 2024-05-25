/******************************************************************************
* Copyright (C),    LiXingang
* Author:      LiXingang  Version: 1.0  Date: 2014-3-25
* Description: 
* History:     
******************************************************************************/
#include "bs.h"
#include "utl/txt_utl.h"

#ifdef IN_LINUX
#include <sys/prctl.h>
#endif

#ifdef IN_UNIXLIKE

#include <sys/syscall.h>
#include "process_inc.h"

#define _PROCESS_LINUX_MAX_ARGC 120

LONG _OS_PROCESS_CreateByFile
(
    IN CHAR *pcFilePath,
    IN CHAR *pcParam,
    IN UINT uiFlag
)
{
    pid_t pid;
    CHAR *apcArgv[_PROCESS_LINUX_MAX_ARGC + 2];
    UINT uiCount;

    apcArgv[0] = pcFilePath;
    uiCount = TXT_StrToToken(pcParam, " ", apcArgv + 1, _PROCESS_LINUX_MAX_ARGC);
    apcArgv[uiCount] = NULL;

    pid = fork();
    if (pid == 0)
    {
        execv(pcFilePath, apcArgv);
        exit(1);
        return 0;
    }

    return pid;
}

BS_STATUS _OS_PROCESS_Resume(IN LONG lPid)
{
    return BS_NOT_SUPPORT;
}

BS_STATUS _OS_PROCESS_SuspendMainThread(IN LONG lPid)
{
    return BS_NOT_SUPPORT;
}

HANDLE _OS_PROCESS_GetProcess(IN LONG lId)
{
    return NULL; 
}

BOOL_T _OS_PROCESS_IsPidExist(IN UINT pid)
{
    FILE *ptr;
    char buff[512];
    char ps[128];

    sprintf(ps,"ps -p %d", pid);
    strcpy(buff,"ABNORMAL");

    if((ptr=popen(ps, "r")) == NULL) {
        return FALSE;
    }

    if (NULL == fgets(buff, sizeof(buff), ptr)) {
        pclose(ptr);
        return FALSE;
    }

    if(strcmp(buff,"ABNORMAL")==0) {  
        pclose(ptr);
        return FALSE;
    }
    
    pclose(ptr);

    return TRUE;
}

BOOL_T _OS_PROCESS_IsProcessNameExist(IN char *process)
{
    FILE *ptr;
    char buff[512];
    char ps[128];

    sprintf(ps,"ps -e | grep %s | grep -v grep", process);
    strcpy(buff,"ABNORMAL");

    if((ptr=popen(ps, "r")) == NULL) {
        return FALSE;
    }

    if (NULL == fgets(buff, sizeof(buff), ptr)) {
        pclose(ptr);
        return FALSE;
    }

    printf("%s\r\n", buff);

    if(strcmp(buff,"ABNORMAL")==0) {  
        pclose(ptr);
        return FALSE;
    }
    
    pclose(ptr);

    return TRUE;
}

int _OS_PROCESS_RenameSelf(IN char *new_name)
{
#ifdef IN_LINUX
    prctl(PR_SET_NAME, new_name, NULL, NULL, NULL);
#endif

    return 0;
}

UINT _OS_PROCESS_GetPid()
{
    return getpid();
}

UINT64 _OS_PROCESS_GetTid()
{
#if 0
    return syscall(__NR_gettid);
#endif
    return (UINT64)(ULONG)pthread_self();
}

#endif

