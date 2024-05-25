/******************************************************************************
* Copyright (C), LiXingang
* Author:      LiXingang  Version: 1.0  Date: 2015-6-19
* Description: my conf file type
* History:     
******************************************************************************/

#ifndef __MCF_UTL_H_
#define __MCF_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

typedef HANDLE MCF_HANDLE;

typedef struct
{
    MKV_MARK_S  stSecRoot;
    BOOL_T     bIsSort;
    BOOL_T     bReadOnly;
    BOOL_T     bIsUtf8;
    CHAR       *pucFileName;
    CHAR       *pucFileContent;
    UINT       uiMemSize;   
}MCF_HEAD_S;



CHAR * MCF_GetProp(IN MCF_HANDLE hMcfHandle, IN CHAR *pcKey, IN CHAR *pcProp);

#ifdef __cplusplus
    }
#endif 

#endif 


