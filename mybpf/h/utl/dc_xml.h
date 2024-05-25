#ifndef __DC_XML_H_
#define __DC_XML_H_

#include "utl/string_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 

HANDLE DC_XML_OpenInstance(IN VOID *pParam);
VOID DC_XML_CloseInstance(IN HANDLE hHandle);
BS_STATUS DC_XML_AddTbl
(
    IN HANDLE hHandle,
    IN CHAR *pcTableName
);
VOID DC_XML_DelTbl
(
    IN HANDLE hHandle,
    IN CHAR *pcTableName
);

BOOL_T DC_XML_IsObjectExist
(
    IN HANDLE hHandle,
    IN CHAR *pcTableName,
    IN DC_DATA_S *pstKey
);

BS_STATUS DC_XML_AddObject
(
    IN HANDLE hHandle,
    IN CHAR *pcTableName,
    IN DC_DATA_S *pstKey
);

VOID DC_XML_DelObject
(
    IN HANDLE hHandle,
    IN CHAR *pcTableName,
    IN DC_DATA_S *pstKey
);

BS_STATUS DC_XML_SetFieldValueAsUint
(
    IN HANDLE hHandle,
    IN CHAR *pcTableName,
    IN DC_DATA_S *pstKey,
    IN CHAR *pcFieldName,
    IN UINT uiValue
);

BS_STATUS DC_XML_SetFieldValueAsString
(
    IN HANDLE hHandle,
    IN CHAR *pcTableName,
    IN DC_DATA_S *pstKey,
    IN CHAR *pcFieldName,
    IN CHAR *pcValue
);

BS_STATUS DC_XML_GetFieldValueAsUint
(
    IN HANDLE hHandle,
    IN CHAR *pcTableName,
    IN DC_DATA_S *pstKey,
    IN CHAR *pcFieldName,
    OUT UINT *puiValue
);

BS_STATUS DC_XML_CpyFieldValueAsString
(
    IN HANDLE hHandle,
    IN CHAR *pcTableName,
    IN DC_DATA_S *pstKey,
    IN CHAR *pcFieldName,
    OUT CHAR *pcValue,
    IN UINT uiValueMaxSize
);

VOID DC_XML_WalkTable
(
    IN HANDLE hHandle,
    IN PF_DC_WALK_TBL_CB_FUNC pfFunc,
    IN HANDLE hUserHandle
);
VOID DC_XML_WalkTableObject
(
    IN HANDLE hHandle,
    IN CHAR *pcTableName,
    IN PF_DC_WALK_OBJECT_CB_FUNC pfFunc,
    IN HANDLE hUserHandle
);

BS_STATUS DC_XML_Save(IN HANDLE hHandle);

HSTRING DC_XML_ToString(IN HANDLE hHandle);

#ifdef __cplusplus
    }
#endif 

#endif 

