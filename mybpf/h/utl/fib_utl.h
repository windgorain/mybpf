/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2007-9-30
* Description: 
* History:     
******************************************************************************/

#ifndef __FIB_UTL_H_
#define __FIB_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

#include "utl/net.h"

typedef HANDLE FIB_HANDLE;

#define FIB_MAX_NEXT_HOP_NUM 8 

#define FIB_FLAG_DELIVER_UP 0x1      
#define FIB_FLAG_DIRECT     0x2      
#define FIB_FLAG_STATIC     0x4      
#define FIB_FLAG_AUTO_IF        0x8      

typedef struct
{
    UINT uiDstOrStartIp; 
    UINT uiMaskOrEndIp;  
}FIB_KEY_S;

typedef struct
{
    FIB_KEY_S stFibKey;
    UINT uiNextHop;      
    UINT uiOutIfIndex;
    UINT uiFlag;
}FIB_NODE_S;

typedef int (*PF_FIB_WALK_FUNC)(IN FIB_NODE_S *pstFibNode, IN HANDLE hUserHandle);

#define FIB_INSTANCE_FLAG_CREATE_LOCK 0x1

FIB_HANDLE FIB_Create(IN UINT uiInstanceFlag );
VOID FIB_Destory(IN FIB_HANDLE hFibHandle);
BS_STATUS FIB_Add(IN FIB_HANDLE hFibHandle, IN FIB_NODE_S *pstFibNode);

VOID FIB_Del(IN FIB_HANDLE hFibHandle, IN FIB_NODE_S *pstFibNode);
VOID FIB_DelAll(IN FIB_HANDLE hFibHandle);
BS_STATUS FIB_Find(IN FIB_HANDLE hFibHandle, IN UINT ip, IN UINT mask);
BS_STATUS FIB_PrefixMatch(IN FIB_HANDLE hFibHandle, IN UINT uiDstIp , OUT FIB_NODE_S *pstFibNode);
VOID FIB_Walk(IN FIB_HANDLE hFibHandle, IN PF_FIB_WALK_FUNC pfWalkFunc, IN HANDLE hUserHandle);
BS_STATUS FIB_GetNext
(
    IN FIB_HANDLE hFibHandle,
    IN FIB_NODE_S *pstFibCurrent,
    OUT FIB_NODE_S *pstFibNext
);
BS_STATUS FIB_Show (IN FIB_HANDLE hFibHandle);
CHAR * FIB_GetFlagString(IN UINT uiFlag, OUT CHAR *pcFlagString);

#ifdef __cplusplus
    }
#endif 

#endif 


