/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2009-1-4
* Description: 
* 在线Domain管理,实现功能如下:
* 1. 创建实例
* 2. 创建/删除Domain
* 3. 根据名字得到DomainId
* 4. 得到Domain属性
* 5. 添加/删除用户
* 6. 挂接/取属性
******************************************************************************/

#ifndef __ODOMAIN_UTL_H_
#define __ODOMAIN_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

typedef enum
{
    _ODOMAIN_USER_TYPE_SUPER_ADMIN = 0,
    _ODOMAIN_USER_TYPE_DOMAIN_ADMIN,
    _ODOMAIN_USER_TYPE_DOMAIN_USER
}DOMAIN_USER_TYPE_E;

BS_STATUS ODOMAIN_CreateInstance
(
    IN UINT ulMaxDomainNum,
    IN UINT ulMaxProPertyNum,
    OUT HANDLE *phODomainInstanceId
);

VOID ODOMAIN_DelInstance(IN HANDLE hODomainInstanceId);

BS_STATUS ODOMAIN_CreateDomain
(
    IN HANDLE hODomainInstanceId,
    IN CHAR *pszDomainName,
    IN UINT ulMaxOnlineUserNum,
    OUT UINT *pulDomainId
);

VOID ODOMAIN_DelDomain(IN HANDLE hODomainInstanceId, IN UINT ulDomainId);

BS_STATUS ODOMAIN_SetProperty
(
    IN HANDLE hODomainInstanceId,
    IN UINT ulDomainId,
    IN UINT ulPropertyIndex,
    IN HANDLE hValue
);

BS_STATUS ODOMAIN_GetProperty
(
    IN HANDLE hODomainInstanceId,
    IN UINT ulDomainId,
    IN UINT ulPropertyIndex,
    OUT HANDLE *phValue
);

UINT ODOMAIN_GetDomainIdByName(IN HANDLE hODomainInstanceId, IN CHAR *pszName);

CHAR * ODOMAIN_GetDomainNameById(IN HANDLE hODomainInstanceId, IN UINT ulDomainId);

UINT ODOMAIN_AddUser
(
    IN HANDLE hODomainInstanceId,
    IN UINT ulDomainId,    
    IN CHAR *pszUserName,
    IN DOMAIN_USER_TYPE_E eUserType
);

BS_STATUS ODMAIN_DelUser
(
    IN HANDLE hODomainInstanceId,
    IN UINT ulDomainId,
    IN DOMAIN_USER_TYPE_E eUserType,
    IN UINT ulUserId
);

UINT ODOMAIN_GetNextUser
(
    IN HANDLE hODomainInstanceId,
    IN UINT ulDomainId,
    IN DOMAIN_USER_TYPE_E eUserType,
    IN UINT uiCurrentUserID
);

BS_STATUS ODOMAIN_GetUserIdByCookie
(
    IN HANDLE hODomainInstanceId,
    IN CHAR *pszCookie,
    OUT UINT *pulDomainId,
    OUT DOMAIN_USER_TYPE_E *peUsrType,
    OUT UINT *pulUserId
);

CHAR * ODOMAIN_GetUserCookieById
(
    IN HANDLE hODomainInstanceId,
    IN UINT ulDomainId,
    IN DOMAIN_USER_TYPE_E eUserType,
    IN UINT ulUserId
);

BS_STATUS ODOMAIN_GetUserInfo
(
    IN HANDLE hODomainInstanceId,
    IN UINT ulDomainId,
    IN DOMAIN_USER_TYPE_E eUserType,
    IN UINT ulUserId,
    OUT ULM_USER_INFO_S *pstUserInfo
);

UINT ODOMAIN_GetUserIdByName
(
    IN HANDLE hODomainInstanceId,
    IN UINT ulDomainId,
    IN DOMAIN_USER_TYPE_E eUserType,
    IN CHAR *pcUserName
);


#ifdef __cplusplus
    }
#endif 

#endif 


