
#ifndef __TMSG2_UTL_H_
#define __TMSG2_UTL_H_

#include "utl/mbuf_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 

typedef HANDLE TMSG2_HANDLE;

typedef BS_STATUS (*PF_TMSG2_SEND_FUNC)(IN MBUF_S *pstMbuf, IN USER_HANDLE_S *pstUserHandle);
typedef BS_STATUS (*PF_TMSG2_RECV_FUNC)(IN MBUF_S *pstMbuf, IN USER_HANDLE_S *pstUserHandle);
typedef VOID (*PF_TMSG2_ERR_FUNC)(IN UINT uiErrno, IN USER_HANDLE_S *pstUserHandle);


TMSG2_HANDLE TMSG2_Create
(
    IN PF_TMSG2_SEND_FUNC pfSendFunc,
    IN PF_TMSG2_RECV_FUNC pfRecvFunc,
    IN PF_TMSG2_ERR_FUNC pfErrFunc,
    IN USER_HANDLE_S *pstUserHandle
);

VOID TMSG2_Destory(IN TMSG2_HANDLE hTmsg2);

BS_STATUS TMSG2_Send(IN TMSG2_HANDLE hTmsg2, IN MBUF_S *pstMbuf);

BS_STATUS TMSG2_Input(IN TMSG2_HANDLE hTmsg2, IN MBUF_S *pstMbuf);



#ifdef __cplusplus
    }
#endif 

#endif 

