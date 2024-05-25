/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _CUCKOO_HASH_H
#define _CUCKOO_HASH_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef int (*PF_CUCKOO_CMP)(void *hash, void *data1, void *data2);
typedef UINT (*PF_CUCKOO_INDEX)(void *hash, void *data); 

typedef struct {
    UINT used:1;
    UINT key_factor;
    void *data;
}CUCKOO_HASH_NODE_S;

typedef struct {
    UINT bucket_mask;
    UINT bucket_depth_mask;
    int bucket_num;
    int bucket_depth;
    int entry_count;
    PF_CUCKOO_CMP cmp_func;
    PF_CUCKOO_INDEX index_func;
    CUCKOO_HASH_NODE_S *table;
    UINT table_alloced:1;
}CUCKOO_HASH_S;

int CuckooHash_Init(CUCKOO_HASH_S *cuckoo_hash, CUCKOO_HASH_NODE_S *table,
        UINT bucket_num, UINT bucket_depth);
void CuckooHash_Fini(CUCKOO_HASH_S *hash);
void CuckooHash_Reinit(CUCKOO_HASH_S *cuckoo_hash);
void CuckooHash_SetIndexFunc(CUCKOO_HASH_S *hash, PF_CUCKOO_INDEX index_func);
void CuckooHash_SetCmpFunc(CUCKOO_HASH_S *hash, PF_CUCKOO_CMP cmp_func);

int CuckooHash_Find(CUCKOO_HASH_S *hash, void *data);
void * CuckooHash_FindData(CUCKOO_HASH_S *hash, void *data);
int CuckooHash_Add(CUCKOO_HASH_S *hash, void *data);
void * CuckooHash_DelIndex(CUCKOO_HASH_S *hash, int index);
void * CuckooHash_Del(CUCKOO_HASH_S *hash, void *data);
void * CuckooHash_GetData(CUCKOO_HASH_S *hash, int index);

int CuckooHash_GetNext(CUCKOO_HASH_S *hash, int curr);

int CuckooHash_Count(CUCKOO_HASH_S *hash);
UINT CuckooHash_GetKeyFactor(CUCKOO_HASH_S *hash, int index);

#ifdef __cplusplus
}
#endif
#endif 
