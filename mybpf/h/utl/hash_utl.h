/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2008-9-20
* Description: 
* History:     
******************************************************************************/

#ifndef __HASH_UTL_H_
#define __HASH_UTL_H_

#include "utl/list_dl.h"

#ifdef __cplusplus
    extern "C" {
#endif 

typedef UINT (*PF_HASH_BY_NODE)(void *node, void *hash); 
typedef UINT (*PF_HASH_BY_KEY)(void *key, void *ud); 

typedef struct {
    U32 bucket_num;
    U32 mask;
    U32 uiNodeCount;   
    void *memcap;
    void *user_data;
    PF_HASH_BY_NODE hash_by_node;
    DL_HEAD_S *pstBuckets;
}HASH_S;

typedef struct {
    DL_NODE_S stNode; 
    UINT hash_factor;
}HASH_NODE_S;

typedef int (*PF_HASH_WALK_FUNC)(void *hashtbl, void *pstNode, void *pUserHandle);
typedef VOID  (*PF_HASH_FREE_FUNC)(void *hashtbl, void *pstHashNode, void *pUserHandle);

void HASH_Init(OUT HASH_S *hash, DL_HEAD_S *buckets, U32 bucket_num, PF_HASH_BY_NODE hash_by_node);
HASH_S * HASH_CreateInstance(void *memcap, UINT bucket_num, PF_HASH_BY_NODE hash_by_node);
void HASH_DestoryInstance(HASH_S *hashtbl);
void HASH_AddWithFactor(HASH_S *hashtbl, void *pstNode, U32 hash_factor);
void HASH_Add(HASH_S *hashtbl, void *pstNode);
void HASH_Del(HASH_S *hashtbl, void *pstNode);
void HASH_DelAll(HASH_S *hashtbl, PF_HASH_FREE_FUNC pfFreeFunc, void *pUserHandle);
void * HASH_FindWithFactor(HASH_S *hash, U32 hash_factor, PF_CMP_EXT_FUNC cmp_func, void *key, void *ud);
void * HASH_FindKey(HASH_S *hash, PF_CMP_EXT_FUNC cmp_func, PF_HASH_BY_KEY hash_by_key, void *key, void *ud);
void * HASH_FindNode(HASH_S *hash, PF_CMP_EXT_FUNC cmp_func, void *node, void *ud);
U32 HASH_Count(HASH_S *hashtbl);
void HASH_Walk(HASH_S * hHashId, PF_HASH_WALK_FUNC pfWalkFunc, void *ud);

HASH_NODE_S * HASH_GetNext(HASH_S *hashtbl, HASH_NODE_S *curr_node );
HASH_NODE_S * HASH_GetNextDict(HASH_S * hHash, PF_CMP_EXT_FUNC pfCmpFunc, HASH_NODE_S *curr_node, void *ud);

static inline void HASH_SetUserData(HASH_S *hash, void *user_data) 
{
    hash->user_data = user_data;
}

static inline void * HASH_GetUserData(HASH_S *hash)
{
    return hash->user_data;
}

#define HASH_SCAN_BEGIN(_hash, _node) do { \
    int _i; \
    DL_NODE_S *_cur, *_tmp; \
    for (_i=0; _i<=(_hash)->mask; _i++) { \
        DL_FOREACH_SAFE(&(_hash)->pstBuckets[_i], _cur, _tmp) { (_node) = (void*)_cur; {

#define HASH_SCAN_END() }}}} while(0)

#ifdef __cplusplus
    }
#endif 

#endif 


