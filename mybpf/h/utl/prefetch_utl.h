/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _PREFETCH_UTL_H
#define _PREFETCH_UTL_H
#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __X86__

static inline void PreFetch0(const volatile void *p)
{
	asm volatile ("prefetcht0 %[p]" : : [p] "m" (*(const volatile char *)p));
}

static inline void PreFetch1(const volatile void *p)
{
	asm volatile ("prefetcht1 %[p]" : : [p] "m" (*(const volatile char *)p));
}

static inline void PreFetch2(const volatile void *p)
{
	asm volatile ("prefetcht2 %[p]" : : [p] "m" (*(const volatile char *)p));
}

static inline void PreFetchNonTemporal(const volatile void *p)
{
	asm volatile ("prefetchnta %[p]" : : [p] "m" (*(const volatile char *)p));
}

#else 

#define PreFetch0
#define PreFetch1
#define PreFetch2
#define PreFetchNonTemporal

#endif

#ifdef __cplusplus
}
#endif
#endif 
