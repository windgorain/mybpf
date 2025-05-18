/******************************************************************************
* Copyright (C), 2000-2006,  LiXingang
* Author:      LiXingang  Version: 1.0  Date: 2016-10-20
* Description: 根据BS_STATUS获取err info
* History:     
******************************************************************************/
#include "bs.h"

static char * g_err_code_info[] = {
    [0] = "OK",
	[-BS_ERR] = "Error",
	[-BS_NO_SUCH] = "No such",
	[-BS_ALREADY_EXIST] = "Already exist",
	[-BS_BAD_PTR] = "Bad ptr",
	[-BS_CAN_NOT_OPEN] = "Can not open",
	[-BS_WRONG_FILE] = "Wrong file",
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
    [-BS_STOLEN] = "Stolen",
    [-BS_QUIT_APP] = "Quit app",
    [-BS_FORCE_QUIT_APP] = "Force quit app",
};

CHAR * ErrInfo_Get(IN BS_STATUS eRet)
{
    int index = -eRet;

    if (index >= ARRAY_SIZE(g_err_code_info)) {
        return "";
    }

    return g_err_code_info[index];
}

