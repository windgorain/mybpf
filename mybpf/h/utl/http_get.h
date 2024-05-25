/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2008-10-20
* Description: 
* History:     
******************************************************************************/

#ifndef __HTTP_GET_H_
#define __HTTP_GET_H_

#ifdef __cplusplus
    extern "C" {
#endif 

typedef VOID (*PF_HTTPGET_NOTIFY_FUNC)(IN CHAR *pszPath, IN CHAR *pszSaveAsFile, IN UINT ulFileSize, IN UINT ulDownLoadSize);



BS_STATUS HTTPGET_GetFile
(
    IN CHAR *pszServer,
    IN USHORT usPort, 
    IN CHAR *pszPath,
    IN time_t ulOldFileTime, 
    IN CHAR *pszSaveAsFile  
);


#ifdef __cplusplus
    }
#endif 

#endif 


