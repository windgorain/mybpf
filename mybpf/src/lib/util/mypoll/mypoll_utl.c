/******************************************************************************
* Copyright (C),  LiXingang
* Author:      LiXingang  Version: 1.0  Date: 2011-3-24
* Description: 
* History:     
******************************************************************************/
#include "bs.h"

#include "utl/darray_utl.h"
#include "utl/mypoll_utl.h"
#include "utl/socket_utl.h"
#include "utl/bit_opt.h"
#include "utl/signal_utl.h"

#include "mypoll_inner.h"
#include "mypoll_proto.h"

static _MYPOLL_CTRL_S * g_mypoll_signal = NULL;

static int _mypoll_process_user_event(_MYPOLL_CTRL_S *pstCtrl)
{
    UINT uiUserEvent;

    if (NULL != pstCtrl->pfUserEventFunc) {
        MUTEX_P(&pstCtrl->lock);
        uiUserEvent = pstCtrl->uiUserEvent;
        pstCtrl->uiUserEvent = 0;
        MUTEX_V(&pstCtrl->lock);

        if (uiUserEvent != 0) {
            return pstCtrl->pfUserEventFunc(uiUserEvent, &(pstCtrl->stUserEventUserHandle));
        }
    }

    return 0;
}

static int _mypoll_process_signal(_MYPOLL_CTRL_S *pstCtrl)
{
    int i;
    int ret;

    for (i=0; i<_MYPOLL_MAX_SINGAL_NUM; i++) {
        if (pstCtrl->singal_processers[i].pfFunc != NULL) {
            if (pstCtrl->singal_processers[i].uiCount != 0) {
                pstCtrl->singal_processers[i].uiCount = 0;
                if ((ret = pstCtrl->singal_processers[i].pfFunc(i)) < 0) {
                    return ret;
                }
            }
        }
    }

    return 0;
}

static int _mypoll_notify_trigger(int fd, UINT uiEvent, USER_HANDLE_S *uh)
{
    _MYPOLL_CTRL_S *pstCtrl = uh->ahUserHandle[0];
    UCHAR aucData[256];
	UINT uiReadLen;
    int ret;

    (VOID) Socket_Read2(pstCtrl->iSocketDst, aucData, sizeof(aucData), &uiReadLen, 0);

    ret = _mypoll_process_user_event(pstCtrl);
    if (ret < 0) {
        return ret;
    }

    ret = _mypoll_process_signal(pstCtrl);
    if (ret < 0) {
        return ret;
    }

    return 0;
}

static int _mypoll_set_fd_info(_MYPOLL_CTRL_S *pstCtrl, int fd, UINT uiEvent,
        PF_MYPOLL_EV_NOTIFY pfNotifyFunc, USER_HANDLE_S *uh)
{
    MYPOLL_FDINFO_S *pstFdInfo;

    pstFdInfo = DARRAY_Get(pstCtrl->hFdInfoTbl, fd);
    if (NULL == pstFdInfo)
    {
        pstFdInfo = MEM_ZMalloc(sizeof(MYPOLL_FDINFO_S));
        if (NULL == pstFdInfo)
        {
            return BS_NO_MEMORY;
        }
        if (BS_OK != DARRAY_Set(pstCtrl->hFdInfoTbl, fd, pstFdInfo))
        {
            MEM_Free(pstFdInfo);
            return BS_ERR;
        }
    }

    pstFdInfo->pfNotifyFunc = pfNotifyFunc;
    pstFdInfo->uiEvent = uiEvent;

    if (BIT_ISSET(pstCtrl->uiFlag, _MYPOLL_FLAG_LOOP_ODD)) {
        BIT_SET(pstFdInfo->flag, _MYPOLL_FDINFO_LOOP_ODD);
    } else {
        BIT_CLR(pstFdInfo->flag, _MYPOLL_FDINFO_LOOP_ODD);
    }

    if (uh) {
        pstFdInfo->stUserHandle = *uh;
    }

    return BS_OK;
}

static inline MYPOLL_FDINFO_S * _mypoll_get_fd_info(_MYPOLL_CTRL_S *pstCtrl, int fd)
{
    return DARRAY_Get(pstCtrl->hFdInfoTbl, fd);
}

static void _mypoll_free_fd_info(_MYPOLL_CTRL_S *pstCtrl, int fd)
{
    MYPOLL_FDINFO_S *pstFdInfo;

    pstFdInfo = DARRAY_Clear(pstCtrl->hFdInfoTbl, fd);
    if (pstFdInfo) {
        MEM_Free(pstFdInfo);
    }
}

static void _mypoll_signal(int iSigno)
{
    if (g_mypoll_signal == NULL) {
        return;
    }

    g_mypoll_signal->singal_processers[iSigno].uiCount ++;
    Socket_Write((UINT)g_mypoll_signal->iSocketSrc, (char*)"0", 1, 0);
}

