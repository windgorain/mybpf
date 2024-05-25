/*================================================================
*   Created：2018.12.08 LiXingang All rights reserved.
*   Description：
*
================================================================*/
#ifndef __ADJ_UTL_H_
#define __ADJ_UTL_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    UINT nexthop;
}ADJ_NODE_S;

typedef struct {
    UINT size; 
    ADJ_NODE_S *array;
}ADJ_S;

int ADJ_Init(IN ADJ_S *adj, IN UINT size, ADJ_NODE_S *array);
int ADJ_Add(IN ADJ_S *adj, IN UINT nexthop);
int ADJ_Find(IN ADJ_S *adj, IN UINT nexthop);
void ADJ_DelByIndex(IN ADJ_S *adj, IN int index);
void ADJ_DelByNexthop(IN ADJ_S *adj, IN UINT nexthop);

#ifdef __cplusplus
}
#endif
#endif 
