/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2008-4-8
* Description: 
* History:     
******************************************************************************/

#ifndef __INJECTDLL_H_
#define __INJECTDLL_H_

#ifdef __cplusplus
    extern "C" {
#endif 


BS_STATUS INJDLL_InjectDll(IN UINT ulProcessId, IN CHAR *pszDllPath);


BS_STATUS INJDLL_EjectDll(IN UINT ulProcessId, IN CHAR *pszDllPath);

BS_STATUS INJDLL_CreateProcessAndInjectDll(IN CHAR *pszFileName, IN CHAR *pszDllPath);


#ifdef __cplusplus
    }
#endif 

#endif 


