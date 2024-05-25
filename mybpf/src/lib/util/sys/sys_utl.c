/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2008-10-23
* Description: 
* History:     
******************************************************************************/
#include "bs.h"

#include "utl/sys_utl.h"
#include "utl/file_utl.h"
#include "utl/txt_utl.h"

#include "sys_os_utl.h"

BOOL_T SYS_IsInstanceExist(IN VOID *pszName)
{
#ifdef IN_WINDOWS
	HANDLE hMutex;
    hMutex = CreateMutex(FALSE, FALSE, pszName);
    if(hMutex != NULL)
	{
		DWORD err=GetLastError();
        if(err==ERROR_ALREADY_EXISTS) { 
            CloseHandle(hMutex);
			return TRUE;
		}
	}
#endif
	return FALSE;
}

#ifdef IN_WINDOWS
static SYS_OS_VER_E sys_GetOsWinVer()
{
    SYS_OS_VER_E eVer;
    OSVERSIONINFO osvi;

    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

    if (0 == GetVersionEx(&osvi))
    {
        return SYS_OS_VER_WIN_OLD;
    }    

    switch (osvi.dwMajorVersion)
    {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        {
            eVer = SYS_OS_VER_WIN_OLD;
            break;
        }

        case 5:
        {
            switch(osvi.dwMinorVersion)
            {  
                case 0:
                {
                    eVer = SYS_OS_VER_WIN2000;
                    break;
                }
                case 1:
                {
                    eVer = SYS_OS_VER_WINXP;
                    break;  
                }
                case 2:
                {
                    eVer = SYS_OS_VER_WIN_SERVER2003;
                    break;
                }
                default:
                {
                    eVer = SYS_OS_VER_WIN_SERVER2003;
                    break;
                }
            }

            break;
        }

        case 6:
        {
            switch(osvi.dwMinorVersion)
            {
                case 0:
                {
                    eVer = SYS_OS_VER_WIN_VISTA;
                    break;
                }

                case 1:
                {
                    eVer = SYS_OS_VER_WIN7;
                    break;
                }

                case 2:
                {
                    eVer = SYS_OS_VER_WIN8;
                    break;
                }

                case 3:
                {
                    eVer = SYS_OS_VER_WIN8_1;
                    break;
                }

                default:
                {
                    eVer = SYS_OS_VER_WIN_LATTER;
                    break;
                }
            }

            break;
        }

        default:
        {
            eVer = SYS_OS_VER_WIN_LATTER;
            break;
        }
    }  
    
    return eVer;
}  

static SYS_OS_BIT_E sys_GetOsWinBits(void)
{
    SYSTEM_INFO si;

    GetNativeSystemInfo(&si);
    if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
    {
        return SYS_OS_BIT_64;
    }

    if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
    {
        return SYS_OS_BIT_UNKNOWN;
    }

    return SYS_OS_BIT_32;
}

#endif

SYS_OS_VER_E SYS_GetOsVer(void)
{
    SYS_OS_VER_E eVer = SYS_OS_VER_OTHER;

#ifdef IN_WINDOWS
    eVer = sys_GetOsWinVer();
#endif

    return eVer;
}

#ifdef IN_WINDOWS
SYS_OS_BIT_E SYS_GetOsBit(void)
{
    return sys_GetOsWinBits();
}
#endif

#ifdef IN_WINDOWS

VOID SYS_HideProcess(void)
{
    HINSTANCE hInst = LoadLibrary("KERNEL32.DLL"); 
	if(hInst) 
	{            
		typedef DWORD (WINAPI *MYFUNC)(DWORD,DWORD);          
		MYFUNC RegisterServiceProcessFun = NULL;     
		RegisterServiceProcessFun = (MYFUNC)GetProcAddress(hInst, "RegisterServiceProcess");
		if(RegisterServiceProcessFun)     
		{             
			RegisterServiceProcessFun(GetCurrentProcessId(),1);     
		}     
		FreeLibrary(hInst); 
	}
}
#endif


BS_STATUS SYS_SetSelfStart(IN CHAR *pcRegName, IN BOOL_T bSelfStart, IN char *arg)
{
    BS_STATUS eRet = BS_OK;

#ifdef IN_WINDOWS
    HKEY   RegKey;   
    CHAR *pcSelfFileName;
    char info[512];
    CHAR *pcRegPath = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";

    if (arg == NULL) {
        arg = "";
    }

    pcSelfFileName = SYS_GetSelfFileName();
    if (NULL == pcSelfFileName)
    {
        BS_WARNNING(("Can't get self file name"));
        return BS_ERR;
    }

    RegKey=NULL;
    
    if (BS_OK != RegOpenKeyEx(HKEY_LOCAL_MACHINE, pcRegPath, 0, KEY_ALL_ACCESS|KEY_WOW64_64KEY, &RegKey))
    {
        BS_WARNNING(("Can't open reg"));
        return BS_CAN_NOT_OPEN;
    }

    if (bSelfStart)
    {
        snprintf(info, sizeof(info), "%s %s", pcSelfFileName, arg);
        if (BS_OK != RegSetValueEx(RegKey, pcRegName, 0, REG_SZ,
                info, strlen(info)))
        {
            BS_WARNNING(("Can't reg self start"));
            eRet = BS_CAN_NOT_WRITE;
        }
    }
    else
    {
        if(BS_OK != RegDeleteValue(RegKey, pcRegName))
        {
            eRet = BS_CAN_NOT_WRITE;
        }
    }

    RegCloseKey(RegKey);


#endif

    return eRet;
}


CHAR * SYS_GetSelfFileName(void)
{
    return _SYS_OS_GetSelfFileName();
}


CHAR * SYS_GetSelfFilePath(void)
{
    return _SYS_OS_GetSelfFilePath();
}

#ifdef IN_WINDOWS

HWND GetConsoleHwnd(void)
{
#define MY_BUFSIZE 1024 
    HWND hwndFound;         
    char pszNewWindowTitle[MY_BUFSIZE]; 
    
    char pszOldWindowTitle[MY_BUFSIZE]; 
    
 
    
 
    GetConsoleTitle(pszOldWindowTitle, MY_BUFSIZE);
 
    
 
    wsprintf(pszNewWindowTitle, "%d/%d",
        GetTickCount(),
        GetCurrentProcessId());
 
    
 
    SetConsoleTitle(pszNewWindowTitle);
 
    
 
    Sleep(40);
 
    
 
    hwndFound = FindWindow(NULL, pszNewWindowTitle);
 
    
 
    SetConsoleTitle(pszOldWindowTitle);
 
    return(hwndFound);
}

void ShowCmdWin(int show)
{
    ShowWindow(GetConsoleHwnd(), show);
}

#endif
