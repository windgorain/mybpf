/******************************************************************************
* Copyright (C), LiXingang
* Author:      LiXingang  Date: 2016-6-7
* Description: 
* History:     
******************************************************************************/

#ifndef __KD_UTL_H_
#define __KD_UTL_H_

#include "utl/lstr_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 

typedef HANDLE KD_HANDLE;

typedef struct
{
    CHAR *pcKey;
    LSTR_S stData;
}KD_S;

typedef int (*PF_KD_WALK_FUNC)(IN KD_S *pstKeyData, IN HANDLE hUserHandle);

KD_HANDLE KD_Create();
VOID KD_Destory(IN KD_HANDLE hKDHandle);
LSTR_S * KD_GetKeyData(IN KD_HANDLE hKDHandle, IN CHAR *pcKey);
BS_STATUS KD_SetKeyData(IN KD_HANDLE hKDHandle, IN CHAR *pcKey, IN LSTR_S *pstData);

BS_STATUS KD_SetKeyHandle(IN KD_HANDLE hKDHandle, IN CHAR *pcKey, IN HANDLE hHandle);
HANDLE KD_GetKeyHandle(IN KD_HANDLE hKDHandle, IN CHAR *pcKey);
KD_S * KD_GetNext(IN KD_HANDLE hKDHandle, IN KD_S *pstCurrent);
VOID KD_DelKey(IN KD_HANDLE hKDHandle, IN CHAR *pcKey);
VOID KD_Walk(IN KD_HANDLE hKDHandle, IN PF_KD_WALK_FUNC pfFunc, IN HANDLE hUserHandle);

#ifdef __cplusplus
    }
#endif 

#endif 


