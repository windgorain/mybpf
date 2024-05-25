/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#include "bs.h"
#include "utl/rdtsc_utl.h"
#include "utl/cpu_utl.h"
#include <sys/ioctl.h>
#include <fcntl.h>
#ifndef __ARM__
#include <cpuid.h>
#endif

UINT64 RDTSC_HZ = 0;
UINT64 RDTSC_US_HZ = 0;
UINT64 RDTSC_MS_HZ = 0;

#ifdef __ARM__
uint64_t get_tsc_freq_arch(void)
{
	uint64_t freq;
	asm volatile("mrs %0, cntfrq_el0" : "=r" (freq));
	return freq;
}
#endif

#ifdef __X86__
static uint32_t check_model_wsm_nhm(uint8_t model)
{
	switch (model) {
	
	case 0x25:
	case 0x2C:
	case 0x2F:
	
	case 0x1E:
	case 0x1F:
	case 0x1A:
	case 0x2E:
		return 1;
	}

	return 0;
}

static uint32_t check_model_gdm_dnv(uint8_t model)
{
	switch (model) {
	
	case 0x5C:
	
	case 0x5F:
		return 1;
	}

	return 0;
}

static int32_t rdmsr(int msr, uint64_t *val)
{
#ifdef IN_LINUX
	int fd;
	int ret;

	fd = open("/dev/cpu/0/msr", O_RDONLY);
	if (fd < 0)
		return fd;

	ret = pread(fd, val, sizeof(uint64_t), msr);

	close(fd);

	return ret;
#else
    return -1;
#endif
}

uint64_t get_tsc_freq_arch(void)
{
	uint64_t tsc_hz = 0;
	uint32_t a, b, c = 0, d, maxleaf;
	uint8_t mult, model;
	int32_t ret;

	
	maxleaf = __get_cpuid_max(0, NULL);

	if (maxleaf >= 0x15) {
		__cpuid(0x15, a, b, c, d);

		
		if (b && c)
			return c * (b / a);
	}

	model = CPU_GetModel();

	if (check_model_wsm_nhm(model))
		mult = 133;
	else if ((c & bit_AVX) || check_model_gdm_dnv(model))
		mult = 100;
	else
		return 0;

	ret = rdmsr(0xCE, &tsc_hz);
	if (ret < 0)
		return 0;

	return ((tsc_hz >> 8) & 0xff) * mult * 1E6;
}
#endif

uint64_t get_tsc_freq(void)
{
#ifdef CLOCK_MONOTONIC_RAW
#define NS_PER_SEC 1E9
#define CYC_PER_10MHZ 1E7

	struct timespec sleeptime = {.tv_nsec = NS_PER_SEC / 10 }; 

	struct timespec t_start, t_end;
	uint64_t tsc_hz;

	if (clock_gettime(CLOCK_MONOTONIC_RAW, &t_start) == 0) {
		uint64_t ns, end, start = RDTSC_Get();
		nanosleep(&sleeptime, NULL);
		clock_gettime(CLOCK_MONOTONIC_RAW, &t_end);
		end = RDTSC_Get();
		ns = ((t_end.tv_sec - t_start.tv_sec) * NS_PER_SEC);
		ns += (t_end.tv_nsec - t_start.tv_nsec);

		double secs = (double)ns/NS_PER_SEC;
		tsc_hz = (uint64_t)((end - start)/secs);
		
		return NUM_NEAR_ALIGN(tsc_hz, CYC_PER_10MHZ);
	}
#endif
	return 0;
}

static uint64_t estimate_tsc_freq(void)
{
#define CYC_PER_10MHZ 1E7
	
	uint64_t start = RDTSC_Get();
	sleep(1);
	
	return NUM_NEAR_ALIGN(RDTSC_Get() - start, CYC_PER_10MHZ);
}

UINT64 RDTSC_GetHz()
{
	UINT64 freq;

	freq = get_tsc_freq_arch();
	if (!freq)
		freq = get_tsc_freq();
	if (!freq)
		freq = estimate_tsc_freq();

	return freq;
}

CONSTRUCTOR(init) {
    RDTSC_HZ = RDTSC_GetHz();
    RDTSC_MS_HZ = RDTSC_HZ / 1000;
    RDTSC_US_HZ = RDTSC_HZ / 1000000;
}
