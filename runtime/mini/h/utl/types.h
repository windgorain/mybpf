/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _TYPES_H
#define _TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum{
	BS_OK = 0,
	BS_ERR = -1,
	BS_NO_SUCH = -2,
	BS_ALREADY_EXIST = -3,
	BS_BAD_PTR = -4,
	BS_CAN_NOT_OPEN = -5,
	BS_WRONG_FILE = -6,
	BS_NOT_SUPPORT = -7,
	BS_OUT_OF_RANGE = -8,
	BS_TIME_OUT = -9,
	BS_NO_MEMORY = -10,
	BS_NULL_PARA = -11,
	BS_NO_RESOURCE = -12,
	BS_BAD_PARA = -13,
	BS_NO_PERMIT = -14,
	BS_FULL = -15,
	BS_EMPTY = -16,
	BS_PAUSE = -17,
	BS_STOP  = -18,
	BS_CONTINUE = -19,
	BS_NOT_FOUND = -20,
	BS_NOT_COMPLETE = -21,
	BS_CAN_NOT_CONNECT = -22,
	BS_CONFLICT = -23,
	BS_TOO_LONG = -24,
	BS_TOO_SMALL = -25,
	BS_BAD_REQUEST = -26,
	BS_AGAIN = -27,
	BS_CAN_NOT_WRITE = -28,
	BS_NOT_READY = -29,
	BS_PROCESSED= -30,
	BS_PEER_CLOSED = -31,
	BS_NOT_MATCHED = -32,
	BS_VERIFY_FAILED = -33,
	BS_NOT_INIT = -34,
	BS_REF_NOT_ZERO = -35, 
    BS_BUSY = -36,
    BS_PARSE_FAILED = -37,
	BS_REACH_MAX = -38,
    BS_STOLEN = -39,

    
    BS_PRIVATE_BASE = -100
}BS_STATUS;




#ifdef __cplusplus
}
#endif
#endif 
