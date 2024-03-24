/*================================================================
* Created by LiXingang
* Author:      Xingang.Li  Version: 1.0  Date: 2007-5-24
* Description: 处理cmd.lst格式的文件
* History: 从cmd_cfg.c move至此
================================================================*/
#include "bs.h"
#include "utl/txt_utl.h"
#include "utl/file_utl.h"
#include "utl/cmd_exp.h"
#include "utl/cmd_lst.h"

static int cmdlst_ParseType(char *type_str)
{
    char *str = TXT_Strim(type_str);

    if (strcmp(str, "view") == 0) {
        return DEF_CMD_EXP_TYPE_VIEW;
    } else if (strcmp(str, "cmd") == 0) {
        return DEF_CMD_EXP_TYPE_CMD;
    } else if (strcmp(str, "save") == 0) {
        return DEF_CMD_EXP_TYPE_SAVE;
    } else if (strcmp(str, "enter") == 0) {
        return DEF_CMD_EXP_TYPE_ENTER;
    }

    return -1;
}


static BS_STATUS cmdlst_GetType(CHAR *type_str, OUT CMDLST_ELE_S *ele)
{
    int type = 0;
    UINT prop = 0;
    UINT level = 0;
    char *token[32];
    char *str;

    UINT num = TXT_StrToToken(type_str, "|", token, 32);
    if (num <= 0) {
        RETURN(BS_ERR);
    }

    type = cmdlst_ParseType(token[0]);
    if (type < 0) {
        RETURN(BS_ERR);
    }

    int i;
    for (i=1; i<num; i++) {
        if (strcmp(token[i], "templet") == 0) {
            
            prop |= DEF_CMD_EXP_PROPERTY_TEMPLET;
        } else if (strcmp(token[i], "hide") == 0) {
            prop |= (DEF_CMD_EXP_PROPERTY_HIDE | DEF_CMD_EXP_PROPERTY_HIDE_CR);
        } else if (strncmp(token[i], "level:", sizeof("level:")-1) == 0) {
            str = token[i];
            str += (sizeof("level:") - 1);
            level = TXT_Str2Ui(str);
        } else if (strcmp(token[i], "view_pcre") == 0) {
            prop |= DEF_CMD_EXP_PROPERTY_VIEW_PATTERN;
        }
    }

    ele->type = type;
    ele->property = prop;
    ele->level = level;

    return BS_OK;
}

static BS_STATUS cmdlst_ParseCmdLine(char *line, OUT CMDLST_ELE_S *ele)
{
    char *token[16];
    int index = 0;

    UINT num = TXT_StrToToken(line, ",", token, 16);
    if (num < 3) {
        RETURN(BS_ERR);
    }

    
    char *type = TXT_Strim(token[index++]);
    if (BS_OK != cmdlst_GetType(type, ele)) {
        RETURN(BS_ERR);
    }

    
    ele->view = TXT_Strim(token[index++]);

    if (CMD_EXP_IS_SAVE(ele->type) || CMD_EXP_IS_ENTER(ele->type)) {
        ele->func_name = token[index++];
        ele->func_name = TXT_Strim(ele->func_name);

        return BS_OK;
    }

    
    ele->cmd = TXT_Strim(token[index++]);
    if (index >= num) {
        RETURN(BS_ERR);
    }

    
    if (CMD_EXP_IS_VIEW(ele->type)) {
        ele->view_name = TXT_StrimAll(token[index++]);
        if (index >= num) {
            RETURN(BS_ERR);
        }
    }

    
    ele->func_name = TXT_Strim(token[index++]);

    if (index < num) {
        char *param = TXT_Strim(token[index++]);
        if (param[0] != 0) {
            ele->param = param;
        }
    }

    return BS_OK;
}

static BS_STATUS cmdlst_Line(CMDLST_S *ctrl, char *line)
{
    CMDLST_ELE_S ele = {0};
    int ret;
    char line_buf[1024];

    strlcpy(line_buf, line, sizeof(line_buf));

    if (BS_OK != (ret = cmdlst_ParseCmdLine(line_buf, &ele))) {
        BS_WARNNING(("Failed to parse cmd line: %s", line));
        return ret;
    }

    ret = ctrl->line_func(ctrl, &ele);
    if (0 != ret) {
        BS_WARNNING(("Failed to reg cmd line: %s", line));
        return ret;
    }

    return 0;
}

BS_STATUS CMDLST_Scan(CMDLST_S *ctrl, char *buf)
{
    char *line= NULL;
    UINT line_len;

    buf = TXT_Strim(buf);

    TXT_SCAN_LINE_BEGIN(buf, line, line_len) {
        
        
        if ((line_len > 5) && (line[0] != '#')) { 
            line[line_len] = '\0';
            cmdlst_Line(ctrl, line);
        }
    }TXT_SCAN_LINE_END();

    return BS_OK;
}

BS_STATUS CMDLST_ScanByFile(CMDLST_S *ctrl, char *filename)
{
    FILE_MEM_S m;

    if (0 != FILE_Mem(filename, &m)) {
        RETURN(BS_CAN_NOT_OPEN);
    }

    CMDLST_Scan(ctrl, (char*)m.data);

    FILE_FreeMem(&m);

    return 0;
}



