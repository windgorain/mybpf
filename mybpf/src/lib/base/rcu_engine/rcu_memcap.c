/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#include "bs.h"
#include "utl/rcu_delay.h"
#include "utl/mem_cap.h"

static MEM_CAP_S g_rcu_engine_memcap;

static void _rcu_engine_call(RCU_NODE_S *rcu_node, PF_RCU_FREE_FUNC rcu_func)
{
    RcuEngine_Call(rcu_node, rcu_func);
}


void * RcuEngine_GetMemcap()
{
    if (! g_rcu_engine_memcap.cap_call) {
        MemCap_Init(&g_rcu_engine_memcap, _mem_rcu_malloc, MEM_RcuFree, _rcu_engine_call);
    }

    return &g_rcu_engine_memcap;
}

