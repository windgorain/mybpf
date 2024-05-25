/*================================================================
*   Created by LiXingang: 2018.12.07
*   Description: 最小二叉堆
*
================================================================*/
#ifndef _BINARY_HEAP_H
#define _BINARY_HEAP_H

#include "utl/darray_utl.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef int (*PF_BINARY_HEAP_CMP)(IN void *data1, IN void * data2);

typedef struct {
    DARRAY_HANDLE hDarray;
    PF_BINARY_HEAP_CMP pfCmp;
    UINT count; 
}BINARY_HEAP_S;

int BinaryHeap_Init(IN BINARY_HEAP_S *heap, IN PF_BINARY_HEAP_CMP pfCmp);
void BinaryHeap_SetCmpFunc(IN BINARY_HEAP_S *heap, IN PF_BINARY_HEAP_CMP pfCmp);

int BinaryHeap_Insert(IN BINARY_HEAP_S *heap, IN void *new_data);

void * BinaryHeap_RemoveIndex(IN BINARY_HEAP_S *heap, IN UINT index);
int BinaryHeap_RemoveData(IN BINARY_HEAP_S *heap, IN void *data);

#ifdef __cplusplus
}
#endif
#endif 
