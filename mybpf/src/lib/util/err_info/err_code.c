/*================================================================
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2018-09-22
* Description: 
================================================================*/
#include "bs.h"
#include "utl/txt_utl.h"
#include "utl/err_code.h"

static THREAD_LOCAL ERR_CODE_S g_err_code;

static char * g_err_code_info[] = {
	[-BS_ERR] = "Error",
	[-BS_NO_SUCH] = "No such",
	[-BS_ALREADY_EXIST] = "Already exist",
	[-BS_BAD_PTR] = "Bad ptr",
	[-BS_CAN_NOT_OPEN] = "Can not open",
	[-BS_WRONG_FILE] = "wrong file",
	[-BS_NOT_SUPPORT] = "Not support",
	[-BS_OUT_OF_RANGE] = "Out of range",
	[-BS_TIME_OUT] = "Time out",
	[-BS_NO_MEMORY] = "No memroy",
	[-BS_NULL_PARA] = "Null param",
	[-BS_NO_RESOURCE] = "No resource",
	[-BS_BAD_PARA] = "Bad params",
	[-BS_NO_PERMIT] = "No permit",
	[-BS_FULL] = "Full",
	[-BS_EMPTY] = "Empty",
    [-BS_PAUSE] = "Pause",
	[-BS_STOP] = "Stop",
	[-BS_CONTINUE] = "Continue",
	[-BS_NOT_FOUND] = "Not found",
	[-BS_NOT_COMPLETE] = "Not complete",
	[-BS_CAN_NOT_CONNECT] = "Can not connect",
	[-BS_CONFLICT] = "Conflict",
	[-BS_TOO_LONG] = "Too long",
	[-BS_TOO_SMALL] = "Too small",
	[-BS_BAD_REQUEST] = "Bad request",
	[-BS_AGAIN] = "Try again",
	[-BS_CAN_NOT_WRITE] = "Can not write",
	[-BS_NOT_READY] = "Not ready",
	[-BS_PROCESSED] = "Already processed",
	[-BS_PEER_CLOSED] = "Peer closed",
	[-BS_NOT_MATCHED] = "Not matched",
	[-BS_VERIFY_FAILED] = "Verify failed",
	[-BS_NOT_INIT] = "Not init",
	[-BS_REF_NOT_ZERO] = "Ref not zero",
    [-BS_BUSY] = "Busy",
    [-BS_PARSE_FAILED] = "Parse failed",
	[-BS_REACH_MAX] = "Reach max",
    [-BS_STOLEN] = "Stolen"
};

void ErrCode_EnablePrint(int enable)
{
    g_err_code.print = enable;
}

void ErrCode_Set(int err_code, char *info, const char *file_name, const char *func_name, int line)
{
    ERR_CODE_S *err_code_ctrl = &g_err_code;

    err_code_ctrl->file_name = file_name;
    err_code_ctrl->func_name = func_name;
    err_code_ctrl->line = line;
    err_code_ctrl->err_code = err_code;
    err_code_ctrl->info[0] = '\0';
    if (info != NULL) {
        strlcpy(err_code_ctrl->info, info, ERR_INFO_SIZE);
    }

    if (err_code_ctrl->print && err_code) {
        ErrCode_Print();
    }
}

void ErrCode_Clear(void)
{
    ERR_CODE_S *err_code_ctrl = &g_err_code;

    err_code_ctrl->file_name = NULL;
    err_code_ctrl->func_name = NULL;
    err_code_ctrl->line = 0;
    err_code_ctrl->err_code = 0;
    err_code_ctrl->info[0] = '\0';
}

const char * ErrCode_GetFileName(void)
{
    if (g_err_code.file_name == NULL) {
        return "null";
    }

    return g_err_code.file_name;
}

int ErrCode_GetLine(void)
{
    return g_err_code.line;
}

int ErrCode_GetErrCode(void)
{
    return g_err_code.err_code;
}

char * ErrCode_GetInfo(void)
{
    if (g_err_code.info[0]) {
        return g_err_code.info;
    }

    int code = -g_err_code.err_code;
    if (code >= ARRAY_SIZE(g_err_code_info)) {
        return "";
    }

    return g_err_code_info[code];
}

char * ErrCode_Build(OUT char *buf, int buf_size)
{
    const char *file = ErrCode_GetFileName();
    int line = ErrCode_GetLine();
    int code = ErrCode_GetErrCode();
    char *errinfo = ErrCode_GetInfo();
    int len = 0;

    if (file) {
        len = snprintf(buf, buf_size, "ErrCode: %s(%d):%d \r\n", file, line, code);
    }

    if (errinfo && errinfo[0]) {
        len += snprintf(buf + len, buf_size-len, "ErrInfo: %s \r\n", errinfo);
    }

    return buf;
}

void ErrCode_Print(void)
{
    const char *file = ErrCode_GetFileName();
    int line = ErrCode_GetLine();
    int code = ErrCode_GetErrCode();
    char *errinfo = ErrCode_GetInfo();

    if (file) {
        fprintf(stderr, "ErrCode: %s(%d):%d \r\n", file, line, code);
    }

    if (errinfo && errinfo[0]) {
        fprintf(stderr, "ErrInfo: %s \r\n", errinfo);
    }
}

void ErrCode_PrintErrInfo(void)
{
    char *errinfo = ErrCode_GetInfo();

    if (errinfo && errinfo[0]) {
        fprintf(stderr, "ErrInfo: %s \r\n", errinfo);
    }
}

void ErrCode_Output(PF_PRINT_FUNC output)
{
    const char *file = ErrCode_GetFileName();
    int line = ErrCode_GetLine();
    int code = ErrCode_GetErrCode();
    char *errinfo = ErrCode_GetInfo();

    if (file) {
        output("ErrCode: %s(%d):%d \r\n", file, line, code);
    }

    if (errinfo && errinfo[0]) {
        output("ErrInfo: %s \r\n", errinfo);
    }
}

