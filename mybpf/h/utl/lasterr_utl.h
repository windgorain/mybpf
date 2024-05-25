/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2007-9-7
* Description: 
* History:     
******************************************************************************/

#ifndef __LASTERR_UTL_H_
#define __LASTERR_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 


VOID LastErr_Print();
VOID LastErr_SPrint(OUT CHAR *pcInfo);
VOID LastErr_PrintByErrno(IN UINT uiErrno);


#ifdef __cplusplus
    }
#endif 

#endif 


