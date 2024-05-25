/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _ARRAY_BIT_H
#define _ARRAY_BIT_H

#include "utl/num_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 

#define ARRAYBIT_SCAN_FREE_BEGIN(_data, _bit_size, _index)  do { \
    INT64 _i, _j; \
    INT64 _uint_num = NUM_UP_ALIGN(bit_size, 32)/32; \
    for (_i=0; _i<_uint_num; _i++) { \
        if (_data[_i] == 0xffffffff) \
            continue; \
        for (_j=0; _j<32; _j++) { \
            if (_data[_i] & (1 << _j)) \
                continue; \
            _index = _i*32 + _j; \
            if (_index >= _bit_size) \
                break; \
            {

#define ARRAYBIT_SCAN_BUSY_BEGIN(_data, _bit_size, _index)  do { \
    INT64 _i, _j; \
    INT64 _uint_num = NUM_UP_ALIGN(bit_size, 32)/32; \
    for (_i=0; _i<_uint_num; _i++) { \
        if (_data[_i] == 0) \
            continue; \
        for (_j=0; _j<32; _j++) { \
            if ((_data[_i] & (1 << _j)) == 0) \
                continue; \
            _index = _i*32 + _j; \
            if (_index >= _bit_size) \
                break; \
            {

#define ARRAYBIT_SCAN_END()  }}}} while(0)


typedef int (*PF_ARRAY_BIT_WALK)(INT64 index, void *ud);


static inline void ArrayBit_Set(UINT *data, INT64 index)
{
    data[index>>5] |= ((UINT)1 << (index & 31));
}

static inline void ArrayBit_Clr(UINT *data, INT64 index)
{
    data[index>>5] &= ~((UINT)1 << (index & 31));
}

static inline int ArrayBit_Test(UINT *data, INT64 index)
{
    U32 ret = data[index>>5] & ((UINT)1 << (index & 31));
    return !!ret;
}


INT64 ArrayBit_GetFree(UINT *data, INT64 bit_size);
INT64 ArrayBit_GetFreeFrom(UINT *data, INT64 bit_size, INT64 from);
INT64 ArrayBit_GetFreeAfter(UINT *data, INT64 bit_size, INT64 curr);
void ArrayBit_WalkFree(UINT *data, INT64 bit_size, PF_ARRAY_BIT_WALK walk_func, void *ud);
UINT64 ArrayBit_GetFreeCount(UINT *data, INT64 bit_size);

INT64 ArrayBit_GetBusyFrom(UINT *data, INT64 bit_size, INT64 from);
INT64 ArrayBit_GetBusyAfter(UINT *data, INT64 bit_size, INT64 curr);
void ArrayBit_WalkBusy(UINT *data, INT64 bit_size, PF_ARRAY_BIT_WALK walk_func, void *ud);
UINT64 ArrayBit_GetBusyCount(UINT *data, INT64 bit_size);


static inline INT64 ArrayBit_GetBusy(UINT *data, INT64 bit_size )
{
    INT64 index;

    ARRAYBIT_SCAN_BUSY_BEGIN(data, bit_size, index) {
        return index;
    }ARRAYBIT_SCAN_END();

    return -1;
}


static inline void ArrayBit_And(UINT *data1, UINT *data2, int uint_count, OUT UINT *data3)
{
    int i;
    for (i=0; i<uint_count; i++) {
        data3[i] = data1[i] & data2[i];
    }
}


static inline void ArrayBit_Or(UINT *data1, UINT *data2, int uint_count, OUT UINT *data3)
{
    int i;
    for (i=0; i<uint_count; i++) {
        data3[i] = data1[i] | data2[i];
    }
}


static inline void ArrayBit_Xor(UINT *data1, UINT *data2, int uint_count, OUT UINT *data3)
{
    int i;
    for (i=0; i<uint_count; i++) {
        data3[i] = data1[i] ^ data2[i];
    }
}


static inline void ArrayBit_And64(U64 *data1, U64 *data2, int u64_count, OUT U64 *data3)
{
    int i;
    for (i=0; i<u64_count; i++) {
        data3[i] = data1[i] & data2[i];
    }
}


static inline void ArrayBit_Or64(U64 *data1, U64 *data2, int u64_count, OUT U64 *data3)
{
    int i;
    for (i=0; i<u64_count; i++) {
        data3[i] = data1[i] | data2[i];
    }
}


static inline void ArrayBit_Xor64(U64 *data1, U64 *data2, int u64_count, OUT U64 *data3)
{
    int i;
    for (i=0; i<u64_count; i++) {
        data3[i] = data1[i] ^ data2[i];
    }
}

#ifdef __cplusplus
    }
#endif 

#endif 
