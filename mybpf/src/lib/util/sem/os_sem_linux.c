/******************************************************************************
* Copyright (C) Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2012-11-20
* Description: 
* History:     
******************************************************************************/

#include "bs.h"

#include "os_sem.h"

#ifdef IN_UNIXLIKE

BS_STATUS _OSSEM_Create(const char *pcName, UINT ulInitNum, OUT OS_SEM *pOsSem)
{
    if (0 != pthread_mutex_init(&pOsSem->mutex, 0))
    {
        return (BS_ERR);
    }
    
    if (0 != pthread_cond_init(&pOsSem->cond, 0))
    {
        pthread_mutex_destroy(&pOsSem->mutex);
        return (BS_ERR);
    }
    
    pOsSem->v = ulInitNum;

    return BS_OK;
}

BS_STATUS _OSSEM_Delete(OS_SEM *pOsSem)
{
    pthread_cond_destroy(&pOsSem->cond);
    pthread_mutex_destroy(&pOsSem->mutex);
    return BS_OK;
}

BS_STATUS _OSSEM_P(OS_SEM *pOsSem, UINT ulFlag, UINT ulMilliseconds)
{
    UINT ulRet = BS_OK;
    
    if (pthread_mutex_lock(&pOsSem->mutex) != 0)
    {
        BS_WARNNING(("Can't lock linux sem!"));
        return (BS_ERR);
    }

    if (pOsSem->v > 0)
    {
        pOsSem->v --;
    }
    else if (BS_WAIT == ulFlag)
    {
        if (BS_WAIT_FOREVER == ulMilliseconds)
        {
            while (pOsSem->v <= 0)
            {
                pthread_cond_wait(&pOsSem->cond, &pOsSem->mutex);
            }
            pOsSem->v -- ;
        }
        else
        {
            struct timespec time;
            clock_gettime(CLOCK_REALTIME, &time);
            time.tv_sec += ulMilliseconds/1000;
            time.tv_nsec += (ulMilliseconds % 1000) * 1000000;

            while (pOsSem->v <= 0)
            {
                if (ETIMEOUT == pthread_cond_timedwait(&pOsSem->cond, &pOsSem->mutex, &time))
                {
                    ulRet = BS_TIME_OUT;
                    break;
                }
                else if (pOsSem->v > 0)
                {
                    pOsSem->v --;
                    break;
                }
            }
        }
    }
    else
    {
        ulRet = BS_NO_RESOURCE;
    }

    pthread_mutex_unlock(&pOsSem->mutex);

    return (ulRet);

}

BS_STATUS _OSSEM_V(OS_SEM *pOsSem)
{
    if (pthread_mutex_lock(&pOsSem->mutex) != 0)
    {
        BS_WARNNING(("Can't lock linux sem!"));
        return (BS_ERR);
    }

    pOsSem->v ++;
    pthread_mutex_unlock(&pOsSem->mutex);

    pthread_cond_signal(&pOsSem->cond);

    return BS_OK;
}

#endif
