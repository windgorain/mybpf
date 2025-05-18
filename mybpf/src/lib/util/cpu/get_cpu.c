/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#include "bs.h"

#ifdef __X86__
#include <cpuid.h>
#define CPUID(INFO, LEAF, SUBLEAF) __cpuid_count(LEAF, SUBLEAF, INFO[0], INFO[1], INFO[2], INFO[3])

#define GETCPU(CPU) {                                   \
        U32 CPUInfo[4];                            \
        CPUID(CPUInfo, 1, 0);                           \
         \
        if ( (CPUInfo[3] & (1 << 9)) == 0) {            \
          CPU = -1;                \
        } else {                                        \
          CPU = (unsigned)CPUInfo[1] >> 24;             \
        }                                               \
        if (CPU < 0) CPU = 0;                           \
      }
#else
#define GETCPU(cpu) do {cpu = 0; BS_WARNNING(("Not support"));} while(0)
#endif

#ifndef IN_LINUX
int sched_getcpu(void)
{
    int cpu;
    GETCPU(cpu);
    return cpu;
}
#endif

