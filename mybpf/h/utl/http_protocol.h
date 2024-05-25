/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2007-5-19
* Description: 
* History:     
******************************************************************************/

#ifndef __HTTP_PROTOCOL_H_
#define __HTTP_PROTOCOL_H_

#ifdef __cplusplus
    extern "C" {
#endif 


#define HTTP_PROTOCOL_MAX_HEAD_LEN 4095


typedef enum
{
    HTTP_PROTOCOL_VER_09 = 0,
    HTTP_PROTOCOL_VER_10,
    HTTP_PROTOCOL_VER_11,
    HTTP_PROTOCOL_VER_UNKNOWN
}HTTP_PROTOCOL_VER_E;

typedef enum
{
    HTTP_PROTOCOL_TYPE_HTTP = 0,
    HTTP_PROTOCOL_TYPE_HTTPS,
    HTTP_PROTOCOL_TYPE_UNKNOWN
}HTTP_PROTOCOL_TYPE_E;


#ifdef __cplusplus
    }
#endif 

#endif 


