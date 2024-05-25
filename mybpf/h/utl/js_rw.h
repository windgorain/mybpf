/******************************************************************************
* Copyright (C), LiXingang
* Author:      LiXingang  Version: 1.0  Date: 2015-10-20
* Description: 
* History:     
******************************************************************************/

#ifndef __JS_RW_H_
#define __JS_RW_H_

#ifdef __cplusplus
    extern "C" {
#endif 

typedef VOID* JS_RW_HANDLE;

typedef VOID (*JS_RW_OUTPUT_PF)(IN CHAR *pcData, IN ULONG ulDataLen, IN VOID *pUserContext);

extern JS_RW_HANDLE JS_RW_Create(IN JS_RW_OUTPUT_PF pfOutput, IN VOID *pUserContext);

extern VOID JS_RW_Destroy(JS_RW_HANDLE hJsRw);

extern VOID JS_RW_Run(IN JS_RW_HANDLE hJsRw, IN CHAR *pcJs, IN UINT uiJsLen);

extern VOID JS_RW_End(IN JS_RW_HANDLE hJsRw);


#ifdef __cplusplus
    }
#endif 

#endif 


