/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2014-8-28
* Description: 
* History:     
******************************************************************************/

#ifndef __VDOMAIN_UTL_H_
#define __VDOMAIN_UTL_H_

#include "utl/dc_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 

#define VDOMAIN_MAX_USER_NAME_LEN 63
#define VDOMAIN_MAX_PASSWORD_LEN  63
#define VDOMAIN_MAX_DOMAIN_NAME_LEN 255

#define VDOMAIN_ROOT_DOMAIN_ID 1

typedef VOID* VDOMAIN_HANDLE;

typedef enum
{
    VDOMAIN_TYPE_SUPER = 0,
    VDOMAIN_TYPE_NORMAL,

    VDOMAIN_TYPE_MAX
}VDOMAIN_TYPE_E;


VDOMAIN_HANDLE VDOMAIN_CreateInstance(IN DC_TYPE_E eType, IN VOID *pParam);
UINT VDOMAIN_AddRootDomain(IN VDOMAIN_HANDLE hVDomain, IN VDOMAIN_TYPE_E eType);
UINT VDOMAIN_AddDomain(IN VDOMAIN_HANDLE hVDomain, IN UINT uiSuperDomainID, IN VDOMAIN_TYPE_E eType);
BOOL_T VDOMAIN_IsDomainExist(IN VDOMAIN_HANDLE hVDomain, IN UINT uiDomainID);
VDOMAIN_TYPE_E VDOMAIN_GetDomainType(IN VDOMAIN_HANDLE hVDomain, IN UINT uiDomainID);
BS_STATUS VDOMAIN_SetDomainName
(
    IN VDOMAIN_HANDLE hVDomain,
    IN UINT uiDomainID,
    IN CHAR *pcDomainName
);
BS_STATUS VDOMAIN_GetDomainName
(
    IN VDOMAIN_HANDLE hVDomain,
    IN UINT uiDomainID,
    OUT CHAR *pcBuf,
    IN UINT uiBufSize
);
UINT VDOMAIN_GetNextDomain(IN VDOMAIN_HANDLE hVDomain, IN UINT uiCurrentDomainID);
BS_STATUS VDOMAIN_SetAdmin
(
    IN VDOMAIN_HANDLE hVDomain,
    IN UINT uiDomainID,
    IN CHAR *pcUserName,
    IN CHAR *pcPassWord
);

BOOL_T VDOMAIN_CheckAdmin
(
    IN VDOMAIN_HANDLE hVDomain,
    IN UINT uiDomainID,
    IN CHAR *pcUserName,
    IN CHAR *pcPassWord
);
BS_STATUS VDOMAIN_AddUser
(
    IN VDOMAIN_HANDLE hVDomain,
    IN UINT uiDomainID,
    IN CHAR *pcUserName,
    IN CHAR *pcPassWord,
    IN UINT uiUserFlag
);

BOOL_T VDOMAIN_CheckUser
(
    IN VDOMAIN_HANDLE hVDomain,
    IN UINT uiDomainID,
    IN CHAR *pcUserName,
    IN CHAR *pcPassWord
);
UINT VDOMAIN_GetUserFlag
(
    IN VDOMAIN_HANDLE hVDomain,
    IN UINT uiDomainID,
    IN CHAR *pcUserName
);
BS_STATUS VDOMAIN_AddTbl
(
    IN VDOMAIN_HANDLE hVDomain,
    IN UINT uiDomainID,
    IN CHAR *pcSuffix
);

#ifdef __cplusplus
    }
#endif 

#endif 


