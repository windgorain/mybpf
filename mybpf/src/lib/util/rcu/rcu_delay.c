/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#include "bs.h"
#include "utl/list_sl.h"
#include "utl/rcu_delay.h"


static inline BOOL_T rcudelay_do_just(RCU_DELAY_S *ctrl)
{
    if ((ctrl->counter[0] == 0) && (ctrl->counter[1] == 0)) {
        return TRUE;
    }

    return FALSE;
}

int RcuDelay_Init(RCU_DELAY_S *ctrl)
{
    memset(ctrl, 0, sizeof(RCU_DELAY_S));
    return 0;
}


void RcuDelay_Wait(RCU_DELAY_S *ctrl)
{
    int old_period_count;
    int now_period_count;

    old_period_count = ctrl->grace_period_count;

    while (1) {
        Sleep(100);
        RcuDelay_Step(ctrl);
        now_period_count = ctrl->grace_period_count;
        if ((now_period_count - old_period_count) >= 2) {
            break;
        }
    }

    return;
}


void RcuDelay_Sync(RCU_DELAY_S *ctrl)
{
    int old_period_count;
    int now_period_count;

    if (rcudelay_do_just(ctrl)) {
        return;
    }

    old_period_count = ctrl->grace_period_count;

    while (1) {
        Sleep(100);
        RcuDelay_Step(ctrl);
        if (rcudelay_do_just(ctrl)) {
            break;
        }
        now_period_count = ctrl->grace_period_count;
        if ((now_period_count - old_period_count) >= 2) {
            break;
        }
    }

    return;
}

void RcuDelay_Call(RCU_DELAY_S *ctrl, RCU_NODE_S *node, PF_RCU_FREE_FUNC func)
{
    int state = ATOM_GET(&ctrl->state);
    node->pfFunc = func;
    SL_AddHead(&ctrl->list[state], &node->node);
}

void RcuDelay_Step(RCU_DELAY_S *ctrl)
{
    RCU_NODE_S *node;

    SpinLock_Lock(&ctrl->lock);

    int next_state = ctrl->state == 0 ? 1 : 0;
    
    if (ctrl->counter[next_state] == 0) {
        while ((node = (void*)SL_DelHead(&ctrl->list[next_state]))) {
            node->pfFunc(node);
        }
        ATOM_BARRIER();
        ctrl->grace_period_count++;
        ctrl->state = next_state;
    }

    SpinLock_UnLock(&ctrl->lock);
}

