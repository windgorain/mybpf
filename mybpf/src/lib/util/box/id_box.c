/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#include "bs.h"
#include "utl/box_utl.h"

static UINT idbox_Index(void *hash, void *data)
{
    return *(UINT*)data;
}

static int idbox_Cmp(void *hash, void *data1, void *data2)
{
    int d1, d2;

    d1 = *(UINT*)data1;
    d2 = *(UINT*)data2;

    return d1 - d2;
}

int IDBox_Init(BOX_S *box, CUCKOO_HASH_NODE_S *table,
        UINT bucket_num, UINT bucket_depth)
{
    int ret;
    ret = CuckooHash_Init(&box->hash, table, bucket_num, bucket_depth);
    if (ret != 0) {
        return ret;
    }

    CuckooHash_SetIndexFunc(&box->hash, idbox_Index);
    CuckooHash_SetCmpFunc(&box->hash, idbox_Cmp);

    return 0;
}
