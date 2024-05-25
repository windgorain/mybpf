/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2009-4-10
* Description: 为了和windows保持一致,模拟windows的DllMain函数
* History:     
******************************************************************************/

    
#include "bs.h"
#include "utl/plug_utl.h"

#ifdef IN_UNIXLIKE

#define DLL_PROCESS_ATTACH 0 
#define DLL_THREAD_ATTACH  1 
#define DLL_THREAD_DETACH  2 
#define DLL_PROCESS_DETACH 3 


PLUG_HDL PLUG_LoadLib(IN CHAR *pszFilePath)
{
    PF_PLUG_Entry pfDllMainFunc = NULL;
    PLUG_HDL hPlugId;

    hPlugId = dlopen(pszFilePath, RTLD_NOW);
    if (0 == hPlugId) {
        return 0;
    }

    
    pfDllMainFunc = PLUG_GET_FUNC_BY_NAME(hPlugId, "PLUG_Entry");
    if (NULL != pfDllMainFunc) {
        pfDllMainFunc(hPlugId, DLL_PROCESS_ATTACH, NULL);
    }

    return hPlugId;
}


void PLUG_UnloadLib(PLUG_HDL plug)
{
    PF_PLUG_Entry pfDllMainFunc = NULL;

    
    pfDllMainFunc = PLUG_GET_FUNC_BY_NAME(plug, "PLUG_Entry");
    if (NULL != pfDllMainFunc) {
        pfDllMainFunc(plug, DLL_PROCESS_DETACH, NULL);
    }

    dlclose(plug);
}

#endif

