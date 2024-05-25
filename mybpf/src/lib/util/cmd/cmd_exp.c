/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2007-5-20
* Description: 
* History:     
******************************************************************************/
#include "bs.h"

#include "utl/txt_utl.h"
#include "utl/sysrun_utl.h"
#include "utl/args_utl.h"
#include "utl/exec_utl.h"
#include "utl/sprintf_utl.h"
#include "utl/passwd_utl.h"
#include "utl/cff_utl.h"
#include "utl/rbuf.h"
#include "utl/mutex_utl.h"
#include "utl/ascii_utl.h"
#include "utl/signal_utl.h"
#include "utl/file_utl.h"
#include "utl/bit_opt.h"
#include "utl/stack_utl.h"
#include "utl/exchar_utl.h"
#include "utl/cmd_exp.h"
#include "pcre.h"

#define _DEF_CMD_EXP_MAX_VIEW_NAME_LEN           31
#define _DEF_CMD_EXP_MAX_MODE_NAME_LEN           31


#define DEF_CMD_OPT_STR_PREFIX    "__OPT_"   
#define DEF_CMD_OPT_FLAG_STR_NOSAVE   'S'    
#define DEF_CMD_OPT_FLAG_STR_NOSHOW   'H'    
#define DEF_CMD_OPT_STR_END           '_'    


#define DEF_CMD_REG_FLAG_CYCLE 0x1


#define _DEF_CMD_EXP_MAX_MODE_VALUE_LEN 255

#define _DEF_CMD_EXP_MAX_CMD_ELEMENT_IN_ONE_CMD  100
#define _DEF_CMD_EXP_MAX_CMD_ELEMENT_LEN         31


#define _DEF_CMD_EXP_IS_CMD_RUNABLE(_pstNode) \
    (((_pstNode)->pfFunc != NULL) || (CMD_EXP_IS_VIEW((_pstNode)->uiType)))



typedef struct cmdTree
{
    DLL_NODE_S link_node; 
    DLL_HEAD_S _subcmds;
    DLL_HEAD_S *subcmd_list; 
    struct cmdTree *pstParent;

    UINT uiType;  
    UINT uiProperty; 
    UCHAR level; 
    CHAR acCmd[_DEF_CMD_EXP_MAX_CMD_ELEMENT_LEN + 1];  
    CHAR *pcHelp;  
    PF_CMD_EXP_RUN pfFunc;  
    HANDLE hParam;
    void *view; 

}_CMD_EXP_TREE_S;

typedef enum
{
    _CMD_EXP_EXACTMACTH = 1,
    _CMD_EXP_PREMACTH = 2,
    _CMD_EXP_NOTMACTH = 3
}_CMD_EXP_MATCH_E;

typedef struct
{
    CHAR *pcCmd;
    CHAR *pcHelp;
    CHAR *pcNext;
}_CMD_EXP_ELEMENT_S;

typedef struct
{
    DLL_NODE_S   stDllNode;
    UCHAR level;
    PF_CMD_EXP_SAVE pfFunc;
    CHAR save_path[FILE_MAX_PATH_LEN + 1];
}_CMD_EXP_SVAE_NODE_S;

typedef struct
{
    DLL_NODE_S   stDllNode;
    UCHAR level;
    PF_CMD_EXP_ENTER pfFunc;
}_CMD_EXP_ENTER_NODE_S;

typedef struct
{
    DLL_NODE_S stLinkNode;  

    CHAR view_name[_DEF_CMD_EXP_MAX_VIEW_NAME_LEN + 1]; 
    UINT property;

    _CMD_EXP_TREE_S stCmdRoot; 
    DLL_HEAD_S stSaveFuncList; 
    DLL_HEAD_S stEnterFuncList; 
    DLL_HEAD_S stSubViewList; 
}_CMD_EXP_VIEW_S;

typedef struct
{
    _CMD_EXP_VIEW_S *pstViewTbl;
    CHAR szModeName[_DEF_CMD_EXP_MAX_MODE_NAME_LEN + 1];  
    CHAR szModeValue[_DEF_CMD_EXP_MAX_MODE_VALUE_LEN + 1];
    char *cmd;
}_CMD_EXP_NEW_MODE_NODE_S;

typedef struct {
    _CMD_EXP_VIEW_S stRootView;
    DLL_HEAD_S stNoDbgFuncList;
    DLL_HEAD_S stCmdViewPatternList; 
    MUTEX_S lock; 
    UINT flag;
}CMD_EXP_S;

#define CMD_EXP_RUNNER_NAME_LEN 31

typedef struct
{
    CMD_EXP_S *cmdexp;
    void *sub_runner; 
    char *runner_dir;
    CMD_EXP_HOOK_FUNC hook_func;
    void *hook_ud;
    _CMD_EXP_NEW_MODE_NODE_S *pstCurrentMode;
    CHAR acInputs[DEF_CMD_EXP_MAX_CMD_LEN + 1];
    CHAR runner_name[CMD_EXP_RUNNER_NAME_LEN + 1];
    USHORT ulInputsLen;
    USHORT uiInputsPos; 
    UINT ulHistoryIndex;
    UINT alt_mode:1; 
    UINT bIsRChangeToN:1;
    UINT deny_history:1; 
    UINT deny_help:1; 
    UINT deny_prefix:1; 
    UINT reserved:3;
    UINT level:8;
    UINT runner_type: 8;
    UINT reserved2:8;
    int muc_id;
    HANDLE hCmdRArray;
    HANDLE hExcharHandle;
    HANDLE hModeStack;
}_CMD_EXP_RUNNER_S;


typedef struct
{
    _CMD_EXP_RUNNER_S *pstRunner; 
    _CMD_EXP_TREE_S *pstCmdNode;    
}_CMD_EXP_ENV_S;

typedef struct
{
    HANDLE hStack;
    _CMD_EXP_ENV_S *pstEnv;
    FILE *fp;
    UINT bIsNewFile:1;
    UINT bHasCfg:1;      
    UINT matched_mode:1;   
    UINT bIsShowAll:1;   
    UCHAR ucPrefixSpaceNum;   
    UCHAR ucMatchedModeIndex; 
    UCHAR ucModeDeepth;       
}_CMD_EXP_SHOW_SAVE_NODE_S;

typedef _CMD_EXP_MATCH_E (*PF_CMD_EXP_PATTERN_PARSE)(IN CHAR *pszCmdElement, IN CHAR *pszInputCmd);

typedef struct
{
    CHAR *pcPattern;
    PF_CMD_EXP_PATTERN_PARSE pfModeParse;
}_CMD_EXP_PATTERN_PARSE_S;

typedef struct {
    DLL_NODE_S link_node;
    CMD_EXP_REG_CMD_PARAM_S cmd_param;
    pcre *pcre;
}_CMD_EXP_CMD_VIEW_PATTERN_S;


static _CMD_EXP_MATCH_E cmdexp_ParsePatternString(IN CHAR *pszCmdElement, IN CHAR *pszInputCmd);
static _CMD_EXP_MATCH_E cmdexp_ParsePatternInt(IN CHAR *pszCmdElement, IN CHAR *pszInputCmd);
static _CMD_EXP_MATCH_E cmdexp_ParsePatternIP(IN CHAR *pszCmdElement, IN CHAR *pszInputCmd);
static _CMD_EXP_MATCH_E cmdexp_ParsePatternOption(IN CHAR *pszCmdElement, IN CHAR *pszInputCmd);
static int cmdexp_QuitCmd(IN UINT ulArgc, IN CHAR **pArgv, IN VOID *pEnv);
static int cmdexp_Run(CMD_EXP_RUNNER hRunner, UCHAR ucCmdChar);
static int cmdexp_RegCmd(CMD_EXP_S *cmdexp, CHAR *pcView, CHAR *pcCmd, CMD_EXP_REG_CMD_PARAM_S *param, UINT flag);

static int cmdexp_AddCmd
(
    CMD_EXP_S *cmdexp,
    _CMD_EXP_VIEW_S *pstViewTblNode,
    _CMD_EXP_TREE_S *pstRoot,
    CHAR *pcCmd,
    CMD_EXP_REG_CMD_PARAM_S *param,
    UINT flag
);

static _CMD_EXP_PATTERN_PARSE_S g_astCmdExpParse[] =
{
    {"%STRING", cmdexp_ParsePatternString},
    {"%INT", cmdexp_ParsePatternInt},
    {"%IP", cmdexp_ParsePatternIP},
    {"%OPTION", cmdexp_ParsePatternOption},
    {0}
};

static THREAD_LOCAL void * g_cmd_exp_thread_env = NULL;

typedef int (*PF_CMD_EXP_VIEW_WALK)(_CMD_EXP_VIEW_S *view_node, void *ud);

static int cmdexp_WalkView(_CMD_EXP_VIEW_S *pstViewTree, PF_CMD_EXP_VIEW_WALK walk_func, void *ud)
{
    int ret;
    _CMD_EXP_VIEW_S *pstNode;

    if ((ret = walk_func(pstViewTree, ud)) < 0) {
        return ret;
    }

    DLL_SCAN(&(pstViewTree->stSubViewList), pstNode) {
        if ((ret = cmdexp_WalkView(pstNode, walk_func, ud)) < 0) {
            return ret;
        }
    }

    return 0;
}

static _CMD_EXP_VIEW_S * cmdexp_FindViewFromViewTree(_CMD_EXP_VIEW_S *pstViewTree, char *pcViewName)
{
    _CMD_EXP_VIEW_S *pstNode;
    _CMD_EXP_VIEW_S *pstFound;
    
    if (strcmp(pstViewTree->view_name, pcViewName) == 0) {
        return pstViewTree;
    }

    DLL_SCAN(&(pstViewTree->stSubViewList), pstNode) {
        pstFound = cmdexp_FindViewFromViewTree(pstNode, pcViewName);
        if (pstFound) {
            return pstFound;
        }
    }

    return NULL;
}

static _CMD_EXP_VIEW_S *cmdexp_FindView(CMD_EXP_S *cmdexp, CHAR *pcViewName)
{
    return cmdexp_FindViewFromViewTree(&cmdexp->stRootView, pcViewName);
}

static int cmdexp_reg_cmd_by_view_pattern(_CMD_EXP_VIEW_S *view_node, void *ud)
{
    int rc;
    int ovector[3];
    USER_HANDLE_S *uh = ud;
    CMD_EXP_S *cmdexp = uh->ahUserHandle[0];
    _CMD_EXP_CMD_VIEW_PATTERN_S *node = uh->ahUserHandle[1];
    char *view_name = view_node->view_name;
    int view_name_len = strlen(view_name);

    if((rc=pcre_exec(node->pcre, NULL, (const char*)view_name, view_name_len, 0, 0, ovector, 3)) <=0) {
        return 0;
    }

    cmdexp_RegCmd(cmdexp, view_name, node->cmd_param.pcCmd, &node->cmd_param, 0);

    return 0;
}


static void cmdexp_WalkViewsToRegPatternCmd(CMD_EXP_S *cmdexp, _CMD_EXP_CMD_VIEW_PATTERN_S *node)
{
    USER_HANDLE_S ud;

    ud.ahUserHandle[0] = cmdexp;
    ud.ahUserHandle[1] = node;

    cmdexp_WalkView(&cmdexp->stRootView, cmdexp_reg_cmd_by_view_pattern, &ud);
}


static void cmdexp_WalkPatternCmdToRegView(CMD_EXP_S *cmdexp, char *view_name)
{
    _CMD_EXP_CMD_VIEW_PATTERN_S *node;
    int rc;
    int ovector[3];
    int view_name_len = strlen(view_name);

    DLL_SCAN(&cmdexp->stCmdViewPatternList, node) {
        if((rc=pcre_exec(node->pcre, NULL, (const char*)view_name, view_name_len, 0, 0, ovector, 3)) <=0) {
            continue;
        }
        cmdexp_RegCmd(cmdexp, view_name, node->cmd_param.pcCmd, &node->cmd_param, 0);
    }
}


static char * cmdexp_DupCmd(UINT argc, char **argv)
{
    char cmd[DEF_CMD_EXP_MAX_CMD_LEN + 1] = "";
    int i;

    for (i=0; i<argc; i++) {
        if (i != 0) {
            TXT_Strlcat(cmd, " ", sizeof(cmd));
        }
        TXT_Strlcat(cmd, argv[i], sizeof(cmd));
    }

    return TXT_Strdup(cmd);
}

static void cmdexp_FreeModeNode(_CMD_EXP_NEW_MODE_NODE_S *pstModeNode)
{
    if (pstModeNode->cmd) {
        MEM_Free(pstModeNode->cmd);
    }

    MEM_Free(pstModeNode);
}

static int cmdexp_NotifyEnter(_CMD_EXP_RUNNER_S *pstRunner, _CMD_EXP_VIEW_S *view)
{
    _CMD_EXP_ENTER_NODE_S *enter_node;
    _CMD_EXP_ENV_S stEnv = {0};

    stEnv.pstRunner = pstRunner;

    DLL_SCAN(&view->stEnterFuncList, enter_node) {
        enter_node->pfFunc(&stEnv);
    }

    return 0;
}


