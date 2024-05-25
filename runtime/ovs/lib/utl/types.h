/******************************************************************************
* Copyright (C), Xingang.Li
* File name     : azHead.h
* Date          : 2005/2/25
* Description:
******************************************************************************/
#ifndef _TYPES_H
#define _TYPES_H

#include "utl/int_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef OUT
#define	IN      /*IN*/
#define	OUT     /*OUT*/
#define	INOUT   /*INOUT*/
#endif

/* 计算宏定义中可变参数个数 */
#define _BS_ARG_N(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,N,...) N
#define BS_ARG_COUNT(...) _BS_ARG_N(0,##__VA_ARGS__,10,9,8,7,6,5,4,3,2,1,0)

/* 获取可变参数中的第几个参数 */
#define _BS_ARG_GET1(a0,a1,...) (a1)
#define _BS_ARG_GET2(a0,a1,a2,...) (a2)
#define _BS_ARG_GET3(a0,a1,a2,a3,...) (a3)
#define _BS_ARG_GET4(a0,a1,a2,a3,a4,...) (a4)
#define _BS_ARG_GET5(a0,a1,a2,a3,a4,a5,...) (a5)
#define _BS_ARG_GET6(a0,a1,a2,a3,a4,a5,a6,...) (a6)
#define _BS_ARG_GET7(a0,a1,a2,a3,a4,a5,a6,a7,...) (a7)
#define _BS_ARG_GET8(a0,a1,a2,a3,a4,a5,a6,a7,a8,...) (a8)
#define _BS_ARG_GET9(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,...) (a9)
#define _BS_ARG_GET10(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,...) (a10)
#define BS_ARG_GET(N,...) _BS_ARG_GET##N(0,##__VA_ARGS__,0,0,0,0,0,0,0,0,0,0)

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef BOOL_TRUE
#define BOOL_TRUE TRUE
#endif

#ifndef BOOL_FALSE
#define BOOL_FALSE FALSE
#endif

typedef struct {
    UCHAR *data; /* 文件数据 */
    UINT64 len;   /* 文件长度 */
}LLDATA_S;

typedef LLDATA_S FILE_MEM_S;

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
