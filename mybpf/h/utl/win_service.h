/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _WIN_SERVICE_H
#define _WIN_SERVICE_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef void (*PF_WINSERVICE_RUN)();
typedef void (*PF_WINSERVICE_STOP)();

int WinService_Init(char *service_name, PF_WINSERVICE_RUN pfWinServiceRun, PF_WINSERVICE_STOP pfWinServiceStop);
int WinService_Run();
BOOL_T WinService_IsInstalled();
BOOL_T WinService_Install(char *filepath);
BOOL_T WinService_Uninstall();

#ifdef __cplusplus
}
#endif
#endif 
