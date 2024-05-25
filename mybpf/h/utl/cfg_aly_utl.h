/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2007-5-18
* Description: 
* History:     
******************************************************************************/

#ifndef __CFG_ALY_UTL_H_
#define __CFG_ALY_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 


#define CFG_ALY_MAX_LINE_LEN 1000


extern BS_STATUS CFG_ALY_Create(IN UCHAR ucSplitChar, OUT HANDLE *phHandle);
extern BS_STATUS CFG_ALY_Delete(IN HANDLE hHandle);
extern BS_STATUS CFG_ALY_FindKeyValueByStream
(
    IN HANDLE hHandle,
    IN CHAR *pcBuf, 
    IN CHAR *pcKeyName, 
    OUT CHAR *pcValue,
    IN UINT ulMaxValueLen
);


#ifdef __cplusplus
    }
#endif 

#endif 


