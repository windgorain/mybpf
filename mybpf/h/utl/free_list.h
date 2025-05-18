/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _FREE_LIST_H
#define _FREE_LIST_H
#include "utl/list_sl.h"
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    SL_HEAD_S free_list;
}FREE_LIST_S;

void FreeList_Init(FREE_LIST_S *list);
void FreeList_Puts(FREE_LIST_S *list, void *nodes, UINT node_len, UINT max_nodes);

static inline void FreeList_Put(FREE_LIST_S *list, void *node)
{
    SL_AddHeadRcu(&list->free_list, node);
}


static inline void * FreeList_Get(FREE_LIST_S *list)
{
    return (void*) SL_DelHead(&list->free_list);
}

static inline BOOL_T FreeList_IsEmpty(FREE_LIST_S *list)
{
    return SL_IsEmpty(&list->free_list);
}

#define FreeList_ForEach(list, node) SL_FOREACH(&(list)->free_list, node)


#ifdef __cplusplus
}
#endif
#endif 
