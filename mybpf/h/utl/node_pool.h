/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _NODE_POOL_H
#define _NODE_POOL_H
#include "utl/list_sl.h"
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    SL_HEAD_S free_list;
}NODE_POOL_S;

int NodePool_Init(NODE_POOL_S *node_pool);
int NodePool_PutNodes(NODE_POOL_S *node_pool, void *nodes, UINT node_len,
        UINT max_nodes);
void * NodePool_Get(NODE_POOL_S *node_pool);
void NodePool_Free(NODE_POOL_S *node_pool, void *node);


#ifdef __cplusplus
}
#endif
#endif 
