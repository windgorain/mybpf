/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#include "bs.h"
#include "utl/percpu_value.h"
#include "utl/get_cpu.h"

static void * _percpu_add(PERCPU_VALUE_S *tbl, int cpu)
{
    void *v = MEM_ZKalloc(tbl->value_size);
    if (! v) {
        return NULL;
    }

    if (__sync_bool_compare_and_swap(&tbl->values[cpu], NULL, v)) {
        return v;
    }

    MEM_KFree(v);

    return (void*)tbl->values[cpu];
}

int PERCPU_Init(OUT PERCPU_VALUE_S *tbl, int value_size)
{
    memset(tbl, 0, sizeof(*tbl));
    tbl->value_size = value_size;

    return 0;
}

void PERCPU_Destroy(PERCPU_VALUE_S *tbl)
{
    for (int i=0; i<MAX_CPU_NUM; i++) {
        if (tbl->values[i]) {
            MEM_KFree((void*)tbl->values[i]);
            tbl->values[i] = NULL;
        }
    }
    tbl->value_size = 0;
}

void * PERCPU_Get(PERCPU_VALUE_S *tbl)
{
    int cpu = sched_getcpu();
    void *v;

    v = (void*)tbl->values[cpu];
    if (v) {
        return v;
    }

    return _percpu_add(tbl, cpu);
}

