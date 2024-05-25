/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _CONSISTENT_HASH_H
#define _CONSISTENT_HASH_H
#include "utl/consistent_hash_def.h"
#ifdef __cplusplus
extern "C"
{
#endif

int ConsistentHash_FindBigger(CONSISTENT_HASH_S *ctrl, int start, int end, UINT key);
int ConsistentHash_AddNode(CONSISTENT_HASH_S *ctrl, UINT key, UINT data);
void ConsistentHash_DelByData(CONSISTENT_HASH_S *ctrl, UINT data);

#ifdef __cplusplus
}
#endif
#endif 
