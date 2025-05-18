/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _RDTSC_UTL_H
#define _RDTSC_UTL_H
#ifdef __cplusplus
extern "C"
{
#endif



#ifdef __X86__
static inline UINT64 RDTSC_Get(void)
{
	union {
		U64 tsc_64;
		struct {
			U32 lo_32;
			U32 hi_32;
		};
	} tsc;

	asm volatile("rdtsc" :
		     "=a" (tsc.lo_32),
		     "=d" (tsc.hi_32));

	return tsc.tsc_64;
}
#elif defined(__ARM__)
static inline UINT64 RDTSC_Get(void)
{
    U64 tsc;
    asm volatile("mrs %0, cntvct_el0" : "=r"(tsc));
    return tsc;
}
#else
static inline UINT64 RDTSC_Get(void)
{
    struct timespec val;
    UINT64 v;

    while (clock_gettime(CLOCK_MONOTONIC_RAW, &val) != 0);
    v = (UINT64) val.tv_sec * 1000000000LL;
    v += (UINT64) val.tv_nsec;
    return v;
}
#endif

extern UINT64 RDTSC_HZ;
extern UINT64 RDTSC_MS_HZ;
extern UINT64 RDTSC_US_HZ;

UINT64 RDTSC_GetHz(void);

#ifdef __cplusplus
}
#endif
#endif 
