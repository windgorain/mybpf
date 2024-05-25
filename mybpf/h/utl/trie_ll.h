/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _TRIE_LL_H
#define _TRIE_LL_H

#include "utl/list_sl.h"
#include "utl/trie_utl.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct trie_ll_node {
    TRIE_COMMON_S common;
    SL_NODE_S link_node;
    SL_HEAD_S next;
    UINT bits[8];
    UCHAR c;
}TRIE_LL_NODE_S;

typedef struct {
    TRIE_LL_NODE_S root;
}TRIE_LL_CTRL_S;


void Triell_Init(TRIE_LL_CTRL_S *ctrl);
void Triell_Fini(TRIE_LL_CTRL_S *ctrl, PF_TRIE_UD_FREE free_ud);

void * Triell_Create();
void Triell_Destroy(void *trie_ctrl, PF_TRIE_UD_FREE free_ud);

TRIE_COMMON_S * Triell_Insert(void *trie_ctrl, UCHAR *data, int data_len, UINT flag, void * ud);
TRIE_COMMON_S * Triell_PrefixMatch(void *trie_ctrl, UCHAR *data, int data_len, PF_TRIE_MATCH_CB func, void *ud);

#ifdef __cplusplus
}
#endif
#endif 
