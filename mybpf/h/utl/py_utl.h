/******************************************************************************
* Copyright (C), 2000-2006,  Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2016-12-22
* Description: 
* History:     
******************************************************************************/

#ifndef __PY_UTL_H_
#define __PY_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 


BS_STATUS PY_Init();
VOID PY_Finit();
VOID PY_RunString(IN CHAR *pcString);
VOID PY_RunFile(IN CHAR *pcFile);

VOID PY_DecRef(IN VOID *pPyObject);

VOID * PY_ImportModule(IN CHAR *pcModule);
VOID * PY_NewClassInstance(IN VOID *pMod, IN CHAR *pcClassName, IN CHAR *pcFormat, ...);
VOID * PY_GetAttrString(IN VOID *pPyObj, IN CHAR *pcAttrName);

VOID * PY_CallObject(IN VOID *pMethod, IN CHAR *pcFormat, ...);
VOID * PY_SimpleCallFunction(IN CHAR *pcModuleName, IN CHAR *pcFunctionName, IN CHAR *pcFormat, ...);
VOID * PY_SimpleCallMethod(IN CHAR *pcModuleName, IN CHAR *pcClass, IN CHAR *pcMethod, IN CHAR *pcFormat, ...);
INT PY_ParseArgAsInt(IN VOID *pArg);
CHAR * PY_ParseArgAsString(IN VOID *pArg);


#ifdef __cplusplus
    }
#endif 

#endif 


