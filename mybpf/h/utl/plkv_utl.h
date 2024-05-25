/******************************************************************************
* Copyright (C), 2000-2006,  Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2016-10-13
* Description: 
* History:     
******************************************************************************/

#ifndef __PLKV_UTL_H_
#define __PLKV_UTL_H_

#include "utl/kv_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 

typedef HANDLE PLKV_HANDLE;

PLKV_HANDLE PLKV_Create(IN UINT uiFlag );
VOID PLKV_Destroy(IN PLKV_HANDLE hKvHandle);

BS_STATUS PLKV_ParseOne(IN PLKV_HANDLE hKvHandle, IN LSTR_S *pstLstr, IN CHAR cEquelChar);
BS_STATUS PLKV_Parse(IN PLKV_HANDLE hKvHandle, IN LSTR_S *pstLstr, IN CHAR cSeparator, IN CHAR cEquelChar);
LSTR_S * PLKV_GetKeyValue(IN PLKV_HANDLE hKvHandle, IN CHAR *pcKey);

#ifdef __cplusplus
    }
#endif 

#endif 


