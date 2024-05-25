/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2008-10-9
* Description: 
* History:     
******************************************************************************/

#define RETCODE_FILE_NUM RETCODE_FILE_NUM_VCLOCK
    
#include "bs.h"

#include "utl/mutex_utl.h"
#include "utl/vclock_utl.h"

static void _VCLOCK_Lock(VCLOCK_INSTANCE_S *pstVClockInstance)
{
    if (pstVClockInstance->create_lock) {
        MUTEX_P(&pstVClockInstance->lock);
    }
}

static void _VCLOCK_UnLock(VCLOCK_INSTANCE_S *pstVClockInstance)
{
    if (pstVClockInstance->create_lock) {
        MUTEX_V(&pstVClockInstance->lock);
    }
}


static VOID _vclock_timer_add(VCLOCK_INSTANCE_S *pstVClockInstance, VCLOCK_NODE_S *pstNode,
        UINT uiTick  )
{
    UINT ulLevel = 0;
    UINT ulTickTmp = uiTick;
    DLL_HEAD_S *head;

    DLL_DelIfInList(&pstNode->stDllNode);  

    pstNode->uiTriggerTick = pstVClockInstance->uiCurrentTick + uiTick;

    while ((ulTickTmp / _VCLOCK_TIMER_SCALE_PER_LEVEL) != 0)
    {
        ulLevel++;
        ulTickTmp = ulTickTmp / _VCLOCK_TIMER_SCALE_PER_LEVEL;
    }

    BS_DBGASSERT(ulLevel < _VCLOCK_TIMER_TOTLE_LEVEL);

    ulTickTmp += pstVClockInstance->ulCurrentLevelTick[ulLevel];
    if (ulTickTmp >= _VCLOCK_TIMER_SCALE_PER_LEVEL)
    {
        ulTickTmp -= _VCLOCK_TIMER_SCALE_PER_LEVEL;
    }

    head = &pstVClockInstance->stTimerLevel[ulLevel][ulTickTmp];
    DLL_ADD_TO_HEAD(head, &pstNode->stDllNode);
}

static inline int _vclock_add_timer
(
    VCLOCK_INSTANCE_S *pstVClockInstance,
    VCLOCK_NODE_S *pstNode,
    UINT first_tick,
    UINT tick,
    UINT flag,
    PF_TIME_OUT_FUNC pfFunc
)
{
    BS_DBGASSERT(NULL != pfFunc);
    BS_DBGASSERT(0 != tick);
    BS_DBGASSERT(NULL != pstNode);

    pstNode->flag = flag;
    pstNode->ulTick = tick;
    pstNode->pfFunc = pfFunc;

    pstVClockInstance->ulNodeCount ++;

    
    _vclock_timer_add(pstVClockInstance, pstNode, first_tick + 1); 

    return 0;
}

static inline BS_STATUS _VCLOCK_DelTimer ( VCLOCK_INSTANCE_S *pstVClockInstance,
        HANDLE hTimer)
{
    VCLOCK_NODE_S *pstNode = (VCLOCK_NODE_S *)hTimer;

    if (0 == hTimer) {
        RETURN(BS_NO_SUCH);
    }

    if (DLL_IN_LIST(&pstNode->stDllNode)) {
        DLL_DEL(DLL_GET_HEAD(pstNode), pstNode);
        pstVClockInstance->ulNodeCount --;
    }

    return BS_OK;
}

static inline BS_STATUS _VCLOCK_Pause(VCLOCK_INSTANCE_S *pstVClockInstance, HANDLE hTimer)
{
    VCLOCK_NODE_S *pstNode;

    pstNode = (VCLOCK_NODE_S*)hTimer;

    DLL_DelIfInList(&pstNode->stDllNode);

    pstNode->flag |= TIMER_FLAG_PAUSE;

    return BS_OK;    
}

