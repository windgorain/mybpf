/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2010-5-20
* Description: 
* History:     
******************************************************************************/

#ifndef __STACK_UTL_H_
#define __STACK_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

#if 1
HANDLE HSTACK_Create(IN UINT uiStackSize);
void HSTACK_Reset(IN HANDLE hHandle);
VOID HSTACK_Destory(IN HANDLE hHandle);
UINT HSTACK_GetStackSize(IN HANDLE hHandle);
BOOL_T HSTACK_IsDynamic(IN HANDLE hHandle);
UINT HSTACK_GetCount(IN HANDLE hHandle);

BS_STATUS HSTACK_Push(IN HANDLE hHandle, IN HANDLE hUserHandle);

BS_STATUS HSTACK_Pop(IN HANDLE hHandle, OUT HANDLE *phUserHandle);

HANDLE HSTACK_GetValueByIndex(IN HANDLE hHandle, IN UINT uiIndex);
BS_STATUS HSTACK_SetValueByIndex(IN HANDLE hHandle, IN UINT uiIndex, IN HANDLE hValue);

BS_STATUS HSTACK_GetTop(IN HANDLE hHandle, OUT HANDLE *phUserHandle);

#endif








#ifdef __cplusplus
    }
#endif 

#endif 


