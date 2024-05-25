/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2008-10-15
* Description: 
* History:     
******************************************************************************/

#ifndef __TRAY_UTL_H_
#define __TRAY_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

#include "utl/types.h"

HANDLE TRAY_Create(IN HANDLE hWnd, IN UINT ulNotifyMsg, IN UINT uiID);
VOID TRAY_Delete(IN HANDLE hTrayId);
BOOL_T TRAY_SetIcon(IN HANDLE hTrayId, IN HANDLE hIcon, IN CHAR *pszTip);

#ifdef __cplusplus
    }
#endif 

#endif 