MYPOLL_HANDLE MyPoll_Create(void)
{
    _MYPOLL_CTRL_S *pstCtrl;
    INT aiFd[2];
    USER_HANDLE_S stUserHandle;

    pstCtrl = MEM_ZMalloc(sizeof(_MYPOLL_CTRL_S));
    if (NULL == pstCtrl)
    {
        return NULL;
    }
    pstCtrl->iSocketDst = -1;
    pstCtrl->iSocketSrc = -1;

    if (BS_OK != Socket_Pair(SOCK_STREAM, aiFd))
    {
        MyPoll_Destory(pstCtrl);
        return NULL;
    }
    pstCtrl->iSocketSrc = aiFd[0];
    pstCtrl->iSocketDst = aiFd[1];

    Socket_SetNoBlock(aiFd[0], 1);
    Socket_SetNoBlock(aiFd[1], 1);
    Socket_SetNoDelay(aiFd[0], 1);
    Socket_SetNoDelay(aiFd[1], 1);

    pstCtrl->hFdInfoTbl = DARRAY_Create(1024, 128);
    if (NULL == pstCtrl->hFdInfoTbl)
    {
        MyPoll_Destory(pstCtrl);
        return NULL;
    }

    if (BS_OK != _Mypoll_Proto_Init(pstCtrl))
    {
        MyPoll_Destory(pstCtrl);
        return NULL;
    }

    stUserHandle.ahUserHandle[0] = pstCtrl;
    if (BS_OK != MyPoll_SetEvent(pstCtrl, aiFd[1], MYPOLL_EVENT_IN,
                    _mypoll_notify_trigger, &stUserHandle))
    {
        MyPoll_Destory(pstCtrl);
        return NULL;
    }

    MUTEX_Init(&pstCtrl->lock);

    return pstCtrl;
}

VOID MyPoll_Destory(MYPOLL_HANDLE hMypoll)
{
    _MYPOLL_CTRL_S *pstCtrl = (_MYPOLL_CTRL_S*)hMypoll;

    if (g_mypoll_signal == pstCtrl) {
        g_mypoll_signal = NULL;
    }

    _MyPoll_Proto_Fini(pstCtrl);

    if (pstCtrl->iSocketSrc >= 0)
    {
        Socket_Close(pstCtrl->iSocketSrc);
        Socket_Close(pstCtrl->iSocketDst);
    }

    if (pstCtrl->hFdInfoTbl != NULL)
    {
        DARRAY_Destory(pstCtrl->hFdInfoTbl);
    }

    MUTEX_Final(&pstCtrl->lock);

    MEM_Free(pstCtrl);
    
    return;
}


BS_STATUS MyPoll_SetEvent(MYPOLL_HANDLE hMypoll, int fd, UINT uiEvent,
        PF_MYPOLL_EV_NOTIFY pfNotifyFunc, USER_HANDLE_S *uh )
{
    _MYPOLL_CTRL_S *pstCtrl = (_MYPOLL_CTRL_S*)hMypoll;
    int add = 0;

    if (NULL == _mypoll_get_fd_info(pstCtrl, fd)) {
        add = 1;
    }

    if (BS_OK != _mypoll_set_fd_info(pstCtrl, fd, uiEvent, pfNotifyFunc, uh)) {
        return BS_ERR;
    }

    if (add) {
        return _Mypoll_Proto_Add(pstCtrl, fd, uiEvent, pfNotifyFunc);
    } else {
        return _Mypoll_Proto_Set(pstCtrl, fd, uiEvent, pfNotifyFunc);
    }
}

BS_STATUS MyPoll_ModifyUserHandle(MYPOLL_HANDLE hMypoll, int fd, USER_HANDLE_S *uh)
{
    _MYPOLL_CTRL_S *pstCtrl = (_MYPOLL_CTRL_S*)hMypoll;
    MYPOLL_FDINFO_S *info;

    info = _mypoll_get_fd_info(pstCtrl, fd);
    if (NULL == info) {
        RETURN(BS_NOT_FOUND);
    }

    if (uh) {
        info->stUserHandle = *uh;
    }

    return BS_OK;
}

USER_HANDLE_S * MyPoll_GetUserHandle(MYPOLL_HANDLE hMypoll, int fd)
{
    _MYPOLL_CTRL_S *pstCtrl = (_MYPOLL_CTRL_S*)hMypoll;
    MYPOLL_FDINFO_S *info;

    info = _mypoll_get_fd_info(pstCtrl, fd);
    if (NULL == info) {
        return NULL;
    }

    return &info->stUserHandle;
}


BS_STATUS MyPoll_AddEvent(MYPOLL_HANDLE hMypoll, int fd, UINT uiEvent)
{
    _MYPOLL_CTRL_S *pstCtrl = (_MYPOLL_CTRL_S*)hMypoll;
    MYPOLL_FDINFO_S *pstFdInfo;

    pstFdInfo = _mypoll_get_fd_info(pstCtrl, fd);
    if (NULL == pstFdInfo)
    {
        return BS_ERR;
    }

    if ((pstFdInfo->uiEvent & uiEvent) == uiEvent)
    {
        return BS_OK;
    }

    pstFdInfo->uiEvent |= uiEvent;

    return MyPoll_SetEvent(hMypoll, fd, pstFdInfo->uiEvent, pstFdInfo->pfNotifyFunc, &pstFdInfo->stUserHandle);
}


