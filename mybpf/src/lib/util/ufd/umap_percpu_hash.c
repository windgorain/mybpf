/******************************************************************
*   Created by LiXingang
*   Description: 
*
******************************************************************/
#include "bs.h"
#include "utl/umap_utl.h"
#include "utl/list_dtq_rcu.h"
#include "utl/jhash_utl.h"
#include "utl/percpu_value.h"

typedef struct {
    SPINLOCK_S lock;
    DTQ_HEAD_S list;
}MAP_PERCPU_HASH_LIST_S;

typedef struct {
    UMAP_HEADER_S hdr; 
    int bucket_num;
    MAP_PERCPU_HASH_LIST_S *buckets;
}MAP_PERCPU_HASH_S;

typedef struct {
    DTQ_NODE_S list_node; 
    RCU_NODE_S rcu;
    void *key;
    PERCPU_VALUE_S pcpu_value;
}MAP_PERCPU_HASH_NODE_S;

static int _map_percpu_hash_cmp(const void *key, const void *node, const void *ud)
{
    const MAP_PERCPU_HASH_S *ctrl = ud;
    const MAP_PERCPU_HASH_NODE_S *n = node;
    int size_key = ctrl->hdr.size_key;
    return memcmp(key, n->key, size_key);
}

static MAP_PERCPU_HASH_NODE_S * _map_percpu_hash_lookup_node(MAP_PERCPU_HASH_S *ctrl, const void *key, U32 hash)
{
    U32 index;
    index = hash & (ctrl->bucket_num - 1);
    return (void*)DTQ_Find(&ctrl->buckets[index].list, key, _map_percpu_hash_cmp, ctrl);
}

static void _map_percpu_hash_free_node_rcu(void *head)
{
    MAP_PERCPU_HASH_NODE_S *node = container_of(head, MAP_PERCPU_HASH_NODE_S, rcu); 
    PERCPU_Destroy(&node->pcpu_value);
    MEM_KFree(node);
}

static inline void _map_percpu_hash_free_node_just(void *n)
{
    MAP_PERCPU_HASH_NODE_S *node = n;
    PERCPU_Destroy(&node->pcpu_value);
    MEM_KFree(node);
}

static inline void _map_percpu_hash_free_node(void *n)
{
    MAP_PERCPU_HASH_NODE_S *node = n;
    RcuEngine_Call(&node->rcu, _map_percpu_hash_free_node_rcu);
}

static MAP_PERCPU_HASH_NODE_S * _map_percpu_hash_first_node(MAP_PERCPU_HASH_S *ctrl)
{
    int i;
    MAP_PERCPU_HASH_NODE_S *node;

    for (i=0; i<ctrl->bucket_num; i++) {
        node = (void*)DTQ_First(&ctrl->buckets[i].list);
        if (node) {
            return node;
        }
    }

    return NULL;
}

static MAP_PERCPU_HASH_NODE_S * _map_percpu_hash_next_node(MAP_PERCPU_HASH_S *ctrl, U32 cur_hash, MAP_PERCPU_HASH_NODE_S *cur)
{
    int i;
    U32 index;
    MAP_PERCPU_HASH_NODE_S *node;

    index = cur_hash & (ctrl->bucket_num - 1);

    node = (void*)DTQ_GetNext(&ctrl->buckets[index].list, &cur->list_node);
    if (node) {
        return node;
    }

    for (i=index+1; i<ctrl->bucket_num; i++) {
        node = (void*)DTQ_First(&ctrl->buckets[i].list);
        if (node) {
            return node;
        }
    }

    return NULL;
}

static long _map_percpu_hash_update_elem_locked(MAP_PERCPU_HASH_S *ctrl, const void *key, const void *value, U32 flag, U32 hash)
{
    MAP_PERCPU_HASH_NODE_S *node;
    MAP_PERCPU_HASH_NODE_S *old = NULL;
    int len;
    U32 index;
    void *v;

    old = _map_percpu_hash_lookup_node(ctrl, key, hash);

    if ((flag == UMAP_UPDATE_EXIST) && (! old)){
		return -ENOENT;
    }

    if ((flag == UMAP_UPDATE_NOEXIST) && (old)){
		return -EEXIST;
    }

    if (old) {
        v = PERCPU_Get(&old->pcpu_value);
        if (! v) {
            return -ENOMEM;
        }
        memcpy(v, value, ctrl->hdr.size_value);
        return 0;
    }

    len = sizeof(MAP_PERCPU_HASH_NODE_S) + ctrl->hdr.size_key;

    node = MEM_Kalloc(len);
    if (! node) {
        return -ENOMEM;
    }

    memset(node, 0, len);

    node->key = (node + 1);
    memcpy(node->key, key, ctrl->hdr.size_key);

    PERCPU_Init(&node->pcpu_value, ctrl->hdr.size_value);

    v = PERCPU_Get(&node->pcpu_value);
    if (! v) {
        return -ENOMEM;
    }

    memcpy(v, value, ctrl->hdr.size_value);

    index = hash & (ctrl->bucket_num - 1);
    DTQ_AddTailRCU(&ctrl->buckets[index].list, &node->list_node);

    return 0;
}

