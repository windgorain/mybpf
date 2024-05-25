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
static inline UINT64 RDTSC_Get()
{
	union {
		uint64_t tsc_64;
		struct {
			uint32_t lo_32;
			uint32_t hi_32;
		};
	} tsc;

	asm volatile("rdtsc" :
		     "=a" (tsc.lo_32),
		     "=d" (tsc.hi_32));

	return tsc.tsc_64;
}
#endif

#if 0
#ifdef __ARM__
static inline UINT64 RDTSC_Get()
{
	unsigned tsc;
	UINT64 final_tsc;

	
	asm volatile("mrc p15, 0, %0, c9, c13, 0" : "=r"(tsc));
	
	final_tsc = ((UINT64)tsc) << 6;

	return final_tsc;
}
#endif
#endif

#ifndef __X86__
static inline UINT64 RDTSC_Get()
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

UINT64 RDTSC_GetHz();

#ifdef __cplusplus
}
#endif
#endif 
