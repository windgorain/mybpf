/*================================================================
*   Created by LiXingang: 2007.2.8
*   Description: 线程工具库
*    History:  2018.11.16 moved from thread_bs to utl   
================================================================*/
#include "bs.h"
#include "utl/thread_utl.h"

#include "thread_os.h"

THREAD_ID ThreadUtl_Create(PF_THREAD_UTL_FUNC func, UINT pri, UINT stack_size, void *user_data)
{
    return _ThreadOs_Create(func, pri, stack_size, user_data);
}

BS_STATUS ThreadUtl_Suspend(THREAD_ID thread_id)
{
    return _ThreadOs_Suspend(thread_id);
}

BS_STATUS ThreadUtl_Resume(THREAD_ID thread_id)
{
    return _ThreadOs_Resume (thread_id);
}

THREAD_ID ThreadUtl_GetSelfID(void)
{
    return _ThreadOs_GetSelfID();
}

