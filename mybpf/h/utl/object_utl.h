/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2008-8-30
* Description: 
* History:     
******************************************************************************/

#ifndef __OBJECT_UTL_H_
#define __OBJECT_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

#include "utl/nap_utl.h"

#if 1 

typedef struct {
    void *memcap;
    UINT uiMaxNum; 
    UINT uiObjSize; 
}OBJECT_PARAM_S;

typedef int (*OBJECT_WALK_FUNC)(IN HANDLE hAggregate, IN UINT64 ulObjectId, IN USER_HANDLE_S *pstUserHandle);
HANDLE OBJECT_CreateAggregate(OBJECT_PARAM_S *p);
VOID OBJECT_DestroyAggregate(IN HANDLE hAggregate);
VOID OBJECT_EnableSeq(IN HANDLE hAggregate, IN UINT64 ulMask, IN UINT uiCount);
VOID * OBJECT_NewObject(IN HANDLE hAggregate);
VOID OBJECT_FreeObject(IN HANDLE hAggregate, IN VOID *pUserObj);
VOID OBJECT_FreeObjectByID(IN HANDLE hAggregate, IN UINT64 ulID);
VOID * OBJECT_GetObjectByID(IN HANDLE hAggregate, IN UINT64 ulID);
UINT64 OBJECT_GetIDByObject(IN HANDLE hAggregate, IN VOID *pUserObj);
VOID OBJECT_FreeAllObjects(IN HANDLE hAggregate);
BS_STATUS OBJECT_SetProperty(IN VOID *pUserObj, IN UINT uiPropertyIndex, IN HANDLE hValue);
BS_STATUS OBJECT_SetPropertyByID(HANDLE hAggregate, UINT64 id, UINT property_index, void *value);
BS_STATUS OBJECT_GetProperty(IN VOID *pUserObj, IN UINT uiPropertyIndex, OUT HANDLE *phValue);
BS_STATUS OBJECT_GetPropertyByID
(
    IN HANDLE hAggregate,
    IN UINT64 ulObjectId,
    IN UINT uiPropertyIndex,
    OUT HANDLE *phValue
);
int OBJECT_SetKeyValue(HANDLE hAggregate, void *obj, char *key, char *value);
int OBJECT_SetKeyValueByID(HANDLE hAggregate, UINT64 id, char *key, char *value);
char * OBJECT_GetKeyValue(void *obj, char *key);
char * OBJECT_GetKeyValueByID(HANDLE hAggregate, UINT64 id, char *key);
VOID OBJECT_Walk(IN HANDLE hAggregate, IN OBJECT_WALK_FUNC pfFunc, IN USER_HANDLE_S *pstUserHandle);
UINT64 OBJECT_GetNextID(IN HANDLE hAggregate, IN UINT64 uiCurrentID);
UINT OBJECT_GetCount(IN HANDLE hAggregate);
#endif

#if 1  
typedef HANDLE NO_HANDLE;
NO_HANDLE NO_CreateAggregate(OBJECT_PARAM_S *p);
VOID NO_DestroyAggregate(IN NO_HANDLE hAggregate);
VOID NO_EnableSeq(IN NO_HANDLE hAggregate, IN UINT64 ulMask, IN UINT uiCount);
VOID * NO_NewObject(IN NO_HANDLE hAggregate, IN CHAR *pcName);

VOID * NO_NewObjectForNameIndex(IN NO_HANDLE hAggregate, IN CHAR *pcNamePrefix );
UINT64 NO_NewObjectID(IN NO_HANDLE hAggregate, IN CHAR *pcName);
VOID NO_FreeObject(IN NO_HANDLE hAggregate, IN VOID *pObject);
VOID NO_FreeObjectByID(IN NO_HANDLE hAggregate, IN UINT64 ulID);
VOID NO_FreeObjectByName(IN NO_HANDLE hAggregate, IN CHAR *pcName);
UINT64 NO_GetObjectID(IN NO_HANDLE hAggregate, IN VOID * pObject);
CHAR * NO_GetName(IN VOID * pObject);
VOID * NO_GetObjectByID(IN NO_HANDLE hAggregate, IN UINT64 ulID);
VOID * NO_GetObjectByName(IN NO_HANDLE hAggregate, IN CHAR *pcName);
CHAR * NO_GetNameByID(IN NO_HANDLE hAggregate, IN UINT64 ulID);
UINT64 NO_GetIDByName(IN NO_HANDLE hAggregate, IN CHAR *pcName);
BS_STATUS NO_SetProperty(IN VOID *pObj, IN UINT uiPropertyIndex, IN HANDLE hValue);
BS_STATUS NO_SetPropertyByID
(
    IN NO_HANDLE hAggregate,
    IN UINT64 ulObjectId,
    IN UINT uiPropertyIndex,
    IN HANDLE hValue
);
BS_STATUS NO_GetProperty(IN VOID *pObject, IN UINT uiPropertyIndex, OUT HANDLE *phValue);
BS_STATUS NO_GetPropertyByID
(
    IN NO_HANDLE hAggregate,
    IN UINT64 ulObjectId,
    IN UINT uiPropertyIndex,
    OUT HANDLE *phValue
);
BS_STATUS NO_SetKeyValue(HANDLE hAggregate, void *obj, char *key, char *value);
BS_STATUS NO_SetKeyValueByID
(
    IN NO_HANDLE hAggregate,
    IN UINT64 ulObjectId,
    IN CHAR *pcKey,
    IN CHAR *pcValue
);
CHAR * NO_GetKeyValue(IN VOID *pObject, IN CHAR *pcKey);
CHAR * NO_GetKeyValueByID
(
    IN NO_HANDLE hAggregate,
    IN UINT64 ulObjectId,
    IN CHAR *pcKey
);
CHAR * NO_GetKeyValueByName
(
    IN NO_HANDLE hAggregate,
    IN CHAR *pcNodeName,
    IN CHAR *pcKey
);

UINT64 NO_GetNextID(IN NO_HANDLE hAggregate, IN UINT64 ulCurrentID);


CHAR * NO_GetNextName(IN NO_HANDLE hAggregate, IN CHAR *pcCurrentName);

UINT NO_GetCount(IN NO_HANDLE hAggregate);

#endif

#ifdef __cplusplus
    }
#endif 

#endif 


