/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _TRIE_UTL_H
#define _TRIE_UTL_H
#ifdef __cplusplus
extern "C"
{
#endif


#define TRIE_NODE_FLAG_MATCHED          0x1 
#define TRIE_NODE_FLAG_WILDCARD         0x2 

typedef HANDLE TRIE_HANDLE;

enum {
    TRIE_MATCH_EXACT=0,  
    TRIE_MATCH_MAXLEN,   
    TRIE_MATCH_WILDCARD, 
    TRIE_MATCH_HOSTNAME, 

    TRIE_MATCH_INVALID
};

enum {
    TRIE_TYPE_SIMPLE = 0, 
    TRIE_TYPE_LL,     
    TRIE_TYPE_DARRAY, 
    TRIE_TYPE_4BITS,  
    TRIE_TYPE_2BITS,  
};

typedef struct {
    void *ud;
    UINT flag;
}TRIE_COMMON_S;

typedef void (*PF_TRIE_UD_FREE)(void *ud);
typedef void* (*PF_TRIE_UD_SET)(void *ud, void *uh);


typedef TRIE_COMMON_S* (*PF_TRIE_MATCH_CB)(TRIE_COMMON_S *old, TRIE_COMMON_S *cur, int matched_len, void *ud);


TRIE_HANDLE Trie_Create(int type);
void Trie_Destroy(TRIE_HANDLE trie_handle, PF_TRIE_UD_FREE free_ud);
TRIE_COMMON_S * Trie_InsertEx(TRIE_HANDLE trie_handle, UCHAR *data, int data_len, UINT flag,
        PF_TRIE_UD_SET ud_set, void * uh);
TRIE_COMMON_S * Trie_Insert(TRIE_HANDLE trie_handle, UCHAR *data, int data_len, UINT flag, void * ud);

void * Trie_Del(TRIE_HANDLE trie_handle, UCHAR *data, int data_len);
TRIE_COMMON_S * Trie_PrefixMatch(TRIE_HANDLE trie_handle, UCHAR *data, int data_len, PF_TRIE_MATCH_CB func, void *ud);
TRIE_COMMON_S * Trie_Match(TRIE_HANDLE trie_handle, UCHAR *data, int data_len, int match_type);

#ifdef __cplusplus
}
#endif
#endif 