static int cmdexp_EnterMode(_CMD_EXP_RUNNER_S *pstRunner, _CMD_EXP_VIEW_S *pstViewTbl, UINT property, UINT uiArgc, CHAR **ppcArgv)
{
    _CMD_EXP_NEW_MODE_NODE_S *pstModeNode;
    CHAR *pcModeValue = ppcArgv[uiArgc - 1];
    char *view_name = pstViewTbl->view_name;

    pstModeNode = MEM_ZMalloc(sizeof(_CMD_EXP_NEW_MODE_NODE_S));
    if (NULL == pstModeNode) {
        RETURN(BS_NO_MEMORY);
    }

    pstModeNode->pstViewTbl = pstViewTbl;

    if (CMD_EXP_IS_TEMPLET(property)) {
        BS_Snprintf(pstModeNode->szModeName, sizeof(pstModeNode->szModeName),
                "%s-%s", view_name, pcModeValue);
        strlcpy(pstModeNode->szModeValue, pcModeValue, sizeof(pstModeNode->szModeValue));
    } else {
        strlcpy(pstModeNode->szModeName, view_name, sizeof(pstModeNode->szModeName));
    }

    pstModeNode->cmd = cmdexp_DupCmd(uiArgc, ppcArgv);
    if (! pstModeNode->cmd) {
        cmdexp_FreeModeNode(pstModeNode);
        RETURN(BS_ERR);
    }

    if (pstRunner->pstCurrentMode != NULL) {
        if (BS_OK != HSTACK_Push(pstRunner->hModeStack, pstRunner->pstCurrentMode)) {
            cmdexp_FreeModeNode(pstModeNode);
            RETURN(BS_ERR);
        }
    }

    pstRunner->pstCurrentMode = pstModeNode;

    cmdexp_NotifyEnter(pstRunner, pstViewTbl);

    return BS_OK;
}


int CmdExp_QuitMode(CMD_EXP_RUNNER hRunner)
{
    _CMD_EXP_RUNNER_S *pstRunner = hRunner;
    _CMD_EXP_NEW_MODE_NODE_S *pstModeTmp;

    if (BS_OK != HSTACK_Pop(pstRunner->hModeStack, (HANDLE*)&pstModeTmp)) {
        return BS_STOP;
    }

    cmdexp_FreeModeNode(pstRunner->pstCurrentMode);

    pstRunner->pstCurrentMode = pstModeTmp; 

    return BS_OK;
}

static int cmdexp_QuitCmd(IN UINT ulArgc, IN CHAR **pArgv,
        IN VOID *pEnv)
{
    _CMD_EXP_ENV_S *pstEnv = pEnv;

    return CmdExp_QuitMode(pstEnv->pstRunner);
}

static void cmdexp_InitViewNode(_CMD_EXP_VIEW_S *pstView, char *view_name) 
{
    DLL_INIT(&pstView->stSubViewList);
    DLL_INIT(&pstView->stSaveFuncList);
    DLL_INIT(&pstView->stEnterFuncList);
    DLL_INIT(&pstView->stCmdRoot._subcmds);
    pstView->stCmdRoot.subcmd_list = &pstView->stCmdRoot._subcmds;

    strlcpy(pstView->view_name, view_name, sizeof(pstView->view_name));
}

static _CMD_EXP_VIEW_S * cmdexp_AddSubView(_CMD_EXP_VIEW_S *pstFatherView, char *view_name)
{
    _CMD_EXP_VIEW_S *pstView;

    pstView = MEM_ZMalloc(sizeof(_CMD_EXP_VIEW_S));
    if (NULL == pstView) {
        return NULL;
    }

    cmdexp_InitViewNode(pstView, view_name);

    DLL_ADD(&pstFatherView->stSubViewList, &pstView->stLinkNode);

    return pstView;
}

static void cmdexp_FreeViewPattern(_CMD_EXP_CMD_VIEW_PATTERN_S *node)
{
    if (node->cmd_param.pcCmd) {
        MEM_Free(node->cmd_param.pcCmd);
    }
    if (node->cmd_param.pcViewName) {
        MEM_Free(node->cmd_param.pcViewName);
    }
    if (node->pcre) {
        free(node->pcre);
    }

    MEM_Free(node);
}

static int cmdexp_AddCmdViewPattern(CMD_EXP_S *cmdexp, CMD_EXP_REG_CMD_PARAM_S *param)
{
    _CMD_EXP_CMD_VIEW_PATTERN_S *node;
    int erroroffset;
    const char* error;

    if (! (param->uiProperty & DEF_CMD_EXP_PROPERTY_VIEW_PATTERN)) {
        RETURN(BS_ERR);
    }

    node = MEM_ZMalloc(sizeof(_CMD_EXP_CMD_VIEW_PATTERN_S));
    if (! node) {
        RETURN(BS_NO_MEMORY);
    }

    node->cmd_param = *param;
    node->cmd_param.pcCmd = NULL;
    node->cmd_param.pcViewName = NULL;
    node->cmd_param.pcViews = NULL;

    if (param->pcCmd) {
        node->cmd_param.pcCmd = TXT_Strdup(param->pcCmd);
        if (! node->cmd_param.pcCmd) {
            cmdexp_FreeViewPattern(node);
            RETURN(BS_NO_MEMORY);
        }
    }

    if (param->pcViewName) {
        node->cmd_param.pcViewName = TXT_Strdup(param->pcViewName);
        if (! node->cmd_param.pcViewName) {
            cmdexp_FreeViewPattern(node);
            RETURN(BS_NO_MEMORY);
        }
    }

    node->pcre = pcre_compile(param->pcViews, 0, &error, &erroroffset, NULL);
    if (! node->pcre) {
        printf("PCRE compilation failed at offset %d: %s \r\n", erroroffset, error);
        cmdexp_FreeViewPattern(node);
        RETURN(BS_NO_MEMORY);
    }

    DLL_ADD(&cmdexp->stCmdViewPatternList, &node->link_node);

    
    cmdexp_WalkViewsToRegPatternCmd(cmdexp, node);

    return 0;
}


static int cmdexp_Init(CMD_EXP_S *cmdexp)
{
    CMD_EXP_REG_CMD_PARAM_S stCmdParam = {0};

    DLL_INIT(&cmdexp->stRootView.stSubViewList);
    DLL_INIT(&cmdexp->stRootView.stSaveFuncList);
    DLL_INIT(&cmdexp->stRootView.stEnterFuncList);
    DLL_INIT(&cmdexp->stNoDbgFuncList);
    DLL_INIT(&cmdexp->stCmdViewPatternList);
    cmdexp_InitViewNode(&cmdexp->stRootView, DEF_CMD_EXP_VIEW_USER);
    MUTEX_Init(&cmdexp->lock);

    stCmdParam.uiType = DEF_CMD_EXP_TYPE_CMD;
    stCmdParam.uiProperty = DEF_CMD_EXP_PROPERTY_VIEW_PATTERN;
    stCmdParam.level = CMD_EXP_LEVEL_LAST;
    stCmdParam.pcViews = "^.+$";
    stCmdParam.pcCmd = "quit(Quit current view)";
    stCmdParam.pcViewName = NULL;
    stCmdParam.pfFunc = cmdexp_QuitCmd;

    cmdexp_AddCmdViewPattern(cmdexp, &stCmdParam);

    stCmdParam.pcCmd = "show(Show information) this [all]";
    stCmdParam.pfFunc = CmdExp_CmdShow;

    cmdexp_AddCmdViewPattern(cmdexp, &stCmdParam);

    return BS_OK;
}

CMD_EXP_HDL CmdExp_Create()
{
    CMD_EXP_S *cmdexp = MEM_ZMalloc(sizeof(CMD_EXP_S));
    if (! cmdexp) {
        return NULL;
    }

    cmdexp_Init(cmdexp);

    return cmdexp;
}

void CmdExp_SetFlag(CMD_EXP_HDL hCmdExp, UINT flag)
{
    CMD_EXP_S *cmdexp = hCmdExp;
    cmdexp->flag = flag;
}


static int cmdexp_GetCmdElementRange(CHAR *pszCmdElemet,
        OUT INT64 *min, OUT INT64 *max)
{
    CHAR *pcSplit = NULL, *pcSplitTmp = NULL;
    CHAR szNum[64] = "";
    INT64 lMin = 0, lMax= 0;
    INT i;

    pcSplit = strchr(pszCmdElemet, '<');
    if (pcSplit == NULL)
    {
        BS_WARNNING(("Bad cmd element!"));
        RETURN(BS_ERR);
    }

    pcSplit++;

    pcSplitTmp = strchr(pcSplit + 1, '-');
    if (pcSplitTmp == NULL)
    {
        BS_WARNNING(("Bad cmd element!"));
        RETURN(BS_ERR);
    }

    for (i=0; i<pcSplitTmp - pcSplit; i++)
    {
        szNum[i] = pcSplit[i];
    }
    szNum[i] = '\0';

    if (szNum[0] == '\0')
    {
        BS_WARNNING(("Bad cmd element!"));
        RETURN(BS_ERR);        
    }

    TXT_Atoll(szNum, &lMin);

    pcSplit = pcSplitTmp + 1;
    pcSplitTmp = strchr(pcSplit, '>');
    if (pcSplitTmp == NULL)
    {
        BS_WARNNING(("Bad cmd element!"));
        RETURN(BS_ERR);
    }

    for (i=0; i<pcSplitTmp - pcSplit; i++)
    {
        szNum[i] = pcSplit[i];
    }
    szNum[i] = '\0';

    if (szNum[0] == '\0')
    {
        BS_WARNNING(("Bad cmd element!"));
        RETURN(BS_ERR);        
    }

    TXT_Atoll(szNum, &lMax);

    *min = lMin;
    *max = lMax;

    return BS_OK;    
}

static _CMD_EXP_MATCH_E cmdexp_ParsePatternString(IN CHAR *pszCmdElement,
        IN CHAR *pszInputCmd)
{
    INT64 min, max;
    int len = strlen(pszInputCmd);

    if (cmdexp_GetCmdElementRange(pszCmdElement, &min, &max) != BS_OK)
    {
        return _CMD_EXP_NOTMACTH;
    }

    if ((len < min) || (len > max))
    {
        return _CMD_EXP_NOTMACTH;
    }

    return _CMD_EXP_PREMACTH;
}

static _CMD_EXP_MATCH_E cmdexp_ParsePatternInt(IN CHAR *pszCmdElement,
        IN CHAR *pszInputCmd)
{
    INT64 var = 0;
    INT64 min, max;

    if (cmdexp_GetCmdElementRange(pszCmdElement, &min, &max) != BS_OK)
    {
        return _CMD_EXP_NOTMACTH;
    }

    if (BS_OK != TXT_Atoll(pszInputCmd, &var))
    {
        return _CMD_EXP_NOTMACTH;
    }
    
    if ((var < min) || (var > max))
    {
        return _CMD_EXP_NOTMACTH;
    }
    
    return _CMD_EXP_PREMACTH;
}

static _CMD_EXP_MATCH_E cmdexp_ParsePatternIP(IN CHAR *pszCmdElement,
        IN CHAR *pszInputCmd)
{
    CHAR *pcElement;
    UINT uiTimes = 0;
    UINT uiNum;

    TXT_SCAN_ELEMENT_BEGIN(pszInputCmd, '.', pcElement)
    {
        uiTimes ++;
        if (uiTimes > 4)
        {
            TXT_SCAN_ELEMENT_STOP();
            return _CMD_EXP_NOTMACTH;
        }

        if (BS_OK != TXT_AtouiWithCheck(pcElement, &uiNum))
        {
            TXT_SCAN_ELEMENT_STOP();
            return _CMD_EXP_NOTMACTH;
        }

        if (uiNum > 255)
        {
            TXT_SCAN_ELEMENT_STOP();
            return _CMD_EXP_NOTMACTH;
        }
    }TXT_SCAN_ELEMENT_END();

    if (uiTimes != 4)
    {
        return _CMD_EXP_NOTMACTH;
    }

    return _CMD_EXP_PREMACTH;
}

static _CMD_EXP_MATCH_E cmdexp_ParsePatternOption(IN CHAR *pszCmdElement,
        IN CHAR *pszInputCmd)
{
    if (pszInputCmd[0] == '-') {
        return _CMD_EXP_PREMACTH;
    }

    return _CMD_EXP_NOTMACTH;
}


static ULONG cmdexp_GetPatternLen(IN CHAR *pcCmdEle)
{
    CHAR *pcSplit;

    pcSplit = strchr(pcCmdEle, '<');
    if (NULL == pcSplit)
    {
        return strlen(pcCmdEle);
    }

    return pcSplit - pcCmdEle;
}

static _CMD_EXP_MATCH_E cmdexp_CmdElementCmp(IN CHAR *pszCmdElement,
        IN CHAR *pszInputCmd)
{
    if (pszCmdElement[0] != '%') {
        if (strcmp(pszCmdElement, pszInputCmd) == 0) {
            return _CMD_EXP_EXACTMACTH;
        }

        if (strncmp(pszCmdElement, pszInputCmd, strlen(pszInputCmd)) == 0) {
            return _CMD_EXP_PREMACTH;
        }

        return _CMD_EXP_NOTMACTH;
    } else {      
        _CMD_EXP_PATTERN_PARSE_S *node;
        for (node=g_astCmdExpParse; node->pcPattern != NULL; node++) {
            if (strncmp(node->pcPattern, pszCmdElement,
                        cmdexp_GetPatternLen(pszCmdElement)) == 0) {
                return node->pfModeParse(pszCmdElement, pszInputCmd);
            }
        }
    }

    return _CMD_EXP_NOTMACTH;
}


static _CMD_EXP_TREE_S * cmdexp_FindCmdElementExact(_CMD_EXP_TREE_S *pstRoot, char *pcCmdElement )
{
    _CMD_EXP_TREE_S *pstNode;

    DLL_SCAN(pstRoot->subcmd_list, pstNode) {
        if (strcmp(pstNode->acCmd, pcCmdElement) == 0) {
            return pstNode;
        }
    }

    return NULL;
}


