/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2014-11-22
* Description: Simple Data Container. 只有三级: Section, Key,Value.  比DC少了TableName
* History:     
******************************************************************************/

#ifndef __SDC_UTL_H_
#define __SDC_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

typedef VOID* SDC_HANDLE;

VOID SDC_Destroy(IN SDC_HANDLE hSdc);
BS_STATUS SDC_SetProp(IN SDC_HANDLE hSdc, IN CHAR *pcTag, IN CHAR *pcProp, IN CHAR *pcValue);
CHAR * SDC_GetProp(IN SDC_HANDLE hSdc, IN CHAR *pcTag, IN CHAR *pcProp);
BS_STATUS SDC_AddTag(IN SDC_HANDLE hSdc, IN CHAR *pcTag);
BOOL_T SDC_IsTagExist(IN SDC_HANDLE hSdc, IN CHAR *pcTag);
VOID SDC_DeleteTag(IN SDC_HANDLE hSdc, IN CHAR *pcTag);
CHAR * SDC_GetNextTag(IN SDC_HANDLE hSdc, IN CHAR *pcCurrentTag);
UINT SDC_GetTagNum(IN SDC_HANDLE hSdc);
SDC_HANDLE SDC_INI_Create(IN CHAR *pcIniFileName);

#ifdef __cplusplus
    }
#endif 

#endif 


