/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _IP_MASK_TBL_H
#define _IP_MASK_TBL_H

#include "utl/hash_utl.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    HASH_NODE_S stHashNode;
    UINT ip; 
    UCHAR depth;
}IPMASKTBL_NODE_S;

typedef struct
{
    HASH_S *hHash;
}IPMASKTBL_S;

typedef struct {
    UINT bf[1024*1024*2]; 
}IPMASKTBL_BF_S;

typedef int (*PF_IPMASKTBL_WALK_FUNC)(IN IPMASKTBL_NODE_S *pstIpMaskTblNode, IN HANDLE hUserHandle);
typedef void (*PF_IPMASKTBL_MERGE_USER_DATA)(IPMASKTBL_NODE_S *node_to, IPMASKTBL_NODE_S *node_from);

int IPMASKTBL_Init(IN IPMASKTBL_S *ipmasktbl);
VOID IPMASKTBL_Fini(IN IPMASKTBL_S *ipmasktbl, PF_HASH_FREE_FUNC pfFree, void *user_handle);
BS_STATUS IPMASKTBL_Add(IN IPMASKTBL_S *ipmasktbl, IN IPMASKTBL_NODE_S *pstIpMaskTblNode);
IPMASKTBL_NODE_S * IPMASKTBL_Get(IN IPMASKTBL_S *ipmasktbl, UINT ip, UCHAR depth);
VOID IPMASKTBL_Del(IN IPMASKTBL_S *ipmasktbl, IN IPMASKTBL_NODE_S *pstNode);
VOID IPMASKTBL_DelAll(IN IPMASKTBL_S *ipmasktbl, PF_HASH_FREE_FUNC pfFree, void *user_handle);
IPMASKTBL_NODE_S * IPMASKTBL_Match(IN IPMASKTBL_S *ipmasktbl, IN UINT uiDstIp );
IPMASKTBL_NODE_S * IPMASKTBL_BfMatch(IN IPMASKTBL_S *ipmasktbl, IPMASKTBL_BF_S *ipmasktbl_bf, IN UINT uiDstIp );
VOID IPMASKTBL_Walk(IN IPMASKTBL_S *ipmasktbl, IN PF_IPMASKTBL_WALK_FUNC pfWalkFunc, IN HANDLE hUserHandle);

IPMASKTBL_NODE_S * IPMASKTBL_GetNext( IN IPMASKTBL_S *ipmasktbl, IN IPMASKTBL_NODE_S *pstFibCurrent);

void IPMASKTBL_MergeUserData(IPMASKTBL_S *ipmasktbl, PF_IPMASKTBL_MERGE_USER_DATA pfFunc);

void IPMASKTBL_BfInit(IPMASKTBL_BF_S *ipmasktbl_bf);
void IPMASKTBL_BfSet(IPMASKTBL_BF_S *ipmasktbl_bf, UINT ip, UCHAR depth);
int IPMASKTBL_BfTest(IPMASKTBL_BF_S *ipmasktbl_bf, UINT ip, UCHAR depth);

#ifdef __cplusplus
}
#endif
#endif 
