/******************************************************************************
* Copyright (C), 2000-2006,  Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2015-6-1
* Description: key-value function Tbl
* History:     
******************************************************************************/

#ifndef __KF_UTL_H_
#define __KF_UTL_H_

#include "utl/mime_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 


typedef VOID* KF_HANDLE;

typedef BS_STATUS (*PF_KF_FUNC)(IN MIME_HANDLE hMime, IN HANDLE hUserHandle, IN HANDLE hRunHandle);

KF_HANDLE KF_Create(IN CHAR *pcActionString);
VOID KF_Destory(IN KF_HANDLE hKf);
BS_STATUS KF_AddFunc(IN KF_HANDLE hKf, IN CHAR *pcKey, IN PF_KF_FUNC pfFunc, IN HANDLE hUserHandle1);
BS_STATUS KF_RunMime(IN KF_HANDLE hKf, IN MIME_HANDLE hMime, IN HANDLE hRunHandle);
BS_STATUS KF_RunString(IN KF_HANDLE hKf, IN CHAR cSeparator, IN CHAR *pcString, IN HANDLE hRunHandle);

#ifdef __cplusplus
    }
#endif 

#endif 