BS_STATUS MyPoll_DelEvent(MYPOLL_HANDLE hMypoll, int fd, UINT uiEvent)
{
    _MYPOLL_CTRL_S *pstCtrl = (_MYPOLL_CTRL_S*)hMypoll;
    MYPOLL_FDINFO_S *pstFdInfo;

    pstFdInfo = _mypoll_get_fd_info(pstCtrl, fd);
    if (NULL == pstFdInfo)
    {
        return BS_ERR;
    }

    if ((pstFdInfo->uiEvent & uiEvent) == 0)
    {
        return BS_OK;
    }

    pstFdInfo->uiEvent &= (~uiEvent);

    return MyPoll_SetEvent(hMypoll, fd, pstFdInfo->uiEvent, pstFdInfo->pfNotifyFunc, &pstFdInfo->stUserHandle);
}


BS_STATUS MyPoll_ClearEvent(MYPOLL_HANDLE hMypoll, INT fd)
{
    return MyPoll_DelEvent(hMypoll, fd, MYPOLL_EVENT_ALL);
}


BS_STATUS MyPoll_ModifyEvent(MYPOLL_HANDLE hMypoll, int fd, UINT uiEvent)
{
    _MYPOLL_CTRL_S *pstCtrl = (_MYPOLL_CTRL_S*)hMypoll;
    MYPOLL_FDINFO_S *pstFdInfo;

    pstFdInfo = _mypoll_get_fd_info(pstCtrl, fd);
    if (NULL == pstFdInfo)
    {
        return BS_ERR;
    }

    if (pstFdInfo->uiEvent == uiEvent)
    {
        return BS_OK;
    }

    pstFdInfo->uiEvent = uiEvent;

    return MyPoll_SetEvent(hMypoll, fd, pstFdInfo->uiEvent, pstFdInfo->pfNotifyFunc, &pstFdInfo->stUserHandle);
}


void MyPoll_Del(MYPOLL_HANDLE hMypoll, int fd)
{
    _MYPOLL_CTRL_S *pstCtrl = (_MYPOLL_CTRL_S*)hMypoll;

    _Mypoll_Proto_Del(pstCtrl, fd);

    _mypoll_free_fd_info(pstCtrl, fd);
}

int MyPoll_Run(MYPOLL_HANDLE hMypoll)
{
    _MYPOLL_CTRL_S *pstCtrl = (_MYPOLL_CTRL_S*)hMypoll;
    return _Mypoll_Proto_Run(pstCtrl);
}




void MyPoll_Restart(MYPOLL_HANDLE hMypoll)
{
    _MYPOLL_CTRL_S *pstCtrl = (_MYPOLL_CTRL_S*)hMypoll;

    pstCtrl->uiFlag |= _MYPOLL_FLAG_RESTART;
}


BS_STATUS MyPoll_SetSignalProcessor(MYPOLL_HANDLE hMypoll, INT signo, PF_MYPOLL_SIGNAL_FUNC pfFunc)
{
    _MYPOLL_CTRL_S *pstCtrl = (_MYPOLL_CTRL_S*)hMypoll;

    if (signo >= _MYPOLL_MAX_SINGAL_NUM) {
        RETURN(BS_OUT_OF_RANGE);
    }

    g_mypoll_signal = pstCtrl;
    pstCtrl->singal_processers[signo].pfFunc = pfFunc;

    SIGNAL_Set(signo, 0, (void*)_mypoll_signal);

    return BS_OK;
}


BS_STATUS MyPoll_SetUserEventProcessor(MYPOLL_HANDLE hMypoll,
        PF_MYPOLL_USER_EVENT_FUNC pfFunc,
        USER_HANDLE_S *uh )
{
    _MYPOLL_CTRL_S *pstCtrl = (_MYPOLL_CTRL_S*)hMypoll;

    pstCtrl->pfUserEventFunc = pfFunc;

    if (uh) {
        pstCtrl->stUserEventUserHandle = *uh;
    }

    return BS_OK;
}


BS_STATUS MyPoll_PostUserEvent(MYPOLL_HANDLE hMypoll, UINT uiEvent)
{
    _MYPOLL_CTRL_S *pstCtrl = (_MYPOLL_CTRL_S*)hMypoll;
    int need_trigger = 0;

    if (NULL != pstCtrl) {
        MUTEX_P(&pstCtrl->lock);
        if (pstCtrl->uiUserEvent == 0) {
            need_trigger = 1;
        }
        pstCtrl->uiUserEvent |= uiEvent;
        MUTEX_V(&pstCtrl->lock);

        if (need_trigger) {
            MyPoll_Trigger(hMypoll);
        }
    }

    return BS_OK;
}


BS_STATUS MyPoll_Trigger(MYPOLL_HANDLE hMyPoll)
{
    _MYPOLL_CTRL_S *pstCtrl = (_MYPOLL_CTRL_S*)hMyPoll;
    Socket_Write((UINT)pstCtrl->iSocketSrc, (char*)"0", 1, 0);

    return 0;
}

