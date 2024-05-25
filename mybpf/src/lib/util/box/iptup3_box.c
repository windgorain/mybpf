/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#include "bs.h"
#include "utl/box_utl.h"
#include "utl/jhash_utl.h"

static UINT iptup3_Index(void *hash, void *data)
{
    return JHASH_GeneralBuffer(data, sizeof(IP_TUP3_KEY_S), 0);
}

static int iptup3_Cmp(void *hash, void *data1, void *data2)
{
    return memcmp(data1, data2, sizeof(IP_TUP3_KEY_S));
}

int IPTup3Box_Init(BOX_S *box, CUCKOO_HASH_NODE_S *table,
        UINT bucket_num, UINT bucket_depth)
{
    int ret;

    ret = CuckooHash_Init(&box->hash, table, bucket_num, bucket_depth);
    if (ret != 0) {
        return ret;
    }

    CuckooHash_SetIndexFunc(&box->hash, iptup3_Index);
    CuckooHash_SetCmpFunc(&box->hash, iptup3_Cmp);

    return 0;
}

