/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _TRIE_2BITS_H
#define _TRIE_2BITS_H
#include "utl/trie_utl.h"
#ifdef __cplusplus
extern "C"
{
#endif

#define TRIE2B_NUM 4

typedef struct trie2b_node {
    TRIE_COMMON_S common;
    struct trie2b_node *nexts[TRIE2B_NUM];
}TRIE_2BITS_NODE_S;

typedef struct {
    TRIE_2BITS_NODE_S root;
}TRIE_2BITS_CTRL_S;

void Trie2b_Init(TRIE_2BITS_CTRL_S *ctrl);
void Trie2b_Fini(TRIE_2BITS_CTRL_S *ctrl, PF_TRIE_UD_FREE free_ud);

void * Trie2b_Create();
void Trie2b_Destroy(void *trie_ctrl, PF_TRIE_UD_FREE free_ud);

TRIE_COMMON_S * Trie2b_Insert(void *trie_ctrl, UCHAR *data, int data_len, UINT flag, void * ud);
TRIE_COMMON_S * Trie2b_PrefixMatch(void *trie_ctrl, UCHAR *data, int data_len, PF_TRIE_MATCH_CB func, void *ud);


#ifdef __cplusplus
}
#endif
#endif 
