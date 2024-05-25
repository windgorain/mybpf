/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _TRIE_DARRAY_H
#define _TRIE_DARRAY_H
#include "utl/darray_utl.h"
#include "utl/trie_utl.h"
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct trie_darray_node {
    TRIE_COMMON_S common;
    UINT bits[8];
    DARRAY_HANDLE nexts;
    UCHAR c;
}TRIE_DARRAY_NODE_S;

typedef struct {
    TRIE_DARRAY_NODE_S root;
}TRIE_DARRAY_CTRL_S;


void Triedarray_Init(TRIE_DARRAY_CTRL_S *ctrl);
void Triedarray_Fini(TRIE_DARRAY_CTRL_S *ctrl, PF_TRIE_UD_FREE free_ud);

void * Triedarray_Create();
void Triedarray_Destroy(void *trie_ctrl, PF_TRIE_UD_FREE free_ud);

TRIE_COMMON_S * Triedarray_Insert(void *trie_ctrl, UCHAR *data, int data_len, UINT flag, void * ud);
TRIE_COMMON_S * Triedarray_PrefixMatch(void *trie_ctrl, UCHAR *data, int data_len, PF_TRIE_MATCH_CB func, void *ud);




#ifdef __cplusplus
}
#endif
#endif 
