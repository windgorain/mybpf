/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _UBOOT_STUB_H
#define _UBOOT_STUB_H

#ifdef __cplusplus
extern "C" {
#endif

#define RETURNI(ret, fmt, ...) do {printf(fmt "\n", ##__VA_ARGS__); return (ret); } while(0)
#define RETURN(x) return(x)
#define MEM_Malloc(x) malloc(x)
#define MEM_ZMalloc(x) calloc((x),1)
#define MEM_Free(x) free(x)
#define ATOM_BARRIER()
#define RCU_NODE_S int

#ifdef __cplusplus
}
#endif
#endif //UBOOT_STUB_H_
