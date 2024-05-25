/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#include "bs.h"
#include "utl/mem_utl.h"
#include "utl/map_utl.h"
#include "utl/maps_utl.h"

typedef struct {
    int map_max_num;
    MAP_HANDLE maps[0];
}MAPS_S;

void * MAPS_Create(int map_max_num)
{
    MAPS_S *ctrl;

    ctrl = MEM_ZMalloc(sizeof(MAPS_S) + sizeof(MAP_HANDLE) * map_max_num);
    if (! ctrl) {
        return NULL;
    }

    ctrl->map_max_num = map_max_num;

    return ctrl;
}

int MAPS_Add(void *maps, int id, void *key, int key_len, void *value, UINT flag)
{
    MAPS_S *ctrl = maps;

    if (id >= ctrl->map_max_num) {
        RETURN(BS_OUT_OF_RANGE);
    }

    if (! ctrl->maps[id]) {
        ctrl->maps[id] = MAP_HashCreate(NULL);
        if (! ctrl->maps[id]) {
            RETURN(BS_NO_MEMORY);
        }
    }

    return MAP_Add(ctrl->maps[id], key, key_len, value, flag);
}

void * MAPS_Del(void *maps, int id, void *key, int key_len)
{
    MAPS_S *ctrl = maps;

    if (id >= ctrl->map_max_num) {
        return NULL;
    }

    if (! ctrl->maps[id]) {
        return NULL;
    }

    return MAP_Del(ctrl->maps[id], key, key_len);
}

void * MAPS_Get(void *maps, int id, void *key, int key_len)
{
    MAPS_S *ctrl = maps;

    if (id >= ctrl->map_max_num) {
        return NULL;
    }

    if (! ctrl->maps[id]) {
        return NULL;
    }

    return MAP_Get(ctrl->maps[id], key, key_len);
}

