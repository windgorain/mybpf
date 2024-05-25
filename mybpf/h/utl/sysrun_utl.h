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

typedef VOID (*PF_SYSRUN_EXIT_NOTIFY_FUNC)(IN INT lExitNum,
        IN USER_HANDLE_S *pstUserHandle);

#if 1
VOID _SysrunBs_Exit(INT lExitNum);
VOID _SysrunUtl_Exit(INT lExitNum);
BS_STATUS _SysrunBs_RegExitNotifyFunc(IN PF_SYSRUN_EXIT_NOTIFY_FUNC pfFunc, IN USER_HANDLE_S *ud);
BS_STATUS _SysrunUtl_RegExitNotifyFunc(IN PF_SYSRUN_EXIT_NOTIFY_FUNC pfFunc, IN USER_HANDLE_S *pstUserHandle);
#endif


#ifdef USE_BS
#define SYSRUN_Exit _SysrunBs_Exit
#define SYSRUN_RegExitNotifyFunc _SysrunBs_RegExitNotifyFunc
#else
#define SYSRUN_Exit _SysrunUtl_Exit
#define SYSRUN_RegExitNotifyFunc _SysrunUtl_RegExitNotifyFunc
#endif


#ifdef __cplusplus
}
#endif
#endif 
