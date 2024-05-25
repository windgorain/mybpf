/*================================================================
*   Created by LiXingang, Copyright LiXingang
*   Description: user map
*
================================================================*/
#include "bs.h"
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
    UMAP_HASH_S *ctrl;
    void *key;
    void *value;
}UMAP_HASH_NODE_S;

static UINT _umap_hash_hash_index(void *node)
{
    UMAP_HASH_NODE_S *n = node;
    return JHASH_GeneralBuffer(n->key, n->ctrl->hdr.size_key, 0);
}

static int _umap_hash_cmp(void *node, void *key)
{
    UMAP_HASH_NODE_S *n = node;
    UMAP_HASH_NODE_S *k = key;

    return memcmp(n->key, k->key, k->ctrl->hdr.size_key);
}

static void _umap_hash_free_node(UMAP_HASH_NODE_S *node)
{
    if (node->key) {
        MEM_RcuFree(node->key);
        node->key = NULL;
    }
    if (node->value) {
        MEM_RcuFree(node->value);
        node->value = NULL;
    }
    MEM_RcuFree(node);
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
        MEM_RcuFree(ctrl);
        return NULL;
    }

    return ctrl;
}

static void * _umap_hash_lookup_elem(void *map, const void *key)
{
    UMAP_HASH_S *ctrl = map;
    UMAP_HASH_NODE_S *found;
    UMAP_HASH_NODE_S node;

    if ((!map) || (!key)) {
        return NULL;
    }

    node.ctrl = ctrl;
    node.key = (void*)key;

    found = HASH_Find(ctrl->hash_tbl, _umap_hash_cmp, &node);
    if (! found) {
        return NULL;
    }

    return found->value;
}

static long _umap_hash_delete_elem(void *map, const void *key)
{
    UMAP_HASH_S *ctrl = map;
    UMAP_HASH_NODE_S *old;

    old = _umap_hash_lookup_elem(map, key);
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

    old = _umap_hash_lookup_elem(map, key);

    if ((flag == UMAP_UPDATE_EXIST) && (! old)){
		return -ENOENT;
    }

    if ((flag == UMAP_UPDATE_NOEXIST) && (old)){
		return -EEXIST;
    }

    node = MEM_RcuZMalloc(sizeof(UMAP_HASH_NODE_S));
    if (! node) {
        return -ENOMEM;
    }

    node->key = MEM_RcuDup((char*)key, ctrl->hdr.size_key);
    node->value = MEM_RcuDup((char*)value, ctrl->hdr.size_value);

    if ((!node->key) || (!node->value)) {
        _umap_hash_free_node(node);
        return -ENOMEM;
    }

    HASH_Add(ctrl->hash_tbl, node);
    if (old) {
        HASH_Del(ctrl->hash_tbl, old);
    }

    if (old) {
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

    found = (void*)HASH_GetNextDict(ctrl->hash_tbl, _umap_hash_cmp, (void*)tmp);
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


