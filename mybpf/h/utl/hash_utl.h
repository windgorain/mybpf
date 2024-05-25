/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2008-9-20
* Description: 
* History:     
******************************************************************************/

#ifndef __HASH_UTL_H_
#define __HASH_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

typedef UINT (*PF_HASH_INDEX_FUNC)(void *pstHashNode); 

typedef struct {
    U32 bucket_num;
    U32 mask;
    U32 uiNodeCount;   
    void *memcap;
    PF_HASH_INDEX_FUNC pfHashIndexFunc;
    DLL_HEAD_S *pstBuckets;
}HASH_S;

typedef struct {
    DLL_NODE_S stNode;
    UINT hash_factor;
}HASH_NODE_S;

typedef INT  (*PF_HASH_CMP_FUNC)(void *pstHashNode, void *pstNodeToFind);
typedef int (*PF_HASH_WALK_FUNC)(void *hashtbl, void *pstNode, void *pUserHandle);
typedef VOID  (*PF_HASH_FREE_FUNC)(void *hashtbl, void *pstHashNode, void *pUserHandle);

void HASH_Init(OUT HASH_S *hash, DLL_HEAD_S *buckets, U32 bucket_num, PF_HASH_INDEX_FUNC pfFunc);
HASH_S * HASH_CreateInstance(void *memcap, U32 ulHashBucketNum, PF_HASH_INDEX_FUNC pfFunc);
void HASH_DestoryInstance(HASH_S *hashtbl);
void HASH_AddWithFactor(HASH_S *hashtbl, void *pstNode, U32 hash_factor);
void HASH_Add(HASH_S *hashtbl, void *pstNode);
void HASH_Del(HASH_S *hashtbl, void *pstNode);
void HASH_DelAll(HASH_S *hashtbl, PF_HASH_FREE_FUNC pfFreeFunc, void *pUserHandle);
void * HASH_FindWithFactor(HASH_S *hashtbl, U32 hash_factor, PF_HASH_CMP_FUNC pfCmpFunc, void *pstNodeToFind);
void * HASH_Find(HASH_S *hashtbl, PF_HASH_CMP_FUNC pfCmpFunc, void *pstNodeToFind);
U32 HASH_Count(HASH_S *hashtbl);
void HASH_Walk(HASH_S *hashtbl, PF_HASH_WALK_FUNC pfWalkFunc, void *pUserHandle);

HASH_NODE_S * HASH_GetNext(HASH_S *hashtbl, HASH_NODE_S *curr_node );

HASH_NODE_S * HASH_GetNextDict(HASH_S *hashtbl, PF_HASH_CMP_FUNC pfCmpFunc, HASH_NODE_S *curr_node );

#ifdef __cplusplus
    }
#endif 

#endif 


