/*================================================================
*   Created：2018.11.03
*   Author:  LiXingang 
*   Description：
================================================================*/
#include "bs.h"

#include "utl/bit_opt.h"
#include "utl/darray_utl.h"
#include "utl/mypoll_utl.h"

#include "mypoll_inner.h"
#include "mypoll_proto.h"

#ifdef IN_LINUX

#include <sys/epoll.h>

typedef struct
{
    _MYPOLL_CTRL_S *pstMyPollCtrl;
    int epoll_id;
}_MYPOLL_EPOLL_CTRL_S;

static BS_STATUS mypoll_epoll_Init(IN _MYPOLL_CTRL_S *pstMyPoll)
{
    _MYPOLL_EPOLL_CTRL_S *pstCtrl;

    pstCtrl = MEM_ZMalloc(sizeof(_MYPOLL_EPOLL_CTRL_S));
    if (NULL == pstCtrl) {
        return BS_NO_MEMORY;
    }

    
    pstCtrl->epoll_id = epoll_create(65535);
    if (pstCtrl->epoll_id < 0) {
        MEM_Free(pstCtrl);
        return BS_CAN_NOT_OPEN;
    }

    pstMyPoll->pProtoHandle = pstCtrl;
    pstCtrl->pstMyPollCtrl = pstMyPoll;

    return BS_OK;
}

static VOID mypoll_epoll_Fini(IN _MYPOLL_CTRL_S *pstMyPoll)
{
    _MYPOLL_EPOLL_CTRL_S *pstCtrl = pstMyPoll->pProtoHandle;

    if (NULL == pstCtrl) {
        return;
    }

    if (pstCtrl->epoll_id >= 0) {
        close(pstCtrl->epoll_id);
    }

    MEM_Free(pstCtrl);

    return;
}

static BS_STATUS mypoll_epoll_Add
(
    IN _MYPOLL_CTRL_S *pstMyPoll,
    IN INT iSocketId,
    IN UINT uiEvent,
    IN PF_MYPOLL_EV_NOTIFY pfNotifyFunc
)
{
    _MYPOLL_EPOLL_CTRL_S *pstCtrl = pstMyPoll->pProtoHandle;
    struct epoll_event ev = {0};

    if (NULL == pstCtrl) {
        BS_DBGASSERT(0);
        RETURN(BS_BAD_PARA);
    }

    ev.data.fd = iSocketId;
    ev.events = 0;

    if (uiEvent & MYPOLL_EVENT_IN) {
        ev.events |= EPOLLIN;
    }

    if (uiEvent & MYPOLL_EVENT_OUT) {
        ev.events |= EPOLLOUT;
    }

    epoll_ctl(pstCtrl->epoll_id, EPOLL_CTL_ADD, iSocketId, &ev);

    return BS_OK;
}

static BS_STATUS mypoll_epoll_Set
(
    IN _MYPOLL_CTRL_S *pstMyPoll,
    IN INT iSocketId,
    IN UINT uiEvent,
    IN PF_MYPOLL_EV_NOTIFY pfNotifyFunc
)
{
    _MYPOLL_EPOLL_CTRL_S *pstCtrl = pstMyPoll->pProtoHandle;
    struct epoll_event ev = {0};

    if (NULL == pstCtrl) {
        BS_DBGASSERT(0);
        RETURN(BS_BAD_PARA);
    }

    ev.data.fd = iSocketId;
    ev.events = 0;

    if (uiEvent & MYPOLL_EVENT_IN) {
        ev.events |= EPOLLIN;
    }

    if (uiEvent & MYPOLL_EVENT_OUT) {
        ev.events |= EPOLLOUT;
    }

    epoll_ctl(pstCtrl->epoll_id, EPOLL_CTL_MOD, iSocketId, &ev);

    return BS_OK;
}

static VOID mypoll_epoll_Del
(
    IN _MYPOLL_CTRL_S *pstMyPoll,
    IN INT iSocketId
)
{
    _MYPOLL_EPOLL_CTRL_S *pstCtrl = pstMyPoll->pProtoHandle;
    struct epoll_event ev = {0};

    if (NULL == pstCtrl) {
        BS_DBGASSERT(0);
        return;
    }

    ev.data.fd = iSocketId;
    ev.events = 0xffffffff;

    epoll_ctl(pstCtrl->epoll_id, EPOLL_CTL_DEL, iSocketId, &ev);

    return;
}

static void _mypoll_epoll_SetOdd(_MYPOLL_CTRL_S *pstMyPoll, struct epoll_event *events, int count, int odd)
{
    int i;
    MYPOLL_FDINFO_S *pstFdInfo;
    UINT odd_bit = 0;

    if (odd) {
        odd_bit = _MYPOLL_FDINFO_LOOP_ODD;
    }

    for (i=0; i<count; i++) {
        pstFdInfo = DARRAY_Get(pstMyPoll->hFdInfoTbl, events[i].data.fd);
        if (pstFdInfo) {
            BIT_SETTO(pstFdInfo->flag, _MYPOLL_FDINFO_LOOP_ODD, odd_bit);
        }
    }
}

static int mypoll_epoll_Run(IN _MYPOLL_CTRL_S *pstMyPoll)
{
    _MYPOLL_EPOLL_CTRL_S *pstCtrl = pstMyPoll->pProtoHandle;
    INT i;
    MYPOLL_FDINFO_S *pstFdInfo;
    struct epoll_event events[64];
    int count;
    int fd;
    int odd;
    int ret = BS_STOP;

    while (1)
    {
        BIT_CLR(pstMyPoll->uiFlag, _MYPOLL_FLAG_RESTART | _MYPOLL_FLAG_PROCESSING_EVENT);
        count = epoll_wait(pstCtrl->epoll_id, events, 64, -1);
        BIT_SET(pstCtrl->pstMyPollCtrl->uiFlag, _MYPOLL_FLAG_PROCESSING_EVENT);

        if (count < 0) {
            continue;
        }

        odd = BIT_ISSET(pstMyPoll->uiFlag, _MYPOLL_FLAG_LOOP_ODD);
        BIT_TURN(pstMyPoll->uiFlag, _MYPOLL_FLAG_LOOP_ODD);
        _mypoll_epoll_SetOdd(pstMyPoll, events, count, odd);

        for (i=0; i<count; i++)
        {
            
            if (pstCtrl->pstMyPollCtrl->uiFlag & _MYPOLL_FLAG_RESTART) {
                break;
            }

            fd = events[i].data.fd;

            pstFdInfo = DARRAY_Get(pstMyPoll->hFdInfoTbl, fd);
            if (NULL == pstFdInfo) {
                continue;
            }

            if (BIT_ISSET(pstFdInfo->flag, _MYPOLL_FDINFO_LOOP_ODD) != odd) {
                continue;
            }

            ret = pstFdInfo->pfNotifyFunc(fd, events[i].events, &(pstFdInfo->stUserHandle));
            if (ret < 0) {
                return ret;
            }
        }
    }

    return ret;
}

static MYPOLL_PROTO_S g_stMypollEpollProto = 
{
    MYPOLL_PROTO_FLAG_AT_ONCE,
    mypoll_epoll_Init,
    mypoll_epoll_Fini,
    mypoll_epoll_Add,
    mypoll_epoll_Set,
    mypoll_epoll_Del,
    mypoll_epoll_Run
};

MYPOLL_PROTO_S * Mypoll_Epoll_GetProtoTbl()
{
    return &g_stMypollEpollProto;
}

#else

MYPOLL_PROTO_S * Mypoll_Epoll_GetProtoTbl()
{
    return NULL;
}
#endif
