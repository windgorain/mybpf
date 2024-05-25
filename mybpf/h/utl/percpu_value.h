/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _PERCPU_VALUE_H
#define _PERCPU_VALUE_H

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_CPU_NUM 256

typedef struct {
    int value_size;
    volatile void *values[MAX_CPU_NUM];
}PERCPU_VALUE_S;

int PERCPU_Init(OUT PERCPU_VALUE_S *tbl, int value_size);
void PERCPU_Destroy(PERCPU_VALUE_S *tbl);
void * PERCPU_Get(PERCPU_VALUE_S *tbl);

#ifdef __cplusplus
}
#endif
#endif 
