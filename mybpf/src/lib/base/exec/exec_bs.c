/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#include "bs.h"

static THREAD_LOCAL HANDLE g_thread_exec = 0;

void EXEC_Attach(HANDLE hExec)
{
    g_thread_exec = hExec;
}

void EXEC_Detach(HANDLE hExec)
{
    if (g_thread_exec == hExec) {
        g_thread_exec = NULL;
    }
}

HANDLE EXEC_GetExec()
{
    return g_thread_exec;
}

