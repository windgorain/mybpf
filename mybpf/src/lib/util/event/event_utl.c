/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2007-9-6
* Description: 
* History:     
******************************************************************************/
#include "bs.h"

#include "utl/sem_utl.h"
#include "utl/event_utl.h"
#include "utl/mutex_utl.h"


typedef struct
{
    UINT64 events;
    UINT64 event_watting;
    MUTEX_S stMutex;
    SEM_HANDLE hEventSem;
    UINT flag;
}_EVENT_CTRL_S;

EVENT_HANDLE Event_Create(void)
{
    _EVENT_CTRL_S *pstEventCtrl;

    pstEventCtrl = MEM_ZMalloc(sizeof(_EVENT_CTRL_S));
    if (NULL == pstEventCtrl) {
        BS_WARNNING (("Can't malloc"));
        return NULL;
    }

    if (0 == (pstEventCtrl->hEventSem = SEM_CCreate("EventSem", 0))) {
        MEM_Free (pstEventCtrl);
        return NULL;
    }

    MUTEX_Init(&pstEventCtrl->stMutex);

    return pstEventCtrl;
}

VOID Event_Delete (IN EVENT_HANDLE hEventID)
{
    _EVENT_CTRL_S *pstEventCtrl;

    BS_DBGASSERT (0 != hEventID);

    pstEventCtrl = (_EVENT_CTRL_S *) hEventID;

    SEM_Destory (pstEventCtrl->hEventSem);
    MUTEX_Final(&pstEventCtrl->stMutex);
    MEM_Free (pstEventCtrl);
}


BS_STATUS Event_Write(EVENT_HANDLE hEventID, UINT64 events)
{
    int success;
    _EVENT_CTRL_S *pstEventCtrl = (_EVENT_CTRL_S*)hEventID;

    MUTEX_P(&pstEventCtrl->stMutex);

    pstEventCtrl->events |= events;

    UINT64 events_ok = pstEventCtrl->event_watting & pstEventCtrl->events;

    if (pstEventCtrl->flag & EVENT_FLAG_WAIT) {
        if (pstEventCtrl->flag & EVENT_FLAG_CARE_ALL) {
            success = (pstEventCtrl->event_watting == events_ok);
        } else {
            success = (0 != events_ok);
        }

        if (success) {
            SEM_V(pstEventCtrl->hEventSem);
        }
    }

    MUTEX_V(&pstEventCtrl->stMutex);

    return BS_OK;
}

BS_STATUS Event_Read(EVENT_HANDLE hEventID, UINT64 events,
        OUT UINT64 *events_out, UINT ulFlag, UINT ulTimeToWait)
{
    BS_STATUS eRet = BS_OK;
    UINT ulIsBS_OK = 0;
    int wait;

    _EVENT_CTRL_S *pstEventCtrl = (_EVENT_CTRL_S*)hEventID;

    BS_DBGASSERT(NULL != events_out);

    if (ulFlag & EVENT_FLAG_WAIT) {
        wait = BS_WAIT;
    }

    MUTEX_P(&pstEventCtrl->stMutex);
    
    for (;;) {
        UINT64 events_ok = pstEventCtrl->events & events;
        if (EVENT_FLAG_CARE_ALL & ulFlag) {
            ulIsBS_OK = (events_ok == events ? 1 : 0);
        } else {
            ulIsBS_OK = (events_ok ? 1 : 0);
        }

        if (ulIsBS_OK) {
            *events_out = events_ok;
            pstEventCtrl->events &= (~events_ok);
            break;
        } else {
            pstEventCtrl->event_watting = events;
            pstEventCtrl->flag = ulFlag;

            MUTEX_V(&pstEventCtrl->stMutex);
            eRet = SEM_P(pstEventCtrl->hEventSem, wait, ulTimeToWait); 
            MUTEX_P(&pstEventCtrl->stMutex);
 
            pstEventCtrl->flag = 0;
            
            if (eRet == BS_TIME_OUT) {
                break;
            }
        }
    }

    MUTEX_V(&pstEventCtrl->stMutex);

    return eRet;
}

