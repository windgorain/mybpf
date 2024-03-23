/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#include "bs.h"
#include "utl/mybpf_loader.h"
#include "utl/mybpf_prog.h"
#include "utl/mybpf_vm.h"
#include "utl/mybpf_vm.h"
#include "utl/mybpf_hookpoint.h"
#include "utl/ufd_utl.h"
#include "mybpf_def_inner.h"
#include "mybpf_osbase.h"

static inline int _mybpf_process_xdp_fd(MYBPF_XDP_BUFF_S *xdp_buf, void *prog)
{
    MYBPF_PARAM_S p;

    p.p[0] = (long)xdp_buf;

    int ret = MYBPF_PROG_Run(prog, &p);
    if (ret < 0) {
        return XDP_PASS;
    }

    return p.bpf_ret;
}

int MYBPF_XdpInput(MYBPF_RUNTIME_S *runtime, MYBPF_XDP_BUFF_S *xdp_buf)
{
    MYBPF_HOOKPOINT_NODE_S *node;
    DLL_HEAD_S *list = &runtime->hp_list[MYBPF_HP_XDP];
    int ret = XDP_PASS;

    int state = RcuEngine_Lock();

    DLL_SCAN(list, node) {
        ret = _mybpf_process_xdp_fd(xdp_buf, node->prog);
        if (ret != XDP_PASS) {
            break;
        }
    }

    RcuEngine_UnLock(state);

    return ret;
}

