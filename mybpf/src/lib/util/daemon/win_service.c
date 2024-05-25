/*================================================================
*   Created by LiXingang
*   Description: windows service
*
================================================================*/
#include "bs.h"
#include "utl/win_service.h"

#ifdef IN_WINDOWS

static TCHAR g_winservice_name[128];
static SERVICE_STATUS_HANDLE g_winservice_hServiceStatus;
static SERVICE_STATUS g_winservice_status;
static PF_WINSERVICE_RUN g_winservice_run;
static PF_WINSERVICE_STOP g_winservice_stop;

static void winservice_LogEvent(LPCTSTR pFormat, ...)
{
    TCHAR    chMsg[256];
    HANDLE  hEventSource;
    LPTSTR  lpszStrings[1];
    va_list pArg;

    va_start(pArg, pFormat);
    _vstprintf(chMsg, pFormat, pArg);
    va_end(pArg);

    lpszStrings[0] = chMsg;

    hEventSource = RegisterEventSource(NULL, g_winservice_name);
    if (hEventSource != NULL) {
        ReportEvent(hEventSource, EVENTLOG_INFORMATION_TYPE, 0, 0, NULL, 1, 0, (LPCTSTR*) &lpszStrings[0], NULL);
        DeregisterEventSource(hEventSource);
    }
}

static void winservice_InitStatus()
{
    g_winservice_hServiceStatus = NULL;
    g_winservice_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS|SERVICE_INTERACTIVE_PROCESS;
    g_winservice_status.dwCurrentState = SERVICE_START_PENDING;
    g_winservice_status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    g_winservice_status.dwWin32ExitCode = 0;
    g_winservice_status.dwServiceSpecificExitCode = 0;
    g_winservice_status.dwCheckPoint = 0;
    g_winservice_status.dwWaitHint = 0;
}

static void WINAPI winservice_ServiceStrl(DWORD dwOpcode)
{
    switch (dwOpcode)
    {
        case SERVICE_CONTROL_STOP:
            g_winservice_status.dwCurrentState = SERVICE_STOP_PENDING;
            SetServiceStatus(g_winservice_hServiceStatus, &g_winservice_status);
            g_winservice_stop();
            break;
        case SERVICE_CONTROL_PAUSE:
            break;
        case SERVICE_CONTROL_CONTINUE:
            break;
        case SERVICE_CONTROL_INTERROGATE:
            break;
        case SERVICE_CONTROL_SHUTDOWN:
            break;
        default:
            winservice_LogEvent(_T("Bad service request"));
            OutputDebugString(_T("Bad service request"));
    }
}

static void WINAPI winservice_ServiceMain()
{
    
    g_winservice_status.dwCurrentState = SERVICE_START_PENDING;
    g_winservice_status.dwControlsAccepted = SERVICE_ACCEPT_STOP;

    
    g_winservice_hServiceStatus = RegisterServiceCtrlHandler(g_winservice_name, winservice_ServiceStrl);
    if (g_winservice_hServiceStatus == NULL)
    {
        winservice_LogEvent(_T("Handler not installed"));
        return;
    }

    SetServiceStatus(g_winservice_hServiceStatus, &g_winservice_status);

    g_winservice_status.dwWin32ExitCode = S_OK;
    g_winservice_status.dwCheckPoint = 0;
    g_winservice_status.dwWaitHint = 0;
    g_winservice_status.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(g_winservice_hServiceStatus, &g_winservice_status);

    g_winservice_run();

    g_winservice_status.dwCurrentState = SERVICE_STOPPED;
    SetServiceStatus(g_winservice_hServiceStatus, &g_winservice_status);
    OutputDebugString(_T("Service stopped"));
}

int WinService_Init(char *service_name, PF_WINSERVICE_RUN pfWinServiceRun, PF_WINSERVICE_STOP pfWinServiceStop)
{
    if (strlen(service_name) >= sizeof(g_winservice_name)) {
        return -1;
    }

    strcpy(g_winservice_name, service_name);
    g_winservice_run = pfWinServiceRun;
    g_winservice_stop = pfWinServiceStop;

    return 0;
}

int WinService_Run()
{
    SERVICE_TABLE_ENTRY st[] = {
        { g_winservice_name, (LPSERVICE_MAIN_FUNCTION)winservice_ServiceMain },
        { NULL, NULL }
    };

    winservice_InitStatus();

    if (! StartServiceCtrlDispatcher(st)) {
        winservice_LogEvent(_T("Register Service Main Function Error!"));
        return -1;
    }

    return 0;
}

BOOL_T WinService_IsInstalled()
{
    BOOL_T bResult = FALSE;

    
    SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if (hSCM != NULL) {
        
        SC_HANDLE hService = OpenService(hSCM, g_winservice_name, SERVICE_QUERY_CONFIG);
        if (hService != NULL) {
            bResult = TRUE;
            CloseServiceHandle(hService);
        }   
        CloseServiceHandle(hSCM);
    }

    return bResult;
}

BOOL_T WinService_Install(char *filepath)
{
    if (WinService_IsInstalled()) {
        WinService_Uninstall();
    }

    SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hSCM == NULL) {
        printf("OpenSCManager failed\r\n");
        return FALSE;
    }

    
    SC_HANDLE hService = CreateService(hSCM, g_winservice_name, g_winservice_name,
            SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS|SERVICE_INTERACTIVE_PROCESS ,SERVICE_AUTO_START , SERVICE_ERROR_NORMAL,
            filepath, NULL, NULL, _T(""), NULL, NULL);
    if (hService == NULL) {
        printf("CreateService failed\r\n");
        CloseServiceHandle(hSCM);
        return FALSE;
    }

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCM);

    printf("Install OK\r\n");

    return TRUE;
}

BOOL_T WinService_Uninstall()
{
    if (! WinService_IsInstalled()) {
        return TRUE;
    }

    SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if (hSCM == NULL) {
        return FALSE;
    }

    SC_HANDLE hService = OpenService(hSCM, g_winservice_name, SERVICE_STOP | DELETE);
    if (hService == NULL) {
        CloseServiceHandle(hSCM);
        return FALSE;
    }

    SERVICE_STATUS status;
    ControlService(hService, SERVICE_CONTROL_STOP, &status);

    BOOL_T bDelete = DeleteService(hService);
    CloseServiceHandle(hService);
    CloseServiceHandle(hSCM);

    if (bDelete) {
        return TRUE;
    }

    winservice_LogEvent(_T("Service could not be deleted"));

    return FALSE;
}

#endif
