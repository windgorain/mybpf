/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _RCU_QSBR_H
#define _RCU_QSBR_H

#include "utl/cache_def.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    volatile int reader_state CACHE_ALIGNED;
}RCU_QSBR_READER_S;

typedef struct {
    int reader_num;
    volatile int writer_state;
    RCU_QSBR_READER_S reader[0];
}RCU_QSBR_S;

RCU_QSBR_S * RcuQsbr_Create(int reader_num);
void RcuQsbr_Sync(RCU_QSBR_S *ctrl);


static inline void RcuQsbr_Quiescent(RCU_QSBR_S *ctrl, int reader_id)
{
    BS_DBGASSERT(reader_id < ctrl->reader_num);
    ctrl->reader[reader_id].reader_state = ctrl->writer_state;
}

#ifdef __cplusplus
}
#endif
#endif 
