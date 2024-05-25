/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _TRIE_SIMPLE_H
#define _TRIE_SIMPLE_H
#include "utl/trie_utl.h"
#ifdef __cplusplus
extern "C"
{
#endif

#define TRIE_CHAR_NUM 256

typedef struct trie_simple_node {
    TRIE_COMMON_S common;
    struct trie_simple_node *nexts[TRIE_CHAR_NUM];
}TRIE_SIMPLE_NODE_S;

typedef struct {
    TRIE_SIMPLE_NODE_S root;
}TRIE_SIMPLE_CTRL_S;

void TrieSimple_Init(TRIE_SIMPLE_CTRL_S *ctrl);
void TrieSimple_Fini(TRIE_SIMPLE_CTRL_S *ctrl, PF_TRIE_UD_FREE free_ud);

void * TrieSimple_Create();
void TrieSimple_Destroy(void *trie_ctrl, PF_TRIE_UD_FREE free_ud);

TRIE_COMMON_S * TrieSimple_Insert(void *trie_ctrl, UCHAR *data, int data_len, UINT flag, void * ud);
TRIE_COMMON_S * TrieSimple_PrefixMatch(void *trie_ctrl, UCHAR *data, int data_len, PF_TRIE_MATCH_CB func, void *ud);


#ifdef __cplusplus
}
#endif
#endif 
