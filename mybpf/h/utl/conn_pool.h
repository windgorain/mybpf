/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2007-6-3
* Description: 
* History:     
******************************************************************************/

#ifndef __CONN_POOL_H_
#define __CONN_POOL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

extern BS_STATUS CONN_POOL_Create(IN UINT ulMaxNodeNum, OUT HANDLE *phHandle);
extern BS_STATUS CONN_POOL_Delete(IN HANDLE hHandle);
extern BS_STATUS CONN_POOL_Add(IN HANDLE hHandle, IN CHAR *pcConnName, IN UINT ulConnHandle);
extern BS_STATUS CONN_POOL_Get(IN HANDLE hHandle, IN CHAR *pcConnName, OUT UINT *pulConnHandle);
extern BS_STATUS CONN_POOL_DelByConnHandle(IN HANDLE hHandle, IN UINT ulConnHandle);

#ifdef __cplusplus
    }
#endif 

#endif 


