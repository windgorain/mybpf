/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _RCU_ENGINE_H
#define _RCU_ENGINE_H
#include "utl/rcu_utl.h"
#ifdef __cplusplus
extern "C"
{
#endif

PLUG_API void * RcuEngine_GetMemcap();
PLUG_API void RcuEngine_Call(RCU_NODE_S *rcu_node, PF_RCU_FREE_FUNC rcu_func);
PLUG_API void RcuEngine_Wait();
PLUG_API void RcuEngine_Sync();
PLUG_API int RcuEngine_Lock();
PLUG_API void RcuEngine_UnLock(int state);

#ifdef __cplusplus
}
#endif
#endif 
