/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _ELF_PROG_H_
#define _ELF_PROG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define ELFLIB_AGGR_FLAG_KEEP_PROG_ORDER 0x1 
#define ELFLIB_AGGR_FLAG_DATA_INIT 0x2 

int ELFLIB_AggrProgs(LLDATA_S *d, OUT VBUF_S *vbuf, U32 flag);
int ELFLIB_AddDataInitSec(LLDATA_S *d, OUT VBUF_S *vbuf);

#ifdef __cplusplus
}
#endif
#endif

