
#ifndef __HTTP_LIB_H_
#define __HTTP_LIB_H_

#include "utl/mempool_utl.h"

#ifdef  __cplusplus
extern "C"{
#endif

#if 1   

#define HTTP_MIN_FIRST_LINE_LEN (sizeof("GET / HTTP/1.1\r\n") - 1)

#define HTTPD_SERVICE_MAX_URLPATH_LEN 255UL



#define HTTP_CRLFCRLF                       "\r\n\r\n"      
#define HTTP_CRLFCRLF_LEN 4UL                               
#define HTTP_LINE_END_FLAG                  "\r\n"          
#define HTTP_LINE_END_FLAG_LEN 2UL                          

#define HTTP_CHUNK_FLAG_MAX_LEN     18UL                    
#define HTTP_CHUNK_EOF_FLAG                 "0\r\n\r\n"     
#define HTTP_CHUNK_EOF_FLAG_LEN 5UL                         

#define HTTP_MAX_UINT64_LEN       20UL      
#define HTTP_MAX_UINT_HEX_LEN     8UL       
#define HTTP_MAX_UINT64_HEX_LEN   16UL      
#define HTTP_MAX_COOKIE_PORT_NUM  16UL
#define HTTP_MAX_COOKIE_LEN       4096UL
#define HTTP_MAX_HEAD_LENGTH      8192UL
#define HTTP_INVALID_VALUE       0xffffffff

#define HTTP_FIELD_BODY_FIRST_MEMBER_DNS             "dns"      
#define HTTP_FIELD_BODY_FIRST_MEMBER_DNS_LEN         3UL        
#define HTTP_FIELD_BODY_FIRST_MEMBER_HOST            "host"     
#define HTTP_FIELD_BODY_FIRST_MEMBER_HOST_LEN        4UL        

#define HTTP_FIELD_BODY_MEMBER_IPS                  "ips"           
#define HTTP_FIELD_BODY_MEMBER_TYPE                 "type"          
#define HTTP_FIELD_BODY_MEMBER_TTL                  "ttl"           
#define HTTP_FIELD_BODY_MEMBER_CLIENT_IP            "client_ip"     

#define HTTP_FIELD_BODY_RESPONS_DNS_MAX              16         


#define HTTP_HEAD_FIELD_STATUS                 "Status"           
#define HTTP_PART_HEAD_DISPOSITION_TYPE        "Disposition-Type" 



#define HTTP_FIELD_ACCEPT                      "Accept"
#define HTTP_FIELD_ACCEPT_CHARSET              "Accept-Charset"
#define HTTP_FIELD_ACCEPT_ENCODING             "Accept-Encoding"
#define HTTP_FIELD_ACCEPT_LANGUAGE             "Accept-Language"
#define HTTP_FIELD_AUTHORIZATION               "Authorization"
#define HTTP_FIELD_EXPECT                      "Expect"
#define HTTP_FIELD_FROM                        "From"
#define HTTP_FIELD_HOST                        "Host"
#define HTTP_FIELD_IF_MATCH                    "If-Match"

#define HTTP_FIELD_IF_MODIFIED_SINCE           "If-Modified-Since"
#define HTTP_FIELD_IF_UNMODIFIED_SINCE         "If-Unmodified-Since"
#define HTTP_FIELD_UNLESS_MODIFIED_SINCE       "Unless-Modified-Since"
#define HTTP_FIELD_IF_RANGE                    "If-Range"

#define HTTP_FIELD_IF_NONE_MATCH               "If-None-Match"
#define HTTP_FIELD_MAX_FORWARDS                "Max-Forwards"
#define HTTP_FIELD_PROXY_AUTHORIZATION         "Proxy-Authorization"
#define HTTP_FIELD_RANGE                       "Range"
#define HTTP_FIELD_REFERER                     "Referer"
#define HTTP_FIELD_TE                          "TE"
#define HTTP_FIELD_USER_AGENT                  "User-Agent"
#define HTTP_FIELD_KEEP_ALIVE                  "Keep-Alive"
#define HTTP_FIELD_COOKIE                      "Cookie"
#define HTTP_FIELD_COOKIE2                     "Cookie2"




#define HTTP_FIELD_ACCEPT_RANGES               "Accept-Ranges"                             
#define HTTP_FIELD_AGE                         "Age"                                       
#define HTTP_FIELD_ETAG                        "ETag"                                      
#define HTTP_FIELD_LOCATION                    "Location"                                  
#define HTTP_FIELD_PROXY_AUTHENTICATE          "Proxy-Authenticate"                        
#define HTTP_FIELD_RETRY_AFTER                 "Retry-After"                               
#define HTTP_FIELD_SERVER                      "Server"                                    
#define HTTP_FIELD_VARY                        "Vary"                                      
#define HTTP_FIELD_WWW_AUTHENTICATE            "WWW-Authenticate"  
#define HTTP_FIELD_SET_COOKIE                  "Set-Cookie"
#define HTTP_FIELD_SET_COOKIE2                 "Set-Cookie2"



#define HTTP_FIELD_CATCH_CONTROL               "Cache-Control"     
#define HTTP_FIELD_CONNECTION                  "Connection"
#define HTTP_FIELD_DATE                        "Date"      
#define HTTP_FIELD_PRAGMA                      "Pragma"    
#define HTTP_FIELD_TRAILER                     "Trailer"    
#define HTTP_FIELD_TRANSFER_ENCODING           "Transfer-Encoding"                                  
#define HTTP_FIELD_UPGRADE                     "Upgrade"                        
#define HTTP_FIELD_VIA                         "Via"                            
#define HTTP_FIELD_WARNING                     "Warning"                        


#define HTTP_FIELD_ALLOW                       "Allow"                                        
#define HTTP_FIELD_CONTENT_ENCODING            "Content-Encoding"       
#define HTTP_FIELD_CONTENT_LANGUAGE            "Content-Language"       
#define HTTP_FIELD_CONTENT_LENGTH              "Content-Length"         
#define HTTP_FIELD_CONTENT_LOCATION            "Content-Location"       
#define HTTP_FIELD_CONTENT_MD5                 "Content-MD5"            
#define HTTP_FIELD_CONTENT_RANGE               "Content-Range"          
#define HTTP_FIELD_CONTENT_TYPE                "Content-Type"           
#define HTTP_FIELD_EXPIRES                     "Expires"                
#define HTTP_FIELD_LAST_MODIFIED               "Last-Modified"                                 
#define HTTP_FIELD_CONTENT_DISPOSITION         "Content-Disposition"    
               

#define HTTP_EQUAL_CHAR                    '='
#define HTTP_AND_CHAR                      '&'
#define HTTP_SEMICOLON_CHAR                ';'
#define HTTP_SEMICOLON_STRING              ";"
#define HTTP_BACKSLASH_CHAR                '/'                          
#define HTTP_BACKSLASH_STRING              "/"                      
#define HTTP_HEAD_FIELD_SPLIT_CHAR         ':'                     
#define HTTP_HEAD_URI_QUERY_CHAR           '?'                     
#define HTTP_HEAD_URI_QUERY_STRING         "?"                     
#define HTTP_HEAD_URI_FRAGMENT_CHAR        '#'                     
#define HTTP_HEAD_URI_FRAGMENT_STRING      "#"                     
#define HTTP_HEAD_DOUBLE_QUOTATION_STRING  "\""                    
#define HTTP_SP_HT_STRING                  " \t"                   

#define HTTP_RFC1123_DATESTR_LEN           29UL                   


#define HTTP_HF_DEFS  \
    _(HTTP_HF_HOST, HTTP_FIELD_HOST) \
    _(HTTP_HF_CONTENT_LENGTH, HTTP_FIELD_CONTENT_LENGTH)  \
    _(HTTP_HF_TRANSFER_ENCODING, HTTP_FIELD_TRANSFER_ENCODING)  \
    _(HTTP_HF_CONNECTION, HTTP_FIELD_CONNECTION)  \
    _(HTTP_HF_USER_AGENT, HTTP_FIELD_USER_AGENT) \

typedef enum {
#define _(a,b) a,
    HTTP_HF_DEFS
#undef _
    HTTP_HF_MAX
}HTTP_HEAD_FIELD_E;


typedef     VOID*     HTTP_HEAD_PARSER;

typedef enum tagHTTP_Version{
    HTTP_VERSION_0_9,                   
    HTTP_VERSION_1_0,                   
    HTTP_VERSION_1_1,                   
    HTTP_VERSION_BUTT                                     
} HTTP_VERSION_E;


typedef struct tagHTTP_HeadField{
    DLL_NODE_S stListNode;                  
    CHAR *pcFieldName;                                    
    CHAR *pcFieldValue;                     
    UCHAR uiFieldNameLen;                   
}HTTP_HEAD_FIELD_S;

typedef enum tagHTTP_Method{
    HTTP_METHOD_OPTIONS,                
    HTTP_METHOD_GET,                    
    HTTP_METHOD_HEAD,                   
    HTTP_METHOD_POST,                   
    HTTP_METHOD_PUT,                        
    HTTP_METHOD_DELETE,                 
    HTTP_METHOD_TRACE,                      
    HTTP_METHOD_CONNECT,                
    
    HTTP_METHOD_BUTT                                       
} HTTP_METHOD_E;


typedef enum tagHTTP_CookieType {
    HTTP_COOKIE_SERVER,    
    HTTP_COOKIE_SERVER2,   
    HTTP_COOKIE_CLIENT,    
    HTTP_COOKIE_CLIENT2,   
    
    HTTP_COOKIE_BUTT             
}HTTP_COOKIE_TYPE_E;

typedef enum tagHTTP_CookieAV{
    
    COOKIE_AV_COMMENT,
    COOKIE_AV_COMMENTURL,
    COOKIE_AV_DOMAIN,
    COOKIE_AV_PATH,
    COOKIE_AV_PORT,

    
    COOKIE_AV_MAXAGE,
    COOKIE_AV_VERSION,

    
    COOKIE_AV_DISCARD,
    COOKIE_AV_SECURE,

    
    COOKIE_AV_BUTT    
}HTTP_COOKIE_AV_E;

typedef struct tagHTTP_CookieKey                               
{                                                                    
    CHAR  *pcName;                             
    CHAR  *pcDomain;                       
    CHAR  *pcPath;                         
} HTTP_COOKIE_KEY_S;                                                 
                                                                     
typedef struct tagHTTP_ServerCookie{                                
    HTTP_COOKIE_KEY_S stCookieKey;               
    CHAR  *pcValue;                             
    CHAR  *pcComment;                           
    CHAR  *pcCommentUrl;                         
    ULONG  ulMaxAge;                            
    USHORT ausPort[HTTP_MAX_COOKIE_PORT_NUM];   
    BOOL_T bDiscard;                            
    BOOL_T bSecure;                             
    ULONG  ulVersion;                           
} HTTP_SERVER_COOKIE_S;                                              
                               




typedef struct tagHTTP_RangePart
{
    UINT uiKeyLow;      
    UINT uiKeyHigh;     
} HTTP_RANGE_PART_S;

typedef struct tagHTTP_Range
{
    ULONG ulRangeNum;                   
    HTTP_RANGE_PART_S *pstRangePart;    
}HTTP_RANGE_S;

typedef enum tagHTTP_Source{
    HTTP_REQUEST,                   
    HTTP_RESPONSE,                  
    HTTP_PART_HEADER,               
    HTTP_FCGI_RESPONSE,             
    HTTP_SOURCE_BUTT                
} HTTP_SOURCE_E;


typedef enum tagHTTP_StatusCode{
    HTTP_STATUS_CONTINUE=0,       
    HTTP_STATUS_SWITCH_PROTO,               
    HTTP_STATUS_OK,                
    HTTP_STATUS_CREATED,           
    HTTP_STATUS_ACCEPTED,          
    HTTP_STATUS_NON_AUTHO,         
    HTTP_STATUS_NO_CONTENT,        
    HTTP_STATUS_RESET_CONTENT,     
    HTTP_STATUS_PARTIAL_CONTENT,   
    HTTP_STATUS_MULTI_CHCS,        
    HTTP_STATUS_MOVED_PMT,         
    HTTP_STATUS_MOVED_TEMP,        
    HTTP_STATUS_SEE_OTHER,               
    HTTP_STATUS_NOT_MODI,                
    HTTP_STATUS_USE_PXY,           
    HTTP_STATUS_REDIRECT_TEMP,    
    HTTP_STATUS_BAD_RQ,                
    HTTP_STATUS_UAUTHO,               
    HTTP_STATUS_PAY_RQ,               
    HTTP_STATUS_FORBIDDEN,               
    HTTP_STATUS_NOT_FOUND,               
    HTTP_STATUS_NOT_ALLOWED,            
    HTTP_STATUS_NOT_ACC,               
    HTTP_STATUS_PXY_AUTH_RQ,          
    HTTP_STATUS_RQ_TIME_OUT,          
    HTTP_STATUS_CONFLICT,                
    HTTP_STATUS_GONE,                    
    HTTP_STATUS_LEN_RQ,                 
    HTTP_STATUS_PRECOND_FAIL,          
    HTTP_STATUS_BODY_TOO_LARGE,       
    HTTP_STATUS_URL_TOO_LARGE,        
    HTTP_STATUS_USUPPORT_MEDIA,          
    HTTP_STATUS_RANGE_ERR,               
    HTTP_STATUS_EXPECT_FAILED,           
    HTTP_STATUS_INTERNAL_ERR,            
    HTTP_STATUS_NOT_IMPLE,              
    HTTP_STATUS_BAD_GATEWAY,           
    HTTP_STATUS_SERVICE_UAVAIL,       
    HTTP_STATUS_GATWAY_TIME_OUT,      
    HTTP_STATUS_VER_NOT_SUPPORT,  
    
    HTTP_STATUS_CODE_BUTT         
}HTTP_STATUS_CODE_E;                                                              
                                                                   
typedef enum tagHTTP_Connection {  
    HTTP_CONNECTION_CLOSE,             
    HTTP_CONNECTION_KEEPALIVE,         
    
    HTTP_CONNECTION_BUTT               
}HTTP_CONNECTION_E;


typedef enum
{
    HTTP_BODY_TRAN_TYPE_CONTENT_LENGTH, 
    HTTP_BODY_TRAN_TYPE_CLOSED,         
    HTTP_BODY_TRAN_TYPE_CHUNKED,        
    HTTP_BODY_TRAN_TYPE_NONE            
}HTTP_BODY_TRAN_TYPE_E;


typedef enum
{
    HTTP_BODY_CONTENT_TYPE_NORMAL,
    HTTP_BODY_CONTENT_TYPE_MULTIPART,
    HTTP_BODY_CONTENT_TYPE_OCSP,
    HTTP_BODY_CONTENT_TYPE_JSON,

    HTTP_BODY_CONTENT_TYPE_MAX
}HTTP_BODY_CONTENT_TYPE_E;


typedef enum tagHTTP_TransferEncoding {                            
    HTTP_TRANSFER_ENCODING_CHUNK,                       
    HTTP_TRANSFER_ENCODING_NOCHUNK,      

    HTTP_TRANSFER_ENCODING_BUTT          
}HTTP_TRANSFER_ENCODING_E;


HTTP_HEAD_PARSER HTTP_CreateHeadParser();
VOID   HTTP_DestoryHeadParser(IN HTTP_HEAD_PARSER hHttpInstance);
UINT HTTP_GetHeadLen(IN CHAR *pcHttpData, IN ULONG ulDataLen);
BS_STATUS  HTTP_ParseHead(IN HTTP_HEAD_PARSER hHttpInstance, IN CHAR *pcHead, IN ULONG ulHeadLen, IN HTTP_SOURCE_E enSourceType);
time_t HTTP_StrToDate(IN UCHAR *pucValue, IN ULONG ulLen);
BS_STATUS  HTTP_DateToStr(IN time_t ulTimeSrc, OUT CHAR szDataStr[HTTP_RFC1123_DATESTR_LEN + 1]);
VOID HTTP_DelHeadField(IN HTTP_HEAD_PARSER hHttpInstance, IN CHAR *pcFieldName);
BS_STATUS  HTTP_SetHeadField(IN HTTP_HEAD_PARSER hHttpInstance, IN CHAR *pcFieldName, IN CHAR *pcFieldValue);
VOID HTTP_SetNoCache(IN HTTP_HEAD_PARSER hHttpInstance);

VOID HTTP_SetRevalidate(IN HTTP_HEAD_PARSER hHttpInstance);
CHAR * HTTP_GetHeadField(IN HTTP_HEAD_PARSER hHttpInstance, IN CHAR *pcFieldName);
CHAR * HTTP_GetHF(HTTP_HEAD_PARSER hHttpInstance, HTTP_HEAD_FIELD_E field);
void HTTP_ClearHF(HTTP_HEAD_PARSER hHttpInstance);
HTTP_HEAD_FIELD_S * HTTP_GetNextHeadField(IN HTTP_HEAD_PARSER hHttpInstance, IN HTTP_HEAD_FIELD_S *pstHeadFieldNode);
BS_STATUS HTTP_GetContentLen(IN HTTP_HEAD_PARSER hHttpInstance, OUT UINT64 *puiContentLen);
BS_STATUS HTTP_SetContentLen(IN HTTP_HEAD_PARSER hHttpInstance, IN UINT64 uiValue);
CHAR * HTTP_GetUriPath (IN HTTP_HEAD_PARSER hHttpInstance);
BS_STATUS  HTTP_SetUriPath(IN HTTP_HEAD_PARSER hHttpInstance, IN CHAR *pcUri, IN UINT uiUriLen);
CHAR * HTTP_GetUriAbsPath (IN HTTP_HEAD_PARSER hHttpInstance);
CHAR * HTTP_GetSimpleUriAbsPath (IN HTTP_HEAD_PARSER hHttpInstance);
CHAR * HTTP_GetFullUri (IN HTTP_HEAD_PARSER hHttpInstance);
CHAR * HTTP_GetUriQuery (IN HTTP_HEAD_PARSER hHttpInstance);
VOID HTTP_ClearUriQuery(IN HTTP_HEAD_PARSER hHttpInstance);
BS_STATUS  HTTP_SetUriQuery (IN HTTP_HEAD_PARSER hHttpInstance, IN CHAR *pcQuery);
CHAR * HTTP_GetUriFragment (IN HTTP_HEAD_PARSER hHttpInstance);
BS_STATUS HTTP_SetUriFragment(IN HTTP_HEAD_PARSER hHttpInstance, IN CHAR *pcFragment);
HTTP_VERSION_E HTTP_GetVersion(IN HTTP_HEAD_PARSER hHttpInstance);
CHAR*  HTTP_VersionConversionStr(IN HTTP_VERSION_E enVer);
BS_STATUS  HTTP_SetVersion (IN HTTP_HEAD_PARSER hHttpInstance, IN HTTP_VERSION_E enVer);
HTTP_METHOD_E HTTP_GetMethod (IN HTTP_HEAD_PARSER hHttpInstance);
BS_STATUS  HTTP_SetMethod (IN HTTP_HEAD_PARSER hHttpInstance,  IN HTTP_METHOD_E enMethod);
CHAR*  HTTP_GetMethodStr(IN HTTP_METHOD_E enMethod );
CHAR*  HTTP_GetMethodData (IN HTTP_HEAD_PARSER hHttpInstance);
HTTP_STATUS_CODE_E HTTP_GetStatusCode( IN HTTP_HEAD_PARSER hHttpInstance );
BS_STATUS  HTTP_SetStatusCode( IN HTTP_HEAD_PARSER hHttpInstance, IN HTTP_STATUS_CODE_E enStatusCode);
CHAR * HTTP_GetReason ( IN HTTP_HEAD_PARSER hHttpInstance);
BS_STATUS  HTTP_SetReason (IN HTTP_HEAD_PARSER hHttpInstance, IN CHAR *pcReason);
HTTP_RANGE_S * HTTP_GetRange(IN HTTP_HEAD_PARSER hHttpInstance);
BS_STATUS  HTTP_SetChunk(IN HTTP_HEAD_PARSER hHttpInstance);
CHAR * HTTP_UriDecode(IN MEMPOOL_HANDLE hMemPool, IN CHAR *pcUri, IN ULONG ulUriLen);
CHAR * HTTP_UriEncode(IN MEMPOOL_HANDLE hMemPool, IN CHAR *pcUri, IN ULONG ulUriLen);
CHAR * HTTP_DataDecode(IN MEMPOOL_HANDLE hMemPool, IN CHAR *pcData, IN ULONG ulDataLen);
CHAR * HTTP_DataEncode(IN MEMPOOL_HANDLE hMemPool, IN CHAR *pcData, IN ULONG ulDataLen);
CHAR * HTTP_GetUriParam (IN HTTP_HEAD_PARSER hHttpInstance);
BS_STATUS  HTTP_SetUriParam (IN HTTP_HEAD_PARSER hHttpInstance, IN CHAR *pcParam);
CHAR * HTTP_BuildHttpHead(IN HTTP_HEAD_PARSER hHttpInstance, IN HTTP_SOURCE_E enHeadType, OUT ULONG *pulHeadLen);
HTTP_TRANSFER_ENCODING_E HTTP_GetTransferEncoding (IN HTTP_HEAD_PARSER hHttpInstance);
HTTP_CONNECTION_E HTTP_GetConnection (IN HTTP_HEAD_PARSER hHttpInstance);
BS_STATUS  HTTP_SetConnection(IN HTTP_HEAD_PARSER hHttpInstance, HTTP_CONNECTION_E enConnType);
HTTP_BODY_TRAN_TYPE_E HTTP_GetBodyTranType (IN HTTP_HEAD_PARSER hHttpInstance );
HTTP_BODY_CONTENT_TYPE_E HTTP_GetContentType (IN HTTP_HEAD_PARSER hHttpInstance );
CHAR * HTTP_BuildServerCookie 
(
    IN MEMPOOL_HANDLE hMemPool,
    IN HTTP_SERVER_COOKIE_S *pstCookie, 
    IN HTTP_COOKIE_TYPE_E enCookieType,
    OUT ULONG *pulCookieLen
);

CHAR * HTTP_StrimHead(IN CHAR *pcData, IN ULONG ulDataLen, IN CHAR *pcSkipChars);
ULONG HTTP_StrimTail(IN CHAR *pcData, IN ULONG ulDataLen, IN CHAR *pcSkipChars);
CHAR * HTTP_Strim(IN CHAR *pcData, IN ULONG ulDataLen, IN CHAR *pcSkipChars, OUT ULONG *pulNewLen);
BOOL_T HTTP_IsValidHeadChars(unsigned char *buf, int len);
BOOL_T HTTP_IsHttpHead(char *buf, int len);

#endif  

#if 1  
typedef int (*PF_HTTP_RAW_SCAN)(char *field, int field_len, char *value, int value_len, void *ud);
int HTTP_HeadRawScan(char *head, int head_len, PF_HTTP_RAW_SCAN out_func, void *ud);

typedef struct {
    LSTR_S first_line;
    DLL_HEAD_S field_list;
}HTTP_HEAD_RAW_S;

int HTTP_HeadRawInit(HTTP_HEAD_RAW_S *ctrl);
int HTTP_HeadRawReset(HTTP_HEAD_RAW_S *ctrl);
int HTTP_HeadRawFin(HTTP_HEAD_RAW_S *ctrl);
int HTTP_HeadRawParse(HTTP_HEAD_RAW_S *ctrl, char *head, int head_len);
LSTR_S * HTTP_HeadRawGetField(HTTP_HEAD_RAW_S *ctrl, char *field);
#endif

#if 1   

typedef     VOID*     HTTP_CHUNK_HANDLE;


VOID  HTTP_Chunk_BuildChunkFlag (IN UINT64 ui64DataLen, OUT CHAR szChunkBeginFlag[HTTP_CHUNK_FLAG_MAX_LEN + 1]);

typedef BS_STATUS (*PF_HTTP_CHUNK_FUNC)(IN UCHAR *pucData, IN UINT uiDataLen, IN VOID *pUserPointer);

BS_STATUS HTTP_Chunk_GetChunkDataLen
(
    IN UCHAR *pucData, 
    IN ULONG ulDataLen, 
    OUT ULONG *pulChunkDataLen, 
    OUT ULONG *pulChunkFlagLen
);
HTTP_CHUNK_HANDLE HTTP_Chunk_CreateParser (IN PF_HTTP_CHUNK_FUNC pfFunc,IN USER_HANDLE_S *pstUserPointer);
VOID  HTTP_Chunk_DestoryParser(IN HTTP_CHUNK_HANDLE hParser);
BS_STATUS HTTP_Chunk_Parse
(
    IN HTTP_CHUNK_HANDLE hParser,
    IN UCHAR *pucData,
    IN UINT uiDataLen,
    OUT UINT *uiParsedLen
);

#endif  

#if 1   

typedef     VOID*     HTTP_PARTHEADER_HANDLE;
typedef     VOID*     HTTP_MULTIPART_HANDLE;

typedef enum tagHTTP_PartType
{
    HTTP_PART_DATA_BEGIN,   
    HTTP_PART_DATA_BODY,    
    HTTP_PART_DATA_END,     
    HTTP_PART_DATA_BUTT     
}HTTP_PART_TYPE_E;


typedef ULONG (*PF_HTTP_MULTIPART_FUNC)
(
    IN HTTP_PART_TYPE_E enType,
    IN HTTP_MULTIPART_HANDLE hMultiPartParser,
    IN HTTP_HEAD_PARSER hPartParser, 
    IN UCHAR *pucData, 
    IN ULONG ulDataLen, 
    IN VOID *pUserPointer
);

HTTP_MULTIPART_HANDLE HTTP_Multipart_CreateParser
(
    IN CHAR *pcBoundary, 
    IN PF_HTTP_MULTIPART_FUNC pfFunc,
    IN VOID *pUserPointer
);
VOID  HTTP_Multipart_DestoryParser(IN HTTP_MULTIPART_HANDLE hMultiParser);
BS_STATUS HTTP_Multipart_Parse(IN HTTP_MULTIPART_HANDLE hMultiParser, IN UCHAR *pucData, IN ULONG ulDataLen);

#endif  

#if 1   

typedef HANDLE HTTP_BODY_PARSER;

typedef BS_STATUS (*PF_HTTP_BODY_FUNC)(IN UCHAR *pucData, IN UINT uiDataLen, IN USER_HANDLE_S *pstUserPointer);

HTTP_BODY_PARSER HTTP_BODY_CreateParser
(
    IN HTTP_HEAD_PARSER hHeadParser,
    IN PF_HTTP_BODY_FUNC pfBodyFunc,
    IN USER_HANDLE_S *pstUserHandle
);
VOID HTTP_BODY_DestroyParser(IN HTTP_BODY_PARSER hBodyParser);

BS_STATUS HTTP_BODY_Parse
(
    IN HTTP_BODY_PARSER hBodyParser,
    IN UCHAR *pucData,
    IN UINT uiDataLen,
    OUT UINT *puiParsedLen  
);

BOOL_T HTTP_BODY_IsFinish(IN HTTP_BODY_PARSER hBodyParser);

#endif  

#if 1 

typedef enum
{
    HTTPC_E_PEER_CLOSED = -100,
    HTTPC_E_AGAIN,
    HTTPC_E_FINISH,
    HTTPC_E_ERR,
}HTTPC_ERR_E;


typedef INT (*PF_HTTP_RECV)(IN ULONG ulFd, OUT UCHAR *pucBuf, IN UINT uiBufSize);


typedef struct
{
    ULONG ulFd;
    PF_HTTP_RECV pfRecv;
}HTTPC_RECVER_PARAM_S;

typedef HANDLE HTTPC_RECVER_HANDLE;

HTTPC_RECVER_HANDLE HttpcRecver_Create(IN HTTPC_RECVER_PARAM_S *pstParam);
VOID HttpcRecver_Destroy(IN HTTPC_RECVER_HANDLE hRecver);
INT HttpcRecver_Read(IN HTTPC_RECVER_HANDLE hRecver, IN UCHAR *pucBuf, IN UINT uiBufSize);
HTTP_HEAD_PARSER HttpcRecver_GetHttpParser(IN HTTPC_RECVER_HANDLE hRecver);


#endif

#ifdef  __cplusplus
}
#endif  

#endif  


