/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _ULIMIT_UTL_H
#define _ULIMIT_UTL_H
#include <sys/resource.h>
#ifdef __cplusplus
extern "C"
{
#endif

static inline void ULIMIT_SetMemLock(unsigned long long cur, unsigned long long max)
{
	struct rlimit r;
    r.rlim_cur = cur;
    r.rlim_max = max;
	setrlimit(RLIMIT_MEMLOCK, &r);
}

#ifdef __cplusplus
}
#endif
#endif 
