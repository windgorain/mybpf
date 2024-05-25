#ifndef __TMSG_UTL_H_
#define __TMSG_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

#define TMSG_FLAG_IS_ACK 0x1  
#define TMSG_FLAG_NEED_ACK 0x10  


#define TMSG_EVENT_SEND     1   
#define TMSG_EVENT_SUCCESS  2   
#define TMSG_EVENT_FAILED   3   

#define TMSG_IS_ACK_MSG(pstMsg) ((pstMsg)->uiFlag & TMSG_FLAG_IS_ACK)
#define TMSG_NEED_MSG(pstMsg) ((pstMsg)->uiFlag & TMSG_FLAG_NEED_ACK)

typedef HANDLE TMSG_HANDLE;

typedef struct
{
    UINT uiFlag;
    UINT uiSequence;     
}TMSG_S;


typedef void (*PF_TMSG_CALL_BACK_FUNC)(IN UINT uiEvent, IN void *pstOpt, IN void *ud);

typedef struct TMSG_OPT_S {
    TMSG_S stTmsg;
    UINT uiTicks;    
    UINT uiRetryTimes; 

    PF_TMSG_CALL_BACK_FUNC pfEventFunc;
}TMSG_OPT_S;

TMSG_HANDLE TMSG_Create();
VOID TMSG_Destory(IN TMSG_HANDLE hTmsgHandle);
BS_STATUS TMSG_Send
(
    IN TMSG_HANDLE hTmsgHandle,
    IN TMSG_OPT_S *pstOpt,
    IN USER_HANDLE_S *pstUserHandle
);
UINT TMSG_GetMsgNum(IN TMSG_HANDLE hTmsgHandle);
VOID TMSG_StopAll(IN TMSG_HANDLE hTmsgHandle);
BOOL_T TMSG_RecvedAck(IN TMSG_HANDLE hTmsgHandle, IN TMSG_S *pstTmsg);
VOID TMSG_TickStep(IN TMSG_HANDLE hTmsgHandle);


#ifdef __cplusplus
    }
#endif 

#endif 