static _CMD_EXP_TREE_S * cmdexp_FindCmdElement(_CMD_EXP_RUNNER_S *pstRunner, _CMD_EXP_TREE_S *pstRoot,
        char *pcCmd )
{
    _CMD_EXP_TREE_S *pstNode;
    _CMD_EXP_TREE_S *pstNodeFoundVisable = NULL;
    _CMD_EXP_TREE_S *pstNodeFoundHide = NULL;
    _CMD_EXP_MATCH_E eMatchResult;
    UINT uiHideCmdCount = 0;
    UINT uiVisableCmdCount = 0;

    
    DLL_SCAN(pstRoot->subcmd_list, pstNode) {
        if (pstNode->level < pstRunner->level) {
            continue;
        }

        eMatchResult = cmdexp_CmdElementCmp(pstNode->acCmd, pcCmd);

        if (eMatchResult == _CMD_EXP_EXACTMACTH) {
            return pstNode;
        }

        if (eMatchResult == _CMD_EXP_PREMACTH) {
            if (BIT_ISSET(pstNode->uiProperty, DEF_CMD_EXP_PROPERTY_HIDE)) {
                uiHideCmdCount ++;
                pstNodeFoundHide = pstNode;
            } else {
                uiVisableCmdCount ++;
                pstNodeFoundVisable = pstNode;
            }
        }
    }

    if (uiVisableCmdCount > 1) {
        EXEC_OutInfo(" There is more than one elements match the cmd element:%s.\r\n", pcCmd);
        EXEC_Flush();
        return NULL;
    }

    if (uiVisableCmdCount == 1) {
        return pstNodeFoundVisable;
    }
    else if (uiHideCmdCount == 1)
    {
        return pstNodeFoundHide;
    }

    return NULL;
}

static _CMD_EXP_TREE_S * cmdexp_FindCmd
(
    _CMD_EXP_RUNNER_S *pstRunner,
    _CMD_EXP_TREE_S *pstRoot,
    UINT uiArgc,
    OUT CHAR **ppArgv
)
{
    _CMD_EXP_TREE_S *pstNode = NULL;

    pstNode = cmdexp_FindCmdElement(pstRunner, pstRoot, ppArgv[0]);
    if (pstNode == NULL) {
        return NULL;
    }

    if (ppArgv[0][0] == '-') { 
        return pstNode;
    }

    if (CMD_EXP_IS_VIEW(pstNode->uiType)) {
        if (uiArgc != 1) {
            return NULL;
        }
    }

    if (pstNode->acCmd[0] != '%')  { 
        ppArgv[0] = pstNode->acCmd;
    }

    if (uiArgc > 1) {
        return cmdexp_FindCmd(pstRunner, pstNode, uiArgc - 1, ppArgv + 1);
    }

    return pstNode;
}

static VOID cmdexp_GetElement(IN CHAR *pcCmd, OUT _CMD_EXP_ELEMENT_S *pstEle)
{
    CHAR *pcTmp = pcCmd;
    BOOL_T bIsHelp = FALSE;

    memset(pstEle, 0, sizeof(_CMD_EXP_ELEMENT_S));
    pstEle->pcCmd = pcCmd;

    while (*pcTmp != '\0')
    {
        if (FALSE == bIsHelp)
        {
            if (*pcTmp == '(') 
            {
                bIsHelp = TRUE;
                *pcTmp = '\0';
                pstEle->pcHelp = pcTmp + 1;
            }
            else if (*pcTmp == ' ') 
            {
                *pcTmp = '\0';
                pstEle->pcNext = pcTmp + 1;
                break;
            }
        }
        else  
        {
            if (*pcTmp == ')')   
            {
                *pcTmp = '\0';
                bIsHelp = FALSE;
            }
        }

        pcTmp ++;
    }

    return;
}

static int cmdexp_cmd_cmp(DLL_NODE_S *pstNode1, DLL_NODE_S *pstNode2, HANDLE hUserHandle)
{
    _CMD_EXP_TREE_S *node1 = (void*)pstNode1;
    _CMD_EXP_TREE_S *node2 = (void*)pstNode2;
    return strcmp(node1->acCmd, node2->acCmd);
}

static void cmdexp_AddSubCmd(IN _CMD_EXP_TREE_S *pstRoot, IN _CMD_EXP_TREE_S *pstNodeToAdd)
{
    pstNodeToAdd->pstParent = pstRoot;
    DLL_SortAdd(pstRoot->subcmd_list, &pstNodeToAdd->link_node, cmdexp_cmd_cmp, NULL);
}

static inline void cmdexp_DelSubCmd(IN _CMD_EXP_TREE_S *pstRoot, IN _CMD_EXP_TREE_S *pstNodeToDel)
{
    DLL_DEL(pstRoot->subcmd_list, &pstNodeToDel->link_node);
}

static int cmdexp_DelCmd(CMD_EXP_S *cmdexp, _CMD_EXP_TREE_S *pstRoot, char *pcCmd)
{
    _CMD_EXP_TREE_S *pstNode;
    _CMD_EXP_ELEMENT_S stEle;
    UINT uiLen;

    cmdexp_GetElement(pcCmd, &stEle);

    uiLen = strlen(stEle.pcCmd);

    if ((uiLen > _DEF_CMD_EXP_MAX_CMD_ELEMENT_LEN) || (uiLen == 0))
    {
        BS_WARNNING(("The cmd element length is too long, cmd:%s", stEle.pcCmd));
        RETURN(BS_NOT_SUPPORT);
    }

    pstNode = cmdexp_FindCmdElementExact(pstRoot, stEle.pcCmd);
    if (pstNode == NULL) {
        return BS_OK;
    } 

    if (stEle.pcNext == NULL) { 
        pstNode->pfFunc = NULL;
        pstNode->hParam = NULL;
        if (DLL_COUNT(pstNode->subcmd_list) == 0) {
            
            cmdexp_DelSubCmd(pstRoot, pstNode);
            MEM_Free(pstNode);
        }
    } else {
        cmdexp_DelCmd(cmdexp, pstNode, stEle.pcNext);
        if (! _DEF_CMD_EXP_IS_CMD_RUNABLE(pstNode)) {
            if (DLL_COUNT(pstNode->subcmd_list) == 0) {
                cmdexp_DelSubCmd(pstRoot, pstNode);
                MEM_Free(pstNode);
            }
        }
    }

    return BS_OK;
}

static void cmdexp_RegView
(
    CMD_EXP_S *cmdexp,
    _CMD_EXP_VIEW_S *pstViewTblNode,
    _CMD_EXP_TREE_S *pstNode,
    char *view_name
)
{
    _CMD_EXP_VIEW_S *view;

    view = cmdexp_FindView(cmdexp, view_name);
    if (!view) {
        view = cmdexp_AddSubView(pstViewTblNode, view_name);
        if (! view) {
            return;
        }
        view->property = pstNode->uiProperty;
        cmdexp_WalkPatternCmdToRegView(cmdexp, view_name);
    }

    pstNode->view = view;
}

static _CMD_EXP_TREE_S * cmdexp_BuildNode(_CMD_EXP_ELEMENT_S *ele, CMD_EXP_REG_CMD_PARAM_S *param)
{
    _CMD_EXP_TREE_S *pstNode = MEM_ZMalloc(sizeof(_CMD_EXP_TREE_S));
    if (pstNode == NULL) {
        BS_WARNNING(("No memory!"));
        return NULL;
    }

    DLL_INIT(&pstNode->_subcmds);
    pstNode->subcmd_list = &pstNode->_subcmds;
    strlcpy(pstNode->acCmd, ele->pcCmd, sizeof(pstNode->acCmd));
    pstNode->pcHelp = TXT_Strdup(ele->pcHelp);
    pstNode->uiProperty = param->uiProperty;
    pstNode->uiType = param->uiType;
    pstNode->level = param->level;
    if (ele->pcNext != NULL) {
        
        BIT_CLR(pstNode->uiType, DEF_CMD_EXP_TYPE_VIEW); 
    }

    return pstNode;
}

static int cmdexp_AddCmd
(
    IN CMD_EXP_S *cmdexp,
    IN _CMD_EXP_VIEW_S *pstViewTblNode,
    IN _CMD_EXP_TREE_S *pstRoot,
    IN CHAR *pcCmd,
    IN CMD_EXP_REG_CMD_PARAM_S *param,
    UINT flag
)
{
    _CMD_EXP_TREE_S *pstNode;
    _CMD_EXP_ELEMENT_S stEle;
    UINT uiLen;
    UINT uiType = param->uiType;
    UINT uiProperty = param->uiProperty;

    cmdexp_GetElement(pcCmd, &stEle);

    uiLen = strlen(stEle.pcCmd);

    if ((uiLen > _DEF_CMD_EXP_MAX_CMD_ELEMENT_LEN) || (uiLen == 0))
    {
        BS_WARNNING(("The cmd element length is too long, cmd:%s", stEle.pcCmd));
        RETURN(BS_NOT_SUPPORT);
    }

    pstNode = cmdexp_FindCmdElementExact(pstRoot, stEle.pcCmd);
    if (pstNode == NULL) {
        pstNode = cmdexp_BuildNode(&stEle, param);
        if (! pstNode) {
            RETURN(BS_NO_MEMORY);
        }
        cmdexp_AddSubCmd(pstRoot, pstNode);
    } else {
        if (stEle.pcNext == NULL) { 
            if (_DEF_CMD_EXP_IS_CMD_RUNABLE(pstNode)) {
                BS_WARNNING(("The cmd(%s) is exist, view=%s ", pcCmd, pstViewTblNode->view_name));
                RETURN(BS_ALREADY_EXIST);
            } 

            if (! BIT_ISSET(pstNode->uiProperty, DEF_CMD_EXP_PROPERTY_HIDE)) {
                uiProperty &= ~((UINT)DEF_CMD_EXP_PROPERTY_HIDE);
            } 
            pstNode->uiProperty |= uiProperty;
        }

        pstNode->level = MAX(pstNode->level, param->level); 
        
        if (! BIT_ISSET(uiProperty, DEF_CMD_EXP_PROPERTY_HIDE)) {
            BIT_CLR(pstNode->uiProperty, DEF_CMD_EXP_PROPERTY_HIDE);
        }
    }

    if (stEle.pcNext == NULL) { 
        pstNode->pfFunc = param->pfFunc;
        pstNode->hParam = param->hParam;
        if (flag & DEF_CMD_REG_FLAG_CYCLE) {
            pstNode->subcmd_list = pstNode->pstParent->subcmd_list;
        }
        if (CMD_EXP_IS_VIEW(uiType)) {
            cmdexp_RegView(cmdexp, pstViewTblNode, pstNode, param->pcViewName);
        }
    } else {
        cmdexp_AddCmd(cmdexp, pstViewTblNode, pstNode, stEle.pcNext, param, flag);
    }

    return BS_OK;
}

static char * cmdexp_ProcessSingleCmd(char *cmd, char *out, int out_size)
{
    unsigned ulLen;

    ulLen = strlen(cmd);
    if (ulLen == 0) {
        BS_WARNNING(("Cmd length is 0!"));
        return NULL;
    }
    if (ulLen > DEF_CMD_EXP_MAX_CMD_LEN) {
        BS_WARNNING(("Cmd length exceed!"));
        return NULL;
    }

    TXT_ReplaceSubStr(cmd, "  ", " ", out, out_size);

    ulLen = strlen(out);
    if (ulLen == 0) {
        BS_WARNNING(("Cmd length is 0!"));
        return NULL;
    }

    return TXT_Strim(out);
}

static int cmdexp_UnregSingleCmd(IN CMD_EXP_S *cmdexp, IN char *pcView, IN char *pcCmd)
{
    _CMD_EXP_VIEW_S *pstViewTblNode;
    char acCmd[DEF_CMD_EXP_MAX_CMD_LEN + 1];
    char *pcCmdTmp;
    
    if ((pcView == NULL) || (pcCmd == NULL)) {
        BS_WARNNING(("Null ptr!"));
        RETURN(BS_NULL_PARA);
    }

    pstViewTblNode = cmdexp_FindView(cmdexp, pcView);
    if (pstViewTblNode == NULL) {  
        BS_DBG_WARNNING(("The %s view does not exist!", pcView));
        RETURN(BS_NO_SUCH);
    }

    pcCmdTmp = cmdexp_ProcessSingleCmd(pcCmd, acCmd, sizeof(acCmd));
    if (! pcCmdTmp) {
        RETURN(BS_ERR);
    }

    return cmdexp_DelCmd(cmdexp, &pstViewTblNode->stCmdRoot, pcCmdTmp);
}


static int cmdexp_RegSingleCmd
(
    CMD_EXP_S *cmdexp,
    CHAR *pcView,
    CHAR *pcCmd,
    CMD_EXP_REG_CMD_PARAM_S *param,
    UINT flag
)
{
    _CMD_EXP_VIEW_S *pstView;
    CHAR            acCmd[DEF_CMD_EXP_MAX_CMD_LEN + 1];
    CHAR            *pcCmdTmp;
    
    if ((pcView == NULL) || (pcCmd == NULL))
    {
        BS_WARNNING(("Null ptr!"));
        RETURN(BS_NULL_PARA);
    }

    pstView = cmdexp_FindView(cmdexp, pcView);
    if (! pstView) {  
        BS_DBG_WARNNING(("The %s view does not exist!", pcView));
        RETURN(BS_NO_SUCH);
    }

    pcCmdTmp = cmdexp_ProcessSingleCmd(pcCmd, acCmd, sizeof(acCmd));
    if (! pcCmdTmp) {
        RETURN(BS_ERR);
    }

    return cmdexp_AddCmd(cmdexp, pstView, &pstView->stCmdRoot, pcCmdTmp, param, flag);
}

static VOID cmdexp_OutPutByFile(IN FILE *fp, IN CHAR *pcString)
{
    if (fp) {
        fputs(pcString, fp);
    } else {
        EXEC_OutString(pcString);
    }
}



BOOL_T CmdExp_IsShowing(IN HANDLE hFileHandle)
{
    _CMD_EXP_SHOW_SAVE_NODE_S *pstSSNode = hFileHandle;

    if (pstSSNode->fp == NULL) {
        return TRUE;
    }

    return FALSE;
}


