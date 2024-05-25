/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _UMAP_DEF_H
#define _UMAP_DEF_H

#define UMAP_UPDATE_ANY		    0 
#define UMAP_UPDATE_NOEXIST	    1 
#define UMAP_UPDATE_EXIST	    2 

#define UMAP_ELF_MAP_MIN_SIZE 20 



typedef struct {
	unsigned int type;
	unsigned int size_key;
	unsigned int size_value;
	unsigned int max_elem;
	unsigned int flags;
}UMAP_ELF_MAP_S;

typedef void* (*PF_UMAP_OPEN)(void *map_def);
typedef void (*PF_UMAP_DESTROY)(void *map);
typedef void* (*PF_UMAP_LOOKUP_ELEM)(void *map, const void *key);
typedef long (*PF_UMAP_DELETE_ELEM)(void *map, const void *key);
typedef long (*PF_UMAP_UPDATE_ELEM)(void *map, const void *key, const void *value, unsigned int flag);
typedef int (*PF_UMAP_GETNEXT_KEY)(void *map, void *key, OUT void *next_key);
typedef long (*PF_UMAP_DIRECT_VALUE)(void *map, OUT U64 *addr, unsigned int off);

typedef struct {
    PF_UMAP_OPEN open_func;
    PF_UMAP_DESTROY destroy_func;
    PF_UMAP_LOOKUP_ELEM lookup_elem_func;
    PF_UMAP_DELETE_ELEM delete_elem_func;
    PF_UMAP_UPDATE_ELEM update_elem_func;
    PF_UMAP_GETNEXT_KEY get_next_key_func;
    PF_UMAP_DIRECT_VALUE direct_value_func;
}UMAP_FUNC_TBL_S;

#define UMAP_NAME_SIZE 128

typedef struct {
    void *magic; 
    UMAP_FUNC_TBL_S *opts;
    char map_name[UMAP_NAME_SIZE];
    int ref_count;
	UINT type;
	UINT size_key;
	UINT size_value;
	UINT max_elem;
	UINT flags;
}UMAP_HEADER_S;


#endif 

