/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2008-4-23
* Description: 
* History:     
******************************************************************************/

#ifndef __CFG_UTL_H_
#define __CFG_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

typedef VOID (*PF_CFGUTL_DEAL_FUNC)(IN UINT ulArgc, IN CHAR **argv, IN UINT ulUserHandle);

typedef struct
{
    CHAR *pszCmd;
    PF_CFGUTL_DEAL_FUNC pfFunc;
    UINT ulUsrHandle;
}CFGUTL_REG_TBL_S;

VOID CFGUTL_Open(IN CHAR *pszFileName, IN CFGUTL_REG_TBL_S *pstRegTbl, IN UINT ulTblSize);

#ifdef __cplusplus
    }
#endif 

#endif 


