/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _CMD_EXP_H
#define _CMD_EXP_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef void* CMD_EXP_HDL; 
typedef void* CMD_EXP_RUNNER;

#define DEF_CMD_EXP_VIEW_USER     "user-view"

#define DEF_CMD_EXP_MAX_CMD_LEN  2047


#define CMD_EXP_LEVEL_DFT   0
#define CMD_EXP_LEVEL_MUC   10
#define CMD_EXP_LEVEL_LAST  255


enum {
    DEF_CMD_EXP_TYPE_CMD = 1,  
    DEF_CMD_EXP_TYPE_VIEW,  
    DEF_CMD_EXP_TYPE_SAVE, 
    DEF_CMD_EXP_TYPE_ENTER, 
};


#define DEF_CMD_EXP_PROPERTY_TEMPLET   0x1    
#define DEF_CMD_EXP_PROPERTY_HIDE      0x2 
#define DEF_CMD_EXP_PROPERTY_HIDE_CR   0x4 
#define DEF_CMD_EXP_PROPERTY_VIEW_PATTERN 0x8 


#define CMD_EXP_IS_VIEW(uiType)        ((uiType) == DEF_CMD_EXP_TYPE_VIEW)
#define CMD_EXP_IS_CMD(uiType)         ((uiType) == DEF_CMD_EXP_TYPE_CMD)
#define CMD_EXP_IS_SAVE(uiType)        ((uiType) == DEF_CMD_EXP_TYPE_SAVE)
#define CMD_EXP_IS_ENTER(uiType)        ((uiType) == DEF_CMD_EXP_TYPE_ENTER)
#define CMD_EXP_IS_TEMPLET(uiProp)     ((uiProp) & DEF_CMD_EXP_PROPERTY_TEMPLET)


#define CMD_EXP_RUNNER_TYPE_NONE    0
#define CMD_EXP_RUNNER_TYPE_CMD     0x1
#define CMD_EXP_RUNNER_TYPE_PIPECMD 0x2
#define CMD_EXP_RUNNER_TYPE_TELNET  0x4
#define CMD_EXP_RUNNER_TYPE_WEB     0x8


typedef int (*PF_CMD_EXP_RUN)(UINT, CHAR**, VOID*);
typedef int (*PF_CMD_EXP_SAVE)(IN HANDLE hFileHandle);
typedef int (*PF_CMD_EXP_ENTER)(void *env);

typedef struct
{
    DLL_NODE_S stLinkNode;
    VOID_FUNC pfNoDbgFunc;
}CMD_EXP_NO_DBG_NODE_S;

typedef struct
{
    UINT uiType;  
    UINT uiProperty;
    UCHAR level;  
    HANDLE hParam;  
    void *pfFunc;
    CHAR *pcViews;
    CHAR *pcCmd;
    CHAR *pcViewName;  
}CMD_EXP_REG_CMD_PARAM_S;


CMD_EXP_HDL CmdExp_Create();

#define CMD_EXP_FLAG_LOCK 0x1 
void CmdExp_SetFlag(CMD_EXP_HDL hCmdExp, UINT flag);

int CmdExp_RegCmd(CMD_EXP_HDL hCmdExp, CMD_EXP_REG_CMD_PARAM_S *pstParam);
int CmdExp_UnregCmd(CMD_EXP_HDL hCmdExp, CMD_EXP_REG_CMD_PARAM_S *pstParam);
int CmdExp_RegCmdSimple(CMD_EXP_HDL hCmdExp, char *view, char *cmd,
        PF_CMD_EXP_RUN func, void *ud);
int CmdExp_UnregCmdSimple(CMD_EXP_HDL hCmdExp, char *view, char *cmd);
int CmdExp_Run(CMD_EXP_RUNNER hRunner, UCHAR ucCmdChar);
int CmdExp_RunLine(CMD_EXP_RUNNER hRunner, char *line);
int CmdExp_RunString(CMD_EXP_RUNNER hRunner, char *string, int len);
int CmdExp_DoCmd(CMD_EXP_HDL hCmdExp, char *cmd);


BOOL_T CmdExp_IsShowing(HANDLE hFileHandle);


BOOL_T CmdExp_IsSaving(HANDLE hFileHandle);
int CmdExp_OutputMode(IN HANDLE hFileHandle, IN CHAR *fmt, ...) __attribute__((warn_unused_result));
int CmdExp_OutputModeQuit(IN HANDLE hFileHandle);
int CmdExp_OutputCmd(IN HANDLE hFileHandle, IN CHAR *fmt, ...);
int CmdExp_RegSave(CMD_EXP_HDL hCmdExp, char *save_path, CMD_EXP_REG_CMD_PARAM_S *param);
int CmdExp_RegEnter(CMD_EXP_HDL hCmdExp, CMD_EXP_REG_CMD_PARAM_S *param);
int CmdExp_UnRegSave(CMD_EXP_HDL hCmdExp, CHAR *save_path, CHAR *pcViews);
int CmdExp_CmdSave(UINT ulArgc, CHAR **pArgv, VOID *pEnv);
int CmdExp_CmdShow(UINT ulArgc, CHAR **pArgv, VOID *pEnv);
void * CmdExp_GetEnvBySaveHandle(HANDLE hFileHandle);
int CmdExp_GetMucIDBySaveHandle(HANDLE hFileHandle);

