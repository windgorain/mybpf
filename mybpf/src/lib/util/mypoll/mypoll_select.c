/******************************************************************************
* Copyright (C), LiXingang
* Author:      LiXingang  Version: 1.0  Date: 2011-3-24
* Description: 
* History:     
******************************************************************************/
#include "bs.h"

#include "utl/asm_utl.h"
#include "utl/bit_opt.h"
#include "utl/darray_utl.h"
#include "utl/mypoll_utl.h"

#include "mypoll_inner.h"
#include "mypoll_proto.h"

#define _MYPOLL_SELECT_HASH_BUCKET     1024

typedef struct
{
    _MYPOLL_CTRL_S *pstMyPollCtrl;
    fd_set socketReadFds;
    fd_set socketWriteFds;
    fd_set socketExpFds;
    INT iMaxSocketId;
}_MYPOLL_SELECT_CTRL_S;

STATIC BS_STATUS mypoll_select_Init(IN _MYPOLL_CTRL_S *pstMyPoll)
{
    _MYPOLL_SELECT_CTRL_S *pstCtrl;

    pstCtrl = MEM_ZMalloc(sizeof(_MYPOLL_SELECT_CTRL_S));
    if (NULL == pstCtrl)
    {
        return BS_NO_MEMORY;
    }

    pstMyPoll->pProtoHandle = pstCtrl;
    pstCtrl->pstMyPollCtrl = pstMyPoll;

    return BS_OK;
}

static VOID mypoll_select_Fini(IN _MYPOLL_CTRL_S *pstMyPoll)
{
    _MYPOLL_SELECT_CTRL_S *pstCtrl = pstMyPoll->pProtoHandle;

    if (NULL == pstCtrl)
    {
        return;
    }

    MEM_Free(pstCtrl);

    return;
}

static BS_STATUS mypoll_select_Set
(
    IN _MYPOLL_CTRL_S *pstMyPoll,
    IN INT iSocketId,
    IN UINT uiEvent,
    IN PF_MYPOLL_EV_NOTIFY pfNotifyFunc
)
{
    _MYPOLL_SELECT_CTRL_S *pstCtrl = pstMyPoll->pProtoHandle;

    if (NULL == pstCtrl)
    {
        BS_DBGASSERT(0);
        RETURN(BS_BAD_PARA);
    }

    pstCtrl->iMaxSocketId = MAX(iSocketId, pstCtrl->iMaxSocketId);
    
    if (uiEvent & MYPOLL_EVENT_IN)
    {
        FD_SET((UINT)iSocketId, &(pstCtrl->socketReadFds));
    }
    else
    {
        FD_CLR((UINT)iSocketId, &(pstCtrl->socketReadFds));
    }

    if (uiEvent & MYPOLL_EVENT_OUT)
    {
        FD_SET((UINT)iSocketId, &(pstCtrl->socketWriteFds));
    }
    else
    {
        FD_CLR((UINT)iSocketId, &(pstCtrl->socketWriteFds));
    }

    FD_SET((UINT)iSocketId, &(pstCtrl->socketExpFds));

    return BS_OK;
}

static VOID mypoll_select_Del
(
    IN _MYPOLL_CTRL_S *pstMyPoll,
    IN INT iSocketId
)
{
    _MYPOLL_SELECT_CTRL_S *pstCtrl = pstMyPoll->pProtoHandle;

    if (NULL == pstCtrl)
    {
        BS_DBGASSERT(0);
        return;
    }

    FD_CLR((UINT)iSocketId, &(pstCtrl->socketReadFds));
    FD_CLR((UINT)iSocketId, &(pstCtrl->socketWriteFds));
    FD_CLR((UINT)iSocketId, &(pstCtrl->socketExpFds));

    return;
}

static void _mypoll_select_SetOdd(_MYPOLL_CTRL_S *pstMyPoll, int odd)
{
    int i;
    MYPOLL_FDINFO_S *pstFdInfo;
    UINT odd_bit = 0;
    _MYPOLL_SELECT_CTRL_S *pstCtrl = pstMyPoll->pProtoHandle;

    if (odd) {
        odd_bit = _MYPOLL_FDINFO_LOOP_ODD;
    }

    for (i=0; i<=pstCtrl->iMaxSocketId; i++) {
        pstFdInfo = DARRAY_Get(pstMyPoll->hFdInfoTbl, i);
        if (pstFdInfo != NULL) {
            BIT_SETTO(pstFdInfo->flag, _MYPOLL_FDINFO_LOOP_ODD, odd_bit);
        }
    }
}

static int mypoll_select_Run(IN _MYPOLL_CTRL_S *pstMyPoll)
{
    _MYPOLL_SELECT_CTRL_S *pstCtrl = pstMyPoll->pProtoHandle;
    fd_set stReadFds, stWriteFds, stExecptFds;
    INT i;
    int odd;
    int count;
    UINT uiEvent;
    MYPOLL_FDINFO_S *pstFdInfo;
    int ret = BS_STOP;

    while (1) {
        BIT_CLR(pstCtrl->pstMyPollCtrl->uiFlag, _MYPOLL_FLAG_RESTART | _MYPOLL_FLAG_PROCESSING_EVENT);
        ATOM_BARRIER();
        stReadFds = pstCtrl->socketReadFds;
        stWriteFds = pstCtrl->socketWriteFds;
        stExecptFds = pstCtrl->socketExpFds;

        count = select(pstCtrl->iMaxSocketId + 1, &stReadFds, &stWriteFds, &stExecptFds, NULL);

        if (count < 0) {
            continue;
        }

        odd = BIT_ISSET(pstMyPoll->uiFlag, _MYPOLL_FLAG_LOOP_ODD);
        BIT_TURN(pstMyPoll->uiFlag, _MYPOLL_FLAG_LOOP_ODD);
        _mypoll_select_SetOdd(pstMyPoll, odd);

        BIT_SET(pstCtrl->pstMyPollCtrl->uiFlag, _MYPOLL_FLAG_PROCESSING_EVENT);

        for (i=0; i<=pstCtrl->iMaxSocketId; i++) {
            
            if (pstCtrl->pstMyPollCtrl->uiFlag & _MYPOLL_FLAG_RESTART) {
                break;
            }

            uiEvent = 0;
            if (FD_ISSET(i, &stReadFds)) {
                uiEvent |= MYPOLL_EVENT_IN;
            }
            if (FD_ISSET(i, &stWriteFds)) {
                uiEvent |= MYPOLL_EVENT_OUT;
            }
            if (FD_ISSET(i, &stExecptFds)) {
                uiEvent |= MYPOLL_EVENT_ERR;
            }

            if (uiEvent == 0) {
                continue;
            }

            pstFdInfo = DARRAY_Get(pstMyPoll->hFdInfoTbl, i);
            if (NULL == pstFdInfo) {
                continue;
            }

            if (BIT_ISSET(pstFdInfo->flag, _MYPOLL_FDINFO_LOOP_ODD) != odd) {
                continue;
            }

            ret = pstFdInfo->pfNotifyFunc(i, uiEvent, &(pstFdInfo->stUserHandle));
            if (ret < 0) {
                return ret;
            }
        }
    }

    return ret;
}

static MYPOLL_PROTO_S g_stMypollSelectProto = 
{
    0,
    mypoll_select_Init,
    mypoll_select_Fini,
    mypoll_select_Set,
    mypoll_select_Set,
    mypoll_select_Del,
    mypoll_select_Run
};

MYPOLL_PROTO_S * Mypoll_Select_GetProtoTbl(void)
{
    return &g_stMypollSelectProto;
}

