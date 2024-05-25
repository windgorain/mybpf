/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2010-5-21
* Description: 
* History:     
******************************************************************************/

#ifndef __XML_PARSE_H_
#define __XML_PARSE_H_

#ifdef __cplusplus
    extern "C" {
#endif 

typedef enum
{
    XML_TYPE_NONE = 0,

    XML_TYPE_MARK_NAME,
    XML_TYPE_MARK_KEYVALUE,
    XML_TYPE_END_MARK,
}XML_TYPE_E;

typedef struct
{
    XML_TYPE_E eType;
    CHAR *pszStr1;
    CHAR *pszStr2;  
    UINT uiStr1Len;
    UINT uiStr2Len; 
}XML_PARSE_S;


typedef BS_STATUS (*PF_XML_PARSE_FUNC)(IN XML_PARSE_S *pstXmlParse, IN HANDLE hUserHandle);

BS_STATUS XML_Parse(IN CHAR *pszContent, IN PF_XML_PARSE_FUNC pfFunc, IN HANDLE hUserHandle);


#ifdef __cplusplus
    }
#endif 

#endif 