HANDLE CmdExp_GetParam(VOID *pEnv);


int CmdExp_CmdNoDebugAll(UINT ulArgc, CHAR **pArgv, VOID *pEnv);
VOID CmdExp_RegNoDbgFunc(CMD_EXP_HDL hCmdExp, CMD_EXP_NO_DBG_NODE_S *pstNode);


CMD_EXP_RUNNER CmdExp_CreateRunner(CMD_EXP_HDL hCmdExp, UINT type);
void CmdExp_AltEnable(CMD_EXP_RUNNER hRunner, int enable);
void CmdExp_SetViewPrefix(CMD_EXP_RUNNER hRunner, char *prefix);
void CmdExp_SetRunnerDir(CMD_EXP_RUNNER hRunner, char *dir);
enum {
    CMD_EXP_HOOK_EVENT_CHAR = 0,
    CMD_EXP_HOOK_EVENT_LINE,
    CMD_EXP_HOOK_EVENT_DESTROY,
};
typedef int (*CMD_EXP_HOOK_FUNC)(int event, void *data, void *ud, void *env);
void CmdExp_SetRunnerHook(CMD_EXP_RUNNER hRunner, CMD_EXP_HOOK_FUNC func, void *ud);
void CmdExp_SetRunnerLevel(CMD_EXP_RUNNER hRunner, UCHAR level);
void CmdExp_SetRunnerMucID(CMD_EXP_RUNNER hRunner, int muc_id);
void CmdExp_SetRunnerHistory(CMD_EXP_RUNNER hRunner, BOOL_T enable);
void CmdExp_SetRunnerHelp(CMD_EXP_RUNNER hRunner, BOOL_T enable);
void CmdExp_SetRunnerPrefixEnable(CMD_EXP_RUNNER hRunner, BOOL_T enable);
void CmdExp_SetRunnerHookMode(CMD_EXP_RUNNER hRunner, BOOL_T enable);
int CmdExp_QuitMode(CMD_EXP_RUNNER hRunner);

void CmdExp_SetRunnerType(CMD_EXP_RUNNER hRunner, UINT type);
UINT CmdExp_GetRunnerType(CMD_EXP_RUNNER hRunner);
void CmdExp_RunnerOutputPrefix(CMD_EXP_RUNNER hRunner);
void CmdExp_SetSubRunner(void *runner, void *muc_runner);
void CmdExp_ClearInputsBuf(CMD_EXP_RUNNER hRunner);


void CmdExp_ResetRunner(CMD_EXP_RUNNER hRunner);
int CmdExp_DestroyRunner(CMD_EXP_RUNNER hRunner);


int CmdExp_RunEnvCmd(char *cmd, void *env);
int CmdExp_GetEnvMucID(void *env);
void * CmdExp_GetCurrentEnv();
void * CmdExp_GetEnvRunner(void *pEnv);

#if 1   

int CmdExp_EnterModeManual(int argc, char **argv, char *view_name, void *env);

void CmdExp_SetCurrentModeValue(void *env, char *mode_value);

CHAR * CmdExp_GetCurrentModeValue(IN VOID *pEnv);

CHAR * CmdExp_GetCurrentModeName(IN VOID *pEnv);

CHAR * CmdExp_GetCurrentViewName(void *pEnv);

CHAR * CmdExp_GetUpModeValue(IN VOID *pEnv, IN UINT uiHistroyIndex);

CHAR * CmdExp_GetUpModeName(IN VOID *pEnv, IN UINT uiHistroyIndex);
#endif

#if 1 
#define DEF_CMD_OPT_FLAG_NOSAVE 0x1
#define DEF_CMD_OPT_FLAG_NOSHOW 0x2
UINT CmdExp_GetOptFlag(IN CHAR *pcString);;

BOOL_T CmdExp_IsOptPermitOutput(IN HANDLE hFile, IN CHAR *pcString);

#endif 

#define CMD_EXP_OutputMode CmdExp_OutputMode
#define CMD_EXP_OutputModeQuit CmdExp_OutputModeQuit
#define CMD_EXP_OutputCmd CmdExp_OutputCmd
#define CMD_EXP_GetUpModeValue CmdExp_GetUpModeValue
#define CMD_EXP_GetCurrentModeName CmdExp_GetCurrentModeName
#define CMD_EXP_GetCurrentModeValue CmdExp_GetCurrentModeValue
#define CMD_EXP_GetParam CmdExp_GetParam
#define CMD_EXP_IsSaving CmdExp_IsSaving
#define CMD_EXP_IsShowing CmdExp_IsShowing
#define CMD_EXP_DestroyRunner CmdExp_DestroyRunner

#ifdef __cplusplus
}
#endif
#endif 
