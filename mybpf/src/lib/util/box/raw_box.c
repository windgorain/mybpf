/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#include "bs.h"
#include "utl/cuckoo_hash.h"
#include "utl/jhash_utl.h"
#include "utl/file_utl.h"
#include "utl/box_utl.h"

int RawBox_Init(BOX_S *box, CUCKOO_HASH_NODE_S *table, 
        UINT bucket_num, UINT bucket_depth,
        PF_CUCKOO_INDEX index_func, PF_CUCKOO_CMP cmp_func)
{
    int ret;
    ret = CuckooHash_Init(&box->hash, table, bucket_num, bucket_depth);
    if (ret != 0) {
        return ret;
    }

    CuckooHash_SetIndexFunc(&box->hash, index_func);
    CuckooHash_SetCmpFunc(&box->hash, cmp_func);

    return 0;
}
