/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _SYSRUN_UTL_H
#define _SYSRUN_UTL_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef VOID (*PF_SYSRUN_EXIT_NOTIFY_FUNC)(int lExitNum, USER_HANDLE_S *pstUserHandle);

#if 1
void _SysrunUtl_Exit(INT lExitNum);
BS_STATUS _SysrunUtl_RegExitNotifyFunc(IN PF_SYSRUN_EXIT_NOTIFY_FUNC pfFunc, IN USER_HANDLE_S *pstUserHandle);
void _SysrunUtl_SetDaemon(void);
BOOL_T _SysrunUtl_IsDaemon(void);
#endif

#if 1
PLUG_API void _SysrunBs_Exit(INT lExitNum);
PLUG_API BS_STATUS _SysrunBs_RegExitNotifyFunc(IN PF_SYSRUN_EXIT_NOTIFY_FUNC pfFunc, IN USER_HANDLE_S *ud);
PLUG_API void _SysrunBs_SetDaemon(void);
PLUG_API BOOL_T _SysrunBs_IsDaemon(void);
#endif

#ifdef USE_BS
#define SYSRUN_Exit _SysrunBs_Exit
#define SYSRUN_RegExitNotifyFunc _SysrunBs_RegExitNotifyFunc
#define SYSRUN_SetDaemon _SysrunBs_SetDaemon
#define SYSRUN_IsDaemon _SysrunBs_IsDaemon
#else
#define SYSRUN_Exit _SysrunUtl_Exit
#define SYSRUN_RegExitNotifyFunc _SysrunUtl_RegExitNotifyFunc
#define SYSRUN_SetDaemon _SysrunUtl_SetDaemon
#define SYSRUN_IsDaemon _SysrunUtl_IsDaemon
#endif

#ifdef __cplusplus
}
#endif
#endif 
