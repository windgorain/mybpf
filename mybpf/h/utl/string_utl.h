/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2007-8-15
* Description: 
* History:     
******************************************************************************/

#ifndef __STRING_UTL_H_
#define __STRING_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

typedef HANDLE HSTRING;

extern HSTRING STRING_Create();
extern VOID STRING_Delete(IN HSTRING hHandle);
extern UINT STRING_GetLength(IN HSTRING hHandle);
extern BS_STATUS STRING_CatFromBuf(IN HSTRING hHandle, IN CHAR *pszData);
extern BS_STATUS STRING_CatFromString(IN HSTRING hHandleDst, IN HSTRING hHandleSrc);
extern BS_STATUS STRING_CpyFromBuf(IN HSTRING hHandle, IN CHAR *pszData);
extern BS_STATUS STRING_CpyFromString(IN HSTRING hHandleDst, IN HSTRING hHandleSrc);
extern INT STRING_CmpByBuf(IN HSTRING hHandle, IN CHAR *pszData);
extern INT STRING_CmpByString(IN HSTRING hHandle1, IN HSTRING hHandle2);
extern CHAR * STRING_GetBuf(IN HSTRING hHandle);
extern void STRING_Clear(HSTRING hHandle);
extern void STRING_CutHead(HSTRING hHandle, int len);

#ifdef __cplusplus
    }
#endif 

#endif 


