/*================================================================
*   Created by LiXingang: 2007.2.8
*   Description: 
*   History:  2018.11.16 moved from thread_bs to utl   
================================================================*/
#include "bs.h"
#include "utl/thread_utl.h"

#include "thread_os.h"

#ifdef IN_UNIXLIKE

typedef void *(*PF_START_ROUTINE_FUNC)(void*);

THREAD_ID _ThreadOs_Create(PF_THREAD_UTL_FUNC func, UINT pri, UINT stack_size, void *arg)
{
    pthread_t tid;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_CREATE_DETACHED);

    if (0 != pthread_create(&tid, NULL, (PF_START_ROUTINE_FUNC)func, arg)) {
        return THREAD_ID_INVALID;
    }

    return tid;
}

BS_STATUS _ThreadOs_Suspend(THREAD_ID thread_id)
{
    pthread_kill((pthread_t)thread_id, SIGSTOP);
    return BS_OK;
}

BS_STATUS _ThreadOs_Resume(THREAD_ID thread_id)
{
    pthread_kill(thread_id, SIGCONT);
    return BS_OK;
}

THREAD_ID _ThreadOs_GetSelfID(void)
{
    return pthread_self();
}
#endif