BOOL_T CmdExp_IsSaving(IN HANDLE hFileHandle)
{
    _CMD_EXP_SHOW_SAVE_NODE_S *pstSSNode = hFileHandle;

    if (pstSSNode->fp == NULL) {
        return FALSE;
    }

    return TRUE;
}

static int cmdexp_save_OutputString(_CMD_EXP_SHOW_SAVE_NODE_S *pstSSNode, CHAR *pcString)
{
    UINT i;

    pcString = TXT_StrimHead(pcString, strlen(pcString), TXT_BLANK_CHARS);

    for (i=0; i<pstSSNode->ucPrefixSpaceNum; i++) {
        cmdexp_OutPutByFile(pstSSNode->fp, "    ");
    }

    cmdexp_OutPutByFile(pstSSNode->fp, pcString);
    cmdexp_OutPutByFile(pstSSNode->fp, "\r\n");

    return BS_OK;
}


static _CMD_EXP_NEW_MODE_NODE_S *cmdexp_save_GetToMatchMode(IN _CMD_EXP_SHOW_SAVE_NODE_S *pstSSNode)
{
    UINT uiIndex = pstSSNode->ucMatchedModeIndex;

    if (uiIndex < HSTACK_GetCount(pstSSNode->pstEnv->pstRunner->hModeStack)) {
        return HSTACK_GetValueByIndex(pstSSNode->pstEnv->pstRunner->hModeStack, uiIndex);
    }

    if (uiIndex == HSTACK_GetCount(pstSSNode->pstEnv->pstRunner->hModeStack)) {
        return pstSSNode->pstEnv->pstRunner->pstCurrentMode;
    }

    return NULL;
}


static VOID cmdexp_OutputPreMode(IN _CMD_EXP_SHOW_SAVE_NODE_S *pstSSNode)
{
    UINT uiStackCount;
    UINT i;
    char *mode;

    
    if (pstSSNode->bHasCfg) {
        return;
    }

    pstSSNode->bHasCfg = TRUE;

    uiStackCount = HSTACK_GetCount(pstSSNode->hStack);

    
    for (i=pstSSNode->ucMatchedModeIndex - 1; i<uiStackCount; i++) {
        mode = HSTACK_GetValueByIndex(pstSSNode->hStack, i);
        cmdexp_save_OutputString(pstSSNode, mode);
        pstSSNode->ucPrefixSpaceNum ++;
    }
}

static void cmdexp_OutputPreModeQuit(IN _CMD_EXP_SHOW_SAVE_NODE_S *pstSSNode)
{
    UINT uiStackCount;
    UINT i;

    if (! pstSSNode->bHasCfg) {
        return;
    }

    uiStackCount = HSTACK_GetCount(pstSSNode->hStack);

    
    for (i=pstSSNode->ucMatchedModeIndex - 1; i<uiStackCount; i++) {
        cmdexp_save_OutputString(pstSSNode, "quit");
        pstSSNode->ucPrefixSpaceNum --;
    }
}

static BOOL_T cmdexp_MatchEnvMode(_CMD_EXP_SHOW_SAVE_NODE_S *pstSSNode, char *cmd)
{
    _CMD_EXP_NEW_MODE_NODE_S *pstMode;

    if (pstSSNode->matched_mode) {
        return TRUE;
    }

    if (pstSSNode->ucModeDeepth > pstSSNode->ucMatchedModeIndex) {
        return FALSE;
    }

    pstMode = cmdexp_save_GetToMatchMode(pstSSNode);

    if (NULL == pstMode) {
        BS_DBGASSERT(0);
        return FALSE;
    }

    if (strcmp(pstMode->cmd, cmd) == 0) {
        pstSSNode->ucMatchedModeIndex ++;
        
        if (pstSSNode->ucMatchedModeIndex == HSTACK_GetCount(pstSSNode->pstEnv->pstRunner->hModeStack) + 1) {
            pstSSNode->matched_mode = TRUE;
        }

        return TRUE;
    }

    return FALSE;
}

int CmdExp_OutputMode(IN HANDLE hFileHandle, IN CHAR *fmt, ...)
{
    va_list args;
    CHAR acMsg[DEF_CMD_EXP_MAX_CMD_LEN + 1];
    _CMD_EXP_SHOW_SAVE_NODE_S *pstSSNode = hFileHandle;

    fmt = TXT_StrimHead(fmt, strlen(fmt), TXT_BLANK_CHARS);
    va_start(args, fmt);
    vsnprintf(acMsg, sizeof(acMsg), fmt, args);
    va_end(args);

    if (pstSSNode->matched_mode) {
        int bPermitOutput = CmdExp_IsOptPermitOutput(hFileHandle, acMsg);
        if (! bPermitOutput) {
            return BS_STOP;
        }

        cmdexp_OutputPreMode(pstSSNode);
        cmdexp_save_OutputString(hFileHandle, acMsg);
        pstSSNode->ucPrefixSpaceNum ++;
    } else if (! cmdexp_MatchEnvMode(pstSSNode, acMsg)) {
        return BS_STOP; 
    }

    pstSSNode->ucModeDeepth ++;

    return BS_OK;
}

int CmdExp_OutputModeQuit(IN HANDLE hFileHandle)
{
    _CMD_EXP_SHOW_SAVE_NODE_S *pstSSNode = hFileHandle;
    BOOL_T bNeedDec = FALSE;

    BS_DBGASSERT(pstSSNode->ucModeDeepth > 1); 

    if (pstSSNode->matched_mode) {
        bNeedDec = TRUE;
    }

    if (pstSSNode->ucModeDeepth == pstSSNode->ucMatchedModeIndex) {
        pstSSNode->ucMatchedModeIndex --;
        pstSSNode->matched_mode = FALSE;
    }

    pstSSNode->ucModeDeepth --;

    if (pstSSNode->matched_mode) {
        cmdexp_save_OutputString(hFileHandle, "quit");
    }

    if (bNeedDec) {
        pstSSNode->ucPrefixSpaceNum --;
    }

    return BS_OK;
}

int CmdExp_OutputCmd(IN HANDLE hFileHandle, IN CHAR *fmt, ...)
{
    va_list args;
    _CMD_EXP_SHOW_SAVE_NODE_S *pstSSNode = hFileHandle;
    CHAR acMsg[DEF_CMD_EXP_MAX_CMD_LEN + 1];

    fmt = TXT_StrimHead(fmt, strlen(fmt), TXT_BLANK_CHARS);

    va_start(args, fmt);
    BS_Vsnprintf(acMsg, sizeof(acMsg), fmt, args);
    va_end(args);

    if (! CmdExp_IsOptPermitOutput(hFileHandle, acMsg)) {
        return BS_STOP;
    }

    cmdexp_OutputPreMode(pstSSNode);

    if (! pstSSNode->matched_mode) { 
        return BS_OK;
    }

    return cmdexp_save_OutputString(hFileHandle, acMsg);
}

void * CmdExp_GetEnvBySaveHandle(HANDLE hFileHandle)
{
    _CMD_EXP_SHOW_SAVE_NODE_S *pstSSNode = hFileHandle;
    return pstSSNode->pstEnv;
}

int CmdExp_GetMucIDBySaveHandle(HANDLE hFileHandle)
{
    _CMD_EXP_SHOW_SAVE_NODE_S *pstSSNode = hFileHandle;
    return pstSSNode->pstEnv->pstRunner->muc_id;
}

static void cmdexp_BuildSaveFileName(_CMD_EXP_SHOW_SAVE_NODE_S *pstSSNode,
        _CMD_EXP_SVAE_NODE_S *pstSaveNode, int muc_id, OUT char *filename, int size)
{
    int sizetmp = size;
    char *runner_dir = pstSSNode->pstEnv->pstRunner->runner_dir;

    if (runner_dir) {
        int len = snprintf(filename, sizetmp, "%s/", runner_dir);
        filename += len;
        sizetmp -= len;
    }

    if (muc_id == 0) {
        snprintf(filename, sizetmp, "%s/config.cfg", pstSaveNode->save_path);
    } else {
        snprintf(filename, sizetmp, "muc_%d/config.cfg", muc_id);
    }
}

static FILE * cmdexp_OpenSaveFile(_CMD_EXP_SHOW_SAVE_NODE_S *pstSSNode,
        _CMD_EXP_SVAE_NODE_S *pstSaveNode, int muc_id)
{
    char filename[FILE_MAX_PATH_LEN + 1];

    cmdexp_BuildSaveFileName(pstSSNode, pstSaveNode, muc_id, filename, sizeof(filename));
    return FILE_Open(filename, TRUE, "ab");
}

static int cmdexp_Save(_CMD_EXP_SHOW_SAVE_NODE_S *pstSSNode, _CMD_EXP_VIEW_S *pstViewTbl)
{
    FILE *fq = NULL;
    UINT uiStackCount = HSTACK_GetCount(pstSSNode->hStack);
    _CMD_EXP_SVAE_NODE_S *pstSaveNode;
    _CMD_EXP_RUNNER_S *runner = pstSSNode->pstEnv->pstRunner;

    if (uiStackCount == 0) {
        return BS_ERR;
    }

    DLL_SCAN(&pstViewTbl->stSaveFuncList, pstSaveNode) {
        if (pstSaveNode->level < runner->level) {
            continue;
        }

        fq = cmdexp_OpenSaveFile(pstSSNode, pstSaveNode, runner->muc_id);
        if (fq == NULL) {
            continue;
        }

        pstSSNode->bIsNewFile = TRUE;
        pstSSNode->bHasCfg = FALSE;
        pstSSNode->fp = fq;

        g_cmd_exp_thread_env = pstSSNode->pstEnv;
        pstSaveNode->pfFunc(pstSSNode);
        g_cmd_exp_thread_env = NULL;

        cmdexp_OutputPreModeQuit(pstSSNode);

        fclose(fq);
    }

    return BS_OK;
}

static int cmdexp_CmdShow(_CMD_EXP_SHOW_SAVE_NODE_S *pstSSNode, _CMD_EXP_VIEW_S *pstViewTbl)
{
    _CMD_EXP_SVAE_NODE_S *pstSaveNode;
    _CMD_EXP_RUNNER_S *runner = pstSSNode->pstEnv->pstRunner;

    pstSSNode->bHasCfg = FALSE; 
    pstSSNode->bIsNewFile = FALSE;
    pstSSNode->fp = 0;

    DLL_SCAN(&pstViewTbl->stSaveFuncList, pstSaveNode) {
        if (runner->level <= pstSaveNode->level) {
            g_cmd_exp_thread_env = pstSSNode->pstEnv;
            pstSaveNode->pfFunc(pstSSNode);
            g_cmd_exp_thread_env = NULL;
        }
    }

    cmdexp_OutputPreModeQuit(pstSSNode);

    return BS_OK;
}

static VOID cmdexp_SaveOrShow(_CMD_EXP_VIEW_S *pstViewTbl,
 BOOL_T bIsShow, _CMD_EXP_SHOW_SAVE_NODE_S *pstSSNode)
{
    _CMD_EXP_VIEW_S *pstNode;

    
    if (! cmdexp_MatchEnvMode(pstSSNode, pstViewTbl->view_name)) {
        return;
    }

    pstSSNode->ucModeDeepth ++;

    if (bIsShow) {
        cmdexp_CmdShow(pstSSNode, pstViewTbl);
    } else {
        cmdexp_Save(pstSSNode, pstViewTbl);
    }

    DLL_SCAN(&pstViewTbl->stSubViewList, pstNode) {
        HSTACK_Push(pstSSNode->hStack, pstNode->view_name);
        cmdexp_SaveOrShow(pstNode, bIsShow, pstSSNode);
        HSTACK_Pop(pstSSNode->hStack, NULL);
    }

    if (pstSSNode->ucModeDeepth == pstSSNode->ucMatchedModeIndex) {
        pstSSNode->ucMatchedModeIndex --;
        pstSSNode->matched_mode = FALSE;
    }

    pstSSNode->ucModeDeepth --;
}

static void cmdexp_DeleteCfgFile(_CMD_EXP_SHOW_SAVE_NODE_S *pstSSNode,
        _CMD_EXP_SVAE_NODE_S *pstSaveNode, int muc_id)
{
    char filename[FILE_MAX_PATH_LEN + 1];

    cmdexp_BuildSaveFileName(pstSSNode, pstSaveNode, muc_id, filename, sizeof(filename));
    FILE *fp = FOPEN(filename, "wb+");
    if (fp != NULL) {
        fclose(fp);
    }
}

static VOID cmdexp_DeleteAllCfgFile(_CMD_EXP_VIEW_S *pstViewTreeNode, _CMD_EXP_SHOW_SAVE_NODE_S *pstSSNode)
{
    _CMD_EXP_VIEW_S *pstNode;
    _CMD_EXP_SVAE_NODE_S *pstSaveNode;
    _CMD_EXP_RUNNER_S *runner = pstSSNode->pstEnv->pstRunner;

    DLL_SCAN(&pstViewTreeNode->stSaveFuncList, pstSaveNode) {
        if (runner->level <= pstSaveNode->level) {
            cmdexp_DeleteCfgFile(pstSSNode, pstSaveNode, runner->muc_id);
        }
    }

    DLL_SCAN(&(pstViewTreeNode->stSubViewList), pstNode) {
        cmdexp_DeleteAllCfgFile(pstNode, pstSSNode);
    }
}

int CmdExp_CmdSave(UINT ulArgc, CHAR **pArgv, VOID *pEnv)
{
    _CMD_EXP_ENV_S *pstEnv = pEnv;
    CMD_EXP_S *cmdexp = pstEnv->pstRunner->cmdexp;
    HANDLE hStack;
    _CMD_EXP_SHOW_SAVE_NODE_S stSSNode = {0};

    hStack = HSTACK_Create(0);
    if (NULL == hStack) {
        return BS_NO_MEMORY;
    }

    stSSNode.hStack = hStack;
    stSSNode.pstEnv = pEnv;
    stSSNode.matched_mode = TRUE;
    stSSNode.ucMatchedModeIndex ++;

    cmdexp_DeleteAllCfgFile(&cmdexp->stRootView, &stSSNode);
    cmdexp_SaveOrShow(&cmdexp->stRootView, FALSE, &stSSNode);

    HSTACK_Destory(hStack);

    return BS_OK;
}

