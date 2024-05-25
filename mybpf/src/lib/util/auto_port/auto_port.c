/*================================================================
*   Created by LiXingang
*   Description: 自动化Port管理
*
================================================================*/
#include "bs.h"
#include "utl/auto_port.h"
#include "utl/process_utl.h"

static int autoport_OpenInc(AUTOPORT_S *ap, PF_AUTOPORT_OPEN open_fn, void *ud)
{
    USHORT low = MIN(ap->v1, ap->v2);
    USHORT high = MAX(ap->v1, ap->v2);

    if (high == 0) {
        RETURN(BS_NO_SUCH);
    }

    if (low == 0) {
        low = 1;
    }

    int i;
    for (i=low; i<high; i++) { 
        if (i == open_fn(i, ud)) {
            ap->port = i;
            return i;
        }
    }

    RETURN(BS_FULL);
}

static int autoport_OpenPid(AUTOPORT_S *ap, PF_AUTOPORT_OPEN open_fn, void *ud)
{
    UINT pid = PROCESS_GetPid();

    ap->port = (pid & 0xfff) + ap->v1;
    if (ap->port == 0) {
        RETURN(BS_EMPTY);
    }

    return open_fn(ap->port, ud);
}

static int autoport_OpenAdd(AUTOPORT_S *ap, PF_AUTOPORT_OPEN open_fn, void *ud)
{
    ap->port = ap->v1 + ap->v2;

    if (ap->port == 0) {
        RETURN(BS_EMPTY);
    }

    return open_fn(ap->port, ud);
}

static int autoport_OpenAny(AUTOPORT_S *ap, PF_AUTOPORT_OPEN open_fn, void *ud)
{
    int ret;

    ret = open_fn(ap->port, ud);
    if (ret < 0) {
        return ret;
    }

    ap->port = ret;

    return ret;
}


int AutoPort_Open(AUTOPORT_S *ap, PF_AUTOPORT_OPEN open_fn, void *ud)
{
    switch (ap->port_type) {
        case AUTOPORT_TYPE_SET:
            return open_fn(ap->port, ud);
        case AUTOPORT_TYPE_INC:
            return autoport_OpenInc(ap, open_fn, ud);
        case AUTOPORT_TYPE_PID:
            return autoport_OpenPid(ap, open_fn, ud);
        case AUTOPORT_TYPE_ADD:
            return autoport_OpenAdd(ap, open_fn, ud);
        case AUTOPORT_TYPE_ANY:
            return autoport_OpenAny(ap, open_fn, ud);
        default:
            RETURN(BS_NOT_SUPPORT);
    }

    RETURN(BS_ERR);
}

