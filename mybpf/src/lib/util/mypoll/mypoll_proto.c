/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2014-7-6
* Description: 
* History:     
******************************************************************************/
#include "bs.h"

#include "utl/darray_utl.h"
#include "utl/mypoll_utl.h"
#include "utl/socket_utl.h"

#include "mypoll_inner.h"
#include "mypoll_proto.h"

static MYPOLL_PROTO_S * mypoll_proto_GetProtoTbl(void)
{
#ifdef IN_LINUX
    return Mypoll_Epoll_GetProtoTbl();
#else
    return Mypoll_Select_GetProtoTbl();
#endif
}

BS_STATUS _Mypoll_Proto_Init(IN _MYPOLL_CTRL_S *pstMyPoll)
{
    MYPOLL_PROTO_S *pstProto;

    pstProto = mypoll_proto_GetProtoTbl();
    if (NULL == pstProto)
    {
        return BS_ERR;
    }

    if (BS_OK != pstProto->pfInit(pstMyPoll))
    {
        return BS_ERR;
    }

    pstMyPoll->pProto = pstProto;

    return BS_OK;
}

VOID _MyPoll_Proto_Fini(IN _MYPOLL_CTRL_S *pstMyPoll)
{
    MYPOLL_PROTO_S *pstProto = pstMyPoll->pProto;

    if (NULL != pstProto)
    {
        pstProto->pfFini(pstMyPoll);
    }
}

BS_STATUS _Mypoll_Proto_Add
(
    IN _MYPOLL_CTRL_S *pstMyPoll,
    IN INT iSocketId,
    IN UINT uiEvent,
    IN PF_MYPOLL_EV_NOTIFY pfNotifyFunc
)
{
    MYPOLL_PROTO_S *pstProto = pstMyPoll->pProto;
    BS_STATUS eRet;

    eRet = pstProto->pfAdd(pstMyPoll, iSocketId, uiEvent, pfNotifyFunc);
    if (eRet != BS_OK) {
        return eRet;
    }

    if (((pstProto->uiFlag & MYPOLL_PROTO_FLAG_AT_ONCE) == 0)
        && ((pstMyPoll->uiFlag & _MYPOLL_FLAG_PROCESSING_EVENT) == 0)) {
        
        Socket_Write((UINT)pstMyPoll->iSocketSrc, (char*)"1", 1, 0);
    }

    return BS_OK;
}

BS_STATUS _Mypoll_Proto_Set
(
    IN _MYPOLL_CTRL_S *pstMyPoll,
    IN INT iSocketId,
    IN UINT uiEvent,
    IN PF_MYPOLL_EV_NOTIFY pfNotifyFunc
)
{
    MYPOLL_PROTO_S *pstProto = pstMyPoll->pProto;
    BS_STATUS eRet;

    eRet = pstProto->pfSet(pstMyPoll, iSocketId, uiEvent, pfNotifyFunc);
    if (eRet != BS_OK) {
        return eRet;
    }

    if (((pstProto->uiFlag & MYPOLL_PROTO_FLAG_AT_ONCE) == 0)
        && ((pstMyPoll->uiFlag & _MYPOLL_FLAG_PROCESSING_EVENT) == 0)) {
        
        Socket_Write((UINT)pstMyPoll->iSocketSrc, (char*)"1", 1, 0);
    }

    return BS_OK;
}

VOID _Mypoll_Proto_Del
(
    IN _MYPOLL_CTRL_S *pstMyPoll,
    IN INT iSocketId
)
{
    MYPOLL_PROTO_S *pstProto = pstMyPoll->pProto;

    pstProto->pfDel(pstMyPoll, iSocketId);
}

int _Mypoll_Proto_Run(IN _MYPOLL_CTRL_S *pstMyPoll)
{
    MYPOLL_PROTO_S *pstProto = pstMyPoll->pProto;
    return pstProto->pfRun(pstMyPoll);
}


