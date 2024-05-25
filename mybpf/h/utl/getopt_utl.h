/******************************************************************************
* Copyright (C) LiXingang
* Author:      LiXingang  Version: 1.0  Date: 2012-10-26
* Description: 
* History:     
******************************************************************************/

#ifndef __GETOPT_UTL_H_
#define __GETOPT_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

typedef HANDLE GETOPT_HANDLE;

GETOPT_HANDLE GETOPT_Create
(
    IN UINT uiAgrc,
    IN CHAR **ppcArgv,
    IN CHAR *pcOpt
);

VOID GETOPT_Destory(IN GETOPT_HANDLE hGetOptHandle);

INT GETOPT_GetOpt
(
    IN GETOPT_HANDLE hGetOptHandle,
    OUT CHAR **ppcValue
);

#ifdef __cplusplus
    }
#endif 

#endif 


