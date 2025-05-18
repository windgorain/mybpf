/*================================================================
*   Created by LiXingang, Copyright LiXingang
*   Description: user map
*
================================================================*/
#include "bs.h"
#include "utl/mem_inline.h"
#include "utl/hash_utl.h"
#include "utl/jhash_utl.h"
#include "utl/ufd_utl.h"
#include "utl/umap_utl.h"

typedef struct {
    UMAP_HEADER_S hdr; 
    U32 buckets_num;
    HASH_S * hash_tbl;
}UMAP_HASH_S;

typedef struct {
    HASH_NODE_S hash_node; 
    RCU_NODE_S rcu_node;
    UMAP_HASH_S *ctrl;
    void *key;
    void *value;
}UMAP_HASH_NODE_S;

static UINT _umap_hash_hash_index(void *node, void *ud)
{
    UMAP_HASH_NODE_S *n = node;
    return JHASH_GeneralBuffer(n->key, n->ctrl->hdr.size_key, 0);
}

static int _umap_hash_cmp(const void *key, const void *node, const void *ud)
{
    const UMAP_HASH_NODE_S *n = node;
    const UMAP_HASH_NODE_S *k = key;

    if (! k) {
        return -1;
    }

    return memcmp(k->key, n->key, k->ctrl->hdr.size_key);
}

static void _umap_hash_free_node_rcu(void *pstRcuNode)
{
    UMAP_HASH_NODE_S *node = container_of(pstRcuNode, UMAP_HASH_NODE_S, rcu_node);

    if (node->key) {
        MEM_KFree(node->key);
        node->key = NULL;
    }

    if (node->value) {
        MEM_KFree(node->value);
        node->value = NULL;
    }

    MEM_KFree(node);
}

static void _umap_hash_free_node(UMAP_HASH_NODE_S *node)
{
    RcuEngine_Call(&node->rcu_node, _umap_hash_free_node_rcu);
}

static void _umap_hash_hash_free_node(void *hash_tbl, void *node, void *ud)
{
    _umap_hash_free_node(node);
}

static void _umap_hash_destroy_map(void *map)
{
    UMAP_HASH_S *ctrl = map;

    HASH_DelAll(ctrl->hash_tbl, _umap_hash_hash_free_node, NULL);
    HASH_DestoryInstance(ctrl->hash_tbl);
    MEM_Free(ctrl); 
}

static void * _umap_hash_open(void *map_def)
{
    UMAP_ELF_MAP_S *elfmap = map_def;

    if ((! elfmap) || (elfmap->max_elem == 0) || (elfmap->size_key == 0)) {
		return NULL;
    }

    UMAP_HASH_S *ctrl = MEM_ZMalloc(sizeof(UMAP_HASH_S));
    if (! ctrl) {
        return NULL;
    }

    ctrl->buckets_num = NUM_To2N(elfmap->max_elem);

    ctrl->hash_tbl = HASH_CreateInstance(RcuEngine_GetMemcap(), ctrl->buckets_num, _umap_hash_hash_index);
    if (! ctrl->hash_tbl) {
        MEM_Free(ctrl);
        return NULL;
    }

    return ctrl;
}

static UMAP_HASH_NODE_S * _umap_hash_find_node(void *map, const void *key)
{
    UMAP_HASH_S *ctrl = map;
    UMAP_HASH_NODE_S node;

    if ((!map) || (!key)) {
        return NULL;
    }

    node.ctrl = ctrl;
    node.key = (void*)key;

    return HASH_FindNode(ctrl->hash_tbl, _umap_hash_cmp, &node, NULL);
}

static void * _umap_hash_lookup_elem(void *map, const void *key)
{
    UMAP_HASH_NODE_S *found = _umap_hash_find_node(map, key);
    if (! found) {
        return NULL;
    }
    return found->value;
}

static long _umap_hash_delete_elem(void *map, const void *key)
{
    UMAP_HASH_S *ctrl = map;
    UMAP_HASH_NODE_S *old;

    old = _umap_hash_find_node(map, key);
    if (! old) {
        return -ENOENT;
    }

    HASH_Del(ctrl->hash_tbl, old);

    _umap_hash_free_node(old);

    return 0;
}

static long _umap_hash_update_elem(void *map, const void *key, const void *value, U32 flag)
{
    UMAP_HASH_S *ctrl = map;
    UMAP_HASH_NODE_S *node;
    UMAP_HASH_NODE_S *old;

    if ((!map) || (!key) || (!value)){
		return -EINVAL;
    }

    old = _umap_hash_find_node(map, key);

    if ((flag == UMAP_UPDATE_EXIST) && (! old)){
		return -ENOENT;
    }

    if ((flag == UMAP_UPDATE_NOEXIST) && (old)){
		return -EEXIST;
    }

    node = MEM_ZKalloc(sizeof(UMAP_HASH_NODE_S));
    if (! node) {
        return -ENOMEM;
    }

    node->ctrl = ctrl;
    node->key = MEM_KDup((char*)key, ctrl->hdr.size_key);
    node->value = MEM_KDup((char*)value, ctrl->hdr.size_value);

    if ((!node->key) || (!node->value)) {
        _umap_hash_free_node(node);
        return -ENOMEM;
    }

    HASH_Add(ctrl->hash_tbl, node);

    if (old) {
        HASH_Del(ctrl->hash_tbl, old);
        _umap_hash_free_node(old);
    }

    return 0;
}


static int _umap_hash_getnext_key(void *map, void *key, OUT void *next_key)
{
    UMAP_HASH_S *ctrl = map;
    UMAP_HASH_NODE_S *found;
    UMAP_HASH_NODE_S *tmp = NULL;
    UMAP_HASH_NODE_S node;

    if (! map) {
        return -1;
    }

    if (key) {
        node.ctrl = ctrl;
        node.key = key;
        tmp = &node;
    }

    found = (void*)HASH_GetNextDict(ctrl->hash_tbl, _umap_hash_cmp, (void*)tmp, NULL);
    if (! found) {
        return -1;
    }

    memcpy(next_key, found->key, ctrl->hdr.size_key);

    return 0;
}

UMAP_FUNC_TBL_S g_umap_hash_ops = {
    .open_func = _umap_hash_open,
    .destroy_func = _umap_hash_destroy_map,
    .lookup_elem_func = _umap_hash_lookup_elem,
    .delete_elem_func = _umap_hash_delete_elem,
    .update_elem_func = _umap_hash_update_elem,
    .get_next_key_func = _umap_hash_getnext_key,
    .direct_value_func = NULL,
};


