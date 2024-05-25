#ifndef VECTOR_INT_H
#define VECTOR_INT_H

#include <stdbool.h>
#include "utl/vector_utl.h"

#define DEFAULT_VECTOR_INT_SIZE 16
#define DEFAULT_VECTOR_FACTOR_SIZE 32

static inline VECTOR_S * VectorInt_Create(int isize)
{
    VECTOR_PARAM_S p = {0};

    p.ele_size = sizeof(int);
    p.init_size = isize ? isize : DEFAULT_VECTOR_INT_SIZE;
    p.resize_factor = DEFAULT_VECTOR_FACTOR_SIZE;

    return VECTOR_Create(&p);
}

static inline int VectorInt_Append(VECTOR_S *vec, int val)
{
    return VECTOR_Add(vec, &val);
}

static inline int VectorInt_Get(VECTOR_S *vec, int pos)
{
    int val = 0;
    VECTOR_Copy(vec, pos, &val);
    return val;
}

BOOL_T VectorInt_IsExist(VECTOR_S *vec, int val);
void VectorInt_Sort(VECTOR_S *vec);
int VectorInt_DelByVal(VECTOR_S *vec, int val);

#endif
