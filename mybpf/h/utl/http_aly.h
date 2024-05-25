/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2007-5-19
* Description: 
* History:     
******************************************************************************/

#ifndef __HTTP_ALY_H_
#define __HTTP_ALY_H_

#ifdef __cplusplus
    extern "C" {
#endif 

#include "utl/http_protocol.h"
#include "utl/mbuf_utl.h"
#include "utl/num_utl.h"


#define HTTP_ALY_MAX_URL_LEN 1024
#define HTTP_ALY_MAX_HEAD_LEN 2048
#define HTTP_ALY_MAX_RSP_BASE_HEAD_LEN 512
#define HTTP_ALY_MAX_SERVER_NAME_LEN 128



#define HTTP_ALY_STATUS_CODE_CONTINUE           100  
#define HTTP_ALY_STATUS_CODE_SWITCH_PROTOCOLS   101  
#define HTTP_ALY_STATUS_CODE_OK                 200  
#define HTTP_ALY_STATUS_CODE_CREATED            201  
#define HTTP_ALY_STATUS_CODE_ACCEPTED           202  
#define HTTP_ALY_STATUS_CODE_NON_AUTH           203  
#define HTTP_ALY_STATUS_CODE_NO_CONTENT         204  
#define HTTP_ALY_STATUS_CODE_RESET_CONTENT      205  
#define HTTP_ALY_STATUS_CODE_PARTIAL_CONTENT    206  
#define HTTP_ALY_STATUS_CODE_MULTIPLE_CHOICES   300  
#define HTTP_ALY_STATUS_CODE_MOVED_PMT          301  
#define HTTP_ALY_STATUS_CODE_FOUND              302  
#define HTTP_ALY_STATUS_CODE_SEE_OTHER          303  
#define HTTP_ALY_STATUS_CODE_NOT_MODIFIED       304  
#define HTTP_ALY_STATUS_CODE_USE_PROXY          305  
#define HTTP_ALY_STATUS_CODE_TMP_REDIRECT       307  
#define HTTP_ALY_STATUS_CODE_BAD_REQUEST        400  
#define HTTP_ALY_STATUS_CODE_UNAUTHORIZED       401  
#define HTTP_ALY_STATUS_CODE_PAYMENT_REQUIRED   402  
#define HTTP_ALY_STATUS_CODE_FORBIDDEN          403  
#define HTTP_ALY_STATUS_CODE_NOT_FOUND          404  
#define HTTP_ALY_STATUS_CODE_METHOD_NOT_ALLOWED 405  
#define HTTP_ALY_STATUS_CODE_NOT_ACCEPTABLE     406  
#define HTTP_ALY_STATUS_CODE_PXY_AUTH_REQUIRED  407  
#define HTTP_ALY_STATUS_CODE_REQUEST_TIME_OUT   408  
#define HTTP_ALY_STATUS_CODE_CONFLICT           409  
#define HTTP_ALY_STATUS_CODE_GONE               410  
#define HTTP_ALY_STATUS_CODE_LEN_REQUIRED       411  
#define HTTP_ALY_STATUS_CODE_PRECOND_FAILED     412  
#define HTTP_ALY_STATUS_CODE_ENTIRY_TOO_LARGE   413  
#define HTTP_ALY_STATUS_CODE_URI_TOO_LARGE      414  
#define HTTP_ALY_STATUS_CODE_UNSUPPORT_MEDIA    415  
#define HTTP_ALY_STATUS_CODE_RANGE_NOT_STAISF   416  
#define HTTP_ALY_STATUS_CODE_EXPECTATION_FAILED 417  
#define HTTP_ALY_STATUS_CODE_INNER_SERVER_ERR   500  
#define HTTP_ALY_STATUS_CODE_NOT_IMP            501  
#define HTTP_ALY_STATUS_CODE_BAD_GATEWAY        502  
#define HTTP_ALY_STATUS_CODE_SERVICE_UNAVAI     503  
#define HTTP_ALY_STATUS_CODE_GATEWAY_TIME_OUT   504  
#define HTTP_ALY_STATUS_CODE_VER_NOT_SUPPORT    505  



#define HTTP_ALY_RESPONSE_MODE_CLOSED  1
#define HTTP_ALY_RESPONSE_MODE_LENGTH  2
#define HTTP_ALY_RESPONSE_MODE_CHUNKED 3




typedef enum
{
    HTTP_ALY_METHOD_GET = 0,
    HTTP_ALY_METHOD_POST,
    HTTP_ALY_METHOD_HEAD,
    HTTP_ALY_METHOD_OPTIONS,
    HTTP_ALY_METHOD_PUT,
    HTTP_ALY_METHOD_DELETE,
    HTTP_ALY_METHOD_TRACE,

    HTTP_ALY_METHOD_UNKNOWN
}HTTP_ALY_METHOD_E;

typedef enum{
    HTTP_ALY_BODY_LEN_KNOWN = 1,
    HTTP_ALY_BODY_LEN_TRUNKED,
    HTTP_ALY_BODY_LEN_CLOSED,
    HTTP_ALY_BODY_LEN_BOUNDRY,
    HTTP_ALY_BODY_LEN_UNKNOWN
}HTTP_ALY_BODY_LEN_TYPE_E;

typedef enum{
    HTTP_ALY_CONTINUE = 0,
    HTTP_ALY_STOP,
    HTTP_ALY_ERR
}HTTP_ALY_READ_RET_E;

typedef HTTP_ALY_READ_RET_E (*PF_HTTP_ALY_READ)(IN UINT ulFd, OUT UCHAR *pucBuf, IN UINT ulBufLen, OUT UINT *pulReadLen);


extern HANDLE HTTP_ALY_Create();
extern BS_STATUS HTTP_ALY_Reset(IN HANDLE hHandle);
extern BS_STATUS HTTP_ALY_Delete(IN HANDLE hHandle);
extern BS_STATUS HTTP_ALY_AlyHead(IN HANDLE hHandle);
extern BS_STATUS HTTP_ALY_AddData(IN HANDLE hHandle, IN MBUF_S *pstMbuf);
extern BS_STATUS HTTP_ALY_SetReadFunc (IN HANDLE hHandle, IN PF_HTTP_ALY_READ pfFunc, IN UINT ulFd);
extern BS_STATUS HTTP_ALY_GetHead(IN HANDLE hHandle, OUT MBUF_S **ppstMbuf);
extern MBUF_S * HTTP_ALY_GetBody(IN HANDLE hHandle);
extern BS_STATUS HTTP_ALY_GetData(IN HANDLE hHandle, OUT MBUF_S **ppstMbuf);
extern BS_STATUS HTTP_ALY_TryHead (IN HANDLE hHandle);
extern BS_STATUS HTTP_ALY_ReadHead (IN HANDLE hHandle, OUT MBUF_S **ppMbuf);
BOOL_T HTTP_ALY_IsRecvBodyOK(IN HANDLE hHandle);

extern BS_STATUS HTTP_ALY_TryBody(IN HANDLE hHandle);
extern BS_STATUS HTTP_ALY_ParseKeyValue(IN HANDLE hHandle);
extern BS_STATUS HTTP_ALY_SetBaseRspHttpHeader(IN HANDLE hHandle, IN UCHAR *pucBuf);


extern BS_STATUS HTTP_ALY_SendByChunk(IN HANDLE hHandle, IN UCHAR *pucBuf, IN UINT ulLen, IN BOOL_T bWaitForComplete, OUT UINT *pulSendLen);

extern CHAR * HTTP_ALY_GetField(IN HANDLE hHandle, IN CHAR *pcFieldName);

extern UINT HTTP_ALY_GetFieldValueOffset(IN HANDLE hHandle, IN CHAR *pcFieldName);

extern UINT HTTP_ALY_GetFieldKeyOffset(IN HANDLE hHandle, IN CHAR *pcFieldName);
extern CHAR * HTTP_ALY_GetKeyValue(IN HANDLE hHandle, IN CHAR *pcKey);
extern UINT HTTP_ALY_GetResponseStatusCode(IN HANDLE hHandle);
extern BOOL_T HTTP_ALY_IsRequestKeepAlive(IN HANDLE hHandle);
extern BOOL_T HTTP_ALY_IsResponseKeepAlive(IN HANDLE hHandle);
extern BOOL_T HTTP_ALY_IsProxyConnection(IN HANDLE hHandle);
extern HTTP_PROTOCOL_VER_E HTTP_ALY_GetRequestVer(IN HANDLE hHandle);
extern CHAR * HTTP_ALY_GetProtocol(IN HANDLE hHandle);
extern USHORT HTTP_ALY_GetPort(IN HANDLE hHandle);
extern UINT HTTP_ALY_GetHeadLen(IN HANDLE hHandle);
extern UINT HTTP_ALY_GetBodyLen(IN HANDLE hHandle);
extern CHAR * HTTP_ALY_GetHost(IN HANDLE hHandle);
extern CHAR * HTTP_ALY_GetUrl(IN HANDLE hHandle);
extern CHAR * HTTP_ALY_GetQueryString(IN HANDLE hHandle);
extern UINT HTTP_ALY_GetFileId(IN HANDLE hHandle);
extern CHAR * HTTP_ALY_GetCookie(IN HANDLE hHandle, IN CHAR *pszCookieKey);
extern CHAR * HTTP_ALY_GetReferer(IN HANDLE hHandle);
extern HTTP_ALY_METHOD_E HTTP_ALY_GetMethod(IN HANDLE hHandle);
extern CHAR * HTTP_ALY_GetMethodString(IN HANDLE hHandle);
extern HTTP_ALY_METHOD_E HTTP_ALY_GetMethodByString(IN CHAR *pcMethod);
extern UINT HTTP_ALY_GetAvailableDataSize(IN HANDLE hHandle);
extern HTTP_ALY_BODY_LEN_TYPE_E HTTP_ALY_GetBodyLenType(IN HANDLE hHandle);
extern VOID HTTP_ALY_SetRemoveChunkFlag(IN HANDLE hHandle);

#if 1
extern BS_STATUS HTTP_ALY_AddRequestHeadField(IN HANDLE hHandle, IN CHAR *pszFieldName, IN CHAR *pszFieldValue);
extern MBUF_S * HTTP_ALY_BuildRequestHead(IN HANDLE hHandle);
#endif

#if 1
extern BS_STATUS HTTP_ALY_SetResponseHeadField(IN HANDLE hHttpHandle, IN CHAR *pszKey, IN CHAR *pszValue);
extern BS_STATUS HTTP_ALY_SetResponseHeadFieldByBuf(IN HANDLE hHttpHandle, IN CHAR *pszString);
extern CHAR * HTTP_ALY_GetResponseKeyValue(IN HANDLE hHttpHandle, IN CHAR *pszKey);
extern BS_STATUS HTTP_ALY_SetResponseStatusCode(IN HANDLE hHttpHandle, IN UINT ulStatusCode, IN CHAR *pszDesc);
extern BS_STATUS HTTP_ALY_SetResponseKeepAlive(IN HANDLE hHandle, IN BOOL_T bIsKeepAlive);
extern BS_STATUS HTTP_ALY_SetResponseContentLen(IN HANDLE hHandle, IN ULONG ulContentLen);
extern BS_STATUS HTTP_ALY_SetCookie(IN HANDLE hHttpHandle, IN CHAR *pszCookieKey, IN CHAR *pszCookieValue, IN CHAR *pszPath);
extern BS_STATUS HTTP_ALY_SetNotModify(IN HANDLE hHttpHandle);
extern BS_STATUS HTTP_ALY_SetNotFound(IN HANDLE hHttpHandle);
extern BS_STATUS HTTP_ALY_SetMethodNotSupport(IN HANDLE hHttpHandle);
extern BS_STATUS HTTP_ALY_SetRedirectTo(IN HANDLE hHttpHandle, IN CHAR *pszRedirectTo);
extern BS_STATUS HTTP_ALY_SetNoCache(IN HANDLE hHttpHandle);
extern BS_STATUS HTTP_ALY_NotBuildHeadField(IN HANDLE hHttpHandle, IN CHAR *pszFieldName);
extern BS_STATUS HTTP_ALY_BuildHeadField(IN HANDLE hHttpHandle, IN CHAR *pszFieldName);
#endif


UCHAR HTTP_ALY_GetResponseMode(IN HANDLE hHttpHandle);

BS_STATUS HTTP_ALY_Send(IN HANDLE hHttpHandle, IN UCHAR *pucData, IN UINT uiDataLen, OUT UINT *pulSendLen);

BS_STATUS HTTP_ALY_Finish(IN HANDLE hHttpHandle);
BS_STATUS HTTP_ALY_BuildResponseHead(IN HANDLE hHttpHandle);
extern UINT HTTP_ALY_GetSendDataSize(IN HANDLE hHttpHandle);
BS_STATUS HTTP_ALY_Flush(IN HANDLE hHttpHandle);


#ifdef __cplusplus
    }
#endif 

#endif 


