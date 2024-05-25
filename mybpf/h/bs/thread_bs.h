/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      lixingang  Version: 1.0  Date: 2007-2-9
* Description: 
* History:     
******************************************************************************/
#ifndef __THREAD_H_
#define __THREAD_H_

#include "utl/thread_utl.h"
#include "utl/thread_named.h"

#ifdef __cplusplus
    extern "C" {
#endif 

PLUG_API THREAD_ID _thread_create
(
    IN CHAR  *pucName,
    IN THREAD_CREATE_PARAM_S *pstParam, 
    IN PF_THREAD_NAMED_FUNC pfFunc,
    IN USER_HANDLE_S *pstUserHandle 
);
PLUG_API void _thread_reg_ob(THREAD_NAMED_OB_S *ob);
PLUG_API BS_STATUS THREAD_Display( );

#ifdef __cplusplus
    }
#endif 

#endif 

