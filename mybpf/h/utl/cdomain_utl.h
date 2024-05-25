/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2008-12-6
* Description: 
* History:     
******************************************************************************/

#ifndef __CDOMAIN_UTL_H_
#define __CDOMAIN_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

#include "utl/dc_utl.h"
#include "utl/ulm_utl.h"

#define CDOMAIN_MAX_CDOMAIN_NEM_LEN 63
#define CDOMAIN_MAX_USER_NAME_LEN  63
#define CDOMAIN_MAX_USER_GROUP_NAME_LEN 63

typedef enum
{
    CDOMAIN_USER_LOGIN_RESULT_OK = 0,
    CDOMAIN_USER_LOGIN_RESULT_FAIL,
    CDOMAIN_USER_LOGIN_RESULT_BLACKLIST
}CDOMAIN_USER_LOGIN_RESULT_E;



BS_STATUS CDOMAIN_OpenInstance
(
    IN UINT ulMaxDomainNum,
    IN DC_TYPE_E eDcType,
    IN VOID *pParam,
    OUT HANDLE *phInstance
);

BS_STATUS CDOMAIN_CloseInstance(IN HANDLE hInstance);



BS_STATUS CDOMAIN_CreateDomain(IN HANDLE hInstance, IN CHAR szDomainName[CDOMAIN_MAX_CDOMAIN_NEM_LEN + 1]);


BS_STATUS CDOMAIN_DelDomain(IN HANDLE hInstance, IN CHAR * pszDomainName);

BS_STATUS CDOMAIN_SetMaxUserNum(IN HANDLE hInstance, IN CHAR * pszDomainName, IN UINT ulMaxUserNum);

UINT CDOMAIN_GetMaxUserNum(IN HANDLE hInstance, IN CHAR * pszDomainName);


BS_STATUS CDOMAIN_SetUserHandleToDomain
(
    IN HANDLE hInstance,
    IN CHAR *pszDomainName,
    IN CHAR *pszKey,
    IN CHAR *pszValue
);


BS_STATUS CDOMAIN_GetUserHandleFromDomain
(
    IN HANDLE hInstance,
    IN CHAR *pszDomainName,
    IN CHAR *pszKey,
    OUT CHAR *pszValue,
    IN UINT ulValueLen  
);


typedef int (*PF_CDOMAIN_WALK_CDOMAIN_FUNC)(IN HANDLE hInstance, IN CHAR * pszDomainName, IN HANDLE hUserHandle);


VOID CDOMAIN_WalkDomain
(
    IN HANDLE hInstance,
    IN PF_CDOMAIN_WALK_CDOMAIN_FUNC pfWalkFunc,
    IN HANDLE hUserHandle
);


BS_STATUS CDOMAIN_AddSuperAdmin(IN HANDLE hInstance, IN CHAR *pszUserName, IN CHAR *pszPasswd);


BS_STATUS CDOMAIN_DelSuperAdmin(IN HANDLE hInstance, IN CHAR *pszUserName);



BS_STATUS CDOMAIN_AddAdmin(IN HANDLE hInstance, IN CHAR * pszDomainName, IN CHAR *pszUserName, IN CHAR *pszPasswd);


BS_STATUS CDOMAIN_DelAdmin(IN HANDLE hInstance, IN CHAR * pszDomainName, IN CHAR *pszUserName);


BS_STATUS CDOMAIN_AddUser(IN HANDLE hInstance, IN CHAR * pszDomainName, IN CHAR *pszUserName, IN CHAR *pszPasswd);


BS_STATUS CDOMAIN_DelUser(IN HANDLE hInstance, IN CHAR * pszDomainName, IN CHAR *pszUserName);


UINT CDOMAIN_GetSAdminNum(IN HANDLE hInstance);


UINT CDOMAIN_GetAdminNum(IN HANDLE hInstance, IN CHAR * pszDomainName);


UINT CDOMAIN_GetUserNum(IN HANDLE hInstance, IN CHAR * pszDomainName);


BS_STATUS CDOMAIN_AddUserToBlackList(IN HANDLE hInstance, IN CHAR * pszDomainName, IN CHAR *pszUserName);


BS_STATUS CDOMAIN_DelUserFromBlackList(IN HANDLE hInstance, IN CHAR * pszDomainName, IN CHAR *pszUserName);


BS_STATUS CDOMAIN_SetUserReadOnly
(
    IN HANDLE hInstance,
    IN CHAR *pszDomainName,
    IN CHAR *pszUserName,
    IN BOOL_T bReadOnly
);

BS_STATUS CDOMAIN_GetUserReadOnlyAttribute
(
    IN HANDLE hInstance,
    IN CHAR *pszDomainName,
    IN CHAR *pszUserName,
    OUT BOOL_T *pbReadOnly
);


