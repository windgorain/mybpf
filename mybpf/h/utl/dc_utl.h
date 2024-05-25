/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2008-11-25
* Description: 
* History:     
******************************************************************************/

#ifndef __DC_UTL_H_
#define __DC_UTL_H_

#include "utl/mkv_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 

#define DC_X_DATA_MAX_NUM 32

typedef struct
{
	CHAR *pcKey;
	CHAR *pcValue;
}KEY_VALUE_S;

typedef struct
{
	UINT uiNum;
	KEY_VALUE_S astKeyValue[DC_X_DATA_MAX_NUM];
}DC_DATA_S;

typedef enum
{
    DC_TYPE_XML,
    DC_TYPE_MYSQL,

    DC_TYPE_MAX
}DC_TYPE_E;

typedef struct
{
    CHAR *pcHost;
    CHAR *pcUserName;
    CHAR *pcPassWord;
    CHAR *pcDbName;
    USHORT usPort;      
}DC_MYSQL_PARAM_S;

typedef int (*PF_DC_WALK_TBL_CB_FUNC)(IN CHAR *pszTblName, IN HANDLE hUserHandle);
typedef int (*PF_DC_WALK_OBJECT_CB_FUNC)(IN DC_DATA_S *pstKey, IN HANDLE hUserHandle);

HANDLE       DC_OpenInstance(IN DC_TYPE_E eDcType, IN VOID *pParam);
VOID        DC_CloseInstance(IN HANDLE hDcHandle);
BS_STATUS  DC_AddTbl
(
    IN HANDLE hDcHandle,
    IN CHAR *pcTableName
);
VOID DC_DelTbl
(
    IN HANDLE hDcHandle,
    IN CHAR *pcTableName
);
UINT DC_GetObjectNum
(
    IN HANDLE hDcHandle,
    IN CHAR *pcTableName
);
BS_STATUS DC_AddObject
(
    IN HANDLE hDcHandle,
    IN CHAR *pcTableName,
    IN DC_DATA_S *pstKey
);
BOOL_T DC_IsObjectExist
(
    IN HANDLE hDcHandle,
    IN CHAR *pcTableName,
    IN DC_DATA_S *pstKey
);
VOID DC_DelObject
(
    IN HANDLE hDcHandle,
    IN CHAR *pcTableName,
    IN DC_DATA_S *pstKey
);
BS_STATUS DC_SetFieldValueAsUint
(
    IN HANDLE hDcHandle,
    IN CHAR *pcTableName,
    IN DC_DATA_S *pstKey,
    IN CHAR *pcFieldName,
    IN UINT uiValue
);
BS_STATUS DC_SetFieldValueAsString
(
    IN HANDLE hDcHandle,
    IN CHAR *pcTableName,
    IN DC_DATA_S *pstKey,
    IN CHAR *pcFieldName,
    IN CHAR *pcValue
);
BS_STATUS DC_GetFieldValueAsUint
(
    IN HANDLE hDcHandle,
    IN CHAR *pcTableName,
    IN DC_DATA_S *pstKey,
    IN CHAR *pcFieldName,
    OUT UINT *puiValue
);
BS_STATUS DC_CpyFieldValueAsString
(
    IN HANDLE hDcHandle,
    IN CHAR *pcTableName,
    IN DC_DATA_S *pstKey,
    IN CHAR *pcFieldName,
    OUT CHAR *pcValue,
    IN UINT uiValueMaxSize
);
VOID DC_WalkTable
(
    IN HANDLE hDcHandle,
    IN PF_DC_WALK_TBL_CB_FUNC pfWalkFunc,
    IN HANDLE hUserHandle
);
VOID DC_WalkObject
(
    IN HANDLE hDcHandle,
    IN CHAR *pcTableName,
    IN PF_DC_WALK_OBJECT_CB_FUNC pfWalkFunc,
    IN HANDLE hUserHandle
);
CHAR * DC_GetKeyValueByName(IN DC_DATA_S *pstKey, IN CHAR *pcKeyName);
BS_STATUS   DC_Save(IN HANDLE hDcHandle);

#ifdef __cplusplus
    }
#endif 

#endif 