static inline BS_STATUS _VCLOCK_Resume( VCLOCK_INSTANCE_S *pstVClockInstance, HANDLE hTimer, UINT first_tick )
{
    VCLOCK_NODE_S *pstNode;

    pstNode = (VCLOCK_NODE_S*)hTimer;

    if (! (pstNode->flag & TIMER_FLAG_PAUSE)) {
        return BS_PROCESSED;
    }

    pstNode->flag &= ~(UINT)TIMER_FLAG_PAUSE;

    _vclock_timer_add(pstVClockInstance, pstNode, first_tick);

    return BS_OK;
}

static inline UINT _VCLOCK_GetTickLeft(VCLOCK_INSTANCE_S *pstVClockInstance, HANDLE hTimer)
{
    UINT ulResTick;
     VCLOCK_NODE_S *pstNode;

    pstNode = (VCLOCK_NODE_S*)hTimer;

    ulResTick = pstNode->uiTriggerTick - pstVClockInstance->uiCurrentTick;

    return ulResTick;
}

static inline BS_STATUS _VCLOCK_GetInfo(VCLOCK_INSTANCE_S *pstVClockInstance, HANDLE hTimer, OUT TIMER_INFO_S *pstTimerInfo)
{
    VCLOCK_NODE_S *pstNode;

    BS_DBGASSERT(NULL != pstTimerInfo);

    pstNode = (VCLOCK_NODE_S*)hTimer;

    pstTimerInfo->ulTime = pstNode->ulTick;
    pstTimerInfo->flag = pstNode->flag;
    pstTimerInfo->pfFunc = pstNode->pfFunc;

    return BS_OK;
}

static inline BS_STATUS _VCLOCK_RestartWithTick
(
    VCLOCK_INSTANCE_S *pstVClockInstance,
    HANDLE hTimer,
    UINT first_tick,
    UINT tick
)
{
    VCLOCK_NODE_S *pstNode;

    pstNode = (VCLOCK_NODE_S*)hTimer;

    pstNode->ulTick = tick;
    _vclock_timer_add(pstVClockInstance, pstNode, first_tick);

    return BS_OK;
}

int VCLOCK_InitInstance(OUT VCLOCK_INSTANCE_S *pstVClockInstance, BOOL_T bCreateLock)
{
    int i, j;

    memset(pstVClockInstance, 0, sizeof(VCLOCK_INSTANCE_S));
    if (bCreateLock) {
        MUTEX_Init(&pstVClockInstance->lock);
    }

    pstVClockInstance->create_lock = bCreateLock;

    for (i=0; i<_VCLOCK_TIMER_TOTLE_LEVEL; i++)
    {
        for (j=0; j<_VCLOCK_TIMER_SCALE_PER_LEVEL; j++)
        {
            DLL_INIT(&pstVClockInstance->stTimerLevel[i][j]);
        }
    }

    return 0;
}

void VCLOCK_FiniInstance(VCLOCK_INSTANCE_S *pstVClockInstance)
{
    if (pstVClockInstance->create_lock) {
        MUTEX_Final(&pstVClockInstance->lock);
    }
}


VCLOCK_INSTANCE_HANDLE VCLOCK_CreateInstance(BOOL_T bCreateLock)
{
    VCLOCK_INSTANCE_S  *vclock;

    vclock = MEM_ZMalloc(sizeof(VCLOCK_INSTANCE_S));
    if (NULL == vclock) {
        return NULL;
    }

    VCLOCK_InitInstance(vclock, bCreateLock);

    return vclock;
}

void VCLOCK_DeleteInstance(VCLOCK_INSTANCE_HANDLE hVClock)
{
    VCLOCK_FiniInstance(hVClock);
    MEM_Free(hVClock);
}

int VCLOCK_AddTimer
(
    VCLOCK_INSTANCE_S *pstVClockInstance,
    VCLOCK_NODE_S *vclock_node,
    UINT first_tick, 
    UINT tick,      
    UINT flag,
    PF_TIME_OUT_FUNC pfFunc,
    USER_HANDLE_S *pstUserHandle
)
{
    int ret;

    BS_DBGASSERT(0 != pstVClockInstance);

    DLL_NODE_INIT(&vclock_node->stDllNode);

    if (pstUserHandle) {
        vclock_node->stUserHandle = *pstUserHandle;
    }

    _VCLOCK_Lock(pstVClockInstance);
    ret = _vclock_add_timer(pstVClockInstance, vclock_node, first_tick, tick, flag, pfFunc);
    _VCLOCK_UnLock(pstVClockInstance);

    return ret;
}