BS_STATUS CDOMAIN_SetUserHandleToUser(IN HANDLE hInstance, IN CHAR * pszDomainName, IN CHAR *pszUserName, IN CHAR *pszKey, IN CHAR *pszValue);


BS_STATUS CDOMAIN_GetUserHandleFromUser
(
    IN HANDLE hInstance,
    IN CHAR * pszDomainName,
    IN CHAR *pszUserName,
    IN CHAR *pszKey,
    OUT CHAR *pszValue,
    IN UINT ulValueLen  
);



typedef int (*PF_CDOMAIN_WALK_USER_IN_CDOMAIN_FUNC) (HANDLE hInstance,
        CHAR *pszDomainName, CHAR *pszUserName, HANDLE hUserHandle);


VOID CDOMAIN_WalkUserInDomain
(
    IN HANDLE hInstance,
    IN CHAR * pszDomainName,
    IN PF_CDOMAIN_WALK_USER_IN_CDOMAIN_FUNC pfWalkFunc,
    IN HANDLE hUserHandle
);

VOID CDOMAIN_WalkAdminInDomain
(
    IN HANDLE hInstance,
    IN CHAR * pszDomainName,
    IN PF_CDOMAIN_WALK_USER_IN_CDOMAIN_FUNC pfWalkFunc,
    IN HANDLE hUserHandle
);

VOID CDOMAIN_WalkSAdminInDomain
(
    IN HANDLE hInstance,
    IN PF_CDOMAIN_WALK_USER_IN_CDOMAIN_FUNC pfWalkFunc,
    IN HANDLE hUserHandle
);



typedef int (*PF_CDOMAIN_WALK_GROUP_OF_USER_FUNC)(HANDLE hInstance, CHAR *pszDomainName,
        CHAR *pszUserName, CHAR *pszUerGroupName, HANDLE hUserHandle);




typedef VOID (*PF_CDOMAIN_USER_EVENT_FUNC)
    (IN HANDLE hInstance, IN CHAR *pszDomainName, IN CHAR *pszUserName, IN UINT ulEvent, IN USER_HANDLE_S *pstUserHandle);


BS_STATUS CDOMAIN_CreateUserGroup(IN HANDLE hInstance, IN CHAR * pszDomainName, IN CHAR *pszUserGroupName);



BS_STATUS CDOMAIN_DelUserGroup(IN HANDLE hInstance, IN CHAR * pszDomainName, IN CHAR *pszUserGroupName);


BS_STATUS CDOMAIN_AddUserToGroup(IN HANDLE hInstance, IN CHAR * pszDomainName, IN CHAR *pszUserName, IN CHAR *pszUserGroupName);


BS_STATUS CDOMAIN_DelUserFromGroup(IN HANDLE hInstance, IN CHAR * pszDomainName, IN CHAR *pszUserName, IN CHAR *pszUserGroupName);


BS_STATUS CDOMAIN_SetUserHandleToUserGroup(IN HANDLE hInstance, IN CHAR * pszDomainName, IN CHAR *pszUserGroupName, IN CHAR *pszKey, IN CHAR *pszValue);


BS_STATUS CDOMAIN_GetUserHandleFromUserGroup
(
    IN HANDLE hInstance,
    IN CHAR * pszDomainName,
    IN CHAR *pszUserGroupName,
    IN CHAR *pszKey,
    OUT CHAR *pszValue,
    IN UINT ulValueLen
);



typedef VOID (*PF_CDOMAIN_WALK_USER_IN_GROUP_FUNC)
    (IN HANDLE hInstance, IN CHAR * pszDomainName, IN CHAR *pszUserGroupName, IN CHAR *pszUserName, IN UINT ulUserHandle);



CDOMAIN_USER_LOGIN_RESULT_E CDOMAIN_CheckSuperAdmin(IN HANDLE hInstance, IN CHAR *pszUserName, IN CHAR *pszPassWd);


CDOMAIN_USER_LOGIN_RESULT_E CDOMAIN_CheckAdmin(IN HANDLE hInstance, IN CHAR *pszDomainName, IN CHAR *pszUserName, IN CHAR *pszPassWd);


CDOMAIN_USER_LOGIN_RESULT_E CDOMAIN_CheckUser(IN HANDLE hInstance, IN CHAR *pszDomainName, IN CHAR *pszUserName, IN CHAR *pszPassWd);

BS_STATUS CDOMAIN_GetUserPropertyAsUint
(
    IN HANDLE hInstance,
    IN CHAR *pszDomainName,
    IN CHAR *pszUserName,
    IN CHAR *pcPropertyName,
    OUT UINT *puiValue
);

BS_STATUS CDOMAIN_Save(IN HANDLE hInstance);

#ifdef __cplusplus
    }
#endif 

#endif 