int CmdExp_CmdShow(UINT ulArgc, CHAR **pArgv, VOID *pEnv)
{
    HANDLE hStack;
    _CMD_EXP_ENV_S *pstEnv = pEnv;
    CMD_EXP_S *cmdexp = pstEnv->pstRunner->cmdexp;
    _CMD_EXP_SHOW_SAVE_NODE_S stSSNode = {0};

    hStack = HSTACK_Create(0);
    if (NULL == hStack) {
        return BS_NO_MEMORY;
    }

    stSSNode.hStack = hStack;
    stSSNode.pstEnv = pEnv;

    
    if ((ulArgc >= 3) && (pArgv[2][0] == 'a')) {
        stSSNode.bIsShowAll = 1;
    }

    if (pstEnv->pstRunner->pstCurrentMode->pstViewTbl == &cmdexp->stRootView) {
        stSSNode.matched_mode = TRUE;
        stSSNode.ucMatchedModeIndex ++;
    }

    cmdexp_SaveOrShow(&cmdexp->stRootView, TRUE, &stSSNode);

    HSTACK_Destory(hStack);

    EXEC_Flush();

    return BS_OK;
}

int CmdExp_CmdNoDebugAll(UINT ulArgc, CHAR **pArgv, VOID *pEnv)
{
    _CMD_EXP_ENV_S *pstEnv = pEnv;
    CMD_EXP_S *cmdexp = pstEnv->pstRunner->cmdexp;
    CMD_EXP_NO_DBG_NODE_S *pstNode;

    DLL_SCAN(&cmdexp->stNoDbgFuncList, pstNode)
    {
        pstNode->pfNoDbgFunc();
    }

    return BS_OK;
}

VOID CmdExp_RegNoDbgFunc(CMD_EXP_HDL hCmdExp, CMD_EXP_NO_DBG_NODE_S *pstNode)
{
    CMD_EXP_S *cmdexp = hCmdExp;

    DLL_ADD(&cmdexp->stNoDbgFuncList, pstNode);
}

static int cmdexp_UnregCmd(CMD_EXP_S *cmdexp, CHAR *pcView, CHAR *pcCmd)
{
    CHAR *pcFindStart, *pcFindEnd;
    CHAR acCmdTmp[_DEF_CMD_EXP_MAX_CMD_ELEMENT_IN_ONE_CMD * (_DEF_CMD_EXP_MAX_CMD_ELEMENT_LEN + 1) + 1];

    TXT_Strlcpy(acCmdTmp, pcCmd, sizeof(acCmdTmp));

    

    
    if (BS_OK == TXT_FindBracket(acCmdTmp, strlen(pcCmd),
                "[]", &pcFindStart, &pcFindEnd)) {
        *pcFindStart = ' ';
        *pcFindEnd = ' ';
        cmdexp_UnregCmd(cmdexp, pcView, acCmdTmp);

        
        TXT_StrCpy (acCmdTmp, pcCmd);
        while (pcFindStart <= pcFindEnd) {
            *pcFindStart = ' ';
            pcFindStart ++;
        }
        cmdexp_UnregCmd(cmdexp, pcView, acCmdTmp);

        return BS_OK;
    }

    
    if (BS_OK == TXT_FindBracket(acCmdTmp, strlen(pcCmd),
                "{}", &pcFindStart, &pcFindEnd)) {
        CHAR *pcFindOr;
        CHAR *pcCopyStart;
        UINT ulCopyLen;

        pcCopyStart = pcCmd + ((pcFindStart - acCmdTmp) + 1);

        for (;;) {
            
            pcFindOr = pcFindStart;
            while (pcFindOr <= pcFindEnd) {
                *pcFindOr = ' ';
                pcFindOr ++;
            }

            pcFindOr = strchr (pcCopyStart, '|');
            if ((pcFindOr == NULL)
                    || ((pcFindOr - pcCmd) > (pcFindEnd - acCmdTmp))) {
                
                ulCopyLen = (UINT)((pcFindEnd - acCmdTmp) - (pcCopyStart - pcCmd));
                MEM_Copy(pcFindStart, pcCopyStart, ulCopyLen);
                pcFindStart[ulCopyLen] = ' ';
                cmdexp_UnregCmd(cmdexp, pcView, acCmdTmp);

                break;
            }

            ulCopyLen = (UINT)(pcFindOr - pcCopyStart);
            MEM_Copy(pcFindStart, pcCopyStart, ulCopyLen);
            pcFindStart[ulCopyLen] = ' ';

            cmdexp_UnregCmd(cmdexp, pcView, acCmdTmp);

            pcCopyStart = pcFindOr + 1;
            TXT_StrCpy(acCmdTmp, pcCmd);
        }

        return BS_OK;
    }

    
    return cmdexp_UnregSingleCmd(cmdexp, pcView, acCmdTmp);
}

static int cmdexp_RegCmd(CMD_EXP_S *cmdexp, CHAR *pcView, CHAR *pcCmd, CMD_EXP_REG_CMD_PARAM_S *param, UINT flag)
{
    CHAR *pcFindStart, *pcFindEnd, *pcClrEnd;
    CHAR acCmdTmp[_DEF_CMD_EXP_MAX_CMD_ELEMENT_IN_ONE_CMD * (_DEF_CMD_EXP_MAX_CMD_ELEMENT_LEN + 1) + 1];
    int len = strlen(pcCmd);

    strlcpy(acCmdTmp, pcCmd, sizeof(acCmdTmp));

    

    
    if (BS_OK == TXT_FindBracket(acCmdTmp, len, "[]", &pcFindStart, &pcFindEnd)) {
        *pcFindStart = ' ';
        *pcFindEnd = ' ';
        cmdexp_RegCmd(cmdexp, pcView, acCmdTmp, param, flag);

        
        strlcpy(acCmdTmp, pcCmd, sizeof(acCmdTmp));
        while (pcFindStart <= pcFindEnd) {
            *pcFindStart = ' ';
            pcFindStart ++;
        }
        cmdexp_RegCmd(cmdexp, pcView, acCmdTmp, param, flag);

        return BS_OK;
    }

    
    if (BS_OK == TXT_FindBracket(acCmdTmp, len, "{}", &pcFindStart, &pcFindEnd)) {
        CHAR *pcFindOr;
        CHAR *pcCopyStart;
        UINT ulCopyLen;
        UINT new_flag = flag;

        pcClrEnd = pcFindEnd;
        if (*(pcClrEnd + 1)== '*') {
            pcClrEnd ++;
            new_flag |= DEF_CMD_REG_FLAG_CYCLE;
        }

        pcCopyStart = pcCmd + ((pcFindStart - acCmdTmp) + 1);

        for (;;) {
            

            pcFindOr = pcFindStart;
            while (pcFindOr <= pcClrEnd) {
                *pcFindOr = ' ';
                pcFindOr ++;
            }

            pcFindOr = strchr (pcCopyStart, '|');
            if ((pcFindOr == NULL) || ((pcFindOr - pcCmd) > (pcFindEnd - acCmdTmp))) {
                
                ulCopyLen = (UINT)((pcFindEnd - acCmdTmp) - (pcCopyStart - pcCmd));
                if (ulCopyLen > 0) {
                    MEM_Copy(pcFindStart, pcCopyStart, ulCopyLen);
                    pcFindStart[ulCopyLen] = ' ';
                    cmdexp_RegCmd(cmdexp, pcView, acCmdTmp, param, new_flag);
                }

                break;
            }

            ulCopyLen = (UINT)(pcFindOr - pcCopyStart);
            MEM_Copy(pcFindStart, pcCopyStart, ulCopyLen);
            pcFindStart[ulCopyLen] = ' ';

            cmdexp_RegCmd(cmdexp, pcView, acCmdTmp, param, new_flag);

            pcCopyStart = pcFindOr + 1;
            strlcpy(acCmdTmp, pcCmd, sizeof(acCmdTmp));
        }

        return BS_OK;
    }

    
    return cmdexp_RegSingleCmd(cmdexp, pcView, acCmdTmp, param, flag);
}

static int cmdexp_RegSave(CMD_EXP_S *cmdexp, char *save_path, char *pcViewName, CMD_EXP_REG_CMD_PARAM_S *param)
{
    _CMD_EXP_VIEW_S *pstView;
    _CMD_EXP_SVAE_NODE_S *pstSaveNode = NULL;

    if (param->pfFunc == NULL) {
        BS_WARNNING(("Null ptr!"));
        RETURN(BS_NULL_PARA);
    }

    pstView = cmdexp_FindView(cmdexp, pcViewName);
    if (NULL == pstView) {
        RETURN(BS_NO_SUCH);
    }

    pstSaveNode =  MEM_ZMalloc(sizeof(_CMD_EXP_SVAE_NODE_S));
    if (NULL == pstSaveNode) {
        RETURN(BS_NO_MEMORY);
    }

    pstSaveNode->pfFunc = param->pfFunc;
    strlcpy(pstSaveNode->save_path, save_path, sizeof(pstSaveNode->save_path));

    pstSaveNode->level = param->level;

    DLL_ADD(&pstView->stSaveFuncList, pstSaveNode);

    return BS_OK;
}

static int cmdexp_RegEnter(CMD_EXP_S *cmdexp, char *pcViewName, CMD_EXP_REG_CMD_PARAM_S *param)
{
    _CMD_EXP_VIEW_S *pstView;
    _CMD_EXP_ENTER_NODE_S *pstEnterNode = NULL;

    if (param->pfFunc == NULL) {
        BS_WARNNING(("Null ptr!"));
        RETURN(BS_NULL_PARA);
    }

    pstView = cmdexp_FindView(cmdexp, pcViewName);
    if (NULL == pstView) {
        RETURN(BS_NO_SUCH);
    }

    pstEnterNode = MEM_ZMalloc(sizeof(_CMD_EXP_ENTER_NODE_S));
    if (NULL == pstEnterNode) {
        RETURN(BS_NO_MEMORY);
    }

    pstEnterNode->pfFunc = param->pfFunc;
    pstEnterNode->level = param->level;

    DLL_ADD(&pstView->stEnterFuncList, pstEnterNode);

    return BS_OK;
}

static int cmdexp_UnRegSave(CMD_EXP_S *cmdexp, char *save_path, CHAR *pcViewName)
{
    _CMD_EXP_VIEW_S *pstView;
    _CMD_EXP_SVAE_NODE_S *pstCmdNode = NULL;

    pstView = cmdexp_FindView(cmdexp, pcViewName);
    if (NULL == pstView) {
        RETURN(BS_NO_SUCH);
    }

    DLL_SCAN(&pstView->stSaveFuncList, pstCmdNode) {
        if (0 == strcmp(pstCmdNode->save_path, save_path)) {
            DLL_DEL(&pstView->stSaveFuncList, pstCmdNode);
            MEM_Free(pstCmdNode);
            break;
        }
    }

    return BS_OK;
}

int CmdExp_RegSave(CMD_EXP_HDL hCmdExp, char *save_path, CMD_EXP_REG_CMD_PARAM_S *param)
{
    CMD_EXP_S *cmdexp = hCmdExp;
    char *pcViews = param->pcViews;
    char *pcView;

    
    TXT_SCAN_ELEMENT_BEGIN(pcViews,'|',pcView) {
        cmdexp_RegSave(cmdexp, save_path, pcView, param);
    }TXT_SCAN_ELEMENT_END();

    return BS_OK;
}

int CmdExp_RegEnter(CMD_EXP_HDL hCmdExp, CMD_EXP_REG_CMD_PARAM_S *param)
{
    CMD_EXP_S *cmdexp = hCmdExp;
    char *pcViews = param->pcViews;
    char *pcView;

    
    TXT_SCAN_ELEMENT_BEGIN(pcViews,'|',pcView) {
        cmdexp_RegEnter(cmdexp, pcView, param);
    }TXT_SCAN_ELEMENT_END();

    return BS_OK;
}

int CmdExp_UnRegSave(CMD_EXP_HDL hCmdExp, CHAR *save_path, CHAR *pcViews)
{
    CMD_EXP_S *cmdexp = hCmdExp;
    CHAR *pcView;

    TXT_SCAN_ELEMENT_BEGIN(pcViews,'|',pcView) {
        cmdexp_UnRegSave(cmdexp, save_path, pcView);
    }TXT_SCAN_ELEMENT_END();

    return BS_OK;
}

int CmdExp_UnregCmd(CMD_EXP_HDL hCmdExp, CMD_EXP_REG_CMD_PARAM_S *pstParam)
{
    CMD_EXP_S *cmdexp = hCmdExp;
    CHAR *pcView;

    
    TXT_SCAN_ELEMENT_BEGIN(pstParam->pcViews,'|',pcView) {
        cmdexp_UnregCmd(cmdexp, pcView, pstParam->pcCmd);
    }TXT_SCAN_ELEMENT_END();

    return BS_OK;
}

