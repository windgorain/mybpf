/*================================================================
*   Created by LiXingang: 2007.2.8
*   Description: 
*
================================================================*/
#include "bs.h"
#include "utl/thread_utl.h"

#include "thread_os.h"

#ifdef IN_WINDOWS

int _ThreadOs_Create(PF_THREAD_UTL_FUNC func, UINT pri, UINT stack_size, void *arg)
{
    HANDLE hHandle;
    UINT os_id;

    hHandle = (HANDLE)_beginthreadex(NULL, stack_size, (LPTHREAD_START_ROUTINE)func, arg, 0, &os_id);
    if (hHandle == 0) {
        RETURN(BS_ERR);
    }
 
    CloseHandle(hHandle);

    return os_id;
}

BS_STATUS _ThreadOs_Suspend(int thread_id)
{
    int ret;
    HANDLE hHandle;

    hHandle = OpenThread(THREAD_ALL_ACCESS, FALSE, thread_id);

    if (hHandle == NULL) {
        RETURN(BS_ERR);
    }

    ret = SuspendThread(hHandle);
    CloseHandle (hHandle);

    if (-1 == ret) {
         RETURN(BS_ERR);
    }

    return BS_OK;
}

BS_STATUS _ThreadOs_Resume(int thread_id)
{
    HANDLE hHandle;
    int ret;

    hHandle = OpenThread(THREAD_ALL_ACCESS, FALSE, thread_id);

    if (hHandle == NULL) {
        RETURN(BS_ERR);
    }
    
    ret = ResumeThread(hHandle);
    CloseHandle (hHandle);
    
    if (-1 == ret) {
        RETURN(BS_ERR);
    }

    return BS_OK;
}

THREAD_ID _ThreadOs_GetSelfID()
{
    return GetCurrentThreadId();
}

#endif
