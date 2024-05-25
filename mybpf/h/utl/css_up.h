/******************************************************************************
* Copyright (C), LiXingang
* Author:      LiXingang  Version: 1.0  Date: 2015-10-20
* Description: 
* History:     
******************************************************************************/

#ifndef __CSS_UP_H_
#define __CSS_UP_H_

#ifdef __cplusplus
    extern "C" {
#endif 


typedef enum
{
    CSS_UP_TYPE_DATA = 0,
    CSS_UP_TYPE_URL
}CSS_UP_TYPE_E;


typedef VOID* CSS_UP_HANDLE;

typedef VOID (*CSS_UP_OUTPUT_PF)
(
    IN CSS_UP_TYPE_E eType,
    IN CHAR *pcData,
    IN ULONG ulDataLen,
    IN VOID *pUserContext
);

extern VOID CSS_UP_Run
(
    IN CSS_UP_HANDLE hParser,
    IN CHAR *pcHtmlData,
    IN ULONG ulHtmlDataLen
);


extern CSS_UP_HANDLE CSS_UP_Create
(
    IN CSS_UP_OUTPUT_PF pfOutput,
    IN VOID *pUserContext
);

VOID CSS_UP_Destroy(IN CSS_UP_HANDLE hParser);
VOID CSS_UP_End(IN CSS_UP_HANDLE hParser);


#ifdef __cplusplus
    }
#endif 

#endif 