int CmdExp_RegCmd(CMD_EXP_HDL hCmdExp, CMD_EXP_REG_CMD_PARAM_S *pstParam)
{
    CMD_EXP_S *cmdexp = hCmdExp;
    CHAR *pcView;

    if (CMD_EXP_IS_VIEW(pstParam->uiType)) {
        if (NULL == pstParam->pcViewName) {
            RETURN(BS_NULL_PARA);
        }
        
        if (strlen(pstParam->pcViewName) > _DEF_CMD_EXP_MAX_VIEW_NAME_LEN) {
            RETURN(BS_BAD_PARA);
        }

        if (cmdexp_FindView(cmdexp, pstParam->pcViewName) != NULL) {
            RETURN(BS_ALREADY_EXIST);
        }
    }

    if (pstParam->uiProperty & DEF_CMD_EXP_PROPERTY_VIEW_PATTERN) {
        return cmdexp_AddCmdViewPattern(cmdexp, pstParam);
    }

    
    TXT_SCAN_ELEMENT_BEGIN(pstParam->pcViews,'|',pcView) {
        cmdexp_RegCmd(cmdexp, pcView, pstParam->pcCmd, pstParam, 0);
    }TXT_SCAN_ELEMENT_END();

    return BS_OK;
}

int CmdExp_RegCmdSimple(CMD_EXP_HDL hCmdExp, char *view, char *cmd,
        PF_CMD_EXP_RUN func, void *ud)
{
    CMD_EXP_REG_CMD_PARAM_S stCmdParam = {0};

    stCmdParam.uiType = DEF_CMD_EXP_TYPE_CMD;
    stCmdParam.pcViews = view;
    stCmdParam.pcCmd = cmd;
    stCmdParam.pfFunc = func;
    stCmdParam.hParam = ud;

    return CmdExp_RegCmd(hCmdExp, &stCmdParam);
}

int CmdExp_UnregCmdSimple(CMD_EXP_HDL hCmdExp, char *view, char *cmd)
{
    CMD_EXP_REG_CMD_PARAM_S stCmdParam = {0};

    stCmdParam.uiType = DEF_CMD_EXP_TYPE_CMD;
    stCmdParam.pcViews = view;
    stCmdParam.pcCmd = cmd;

    return CmdExp_UnregCmd(hCmdExp, &stCmdParam);
}



CMD_EXP_RUNNER CmdExp_CreateRunner(CMD_EXP_HDL hCmdExp, UINT type)
{
    CMD_EXP_S *cmdexp = hCmdExp;
    _CMD_EXP_RUNNER_S *pstRunner;
    CHAR *ppArgv[1];

    pstRunner = MEM_ZMalloc(sizeof(_CMD_EXP_RUNNER_S));
    if (pstRunner == NULL) {
        BS_WARNNING(("No Memory!"));
        return NULL;
    }

    pstRunner->hExcharHandle = EXCHAR_Create();
    if (NULL == pstRunner->hExcharHandle) {
        CmdExp_DestroyRunner(pstRunner);
        return NULL;
    }

    pstRunner->hModeStack = HSTACK_Create(0);
    if (NULL == pstRunner->hModeStack) {
        EXCHAR_Delete(pstRunner->hExcharHandle);
        CmdExp_DestroyRunner(pstRunner);
        return NULL;
    }

    pstRunner->hCmdRArray = RArray_Create(100, DEF_CMD_EXP_MAX_CMD_LEN+1);
    if (pstRunner->hCmdRArray == NULL) {
        CmdExp_DestroyRunner(pstRunner);
        return NULL;
    }

    pstRunner->cmdexp = hCmdExp;
    pstRunner->runner_type = type;

    ppArgv[0] = cmdexp->stRootView.view_name;
    cmdexp_EnterMode(pstRunner, &cmdexp->stRootView, cmdexp->stRootView.property, 1, ppArgv);

    return pstRunner;
}

void CmdExp_AltEnable(CMD_EXP_RUNNER hRunner, int enable)
{
    _CMD_EXP_RUNNER_S *pstRunner = hRunner;

    pstRunner->alt_mode = 0;
    if (enable) {
        pstRunner->alt_mode = 1;
    }
}

void CmdExp_SetRunnerLevel(CMD_EXP_RUNNER hRunner, UCHAR level)
{
    _CMD_EXP_RUNNER_S *pstRunner = hRunner;
    pstRunner->level = level;
}

void CmdExp_SetRunnerMucID(CMD_EXP_RUNNER hRunner, int muc_id)
{
    _CMD_EXP_RUNNER_S *pstRunner = hRunner;
    pstRunner->muc_id = muc_id;
}

void CmdExp_SetViewPrefix(CMD_EXP_RUNNER hRunner, char *prefix)
{
    _CMD_EXP_RUNNER_S *pstRunner = hRunner;
    strlcpy(pstRunner->runner_name, prefix, sizeof(pstRunner->runner_name));
}

void CmdExp_SetRunnerDir(CMD_EXP_RUNNER hRunner, char *dir)
{
    _CMD_EXP_RUNNER_S *pstRunner = hRunner;
    pstRunner->runner_dir = dir;
}

void CmdExp_SetRunnerHook(CMD_EXP_RUNNER hRunner, CMD_EXP_HOOK_FUNC func, void *ud)
{
    _CMD_EXP_RUNNER_S *pstRunner = hRunner;
    pstRunner->hook_ud = ud;
    pstRunner->hook_func = func;
}

void CmdExp_SetRunnerHistory(CMD_EXP_RUNNER hRunner, BOOL_T enable)
{
    _CMD_EXP_RUNNER_S *pstRunner = hRunner;

    if (enable) {
        pstRunner->deny_history = 0;
    } else {
        pstRunner->deny_history = 1;
    }
}

void CmdExp_SetRunnerHookMode(CMD_EXP_RUNNER hRunner, BOOL_T enable)
{
    CmdExp_SetRunnerHistory(hRunner, !enable);
    CmdExp_SetRunnerHelp(hRunner, !enable);
    CmdExp_SetRunnerPrefixEnable(hRunner, !enable);
}

void CmdExp_SetRunnerHelp(CMD_EXP_RUNNER hRunner, BOOL_T enable)
{
    _CMD_EXP_RUNNER_S *pstRunner = hRunner;

    if (enable) {
        pstRunner->deny_help = 0;
    } else {
        pstRunner->deny_help = 1;
    }
}

void CmdExp_SetRunnerType(CMD_EXP_RUNNER hRunner, UINT type)
{
    _CMD_EXP_RUNNER_S *pstRunner = hRunner;
    pstRunner->runner_type = type;
}

UINT CmdExp_GetRunnerType(CMD_EXP_RUNNER hRunner)
{
    _CMD_EXP_RUNNER_S *pstRunner = hRunner;
    return pstRunner->runner_type;
}

void CmdExp_SetRunnerPrefixEnable(CMD_EXP_RUNNER hRunner, BOOL_T enable)
{
    _CMD_EXP_RUNNER_S *pstRunner = hRunner;

    if (enable) {
        pstRunner->deny_prefix = 0;
    } else {
        pstRunner->deny_prefix = 1;
    }
}

static void cmdexp_OutputPrefix(_CMD_EXP_RUNNER_S *pstRunner)
{
    if (pstRunner->sub_runner) {
        cmdexp_OutputPrefix(pstRunner->sub_runner);
        return;
    }

    if (pstRunner->deny_prefix) {
        return;
    }

    if (pstRunner->alt_mode) {
        if (pstRunner->runner_name[0]) {
            EXEC_OutInfo("%s-%s>", pstRunner->runner_name, pstRunner->pstCurrentMode->szModeName);
        } else {
            EXEC_OutInfo("%s>", pstRunner->pstCurrentMode->szModeName);
        }
    }
}


void CmdExp_RunnerOutputPrefix(CMD_EXP_RUNNER hRunner)
{
    _CMD_EXP_RUNNER_S *pstRunner = hRunner;
    if (NULL != pstRunner) {
        cmdexp_OutputPrefix(pstRunner);
        EXEC_Flush();
    }
}

static inline void cmdexp_ClearInputsBuf(_CMD_EXP_RUNNER_S *pstRunner)
{
    pstRunner->acInputs[0] = '\0';
    pstRunner->ulInputsLen = 0;
    pstRunner->uiInputsPos = 0;
}


void CmdExp_ResetRunner(CMD_EXP_RUNNER hRunner)
{
    _CMD_EXP_RUNNER_S *pstRunner = hRunner;

    if (pstRunner == NULL) {
        BS_WARNNING(("Null ptr!"));
        return;
    }

    if (NULL != pstRunner->hCmdRArray) {
        RArray_Reset(pstRunner->hCmdRArray);
    }

    if (NULL != pstRunner->hExcharHandle) {
        EXCHAR_Reset(pstRunner->hExcharHandle);
    }

    if (NULL != pstRunner->hModeStack) {
        int eRet;
        do {
            eRet = CmdExp_QuitMode(pstRunner);
        }while (eRet == BS_OK);

        HSTACK_Reset(pstRunner->hModeStack);
    }

    cmdexp_ClearInputsBuf(pstRunner);
    pstRunner->ulHistoryIndex = 0;
}

static int cmdexp_hook_notify(_CMD_EXP_RUNNER_S *runner, int event, void *data)
{
    _CMD_EXP_ENV_S stEnv = {0};

    stEnv.pstRunner = runner;

    return runner->hook_func(event, data, runner->hook_ud, &stEnv);
}


int CmdExp_DestroyRunner(CMD_EXP_RUNNER hRunner)
{
    _CMD_EXP_RUNNER_S *pstRunner = hRunner;
    int eRet;
    
    if (pstRunner == NULL) {
        BS_WARNNING(("Null ptr!"));
        RETURN(BS_NULL_PARA);
    }

    if (pstRunner->hook_func) {
        cmdexp_hook_notify(pstRunner, CMD_EXP_HOOK_EVENT_DESTROY, NULL);
    }

    if (NULL != pstRunner->hCmdRArray) {
        RArray_Delete(pstRunner->hCmdRArray);
    }

    if (NULL != pstRunner->hExcharHandle) {
        EXCHAR_Delete(pstRunner->hExcharHandle);
    }

    if (NULL != pstRunner->hModeStack) {
        do {
            eRet = CmdExp_QuitMode(pstRunner);
        }while (eRet == BS_OK);

        HSTACK_Destory(pstRunner->hModeStack);
    }

    if (pstRunner->pstCurrentMode) {
        cmdexp_FreeModeNode(pstRunner->pstCurrentMode);
    }

    MEM_Free(pstRunner);
    
    return BS_OK;    
}

static int cmdexp_CmdHelp(_CMD_EXP_RUNNER_S *pstRunner)
{
    CHAR *pcSplit,  *pcHelpCmd;
    CHAR acCmdTmp[DEF_CMD_EXP_MAX_CMD_LEN + 1];
    _CMD_EXP_TREE_S *pstNode, *pstNodeNext;
    UINT           ulLen;
    _CMD_EXP_TREE_S *pstFindView = NULL;

    TXT_StrCpy(acCmdTmp, pstRunner->acInputs);
    pcSplit = acCmdTmp;
    pcHelpCmd  = acCmdTmp;

    pstNode = &pstRunner->pstCurrentMode->pstViewTbl->stCmdRoot;

    do {
        pcSplit = strchr(pcSplit, ' ');
        if (pcSplit != NULL) {
            *pcSplit = '\0';
            
            pstNode = cmdexp_FindCmdElement(pstRunner, pstNode, pcHelpCmd);
            if (pstNode == NULL) {
                RETURN(BS_NO_SUCH);
            }

            if (CMD_EXP_IS_VIEW(pstNode->uiType)) {
                pstFindView = pstNode;
            }

            pcSplit++;
            pcHelpCmd = pcSplit;
        }
    }while (pcSplit);

    if (pstFindView != NULL)  { 
        if (pstNode != pstFindView)  { 
            RETURN(BS_NO_SUCH);
        } else {
            if (! BIT_ISSET(pstNode->uiProperty, DEF_CMD_EXP_PROPERTY_HIDE_CR)) {
                EXEC_OutString(" <CR>\r\n");
            }
            return BS_OK;
        }
    }

    ulLen = strlen(pcHelpCmd);
 
    DLL_SCAN(pstNode->subcmd_list, pstNodeNext) {
        if (pstNodeNext->level < pstRunner->level) {
            continue;
        }
        if (pstNodeNext->uiProperty & DEF_CMD_EXP_PROPERTY_HIDE) {
            continue;
        }
        if ((ulLen == 0 )
                || (strncmp(pstNodeNext->acCmd, pcHelpCmd, ulLen) == 0)) {
            if ((pstRunner->alt_mode)
                    || (pstNodeNext->pfFunc != cmdexp_QuitCmd)) {
                EXEC_OutInfo(" %-20s %s\r\n",
                        pstNodeNext->acCmd,
                        pstNodeNext->pcHelp == NULL ? "": pstNodeNext->pcHelp);
            }
        }
    }

    if (! CMD_EXP_IS_VIEW(pstNode->uiType)) {
        if ((pstNode->pfFunc != NULL)
            && (! BIT_ISSET(pstNode->uiProperty, DEF_CMD_EXP_PROPERTY_HIDE_CR))) {
            EXEC_OutString(" <CR>\r\n");
        }
    }

    return BS_OK;
}

