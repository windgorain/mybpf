/******************************************************************************
* Copyright (C), LiXingang
* Author:      lixingang  Version: 1.0  Date: 2008-2-1
* Description: 
* History:     
******************************************************************************/

#ifndef __ULM_UTL_H_
#define __ULM_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

#define ULM_MAX_USER_NAME_LEN   63
#define ULM_USER_COOKIE_LEN     31

typedef HANDLE ULM_HANDLE;

typedef VOID (*PF_ULM_USER_DEL_NOTIFY)(IN ULM_HANDLE hUlmHandle, IN UINT ulUserId);

typedef struct
{
    CHAR szUserName[ULM_MAX_USER_NAME_LEN + 1];
    UINT ulLoginTime; 
}ULM_USER_INFO_S;


ULM_HANDLE ULM_CreateInstance(IN UINT ulMaxUserNum );
BS_STATUS ULM_DesTroyInstance(IN ULM_HANDLE hUlmHandle);
UINT ULM_AddUser(IN ULM_HANDLE hUlmHandle, IN CHAR *pszUserName);
BS_STATUS ULM_DelUser(IN ULM_HANDLE hUlmHandle, IN UINT ulUserId);
BS_STATUS ULM_DelAllUser(IN ULM_HANDLE hUlmHandle);
BS_STATUS ULM_SetUserHandle(IN ULM_HANDLE hUlmHandle, IN UINT ulUserId, IN HANDLE hUserHandle);
BS_STATUS ULM_GetUserHandle(IN ULM_HANDLE hUlmHandle, IN UINT ulUserId, OUT HANDLE *phUserHandle);
BS_STATUS ULM_SetUserKeyValue(IN ULM_HANDLE hUlmHandle, IN UINT ulUserId, IN CHAR *pcKey, IN CHAR *pcValue);
CHAR * ULM_GetUserKeyValue(IN ULM_HANDLE hUlmHandle, IN UINT ulUserId, IN CHAR *pcKey);
BS_STATUS ULM_SetUserFlag(IN ULM_HANDLE hUlmHandle, IN UINT ulUserId, IN UINT uiFlag);
BS_STATUS ULM_GetUserFlag(IN ULM_HANDLE hUlmHandle, IN UINT ulUserId, OUT UINT *puiFlag);
BS_STATUS ULM_SetUserDelNotifyFunc(IN ULM_HANDLE hUlmHandle, IN PF_ULM_USER_DEL_NOTIFY pfFunc);
BS_STATUS ULM_GetUserInfo(IN ULM_HANDLE hUlmHandle, IN UINT ulUserId, OUT ULM_USER_INFO_S *pstUserInfo);
BS_STATUS ULM_SetUserCookie(IN ULM_HANDLE hUlmHandle, IN UINT ulUserId, IN CHAR szCookie[ULM_USER_COOKIE_LEN+1]);
CHAR * ULM_GetUserCookie(IN ULM_HANDLE hUlmHandle, IN UINT ulUserId);
UINT ULM_GetUserIDByCookie(IN ULM_HANDLE hUlmHandle, IN CHAR* pcCookie);
UINT ULM_GetNextUserID(IN ULM_HANDLE hUlmHandle, IN UINT uiCurrentUserID);
UINT ULM_GetUserIdByName(IN ULM_HANDLE hUlmHandle, IN CHAR *pszName);
CHAR * ULM_GetUserNameById(IN ULM_HANDLE hUlmHandle, IN UINT uiUserID);
BS_STATUS ULM_SetMaxUserRefNum(IN ULM_HANDLE hUlmHandle, IN UINT ulUserId, IN UINT ulMaxRefNum);
BS_STATUS ULM_IncUserRefNum(IN ULM_HANDLE hUlmHandle, IN UINT ulUserId);
BS_STATUS ULM_DecUserRefNum(IN ULM_HANDLE hUlmHandle, IN UINT ulUserId);
BS_STATUS ULM_GetUserRefNum(IN ULM_HANDLE hUlmHandle, IN UINT ulUserId, OUT UINT *pulRefNum);
BS_STATUS ULM_SetTimeOutTime(IN ULM_HANDLE hUlmHandle, IN UINT ulTimeOutTime );
BS_STATUS ULM_GetTimeOutTime(IN ULM_HANDLE hUlmHandle, OUT UINT *pulTimeOutTime );
BS_STATUS ULM_StartTimeOut(IN ULM_HANDLE hUlmHandle, IN UINT ulUserId);
BS_STATUS ULM_StopTimeOut(IN ULM_HANDLE hUlmHandle, IN UINT ulUserId);
BS_STATUS ULM_StopAllUserTimeOut(IN ULM_HANDLE hUlmHandle);
BS_STATUS ULM_StartAllUserTimeOut(IN ULM_HANDLE hUlmHandle);
VOID ULM_SetIfTimeOutIgnoreRef(IN ULM_HANDLE hUlmHandle, IN BOOL_T bIgnore);

VOID ULM_TimeOut(IN ULM_HANDLE hUlmHandle);
#ifdef __cplusplus
    }
#endif 

#endif 


