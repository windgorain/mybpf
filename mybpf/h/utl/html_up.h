/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2014-10-24
* Description: HTML Url Parser
* History:     
******************************************************************************/

#ifndef __HTML_UP_H_
#define __HTML_UP_H_

#ifdef __cplusplus
    extern "C" {
#endif 

typedef VOID* HTML_UP_HANDLE;

typedef enum
{
    HTML_UP_DATA = 0,
    HTML_UP_TAG_START,
    HTML_UP_TAG_CONTENT,
    HTML_UP_TAG_END,
    HTML_UP_JS_START,
    HTML_UP_JS,
    HTML_UP_JS_END,
    HTML_UP_CSS_START,
    HTML_UP_CSS,
    HTML_UP_CSS_END,
    HTML_UP_URL
}HTML_UP_TYPE_E;

typedef struct
{
    HTML_UP_TYPE_E enType;
    CHAR *pcData;
    UINT uiDataLen;
}HTML_UP_PARAM_S;

typedef VOID (*PF_HTML_UP_OUTPUT)(IN HTML_UP_PARAM_S *pstParam, IN VOID *pUserContext);

HTML_UP_HANDLE HTML_UP_Create
(
    IN PF_HTML_UP_OUTPUT pfOutput,
    IN VOID *pUserContext
);
VOID HTML_UP_Destroy(IN HTML_UP_HANDLE hUp);
VOID HTML_UP_InputHtml(IN HTML_UP_HANDLE hUp, IN CHAR *pcHtml, IN UINT uiHtmlLen);
VOID HTML_UP_InputHtmlEnd(IN HTML_UP_HANDLE hUp);

#ifdef __cplusplus
    }
#endif 

#endif 


