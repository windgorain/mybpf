/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _BS_H
#define _BS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#include "utl/types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char       U8;
typedef unsigned short      U16;
typedef unsigned int        U32;
typedef unsigned long long  U64;

typedef char S8;
typedef short S16;
typedef int S32;
typedef long long S64;

#define RETURNI(ret, fmt, ...) do {printf(fmt "\n", ##__VA_ARGS__); return (ret); } while(0)

#ifdef __cplusplus
}
#endif
#endif 
