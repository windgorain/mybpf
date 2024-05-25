/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _HEAVYKEEP_TOPN_H
#define _HEAVYKEEP_TOPN_H

#include "utl/box_utl.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define HEAVYKEEP_TOPN_HASH_NUM  4       
typedef unsigned int (*PF_HEAVYKEEP_TOPN_HASH_FUNC)(int level, void *key);

typedef struct {
    UINT64 freq;
    UINT64 score;
    UINT factor;
    UINT key_size;
    UCHAR key[0];
}HEAVYKEEP_TOPN_NODE_S;

typedef struct {
    int n; 
    int current_num; 
    HEAVYKEEP_TOPN_NODE_S *nodes[0];
}HEAVYKEEP_TOPN_RESULT_S;

typedef struct {
    UINT hash_size; 
    UINT key_size;
    UINT cycle;     
	UINT freq;
	UINT score;
    HEAVYKEEP_TOPN_NODE_S *hash;
}HEAVYKEEP_TOPN_S;

#define HEAVYKEEP_TOPN_RESULT_NODE_SIZE(_key_size) (sizeof(HEAVYKEEP_TOPN_NODE_S) + (_key_size))
#define HEAVYKEEP_TOPN_RESULT_MEM_SIZE(_n) (sizeof(HEAVYKEEP_TOPN_RESULT_S) + sizeof(HEAVYKEEP_TOPN_NODE_S*) * (_n))
#define HEAVYKEEP_TOPN_CTRL_MEM_SIZE(_key_size, _hash_size) ((sizeof(HEAVYKEEP_TOPN_NODE_S)+_key_size)*(_hash_size))

#define HEAVYKEEP_TOPN_RESULT_CURRENT_NUM(_result) ((_result)->current_num)

int HeavyKeep_Topn_Create(INOUT HEAVYKEEP_TOPN_S *topn, int hash_size, int key_size, int cycle);
void HeavyKeep_Topn_Destroy(HEAVYKEEP_TOPN_S *topn);
void HeavyKeep_Topn_Reset(HEAVYKEEP_TOPN_S *topn);
void HeavyKeep_Topn_Score(HEAVYKEEP_TOPN_S *topn, void *key, int key_len, UINT score);
VOID HeavyKeep_Topn_SetCycle(HEAVYKEEP_TOPN_S* topn, int cycle); 
int HeavyKeep_Topn_Get(HEAVYKEEP_TOPN_S *topn, OUT HEAVYKEEP_TOPN_RESULT_S *result);

#ifdef __cplusplus
}
#endif
#endif 
