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


BS_STATUS HTTPGET_GetFile
(
    char *server,
    U16 port,  
    char *path,
    time_t old_file_time, 
    char *save_file_name 
);

BS_STATUS HTTPSGET_GetFile
(
    char *server,
    U16 port, 
    char *path,
    time_t old_file_time, 
    char *save_file_name 
);

#ifdef __cplusplus
    }
#endif 

#endif 