static int cmdexp_RunCmdExpand(_CMD_EXP_RUNNER_S *pstRunner)
{
    CHAR *pcSplit,  *pcHelpCmd;
    CHAR acCmdTmp[DEF_CMD_EXP_MAX_CMD_LEN + 1];
    _CMD_EXP_TREE_S *pstNode, *pstNodeTmp;
    UINT ulLen;
    UINT uiSamePrefixLen = 0;  
    UINT uiSamePrefixLenTmp;
    UINT uiFindCount = 0;

    TXT_StrCpy(acCmdTmp, pstRunner->acInputs);
    pcSplit = acCmdTmp;
    pcHelpCmd  = acCmdTmp;

    pstNode = &pstRunner->pstCurrentMode->pstViewTbl->stCmdRoot;

    do {
        pcSplit = strchr(pcSplit, ' ');
        if (pcSplit != NULL) {
            *pcSplit = '\0';
            
            pstNode = cmdexp_FindCmdElement(pstRunner, pstNode, pcHelpCmd);
            if (pstNode == NULL) {
                RETURN(BS_NO_SUCH);
            }

            if (CMD_EXP_IS_VIEW(pstNode->uiType)) {
                RETURN(BS_NO_SUCH);
            }
            
            pcSplit++;
            pcHelpCmd = pcSplit;
        }
    }while (pcSplit);

    ulLen = strlen(pcHelpCmd);
    pstNodeTmp = NULL;

    DLL_HEAD_S *subcmd_list = pstNode->subcmd_list;

    DLL_SCAN(subcmd_list, pstNode) {
        if (pstNode->level < pstRunner->level) {
            continue;
        }
        if (pstNode->uiProperty & DEF_CMD_EXP_PROPERTY_HIDE) {
            continue;
        }
        if ((ulLen > 0) && (strncmp(pstNode->acCmd, pcHelpCmd, ulLen) != 0)) {
            continue;
        }

        
        if (pstNode->acCmd[0] == '%') {
            return cmdexp_CmdHelp(pstRunner);
        }

        
        if (pstNodeTmp != NULL) {
            uiSamePrefixLenTmp = (UINT)TXT_GetSamePrefixLen(pstNodeTmp->acCmd, pstNode->acCmd);
            if (uiSamePrefixLenTmp < uiSamePrefixLen) {
                uiSamePrefixLen = uiSamePrefixLenTmp;
            }
        } else {
            uiSamePrefixLen = strlen(pstNode->acCmd);
        }

        pstNodeTmp = pstNode;
        uiFindCount ++;
    }

    
    if (pstNodeTmp != NULL) {
        if (pstRunner->ulInputsLen + strlen(pstNodeTmp->acCmd) - ulLen < DEF_CMD_EXP_MAX_CMD_LEN) {
            TXT_Strlcat(pstRunner->acInputs, pstNodeTmp->acCmd + ulLen, sizeof(pstRunner->acInputs));
            pstRunner->ulInputsLen += (uiSamePrefixLen - ulLen);
            pstRunner->acInputs[pstRunner->ulInputsLen] = '\0';

            if (uiFindCount == 1) {
                pstRunner->acInputs[pstRunner->ulInputsLen] = ' ';
                pstRunner->acInputs[pstRunner->ulInputsLen + 1] = '\0';
                pstRunner->ulInputsLen++;
            }
        }
    }

    if (uiFindCount > 1) {
        cmdexp_CmdHelp(pstRunner);
    }

    return BS_OK;
}

static UINT cmdexp_ParseCmd(IN CHAR *pcCmd, OUT CHAR *ppArgv[_DEF_CMD_EXP_MAX_CMD_ELEMENT_IN_ONE_CMD])
{
    Mem_Zero(ppArgv, sizeof(CHAR*)*_DEF_CMD_EXP_MAX_CMD_ELEMENT_IN_ONE_CMD);

    return ARGS_Split(pcCmd, ppArgv, _DEF_CMD_EXP_MAX_CMD_ELEMENT_IN_ONE_CMD);
}

static int cmdexp_Call(IN _CMD_EXP_TREE_S *pstNode, IN UINT uiArgc, IN CHAR **ppArgv, IN _CMD_EXP_ENV_S *pstEnv)
{
    g_cmd_exp_thread_env = pstEnv;
    int ret = pstNode->pfFunc(uiArgc, ppArgv, pstEnv);
    g_cmd_exp_thread_env = NULL;

    return ret;
}

static int cmdexp_RunCmd(_CMD_EXP_RUNNER_S *pstRunner,
        _CMD_EXP_NEW_MODE_NODE_S *pstMode, CHAR *pcCmd)
{
    _CMD_EXP_TREE_S *pstNode;
    int enRet;
    _CMD_EXP_ENV_S stEnv = {0};
    UINT uiArgc;
    CHAR *ppArgv[_DEF_CMD_EXP_MAX_CMD_ELEMENT_IN_ONE_CMD];

    uiArgc = cmdexp_ParseCmd(pcCmd, ppArgv);
    if (uiArgc == 0) {
        EXEC_OutInfo(" Command not found. \r\n");
        EXEC_Flush();
        RETURN(BS_NO_SUCH);
    }

    pstNode = cmdexp_FindCmd(pstRunner, &pstMode->pstViewTbl->stCmdRoot, uiArgc, ppArgv);
    if (pstNode == NULL) {
        EXEC_OutInfo(" Command not found. \r\n");
        EXEC_Flush();
        RETURN(BS_NO_SUCH);
    }

    stEnv.pstRunner = pstRunner;
    stEnv.pstCmdNode = pstNode;

    if (CMD_EXP_IS_VIEW(pstNode->uiType)) {
        if (0 != cmdexp_EnterMode(pstRunner, pstNode->view, pstNode->uiProperty, uiArgc, ppArgv)) {
            RETURN(BS_ERR);
        }

        if (pstNode->pfFunc != NULL) {
            enRet = cmdexp_Call(pstNode, uiArgc, ppArgv, &stEnv);
            if (BS_OK != enRet) {
                CmdExp_QuitMode(pstRunner);
                return enRet;
            }
        }

        return BS_OK;
    }

    if (pstNode->pfFunc != NULL) {
        return cmdexp_Call(pstNode, uiArgc, ppArgv, &stEnv);
    } else {
        EXEC_OutInfo(" No action for this cmd.\r\n");
        EXEC_Flush();
        RETURN(BS_NO_SUCH);
    }
}

static void cmdexp_Record2History(_CMD_EXP_RUNNER_S *pstRunner)
{
    char *pszLastedCmd = NULL;

    if (pstRunner->deny_history) {
        return;
    }

    RArray_ReadReversedIndex(pstRunner->hCmdRArray, 0, (UCHAR**)&pszLastedCmd, NULL);

    if ((pszLastedCmd != NULL) && (strcmp(pstRunner->acInputs, pszLastedCmd) == 0)) {
        return;
    }

    RArray_WriteForce(pstRunner->hCmdRArray, (UCHAR*)pstRunner->acInputs,
            strlen(pstRunner->acInputs)+1);

    return;
}


static int cmdexp_RunCR(_CMD_EXP_RUNNER_S *pstRunner)
{
    int eRet = BS_OK;

    if (pstRunner->hook_func) {
        eRet = cmdexp_hook_notify(pstRunner, CMD_EXP_HOOK_EVENT_LINE, pstRunner->acInputs);
        if (BS_STOLEN == eRet) {
            cmdexp_ClearInputsBuf(pstRunner);
            return 0;
        }
    }

    if (pstRunner->alt_mode) {
        EXEC_OutString("\r\n");
        EXEC_Flush();
    }

    
    while ((pstRunner->ulInputsLen > 0)
            && (pstRunner->acInputs[pstRunner->ulInputsLen - 1] == ' ')) {
        pstRunner->ulInputsLen--;
        pstRunner->acInputs[pstRunner->ulInputsLen] = '\0';
    }

    if (pstRunner->ulInputsLen > 0) {
        cmdexp_Record2History(pstRunner);
        eRet = cmdexp_RunCmd(pstRunner, pstRunner->pstCurrentMode, pstRunner->acInputs);
    }

    cmdexp_ClearInputsBuf(pstRunner);

    if (eRet != BS_STOP) {
        cmdexp_OutputPrefix(pstRunner);
    }

    EXEC_Flush();

    return eRet;
}


static void cmdexp_RunDelChar(_CMD_EXP_RUNNER_S *pstRunner)
{
    int i;

    if (pstRunner->ulInputsLen - pstRunner->uiInputsPos <= 0) {
        return;
    }

    TXT_RemoveChar(pstRunner->acInputs, pstRunner->ulInputsLen - pstRunner->uiInputsPos - 1);
    pstRunner->ulInputsLen--;
    EXEC_OutString("\b");
    EXEC_OutString(pstRunner->acInputs + pstRunner->ulInputsLen - pstRunner->uiInputsPos);
    EXEC_OutString(" ");
    for (i=0; i<=pstRunner->uiInputsPos; i++) {
        EXEC_OutString("\b");
    }
    EXEC_Flush();
}


static void cmdexp_RunQuaestio(_CMD_EXP_RUNNER_S *pstRunner)
{
    if (pstRunner->deny_help) {
        return;
    }

    if (pstRunner->alt_mode) {
        EXEC_OutString("\r\n");
    }
    TXT_ReplaceSubStr(pstRunner->acInputs, "  ", " ",
            pstRunner->acInputs, sizeof(pstRunner->acInputs));
    pstRunner->ulInputsLen = strlen(pstRunner->acInputs);
    cmdexp_CmdHelp(pstRunner);
    pstRunner->uiInputsPos = 0;
    if (pstRunner->alt_mode) {
        cmdexp_OutputPrefix(pstRunner);
        EXEC_OutInfo("%s", pstRunner->acInputs);
    }
    EXEC_Flush();
}


static void cmdexp_RunTab(_CMD_EXP_RUNNER_S *pstRunner)
{
    if (pstRunner->deny_help) {
        return;
    }

    EXEC_OutString("\r\n");
    TXT_ReplaceSubStr(pstRunner->acInputs, "  ", " ",
            pstRunner->acInputs, sizeof(pstRunner->acInputs));
    pstRunner->ulInputsLen = strlen(pstRunner->acInputs);
    cmdexp_RunCmdExpand(pstRunner);
    pstRunner->uiInputsPos = 0;
    if (pstRunner->alt_mode) {
        cmdexp_OutputPrefix(pstRunner);
        EXEC_OutInfo("%s", pstRunner->acInputs);
    }
    EXEC_Flush();
}

static int cmdexp_RunNormalChar(_CMD_EXP_RUNNER_S *pstRunner, CHAR cCmdChar)
{
    int i;

    if ((cCmdChar == ' ') && (pstRunner->ulInputsLen == 0)) {
        return 0;
    }

    if (pstRunner->ulInputsLen >= DEF_CMD_EXP_MAX_CMD_LEN) {
        EXEC_OutString(" The input is too long.\r\n");
        EXEC_Flush();
        cmdexp_ClearInputsBuf(pstRunner);
        RETURN(BS_NOT_SUPPORT);
    }

    if (TRUE == TXT_InsertChar(pstRunner->acInputs,
                pstRunner->ulInputsLen - pstRunner->uiInputsPos,
                cCmdChar)) {
        pstRunner->ulInputsLen++;
        if (pstRunner->alt_mode) {
            EXEC_OutString(pstRunner->acInputs
                    + pstRunner->ulInputsLen - pstRunner->uiInputsPos - 1);
        }
        for (i=0; i<pstRunner->uiInputsPos; i++) {
            EXEC_OutString("\b");
        }
        EXEC_Flush();
    }

    return 0;
}

static int cmdexp_RunOneChar(_CMD_EXP_RUNNER_S *pstRunner, CHAR cCmdChar)
{
    int eRet = BS_OK;

    BS_DBGASSERT (pstRunner != NULL);

    if (pstRunner->hook_func) {
        eRet = cmdexp_hook_notify(pstRunner, CMD_EXP_HOOK_EVENT_CHAR, &cCmdChar);
        if (eRet == BS_STOLEN) {
            return 0;
        }
    }

    if (cCmdChar == '\n') {   
        eRet = cmdexp_RunCR(pstRunner);
    } else if (cCmdChar == '\b') {
        cmdexp_RunDelChar(pstRunner);
    } else if (cCmdChar == '?') {
        cmdexp_RunQuaestio(pstRunner);
    } else if (cCmdChar == '\t') {
        cmdexp_RunTab(pstRunner);
    } else {
        eRet = cmdexp_RunNormalChar(pstRunner, cCmdChar);
    }

    return eRet;
}

static int cmdexp_RunHistroy(_CMD_EXP_RUNNER_S *pstRunner, EXCHAR enCmdType)
{
    UINT ulLen;
    CHAR *pszLastedCmd = NULL;

    if (pstRunner->deny_history) {
        return 0;
    }

    if ((enCmdType != EXCHAR_EXTEND_UP) && (enCmdType != EXCHAR_EXTEND_DOWN)) {
        RETURN(BS_ERR);
    }

    pstRunner->uiInputsPos = 0;

    
    for (ulLen = 0; ulLen < pstRunner->ulInputsLen; ulLen++) {
        EXEC_OutInfo("\b \b");
    }

    if (enCmdType == EXCHAR_EXTEND_UP) {      
        if (RArray_ReadReversedIndex(pstRunner->hCmdRArray,
                    pstRunner->ulHistoryIndex,
                    (UCHAR**)&pszLastedCmd, &ulLen) == BS_OK) {
            TXT_StrCpy (pstRunner->acInputs, pszLastedCmd);
            pstRunner->ulInputsLen = ulLen - 1;
            pstRunner->ulHistoryIndex++;
        }
    } else if (enCmdType == EXCHAR_EXTEND_DOWN) {
        if ((pstRunner->ulHistoryIndex == 0) || (pstRunner->ulHistoryIndex == 1)) {
            cmdexp_ClearInputsBuf(pstRunner);
            if (pstRunner->ulHistoryIndex == 1) {
                pstRunner->ulHistoryIndex = 0;
            }
        } else if (RArray_ReadReversedIndex(pstRunner->hCmdRArray,
                    pstRunner->ulHistoryIndex - 2,
                    (UCHAR**)&pszLastedCmd, &ulLen) != BS_OK) {
            cmdexp_ClearInputsBuf(pstRunner);
        } else {
            TXT_StrCpy(pstRunner->acInputs, pszLastedCmd);
            pstRunner->ulInputsLen = ulLen - 1;
            pstRunner->ulHistoryIndex--;
        }
    }

    EXEC_OutInfo("%s", pstRunner->acInputs);
    EXEC_Flush();
    
    return BS_OK;
}

HANDLE CmdExp_GetParam(VOID *pEnv)
{
    _CMD_EXP_ENV_S *pstEnv = pEnv;

    return pstEnv->pstCmdNode->hParam;
}

