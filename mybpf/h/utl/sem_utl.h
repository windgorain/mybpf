/******************************************************************************
* Copyright (C) LiXingang
* Author:      LiXingang  Version: 1.0  Date: 2012-11-20
* Description: 
* History:     
******************************************************************************/

#ifndef __SEM_UTL_H_
#define __SEM_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

typedef HANDLE SEM_HANDLE;


SEM_HANDLE SEM_MCreate (const char *pcName);

SEM_HANDLE SEM_CCreate (const char *pcName, IN INT iInitNum);
VOID SEM_Destory(IN SEM_HANDLE hSem);
BS_STATUS SEM_P (IN SEM_HANDLE hSem, IN BS_WAIT_E eWaitMode, IN UINT ulMilliseconds);

BS_STATUS SEM_PV
(
    IN SEM_HANDLE hSemToP,
    IN SEM_HANDLE hSemToV,
    IN BS_WAIT_E eWaitMode,
    IN UINT ulMilliseconds
);
BS_STATUS SEM_V(IN SEM_HANDLE hSem);
BS_STATUS SEM_VAll(IN SEM_HANDLE hSem);

INT SEM_CountPending(IN SEM_HANDLE hSem);

#define SEM_LOCK_IF_EXIST(hSem)    do{if ((hSem) != NULL){SEM_P(hSem, BS_WAIT, BS_WAIT_FOREVER);}}while(0)
#define SEM_UNLOCK_IF_EXIST(hSem)  do{if ((hSem) != NULL){SEM_V(hSem);}}while(0)


#ifdef __cplusplus
    }
#endif 

#endif 


