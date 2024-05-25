/*================================================================
*   Created by LiXingang, Copyright LiXingang
*   Description: 
*
================================================================*/
#ifndef _UMAP_UTL_H
#define _UMAP_UTL_H
#include "utl/ulc_def.h"
#include "utl/umap_def.h"
#ifdef __cplusplus
extern "C"
{
#endif


char * UMAP_TypeName(unsigned int type);
void * UMAP_Open(UMAP_ELF_MAP_S *elfmap, char *map_name);
void UMAP_Close(UMAP_HEADER_S *map);

void * UMAP_LookupElem(UMAP_HEADER_S *map, const void *key);
long UMAP_DeleteElem(UMAP_HEADER_S *map, const void *key);
long UMAP_UpdateElem(UMAP_HEADER_S *map, const void *key, const void *value, U32 flag);
int UMAP_DirectValue(UMAP_HEADER_S *map, OUT UINT64 *addr, UINT off);
int UMAP_GetNextKey(UMAP_HEADER_S *map, void *curr_key, OUT void *next_key);


static inline void UMAP_IncRef(UMAP_HEADER_S *map)
{
    map->ref_count ++;
}

#ifdef __cplusplus
}
#endif
#endif 
