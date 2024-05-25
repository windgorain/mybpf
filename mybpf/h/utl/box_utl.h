/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _BOX_UTL_H
#define _BOX_UTL_H
#include "utl/cuckoo_hash.h"
#include "utl/net.h"
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    CUCKOO_HASH_S hash;
}BOX_S;

typedef void (*PF_BOX_FREE_DATA)(BOX_S *box, void *data, void *ud);

static inline BOX_S * Box_GetBoxByHash(void *hash)
{
    BOX_S *box = container_of(hash, BOX_S, hash);
    return box;
}

static inline int Box_Add(BOX_S *box, void *data)
{
    return CuckooHash_Add(&box->hash, data);
}

static inline int Box_Find(BOX_S *box, void *data)
{
    return CuckooHash_Find(&box->hash, data);
}

static inline void * Box_FindData(BOX_S *box, void *data)
{
    return CuckooHash_FindData(&box->hash, data);
}

static inline void * Box_Del(BOX_S *box, void *data)
{
    return CuckooHash_Del(&box->hash, data);
}

static inline void * Box_DelByIndex(BOX_S *box, int index)
{
    return CuckooHash_DelIndex(&box->hash, index);
}

static inline void * Box_GetData(BOX_S *box, int index)
{
    return CuckooHash_GetData(&box->hash, index);
}

static inline int Box_GetNext(BOX_S *box, int index)
{
    return CuckooHash_GetNext(&box->hash, index);
}

static inline int Box_Count(BOX_S *box)
{
    return box->hash.entry_count;
}

static inline void Box_DelAll(BOX_S *box, PF_BOX_FREE_DATA free_data, void *ud) 
{
    int index = -1;

    while(-1 != (index = Box_GetNext(box, index))) {
        void * data = Box_DelByIndex(box, index);
        if (free_data) {
            free_data(box, data, ud);
        }
    }
}

static inline void Box_Fini(BOX_S *box, PF_BOX_FREE_DATA free_data, void *ud)
{
    if (free_data) {
        Box_DelAll(box, free_data, ud);
    }
    CuckooHash_Fini(&box->hash);
}

static inline UINT Box_GetKeyHash(BOX_S *box, int index)
{
    return CuckooHash_GetKeyFactor(&box->hash, index);
}


int IntBox_Init(BOX_S *box, CUCKOO_HASH_NODE_S *table,
        UINT bucket_num, UINT bucket_depth);
void IntBox_Show(BOX_S *box);


int IDBox_Init(BOX_S *box, CUCKOO_HASH_NODE_S *table,
        UINT bucket_num, UINT bucket_depth);


int StrBox_Init(BOX_S *box, CUCKOO_HASH_NODE_S *table,
        UINT bucket_num, UINT bucket_depth);
void StrBox_Show(BOX_S *box);


typedef union {
    union {
        UINT ip4;
        UCHAR aucIP[4];
    };
    UINT ip6[4];
}IP_ADDR46_U;

typedef struct {
    UCHAR family;
    UCHAR protocol;
    UCHAR reserved1;
    UCHAR reserved2;
    IP_ADDR46_U ip[2];
    USHORT port[2];
    UINT tun_id;
}IP_TUP_KEY_S;

int IPTupBox_Init(BOX_S *box, CUCKOO_HASH_NODE_S *table,
        UINT bucket_num, UINT bucket_depth);


typedef struct {
    UCHAR protocol;
    UCHAR reserved;
    USHORT port;
    UINT ip;
}IP_TUP3_KEY_S;

int IPTup3Box_Init(BOX_S *box, CUCKOO_HASH_NODE_S *table,
        UINT bucket_num, UINT bucket_depth);


int DataBox_Init(BOX_S *box, CUCKOO_HASH_NODE_S *table,
        UINT bucket_num, UINT bucket_depth);


int RawBox_Init(BOX_S *box, CUCKOO_HASH_NODE_S *table,
        UINT bucket_num, UINT bucket_depth,
        PF_CUCKOO_INDEX index_func, PF_CUCKOO_CMP cmp_func);

#ifdef __cplusplus
}
#endif
#endif 
