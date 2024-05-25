/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2012-9-10
* Description: 
* History:     
******************************************************************************/

#ifndef __STR_CONVERSION_H_
#define __STR_CONVERSION_H_

#ifdef __cplusplus
    extern "C" {
#endif 



UINT STR_UnicodeToAnsi(IN VOID * pUnicode, OUT CHAR * pcStr, IN UINT uiSize);


UINT STR_AnsiToUnicode(IN CHAR *pcStr, OUT VOID *pUnicode, IN UINT uiSize);


UINT STR_UnicodeToUtf8(IN VOID *pUnicode, OUT VOID *pUtf8, IN UINT uiSize);


UINT STR_Utf8ToUnicode(IN CHAR *pUtf8, OUT VOID *pUnicode, IN UINT uiSize);


UINT STR_AnsiToUtf8(IN CHAR *pcStr, OUT VOID *pUtf8, IN UINT uiSize);


UINT STR_Utf8ToAnsi(IN VOID *pUtf8, OUT CHAR * pcStr, IN UINT uiSize);


#ifdef __cplusplus
    }
#endif 

#endif 


