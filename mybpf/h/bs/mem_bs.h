/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      lixingang  Version: 1.0  Date: 2007-2-2
* Description: 
* History:     
******************************************************************************/

#ifndef __MEM_H_
#define __MEM_H_

#include "utl/types.h"
#include <memory.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif 

PLUG_API void * MEM_MallocMem(ULONG ulSize, const char *pszFileName, UINT ulLine);
PLUG_API void MEM_FreeMem(IN VOID *pMem, const char *pszFileName, IN UINT ulLine);

BS_STATUS MEM_ShowStat(IN UINT ulArgc, IN CHAR **argv);
BS_STATUS MEM_ShowSizeOfMemStat(int argc, char **argv);

int MemDebug_Check(int argc, char **argv);
BS_STATUS MemDebug_ShowSizeOfMem(int argc, char **argv);
BS_STATUS MemDebug_ShowLineConflict(int argc, char **argv);

#ifdef __cplusplus
}
#endif 

#endif 


