/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _RCU_QSBR_H
#define _RCU_QSBR_H
#ifdef __cplusplus
extern "C"
{
#endif

#define RCU_QSBR_MAX_READER 128

typedef struct {
    volatile int writer_state;
    volatile int reader_state[RCU_QSBR_MAX_READER];
    volatile int reader_num;
}RCU_QSBR_S;

void RcuQsbr_Free(RCU_QSBR_S *ctrl, RCU_NODE_S *node, PF_RCU_FREE_FUNC func);
void RcuQsbr_Quiescent(RCU_QSBR_S *ctrl, int reader_id);
void RcuQsbr_SetReaderNum(RCU_QSBR_S *ctrl, int reader_num);

#ifdef __cplusplus
}
#endif
#endif 