static void * _map_percpu_hash_open(void *map_def)
{
    UMAP_ELF_MAP_S *map = map_def;
    int len;
    int i;

    if ((! map) || (map->max_elem == 0) || (map->size_key == 0)) {
		return NULL;
    }

    len = sizeof(MAP_PERCPU_HASH_S);

    MAP_PERCPU_HASH_S *mem = MEM_ZMalloc(len);
    if (! mem) {
        return NULL;
    }

    memset(mem, 0, len);

    mem->bucket_num = NUM_To2N(map->max_elem);
    if (mem->bucket_num > 1024) {
        mem->bucket_num = 1024;
    }

    len = sizeof(MAP_PERCPU_HASH_LIST_S) * mem->bucket_num;
    mem->buckets = MEM_ZMalloc(len);
    if (! mem->buckets) {
        MEM_Free(mem);
        return NULL;
    }

    memset(mem->buckets, 0, len);

    for (i=0; i<mem->bucket_num; i++) {
        DTQ_Init(&mem->buckets[i].list);
    }

    return mem;
}

static void _map_percpu_hash_destroy_map(void *map)
{
    int i;
    MAP_PERCPU_HASH_S *ctrl = map;

    
    for (i=0; i<ctrl->bucket_num; i++) {
        DTQ_FreeAll(&ctrl->buckets[i].list, _map_percpu_hash_free_node_just);
    }

    MEM_Free(ctrl->buckets);
    MEM_Free(ctrl);
}

static void * _map_percpu_hash_lookup_elem(void *map, const void *key)
{
    MAP_PERCPU_HASH_S *ctrl = map;
    MAP_PERCPU_HASH_NODE_S *found;
    U32 hash;

    if ((!map) || (!key)) {
        return NULL;
    }

    hash = JHASH_Buffer(key, ctrl->hdr.size_key, 0);

    found = _map_percpu_hash_lookup_node(map, key, hash);
    if (! found) {
        return NULL;
    }

    return PERCPU_Get(&found->pcpu_value);
}

static long _map_percpu_hash_delete_elem(void *map, const void *key)
{
    MAP_PERCPU_HASH_S *ctrl = map;
    MAP_PERCPU_HASH_NODE_S *old;
    U32 hash;
    U32 index;

    hash = JHASH_Buffer(key, ctrl->hdr.size_key, 0);
    index = hash & (ctrl->bucket_num - 1);

    SpinLock_Lock(&ctrl->buckets[index].lock);

    old = _map_percpu_hash_lookup_node(map, key, hash);
    if (old) {
        DTQ_Del(&old->list_node);
    }

    SpinLock_UnLock(&ctrl->buckets[index].lock);

    if (! old) {
        return -ENOENT;
    } 

    _map_percpu_hash_free_node(old);

    return 0;
}

static long _map_percpu_hash_update_elem(void *map, const void *key, const void *value, U32 flag)
{
    MAP_PERCPU_HASH_S *ctrl = map;
    int ret;
    U32 hash;
    U32 index;

    if ((!map) || (!key) || (!value)){
		return -EINVAL;
    }

    hash = JHASH_Buffer(key, ctrl->hdr.size_key, 0);
    index = hash & (ctrl->bucket_num - 1);

    SpinLock_Lock(&ctrl->buckets[index].lock);
    ret = _map_percpu_hash_update_elem_locked(ctrl, key, value, flag, hash);
    SpinLock_UnLock(&ctrl->buckets[index].lock);

    return ret;
}

static int _map_percpu_hash_get_next_key(void *map, void *key, OUT void *next_key)
{
    MAP_PERCPU_HASH_S *ctrl = map;
    MAP_PERCPU_HASH_NODE_S *node;
    U32 hash;

    hash = JHASH_Buffer(key, ctrl->hdr.size_key, 0);
    node = _map_percpu_hash_lookup_node(map, key, hash);
    if (node) {
        node = _map_percpu_hash_next_node(map, hash, node);
    } else {
        node = _map_percpu_hash_first_node(map);
    }

    if (! node) {
        return -1;
    }

    memcpy(next_key, node->key, ctrl->hdr.size_key);

    return 0;
}

UMAP_FUNC_TBL_S g_umap_percpu_hash_ops = {
    .open_func = _map_percpu_hash_open,
    .destroy_func = _map_percpu_hash_destroy_map,
    .lookup_elem_func = _map_percpu_hash_lookup_elem,
    .delete_elem_func = _map_percpu_hash_delete_elem,
    .update_elem_func = _map_percpu_hash_update_elem,
    .get_next_key_func = _map_percpu_hash_get_next_key,
    .direct_value_func = NULL,
};

