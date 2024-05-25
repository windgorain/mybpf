/*================================================================
* Author：LiXingang
* Description: 放置五元组的box
*
================================================================*/
#include "bs.h"
#include "utl/jhash_utl.h"
#include "utl/cuckoo_hash.h"
#include "utl/box_utl.h"

static UINT iptup_Index(void *hash, void *data)
{
    return JHASH_GeneralBuffer(data, sizeof(IP_TUP_KEY_S), 0);
}

static int iptup_Cmp(void *hash, void *data1, void *data2)
{
    return memcmp(data1, data2, sizeof(IP_TUP_KEY_S));
}

int IPTupBox_Init(BOX_S *box, CUCKOO_HASH_NODE_S *table,
        UINT bucket_num, UINT bucket_depth)
{
    int ret;

    ret = CuckooHash_Init(&box->hash, table, bucket_num, bucket_depth);
    if (ret != 0) {
        return ret;
    }

    CuckooHash_SetIndexFunc(&box->hash, iptup_Index);
    CuckooHash_SetCmpFunc(&box->hash, iptup_Cmp);

    return 0;
}

