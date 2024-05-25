/*================================================================
*   Created by LiXingang: 
*   Description: 
*
================================================================*/
#ifndef _THREAD_UTL_H
#define _THREAD_UTL_H
#ifdef __cplusplus
extern "C"
{
#endif

#ifdef IN_UNIXLIKE
typedef pthread_t THREAD_ID;
#endif

#ifdef IN_WINDOWS
typedef ULONG THREAD_ID;
#endif

#define THREAD_ID_INVALID 0

typedef void (*PF_THREAD_UTL_FUNC)(void *user_data);

THREAD_ID ThreadUtl_Create(PF_THREAD_UTL_FUNC func, UINT pri, UINT stack_size, void *user_data);
BS_STATUS ThreadUtl_Suspend(THREAD_ID thread_id);
BS_STATUS ThreadUtl_Resume(THREAD_ID thread_id);
THREAD_ID ThreadUtl_GetSelfID(void);

#define THREAD_GetSelfID() ThreadUtl_GetSelfID()

#ifdef __cplusplus
}
#endif
#endif 