int CmdExp_RunEnvCmd(char *cmd, void *env)
{
    _CMD_EXP_ENV_S *pstEnv = env;
    return cmdexp_RunCmd(pstEnv->pstRunner, pstEnv->pstRunner->pstCurrentMode, cmd);
}

int CmdExp_EnterModeManual(int argc, char **argv, char *view_name, void *env)
{
    _CMD_EXP_ENV_S *pstEnv = env;
    _CMD_EXP_VIEW_S *view = cmdexp_FindView(pstEnv->pstRunner->cmdexp, view_name);
    if (! view) {
        RETURN(BS_NO_SUCH);
    }

    return cmdexp_EnterMode(pstEnv->pstRunner, view, DEF_CMD_EXP_PROPERTY_TEMPLET, argc, argv);
}

void CmdExp_SetCurrentModeValue(void *env, char *mode_value)
{
    _CMD_EXP_ENV_S *pstEnv = env;
    _CMD_EXP_NEW_MODE_NODE_S *pstModeNode = pstEnv->pstRunner->pstCurrentMode;

    TXT_Strlcpy(pstModeNode->szModeValue, mode_value, sizeof(pstModeNode->szModeValue));
    BS_Snprintf(pstModeNode->szModeName, sizeof(pstModeNode->szModeName),
            "%s-%s", pstModeNode->pstViewTbl->view_name, mode_value);
}


CHAR * CmdExp_GetCurrentModeValue(IN VOID *pEnv)
{
    _CMD_EXP_ENV_S *pstEnv = pEnv;
    return pstEnv->pstRunner->pstCurrentMode->szModeValue;
}


CHAR * CmdExp_GetCurrentViewName(void *pEnv)
{
    _CMD_EXP_ENV_S *pstEnv = pEnv;
    return pstEnv->pstRunner->pstCurrentMode->pstViewTbl->view_name;
}

void * CmdExp_GetEnvRunner(void *pEnv)
{
    _CMD_EXP_ENV_S *pstEnv = pEnv;
    return pstEnv->pstRunner;
}

void * CmdExp_GetCurrentEnv()
{
    return g_cmd_exp_thread_env;
}


CHAR * CmdExp_GetCurrentModeName(IN VOID *pEnv)
{
    _CMD_EXP_ENV_S *pstEnv = pEnv;
    return pstEnv->pstRunner->pstCurrentMode->szModeName;
}

int CmdExp_GetEnvMucID(void *env)
{
    _CMD_EXP_ENV_S *pstEnv = env;
    return pstEnv->pstRunner->muc_id;
}

void CmdExp_SetSubRunner(void *runner, void *sub_runner)
{
    _CMD_EXP_RUNNER_S *pr = runner;
    _CMD_EXP_RUNNER_S *r = sub_runner;

    pr->sub_runner = sub_runner;

    EXEC_OutInfo("  Enter %s environment \r\n", r->runner_name);

    return;
}

void CmdExp_ClearInputsBuf(CMD_EXP_RUNNER hRunner)
{
    return cmdexp_ClearInputsBuf(hRunner);
}

static _CMD_EXP_NEW_MODE_NODE_S * cmdexp_GetHistroyMode(IN VOID *pEnv,
        IN UINT uiHistroyIndex)
{
    _CMD_EXP_ENV_S *pstEnv = pEnv;
    UINT uiIndex;
    UINT uiCount;

    if (uiHistroyIndex == 0) {
        return pstEnv->pstRunner->pstCurrentMode;
    }

    uiCount = HSTACK_GetCount(pstEnv->pstRunner->hModeStack);

    if (uiHistroyIndex > uiCount) {
        return NULL;
    }    

    uiIndex = uiCount - uiHistroyIndex;

    return HSTACK_GetValueByIndex(pstEnv->pstRunner->hModeStack, uiIndex);
}


CHAR * CmdExp_GetUpModeValue(IN VOID *pEnv, IN UINT uiHistroyIndex)
{
    _CMD_EXP_NEW_MODE_NODE_S *pstModeTmp;

    pstModeTmp = cmdexp_GetHistroyMode(pEnv, uiHistroyIndex);
    if (NULL == pstModeTmp) {
        return NULL;
    }

    return pstModeTmp->szModeValue;
}


CHAR * CmdExp_GetUpModeName(IN VOID *pEnv, IN UINT uiHistroyIndex)
{
    _CMD_EXP_NEW_MODE_NODE_S *pstModeTmp;

    pstModeTmp = cmdexp_GetHistroyMode(pEnv, uiHistroyIndex);
    if (NULL == pstModeTmp) {
        return NULL;
    }

    return pstModeTmp->szModeName;
}

static int cmdexp_MoveCursor(_CMD_EXP_RUNNER_S *pstRunner, EXCHAR enCmdType)
{
    UINT uiMoveLen;
    UINT i;

    if (enCmdType == EXCHAR_EXTEND_LEFT) {
        if (pstRunner->uiInputsPos >= pstRunner->ulInputsLen) {
            return BS_OK;
        }
        pstRunner->uiInputsPos ++;

        EXEC_OutInfo("\b");
        EXEC_Flush();
    } else if (enCmdType == EXCHAR_EXTEND_RIGHT) {
        if (pstRunner->uiInputsPos == 0) {
            return BS_OK;
        }

        int off = pstRunner->ulInputsLen - pstRunner->uiInputsPos;
        EXEC_OutInfo("%c", pstRunner->acInputs[off]);
        EXEC_Flush();

        pstRunner->uiInputsPos --;
    }
    else if (enCmdType == EXCHAR_EXTEND_HOME)
    {
        if (pstRunner->uiInputsPos >= pstRunner->ulInputsLen)
        {
            return BS_OK;
        }

        uiMoveLen = pstRunner->ulInputsLen - pstRunner->uiInputsPos;
        pstRunner->uiInputsPos = pstRunner->ulInputsLen;

        for (i=0; i<uiMoveLen; i++)
        {
            EXEC_OutInfo("\b");
        }
        EXEC_Flush();
    }
    else if (enCmdType == EXCHAR_EXTEND_END)
    {
        if (pstRunner->uiInputsPos == 0)
        {
            return BS_OK;
        }

        uiMoveLen = pstRunner->uiInputsPos;
        pstRunner->uiInputsPos = 0;
        for (i=0; i<uiMoveLen; i++)
        {
            EXEC_OutInfo("%c", pstRunner->acInputs[pstRunner->ulInputsLen - uiMoveLen + i]);
        }
        EXEC_Flush();
    }

    return BS_OK;
}

static void cmdexp_RunSubRunner(_CMD_EXP_RUNNER_S *runner, UCHAR ch)
{
    _CMD_EXP_RUNNER_S *sub_runner = runner->sub_runner;

    if (BS_STOP == cmdexp_Run(sub_runner, ch)) {
        EXEC_OutInfo("  Quit %s environment \r\n", sub_runner->runner_name);
        CMD_EXP_DestroyRunner(sub_runner);
        runner->sub_runner = NULL;
        cmdexp_OutputPrefix(runner);
        EXEC_Flush();
    }
}

static int cmdexp_Run(CMD_EXP_RUNNER hRunner, UCHAR ucCmdChar)
{
    _CMD_EXP_RUNNER_S *pstRunner = hRunner;
    EXCHAR exchar;

    if (pstRunner->sub_runner) {
        cmdexp_RunSubRunner(pstRunner, ucCmdChar);
        return 0;
    }

    if (pstRunner->alt_mode) {
        exchar = EXCHAR_Parse(pstRunner->hExcharHandle, ucCmdChar);
        if (exchar == EXCHAR_EXTEND_TEMP) {
            return BS_OK;
        }

        if ((exchar == EXCHAR_EXTEND_UP) || (exchar == EXCHAR_EXTEND_DOWN)) {
            cmdexp_RunHistroy(pstRunner, exchar);
            return BS_OK;
        }

        if ((exchar == EXCHAR_EXTEND_LEFT)
                || (exchar == EXCHAR_EXTEND_RIGHT)
                || (exchar == EXCHAR_EXTEND_HOME)
                || (exchar == EXCHAR_EXTEND_END)) {
            cmdexp_MoveCursor(pstRunner, exchar);
            return BS_OK;
        }

        if (exchar == EXCHAR_NORMAL_DELETE) {
            ucCmdChar = '\b';
        }
    }

    
    if ((ucCmdChar == '\n') && (pstRunner->bIsRChangeToN == TRUE)) {
        
        pstRunner->bIsRChangeToN = FALSE;
        return BS_OK;
    } else if (ucCmdChar == '\r') {
        ucCmdChar = '\n';
        pstRunner->bIsRChangeToN = TRUE;
    } else {
        pstRunner->bIsRChangeToN = FALSE;
    }

    if (ucCmdChar == '\n') {
        pstRunner->ulHistoryIndex = 0;
    }

    return cmdexp_RunOneChar(pstRunner, (CHAR)ucCmdChar);
}

static void cmdexp_p(CMD_EXP_S *pstExp)
{
    if (pstExp->flag & CMD_EXP_FLAG_LOCK) {
        MUTEX_P(&pstExp->lock);
    }
}

static void cmdexp_v(CMD_EXP_S *pstExp)
{
    if (pstExp->flag & CMD_EXP_FLAG_LOCK) {
        MUTEX_V(&pstExp->lock);
    }
}

int CmdExp_Run(CMD_EXP_RUNNER hRunner, UCHAR ucCmdChar)
{
    _CMD_EXP_RUNNER_S *pstRunner = hRunner;
    int ret;

    cmdexp_p(pstRunner->cmdexp);
    ret = cmdexp_Run(hRunner, ucCmdChar);
    cmdexp_v(pstRunner->cmdexp);

    return ret;
}

static BOOL_T cmdexp_NeedCR(char c)
{
    if ((c == '?') || (c == '\r') || (c == '\n')) {
        return FALSE;
    }

    return TRUE;
}

static int cmdexp_RunString(CMD_EXP_RUNNER hRunner, char *string, int len, int auto_cr)
{
    int i;
    int is_have_err = 0;
    int ret;
    char c, tmp;
    int in_qout = 0; 

    for (i=0; i<len; i++) {
        c = string[i];

        if ((in_qout == 0) && (c == '"')) {
            in_qout = 1;
            continue;
        }
        if ((in_qout == 1) && (c == '"')) {
            in_qout = 0;
            continue;
        }

        tmp = c;
        if (in_qout == 0) {
            if ((c == '&') || (c == ';')) {
                tmp = '\n';
            }
        }

        ret = cmdexp_Run(hRunner, tmp);

        if (BS_STOP == ret) {
            return BS_STOP;
        } else if (ret < 0) {
            is_have_err = ret;
            if ((in_qout == 0) && (c == '&')) { 
                break;
            }
        }
    }

    if (auto_cr) {
        if (cmdexp_NeedCR(string[len-1])) {
            ret = cmdexp_Run(hRunner, '\n');
        }
    }

    return is_have_err;
}

int CmdExp_RunString(CMD_EXP_RUNNER hRunner, char *string, int len)
{
    _CMD_EXP_RUNNER_S *pstRunner = hRunner;
    int ret;

    if (len == 0) {
        return 0;
    }

    cmdexp_p(pstRunner->cmdexp);
    ret = cmdexp_RunString(hRunner, string, len, 0);
    cmdexp_v(pstRunner->cmdexp);

    return ret;
}

int CmdExp_RunLine(CMD_EXP_RUNNER hRunner, char *line)
{
    _CMD_EXP_RUNNER_S *pstRunner = hRunner;
    int ret;
    int len = strlen(line);

    if (len == 0) {
        return 0;
    }

    cmdexp_p(pstRunner->cmdexp);
    ret = cmdexp_RunString(hRunner, line, len, 1);
    cmdexp_v(pstRunner->cmdexp);

    return ret;
}

UINT CmdExp_GetOptFlag(IN CHAR *pcString)
{
    CHAR *pcTmp;
    UINT uiPrefixLen = sizeof(DEF_CMD_OPT_STR_PREFIX) - 1;
    UINT uiFlag = 0;

    pcTmp = strstr(pcString, DEF_CMD_OPT_STR_PREFIX);
    if (! pcTmp) {
        return 0;
    }

    pcTmp += uiPrefixLen;

    while ((*pcTmp != '\0') && (*pcTmp != DEF_CMD_OPT_STR_END)) {
        if (*pcTmp == DEF_CMD_OPT_FLAG_STR_NOSAVE) {
            uiFlag |= DEF_CMD_OPT_FLAG_NOSAVE;
        } else if (*pcTmp == DEF_CMD_OPT_FLAG_STR_NOSHOW) {
            uiFlag |= DEF_CMD_OPT_FLAG_NOSHOW;
        }

        pcTmp ++;
    }

    return uiFlag;
}


BOOL_T CmdExp_IsOptPermitOutput(IN HANDLE hFile, IN CHAR *pcString)
{
    UINT uiFlag;
    _CMD_EXP_SHOW_SAVE_NODE_S *pstSSNode = hFile;

    uiFlag = CmdExp_GetOptFlag(pcString);

    if ((uiFlag & DEF_CMD_OPT_FLAG_NOSHOW)
            && (pstSSNode->bIsShowAll == 0)) {
        return FALSE;
    }

    if ((uiFlag & DEF_CMD_OPT_FLAG_NOSAVE)
            && (CmdExp_IsSaving(hFile))) {
        return FALSE;
    }

    return TRUE;
}

int CmdExp_DoCmd(CMD_EXP_HDL hCmdExp, char *cmd)
{
    CMD_EXP_RUNNER hRunner;

    hRunner = CmdExp_CreateRunner(hCmdExp, 0);
    if (NULL == hRunner) {
        RETURN(BS_NO_MEMORY);
    }

    CmdExp_RunLine(hRunner, cmd);
    CmdExp_DestroyRunner(hRunner);

    return 0;
}