BS_STATUS VCLOCK_DelTimer(VCLOCK_INSTANCE_S *pstVClockInstance, VCLOCK_NODE_S *vclock_node)
{
    BS_STATUS eRet;

    BS_DBGASSERT(0 != pstVClockInstance);
    BS_DBGASSERT(0 != vclock_node);

    _VCLOCK_Lock(pstVClockInstance);
    eRet = _VCLOCK_DelTimer(pstVClockInstance, vclock_node);
    _VCLOCK_UnLock(pstVClockInstance);

    return eRet;
}

BOOL_T VCLOCK_IsRunning(VCLOCK_NODE_S *vclock_node)
{
    return DLL_IN_LIST(&vclock_node->stDllNode);
}

VCLOCK_NODE_S * VCLOCK_CreateTimer
(
    VCLOCK_INSTANCE_HANDLE hVClockInstanceId,
    UINT first_tick, 
    UINT tick,      
    UINT flag,
    PF_TIME_OUT_FUNC pfFunc,
    USER_HANDLE_S *pstUserHandle
)
{
    VCLOCK_NODE_S *pstNode;

    pstNode = MEM_ZMalloc(sizeof(VCLOCK_NODE_S));
    if (pstNode) {
        VCLOCK_AddTimer(hVClockInstanceId, pstNode, first_tick, tick,
                flag, pfFunc, pstUserHandle);
    }

    return pstNode;
}

void VCLOCK_DestroyTimer(VCLOCK_INSTANCE_S *pstVClockInstance, VCLOCK_NODE_S *hTimer)
{
    VCLOCK_DelTimer(pstVClockInstance, hTimer);
    MEM_Free(hTimer);
}

BS_STATUS VCLOCK_Pause(VCLOCK_INSTANCE_S *pstVClockInstance, VCLOCK_NODE_S *hTimer)
{
    BS_STATUS eRet;

    BS_DBGASSERT(0 != pstVClockInstance);
    BS_DBGASSERT(0 != hTimer);

    _VCLOCK_Lock(pstVClockInstance);
    eRet = _VCLOCK_Pause(pstVClockInstance, hTimer);
    _VCLOCK_UnLock(pstVClockInstance);

    return eRet;
}

BS_STATUS VCLOCK_Resume(VCLOCK_INSTANCE_S *pstVClockInstance, VCLOCK_NODE_S *hTimer, UINT first_tick)
{
    BS_STATUS eRet;

    BS_DBGASSERT(0 != pstVClockInstance);
    BS_DBGASSERT(0 != hTimer);

    _VCLOCK_Lock(pstVClockInstance);
    eRet = _VCLOCK_Resume(pstVClockInstance, hTimer, first_tick);
    _VCLOCK_UnLock(pstVClockInstance);

    return eRet;
}

BS_STATUS VCLOCK_GetInfo(VCLOCK_INSTANCE_S *pstVClockInstance, VCLOCK_NODE_S *hTimer, OUT TIMER_INFO_S *pstTimerInfo)
{
    BS_STATUS eRet;

    BS_DBGASSERT(0 != pstVClockInstance);
    BS_DBGASSERT(0 != hTimer);

    _VCLOCK_Lock(pstVClockInstance);
    eRet = _VCLOCK_GetInfo(pstVClockInstance, hTimer, pstTimerInfo);
    _VCLOCK_UnLock(pstVClockInstance);

    return eRet;
}

