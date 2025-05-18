/*================================================================
*   Created by LiXingang, Copyright LiXingang
*   Date: 2017.1.2
*   Description: user bpf map
*
================================================================*/
#include "bs.h"
#include "utl/umap_utl.h"

static char * g_umap_type_name[] = {
    "unspec",
    "hash",
    "array",
    "prog_array",
    "perf_event_array",
    "percpu_hash",
    "percpu_array",
};

extern UMAP_FUNC_TBL_S g_umap_hash_ops;
extern UMAP_FUNC_TBL_S g_umap_array_ops;
extern UMAP_FUNC_TBL_S g_umap_percpu_hash_ops;

static UMAP_FUNC_TBL_S *g_umap_func_tbl[BPF_MAP_TYPE_MAX] = {
    [BPF_MAP_TYPE_HASH] = &g_umap_hash_ops,
    [BPF_MAP_TYPE_ARRAY] = &g_umap_array_ops,
    [BPF_MAP_TYPE_PERCPU_HASH] = &g_umap_percpu_hash_ops,
};

static inline UMAP_FUNC_TBL_S * _umap_get_opts(U32 type)
{
    if (type >= BPF_MAP_TYPE_MAX) {
		return NULL;
    }

    return g_umap_func_tbl[type];
}

char * UMAP_TypeName(unsigned int type)
{
    if (type >= ARRAY_SIZE(g_umap_type_name)) {
        return NULL;
    }

    return g_umap_type_name[type];
}

void * UMAP_Open(UMAP_ELF_MAP_S *elfmap, char *map_name)
{
    UINT type = elfmap->type;
    UMAP_HEADER_S *hdr;

    if (! map_name) {
        map_name = "";
    }

    UMAP_FUNC_TBL_S *opts = _umap_get_opts(type);
    if (! opts) {
        return NULL;
    }

    hdr = opts->open_func(elfmap);
    if (! hdr) {
        return NULL;
    }

    hdr->ref_count = 1;
    hdr->type = elfmap->type;
    hdr->size_key = elfmap->size_key;
    hdr->size_value = elfmap->size_value;
    hdr->max_elem = elfmap->max_elem;
    hdr->flags = elfmap->flags;
    strlcpy(hdr->map_name, map_name, sizeof(hdr->map_name));

    return hdr;
}

void UMAP_Close(UMAP_HEADER_S *map)
{
    UMAP_FUNC_TBL_S *opts = _umap_get_opts(map->type);
    if (! opts) {
        return;
    }

    map->ref_count --;

    if (map->ref_count <= 0) {
        opts->destroy_func(map);
    }
}

void * UMAP_LookupElem(UMAP_HEADER_S *map, const void *key)
{
    if (! map) {
        return NULL;
    }

    
    UMAP_FUNC_TBL_S *opts = _umap_get_opts(map->type);
    if (! opts) {
        return NULL;
    }

    return opts->lookup_elem_func(map, key);
}

long UMAP_DeleteElem(UMAP_HEADER_S *map, const void *key)
{
    if (! map) {
		return -EINVAL;
    }

    UMAP_FUNC_TBL_S *opts = _umap_get_opts(map->type);
    if (! opts) {
		return -EINVAL;
    }

    return opts->delete_elem_func(map, key);
}

long UMAP_UpdateElem(UMAP_HEADER_S *map, const void *key, const void *value, U32 flag)
{
    if (! map) {
		return -EINVAL;
    }

    UMAP_FUNC_TBL_S *opts = _umap_get_opts(map->type);
    if (! opts) {
		return -EINVAL;
    }

    return opts->update_elem_func(map, key, value, flag);
}


int UMAP_DirectValue(UMAP_HEADER_S *map, OUT U64 *addr, UINT off)
{
    if (! map) {
        RETURN(BS_ERR);
    }

    UMAP_FUNC_TBL_S *opts = _umap_get_opts(map->type);
    if (! opts) {
        RETURN(BS_ERR);
    }

    if (! opts->direct_value_func) {
        RETURN(BS_ERR);
    }

    return opts->direct_value_func(map, addr, off);
}


int UMAP_GetNextKey(UMAP_HEADER_S *map, void *curr_key, OUT void *next_key)
{
    if (! map) {
        return -1;
    }

    if (map->magic) {
        return -1; 
    }

    UMAP_FUNC_TBL_S *opts = _umap_get_opts(map->type);
    if (! opts) {
        return -1;
    }

    return opts->get_next_key_func(map, curr_key, next_key);
}

#if 0
void UMAP_DumpMap(UFD_S *ctx, int map_fd, PF_PRINT_FUNC print_func)
{
    char key[1024] = {0};
    void *data;
    int ret;

    UMAP_HEADER_S *hdr = UMAP_GetByFd(ctx, map_fd);
    if (! hdr) {
        return;
    }

    if (hdr->size_key > sizeof(key)) {
        return;
    }

    while ((ret = UMAP_GetNextKey(hdr, key, key)) == 0) {
        data = UMAP_LookupElem(hdr, key);
        if (! data) {
            continue;
        }

        print_func("Key:\r\n");
        MEM_Print(key, hdr->size_key, (void*)print_func);
        print_func("Value:\r\n");
        MEM_Print(data, hdr->size_value, (void*)print_func);
        print_func("\r\n");
    }
}
#endif

