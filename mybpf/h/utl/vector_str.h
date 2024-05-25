#ifndef __VECTOR_STR_H__
#define __VECTOR_STR_H__


#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utl/vector_utl.h"

#define DEFAULT_VECTOR_STRING_CAPACITY  16
#define DEFAULT_VECTOR_STRING_EXPAND_FACTOR  16

static inline VECTOR_S * VectorStr_Create(int init_size, int ele_size)
{   
    VECTOR_PARAM_S p = {0};

    p.ele_size = ele_size;
    p.init_size = (init_size != 0) ? init_size: DEFAULT_VECTOR_STRING_CAPACITY;
    p.resize_factor = DEFAULT_VECTOR_STRING_EXPAND_FACTOR;

    return VECTOR_Create(&p);
}

static inline void VectorStr_Destroy(VECTOR_S *sa)
{
    VECTOR_Destroy(sa);
}

static inline UINT VectorStr_Count(VECTOR_S *sa)
{
    return VECTOR_Count(sa);
}

static inline char * VectorStr_Get(VECTOR_S *sa, UINT pos)
{
    return VECTOR_Get(sa, pos);
}

static inline int VectorStr_Append(VECTOR_S *sa, char *new_str)
{
    if (! new_str) {
        RETURN(BS_ERR);
    }
    return VECTOR_AddBySize(sa, new_str, strlen(new_str) + 1);
}

#endif
