/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2012-11-20
* Description: 
* History:     
******************************************************************************/

#ifndef __OS_SEM_H_
#define __OS_SEM_H_

#ifdef __cplusplus
    extern "C" {
#endif 


#ifdef IN_UNIXLIKE
typedef struct
{
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
    UINT           v;
}OS_SEM;
#endif

#ifdef IN_WINDOWS
typedef UINT OS_SEM;
#endif

BS_STATUS _OSSEM_Create(const char *pcName, UINT ulInitNum, OUT OS_SEM *pOsSem);
BS_STATUS _OSSEM_Delete(OS_SEM *pOsSem);
BS_STATUS _OSSEM_P(OS_SEM *pOsSem, BS_WAIT_E eWaitMode, UINT ulMilliseconds);
BS_STATUS _OSSEM_V(OS_SEM *pOsSem);

#ifdef __cplusplus
    }
#endif 

#endif 


