/******************************************************************************
* Copyright (C), LiXingang
* Author:      LiXingang  Version: 1.0  Date: 2015-11-5
* Description: 
* History:     
******************************************************************************/

#ifndef __SSL_UTL_H_
#define __SSL_UTL_H_

#include "utl/pki_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 

#define SSL_UTL_E_NONE       1      
#define SSL_UTL_E_ERROR      -1
#define SSL_UTL_E_WANT_READ  -2
#define SSL_UTL_E_WANT_WRITE -3

typedef int (*PF_SSL_UTL_CTX_NextProtoSelectCb)(VOID *pstSsl, unsigned char **out, unsigned char *outlen, const unsigned char *in, unsigned int inlen, void *arg);

VOID SSL_UTL_Init();
VOID * SSL_UTL_Ctx_Create(int min_version, int max_version);
VOID SSL_UTL_Ctx_Free(IN VOID *pstSslCtx);
VOID SSL_UTL_Ctx_DisableSsl3(IN VOID *pstSslCtx, IN BOOL_T bDisable);
BS_STATUS SSL_UTL_Ctx_LoadCert(IN VOID *pstSslCtx, IN CHAR *pcCACert, IN CHAR *pcLocalCert, IN CHAR *pcKeyFile);
BS_STATUS SSL_UTL_Ctx_LoadSelfSignCert(IN VOID *pstSslCtx, IN PKI_DOMAIN_CONFIGURE_S *pstConf);
int SSL_UTL_Ctx_LoadVerifyLocations(IN VOID *pstSslCtx, IN CHAR *caFile, IN CHAR *caPath);
VOID * SSL_UTL_New(IN VOID *pstSslCtx);
VOID SSL_UTL_Free(IN VOID *pstSsl);
INT SSL_UTL_SetFd(IN VOID * pstSsl, IN INT iFd);
INT SSL_UTL_Connect(IN VOID *pstSsl);
void * SSL_UTL_BlockConnect(unsigned int ip, unsigned short port, char * host_name, int timeout);
INT SSL_UTL_Accept(IN VOID *pstSsl);
INT SSL_UTL_Read(IN VOID *pstSsl, IN VOID *pBuf, IN INT iNum);
INT SSL_UTL_Write(IN VOID *pstSsl, IN VOID *pBuf, IN INT iNum);
INT SSL_UTL_Pending(IN VOID *pstSsl);
BS_STATUS SSL_UTL_SetAppData(IN VOID *pstSsl, IN CHAR *pcAppString);
BS_STATUS SSL_UTL_SetHostName(IN VOID *pstSsl, IN CHAR *pcHostName);
BS_STATUS SSL_UTL_Renegotiate(IN VOID *pstSsl);
void * SSL_UTL_GetSslCert(void *ssl);

#ifdef __cplusplus
    }
#endif 

#endif 


