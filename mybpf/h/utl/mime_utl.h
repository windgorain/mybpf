/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2007-9-7
* Description: 
* History:     
******************************************************************************/

#ifndef __MIME_UTL_H_
#define __MIME_UTL_H_

#include "utl/mempool_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 

typedef     VOID*   MIME_HANDLE;


typedef struct tagMIME_DataNode
{
    DLL_NODE_S stLink;    
    CHAR *pcKey;          
    CHAR *pcValue;        
} MIME_DATA_NODE_S;



typedef struct tagMIME_DataList
{
    DLL_HEAD_S  stDataList;
    MEMPOOL_HANDLE hMemPool;
} MIME_DATALIST_S;

MIME_HANDLE MIME_Create (VOID);
VOID MIME_Destroy (IN MIME_HANDLE hMimeHandle);
BS_STATUS MIME_Parse(IN MIME_HANDLE hMimeHandle, IN CHAR cSeparator, IN CHAR *pcString);
BS_STATUS MIME_ParseParam(IN MIME_HANDLE hMimeHandle, IN CHAR *pcParam);
BS_STATUS MIME_ParseData (IN MIME_HANDLE hMimeHandle, IN CHAR *pcData);
BS_STATUS MIME_ParseCookie (IN MIME_HANDLE hMimeHandle, IN CHAR *pcData);
BS_STATUS MIME_ParseContentDispos(IN MIME_HANDLE hMimeHandle, IN CHAR *pcData);

BS_STATUS MIME_SetKeyValue(IN MIME_HANDLE hMimeHandle, IN CHAR *pcKey, IN CHAR *pcValue);
CHAR* MIME_GetKeyValue(IN MIME_HANDLE hMimeHandle, IN CHAR *pcName);
MIME_DATA_NODE_S * MIME_GetNextParam(IN MIME_HANDLE hMimeHandle, IN MIME_DATA_NODE_S *pstParam);
void MIME_Cat(IN MIME_HANDLE hMimeHandleDst, IN MIME_HANDLE hMimeHandleSrc);
void MIME_Print(MIME_HANDLE hMime);

#ifdef __cplusplus
    }
#endif 

#endif 





