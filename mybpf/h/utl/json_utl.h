/******************************************************************************
* Copyright (C), 2000-2006,  Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2016-11-1
* Description: 
* History:     
******************************************************************************/

#ifndef __JSON_UTL_H_
#define __JSON_UTL_H_

#include "utl/cjson.h"
#include "utl/object_utl.h"
#include "utl/mime_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 


typedef BOOL_T (*PF_JSON_DEL_NOTIFY)(IN HANDLE hUserHandle, IN CHAR *pcNodeName);
typedef BOOL_T (*PF_JSON_LIST_IS_PERMIT)(IN HANDLE hUserHandle, IN UINT64 ulNodeID);

typedef char* (*PF_JSON_LIST_GET_NEXT)(char *cur_name);
typedef char* (*PF_JSON_LIST_GET_PROPERTY)(char *name, char *property);

VOID JSON_SetSuccess(IN cJSON * pstJson);
VOID JSON_SetFailed(IN cJSON * pstJson, IN CHAR *pcReason);
BS_STATUS JSON_AppendInfo(IN cJSON * pstJson, IN CHAR *pcInfo);
VOID JSON_AddString(IN cJSON * pstJson, IN CHAR *pcKey, IN CHAR *pcValue);


BS_STATUS JSON_NO_IsExist
(
    IN NO_HANDLE hNo,
    IN MIME_HANDLE hMime,
    IN cJSON * pstJson
);
BS_STATUS JSON_NO_List
(
    IN NO_HANDLE hNo,
    IN cJSON * pstJson,
    IN CHAR **apcPropertys
);
BS_STATUS JSON_NO_ListWithCallBack
(
    IN NO_HANDLE hNo,
    IN cJSON * pstJson,
    IN CHAR **apcPropertys,
    IN PF_JSON_LIST_IS_PERMIT pfIsPermit,
    IN HANDLE hUserHandle
);


BS_STATUS JSON_NO_Add
(
    IN NO_HANDLE hNo,
    IN MIME_HANDLE hMime,
    IN cJSON * pstJson,
    IN CHAR **apcPropertys
);
BS_STATUS JSON_NO_Modify
(
    IN NO_HANDLE hNo,
    IN MIME_HANDLE hMime,
    IN cJSON * pstJson,
    IN CHAR **apcPropertys
);
VOID JSON_NO_Get
(
    IN NO_HANDLE hNo,
    IN MIME_HANDLE hMime,
    IN cJSON * pstJson,
    IN CHAR **apcPropertys
);
VOID JSON_NO_Delete
(
    IN NO_HANDLE hNo,
    IN MIME_HANDLE hMime,
    IN cJSON * pstJson
);
VOID JSON_NO_DeleteWithNotify
(
    IN NO_HANDLE hNo,
    IN MIME_HANDLE hMime,
    IN cJSON * pstJson,
    IN PF_JSON_DEL_NOTIFY pfDelNotify,
    IN HANDLE hUserHandle
);

BS_STATUS JSON_List(cJSON *pstJson, CHAR **apcPropertys,
        PF_JSON_LIST_GET_NEXT get_next, PF_JSON_LIST_GET_PROPERTY get_property);

#ifdef __cplusplus
    }
#endif 

#endif 


