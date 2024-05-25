/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _GET_CPU_H
#define _GET_CPU_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef IN_LINUX
int sched_getcpu(void);
#endif

#ifdef __cplusplus
}
#endif
#endif 
