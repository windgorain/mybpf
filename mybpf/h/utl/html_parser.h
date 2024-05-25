/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2014-10-14
* Description: 
* History:     
******************************************************************************/

#ifndef __HTML_PARSER_H_
#define __HTML_PARSER_H_

#ifdef __cplusplus
    extern "C" {
#endif 

typedef VOID* HTML_PARSER_HANDLE;

typedef enum
{
    HTML_PARSER_DATA_TYPE_CONTENT = 0,
    HTML_PARSER_DATA_TYPE_TAG,
    HTML_PARSER_DATA_TYPE_JS_START,
    HTML_PARSER_DATA_TYPE_JS,
    HTML_PARSER_DATA_TYPE_JS_END,
    HTML_PARSER_DATA_TYPE_CSS_START,
    HTML_PARSER_DATA_TYPE_CSS,
    HTML_PARSER_DATA_TYPE_CSS_END,
}HTML_PARSE_DATA_TYPE_E;

#define HTML_PARSER_MAX_TAG_NAME_LEN 127
#define HTML_PARSER_MAX_ATTR_NAME_LEN 127
#define HTML_PARSER_MAX_ATTR_VALUE_LEN 8191

#define HTML_PARSER_TAG_FLAG_TAG_START       0x1 
#define HTML_PARSER_TAG_FLAG_TAG_END         0x2 
#define HTML_PARSER_TAG_FLAG_SELF_CLOSE      0x4 
#define HTML_PARSER_TAG_FLAG_WITH_END_SLASH  0x8 

typedef struct
{
    UINT uiTagFlag;
    CHAR szTagName[HTML_PARSER_MAX_TAG_NAME_LEN + 1];
    CHAR szAttrName[HTML_PARSER_MAX_ATTR_NAME_LEN + 1];
    CHAR szAttrValue[HTML_PARSER_MAX_ATTR_VALUE_LEN + 1];
    USHORT usTagNameLen;
    USHORT usAttrNameLen;
    USHORT usAttrValueLen;
}HTML_PARSER_TAG_S;

typedef VOID (*PF_HTML_OUTPUT_FUNC)
(
    IN HTML_PARSE_DATA_TYPE_E enType,
    IN VOID *pData, 
    IN VOID *pUserContext
);

typedef enum
{
    HTML_PARSER_ELE_TYPE_DATA = 0,
    HTML_PARSER_ELE_TYPE_TAG
}HTML_PARSER_ELE_TYPE_E;

HTML_PARSER_HANDLE HTML_Parser_Create(IN PF_HTML_OUTPUT_FUNC pfOutput, IN VOID *pUserContext);
VOID HTML_Parser_Destroy(IN HTML_PARSER_HANDLE hPraser);
VOID HTML_Parser_Run
(
    IN HTML_PARSER_HANDLE hParser,
    IN CHAR *pcHtmlData,
    IN UINT uiHtmlDataLen
);
VOID HTML_Parser_End(IN HTML_PARSER_HANDLE hParser);

#ifdef __cplusplus
    }
#endif 

#endif 


