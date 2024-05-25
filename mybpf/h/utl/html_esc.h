/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2014-10-9
* Description: 
* History:     
******************************************************************************/

#ifndef __HTML_ESC_H_
#define __HTML_ESC_H_

#ifdef __cplusplus
    extern "C" {
#endif 

BS_STATUS HTML_ESC_Decode
(
    IN CHAR *pcInput,
    IN UINT uiInputLen,
    IN UINT uiOutBufSize,
    OUT CHAR *pcOutput
);

UINT HTML_ESC_EncodeLen
(
    IN CHAR *pcInput,
    IN UINT uiInputLen,
    IN CHAR *pcTranslateCharset    
);
BS_STATUS HTML_ESC_Encode
(
    IN CHAR *pcInput,
    IN UINT uiInputLen,
    IN CHAR *pcTranslateCharset,    
    IN UINT uiOutBufSize,
    OUT CHAR *pcOutput
);

#ifdef __cplusplus
    }
#endif 

#endif 


