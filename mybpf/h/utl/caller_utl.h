/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2014-1-6
* Description: 
* History:     
******************************************************************************/

#ifndef __CALLER_UTL_H_
#define __CALLER_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

typedef enum{
    CALLER_RET_OK, 
    CALLER_RET_INCOMPLETE,  
    CALLER_RET_ERR,  

    CALLER_RET_MAX
}CALLER_RET_E;

typedef CALLER_RET_E (*PF_CALLER_FUNC)(void *ud);


HANDLE Caller_Create();
VOID Caller_Destory(IN HANDLE hCaller);
BS_STATUS Caller_Add
(
    IN HANDLE hCaller,
    IN CHAR *pcName, 
    IN UINT uiPRI,
    IN PF_CALLER_FUNC pfFunc,
    IN USER_HANDLE_S *pstUserHandle
);
CALLER_RET_E Caller_Call(IN HANDLE hCaller);

CALLER_RET_E Caller_Finished(IN HANDLE hCaller);
BS_STATUS Caller_SetByName(IN HANDLE hCaller, IN CHAR *pcName);
VOID Caller_Reset(IN HANDLE hCaller);

VOID Caller_SetDbg(IN HANDLE hCaller);
VOID Caller_ClrDbg(IN HANDLE hCaller);

#ifdef __cplusplus
    }
#endif 

#endif 


