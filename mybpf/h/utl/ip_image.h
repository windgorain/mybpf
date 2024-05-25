/*================================================================
* Author：LiXingang. Data: 2019.07.25
* Description：
*
================================================================*/
#ifndef _IP_IMAGE_H
#define _IP_IMAGE_H

#include "utl/idkey_utl.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef HANDLE IPIMG_HANDLE;

#define IPIMG_MAX_ID_PER_IP       32

typedef struct {
    UINT64 id;
    UINT64 count;
}IPIMG_ID_NODE_S;

typedef struct {
    UINT count;
    IPIMG_ID_NODE_S node[IPIMG_MAX_ID_PER_IP];
}IPIMG_ID_S;

typedef struct {
    UINT ip;
    IPIMG_ID_S propertys[0];
}IPIMG_IP_ID_S;

typedef void (*PF_IPIMG_WALK_IP)(IPIMG_IP_ID_S *ipid, void *ud);


IPIMG_HANDLE IPIMG_Create(int max_property);
void IPIMG_Destroy(IPIMG_HANDLE hIpImg);
void IPIMG_Reset(IPIMG_HANDLE hIpImg);

void IPIMG_SetIDCapacity(IPIMG_HANDLE hIpImg, UINT64 capacity);

void IPIMG_SetIPCapacity(IPIMG_HANDLE hIpImg, UINT capacity);
int IPIMG_Add(IPIMG_HANDLE ipimg_handle, UINT ip, int property, void *value, int value_len);
IDKEY_KL_S * IPIMG_GetKey(IPIMG_HANDLE ipimg_handle, UINT64 id);
void IPIMG_Walk(IPIMG_HANDLE ipimg_handle, PF_IPIMG_WALK_IP walk_func, void *ud);

#ifdef __cplusplus
}
#endif
#endif 
