/******************************************************************************
* Copyright (C), LiXingang
* Author:      LiXingang  Version: 1.0  Date: 2015-11-28
* Description: 
* History:     
******************************************************************************/

#ifndef __HTTP_AUTH_H_
#define __HTTP_AUTH_H_

#ifdef __cplusplus
    extern "C" {
#endif 

typedef VOID* HTTP_AUTH_HANDLE;

#define HTTP_AUTH_DIGEST_LEN 511

BS_STATUS HTTP_AUTH_BasicBuild
(
    IN CHAR *pcUserName,
    IN CHAR *pcPassWord,
    OUT CHAR szResult[HTTP_AUTH_DIGEST_LEN + 1]
);

BOOL_T HTTP_AUTH_IsDigestUnauthorized(IN CHAR *pcWwwAuthenticate);
HTTP_AUTH_HANDLE HTTP_Auth_ClientCreate();
VOID HTTP_Auth_ClientDestroy(IN HTTP_AUTH_HANDLE hAuthHandle);
BS_STATUS HTTP_Auth_ClientSetAuthContext(IN HTTP_AUTH_HANDLE hAuthHandle, IN CHAR *pcWwwAuthenticate);
BS_STATUS HTTP_Auth_ClientSetUser(IN HTTP_AUTH_HANDLE hAuthHandle, IN CHAR *pcUser, IN CHAR *pcPassword);
BS_STATUS HTTP_AUTH_ClientDigestBuild
(
    IN HTTP_AUTH_HANDLE hAuthHandle,
    IN CHAR *pcMethod,
    IN CHAR *pcUri,
    OUT CHAR szDigest[HTTP_AUTH_DIGEST_LEN + 1]
);

#ifdef __cplusplus
    }
#endif 

#endif 


