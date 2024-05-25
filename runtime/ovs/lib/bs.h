/******************************************************************************
* Copyright (C), LiXingang
* Author:      LiXingang  Version: 1.0  Date: 2008-3-26
* Description: 
* History:     
******************************************************************************/

#ifndef __BS_H_
#define __BS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#include "utl/types.h"

#ifdef __cplusplus
extern "C" {
#endif


#define RETURN(ret)  return (ret)
#define RETURNI(ret, fmt, ...) do {printf(fmt "\n", ##__VA_ARGS__); return (ret); } while(0)
#define MEM_ZMalloc(x) calloc((x),1)
#define MEM_Free(x) free(x)

#ifdef __cplusplus
}
#endif
#endif 