BS_STATUS VCLOCK_RestartWithTick
(
    VCLOCK_INSTANCE_S *pstVClockInstance,
    VCLOCK_NODE_S *hTimer,
    UINT first_tick,
    UINT tick
)
{
    BS_STATUS eRet;

    BS_DBGASSERT(0 != pstVClockInstance);
    BS_DBGASSERT(0 != hTimer);

    _VCLOCK_Lock(pstVClockInstance);
    eRet = _VCLOCK_RestartWithTick(pstVClockInstance, hTimer, first_tick, tick);
    _VCLOCK_UnLock(pstVClockInstance);

    return eRet;
}

BS_STATUS VCLOCK_Refresh(VCLOCK_INSTANCE_HANDLE hVClockInstance, VCLOCK_NODE_S *hTimer)
{
    VCLOCK_NODE_S *pstNode;

    BS_DBGASSERT(NULL != hTimer);

    pstNode = (VCLOCK_NODE_S*)hTimer;

    
    if (pstNode->ulTick == _VCLOCK_GetTickLeft(hVClockInstance, hTimer)) {
        return BS_OK;
    }

    return VCLOCK_RestartWithTick(hVClockInstance, hTimer, pstNode->ulTick, pstNode->ulTick);
}



UINT VCLOCK_GetTickLeft(VCLOCK_INSTANCE_HANDLE hVClockInstance, VCLOCK_NODE_S *hTimer)
{
    VCLOCK_INSTANCE_S *pstVClockInstance = (VCLOCK_INSTANCE_S *)hVClockInstance;
    UINT ulTick;

    BS_DBGASSERT(0 != hVClockInstance);
    BS_DBGASSERT(0 != hTimer);

    _VCLOCK_Lock(pstVClockInstance);
    ulTick = _VCLOCK_GetTickLeft(pstVClockInstance, hTimer);
    _VCLOCK_UnLock(pstVClockInstance);

    return ulTick;
}


BS_STATUS VCLOCK_Step(VCLOCK_INSTANCE_HANDLE hVClockInstance)
{
    VCLOCK_NODE_S *pstNode, *pstNodeTmp;
    INT i;
    VCLOCK_INSTANCE_S *pstVClockInstance = (VCLOCK_INSTANCE_S *)hVClockInstance;
    DLL_HEAD_S *dll_head;
    int index;

    _VCLOCK_Lock(pstVClockInstance);

    pstVClockInstance->uiCurrentTick ++;

    for (i=0; i<_VCLOCK_TIMER_TOTLE_LEVEL; i++) {
        pstVClockInstance->ulCurrentLevelTick[i]++;

        if (pstVClockInstance->ulCurrentLevelTick[i] >= _VCLOCK_TIMER_SCALE_PER_LEVEL) {
            pstVClockInstance->ulCurrentLevelTick[i] = 0;
        } else {
            break;
        }
    }

    if (i >= _VCLOCK_TIMER_TOTLE_LEVEL) {
        i = _VCLOCK_TIMER_TOTLE_LEVEL - 1;
    }

    
    for (; i>0; i--) {
        index = pstVClockInstance->ulCurrentLevelTick[i];
        dll_head = &pstVClockInstance->stTimerLevel[i][index];
        DLL_SAFE_SCAN(dll_head, pstNode, pstNodeTmp) {
            _vclock_timer_add(pstVClockInstance, pstNode, pstNode->uiTriggerTick - pstVClockInstance->uiCurrentTick);
        }
    }

    
    while (1) {
        index = pstVClockInstance->ulCurrentLevelTick[0];
        dll_head = &pstVClockInstance->stTimerLevel[0][index];
        pstNode = DLL_Get(dll_head);
        if (NULL == pstNode) {
            break;
        }

        if (pstNode->flag & TIMER_FLAG_CYCLE) {
            _vclock_timer_add(pstVClockInstance, pstNode, pstNode->ulTick);
        } else {
            pstVClockInstance->ulNodeCount --;
        }

        _VCLOCK_UnLock(pstVClockInstance);
        pstNode->pfFunc(pstNode, &pstNode->stUserHandle);
        _VCLOCK_Lock(pstVClockInstance);
    }
    
    _VCLOCK_UnLock(pstVClockInstance);

	return BS_OK;
}

