/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _IC_BS_H
#define _IC_BS_H
#ifdef __cplusplus
extern "C"
{
#endif

#define IC_LEVEL_FATAL      0x1 
#define IC_LEVEL_ERR        0x2 
#define IC_LEVEL_WARNING    0x4 
#define IC_LEVEL_FAIL       0x8 
#define IC_LEVEL_INFO       0x10
#define IC_LEVEL_DBG        0x20

typedef VOID (*PF_IC_PRINT_FUNC)(CHAR *pcMsg, USER_HANDLE_S *pstUserHandle);
typedef HANDLE IC_HANDLE;

int IC_GetLogEvent(char *event);
int IC_GetLogEventStr(UINT events, OUT char **strs);
void IC_OutString(UINT uiLevel, char *msg);
IC_HANDLE IC_Reg(PF_IC_PRINT_FUNC pfFunc, USER_HANDLE_S *pstUserHandle, UINT events);
VOID IC_UnReg(IN IC_HANDLE hIcHandle);
void IC_SetEvents(IC_HANDLE hIcHandle, UINT events);
void IC_Print(UINT uiLevel, char *fmt, ...);

#define IC_FatalInfo(fmt, ...) IC_Print(IC_LEVEL_FATAL, fmt, ##__VA_ARGS__)
#define IC_ErrInfo(fmt, ...) IC_Print(IC_LEVEL_ERR, fmt, ##__VA_ARGS__)
#define IC_WrnInfo(fmt, ...) IC_Print(IC_LEVEL_WARNING, fmt, ##__VA_ARGS__)
#define IC_FailInfo(fmt, ...) IC_Print(IC_LEVEL_FAIL, fmt, ##__VA_ARGS__)
#define IC_Info(fmt, ...)    IC_Print(IC_LEVEL_INFO, fmt, ##__VA_ARGS__)
#define IC_DbgInfo(fmt, ...) IC_Print(IC_LEVEL_DBG, fmt, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif
#endif 
