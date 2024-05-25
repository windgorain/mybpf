/*================================================================
*   Created by LiXingang
*   Description: data->id映射
*
================================================================*/
#include "bs.h"
#include "utl/mem_cap.h"
#include "utl/box_utl.h"

typedef struct {
    void *data;
    int id;
}DATA_ID_S;

typedef struct {
    CUCKOO_HASH_S hash;
    BOX_S box;
    MEM_CAP_S *memcap;
}DATA_ID_CTRL_S;

int DataID_Init(DATA_ID_CTRL_S *ctrl, MEM_CAP_S *memcap,
        int bucket_num, int bucket_depth)
{
    CUCKOO_HASH_NODE_S * table;

    table = MemCap_ZAlloc(memcap, sizeof(CUCKOO_HASH_NODE_S)*bucket_num*bucket_depth);
    if (! table) {
        RETURN(BS_NO_MEMORY);
    }

    ret = DataBox_Init(&ctrl->box, table, bucket_num, bucket_depth);
    if (ret != 0) {
        MemCap_Free(memcap, table);
        return ret;
    }

    ctrl->memcap = memcap;

    return 0;
}

void DataID_Fini(DATA_ID_CTRL_S *ctrl)
{
    if (ctrl->hash.table) {
        MemCap_Free(ctrl->hash.table);
    }

    Box_Fini(&ctrl->box);
}

int DataID_

