/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _ELF_REDUCE_H
#define _ELF_REDUCE_H

#ifdef __cplusplus
extern "C" {
#endif

void ELFLIB_Reduce(LLDATA_S *d, char **prefix, int count);
void ELFLIB_ReduceDebug(LLDATA_S *d);
void ELFLIB_ReduceBTF(LLDATA_S *d);
void ELFLIB_ReduceLLVM(LLDATA_S *d);

#ifdef __cplusplus
}
#endif
#endif 
