/*================================================================
*   Created by LiXingang 
*   Description: 
*
================================================================*/
#ifndef _THREAD_OS_H
#define _THREAD_OS_H
#ifdef __cplusplus
extern "C"
{
#endif

THREAD_ID _ThreadOs_Create(PF_THREAD_UTL_FUNC func, UINT pri, UINT stack_size, void *arg);
BS_STATUS _ThreadOs_Suspend(THREAD_ID thread_id);
BS_STATUS _ThreadOs_Resume(THREAD_ID thread_id);
THREAD_ID _ThreadOs_GetSelfID(void);


#ifdef __cplusplus
}
#endif
#endif 
