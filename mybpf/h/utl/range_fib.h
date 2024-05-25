/******************************************************************************
* Copyright (C), 2000-2006,  Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2017-4-20
* Description: 
* History:     
******************************************************************************/

#ifndef __RANGE_FIB_H_
#define __RANGE_FIB_H_

#ifdef __cplusplus
    extern "C" {
#endif 

typedef HANDLE RANGE_FIB_HANDLE;


RANGE_FIB_HANDLE RangeFib_Create(IN BOOL_T bCreateLock);
VOID RangeFib_Destory(IN RANGE_FIB_HANDLE hFibHandle);
BS_STATUS RangeFib_Add(IN RANGE_FIB_HANDLE hFibHandle, IN FIB_NODE_S *pstFibNode);
VOID RangeFib_Del(IN RANGE_FIB_HANDLE hFibHandle, IN FIB_KEY_S *pstFibKey);
BS_STATUS RangeFib_Match(IN RANGE_FIB_HANDLE hFibHandle, IN UINT uiDstIp , OUT FIB_NODE_S *pstFibNode);
BS_STATUS RangeFib_Show (IN RANGE_FIB_HANDLE hFibHandle);


#ifdef __cplusplus
    }
#endif 

#endif 


