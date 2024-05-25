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

static UINT databox_Index(void *hash, void *data)
{
    LDATA_S *pstData = data;

    return JHASH_GeneralBuffer(pstData->data, pstData->len, 0);
}

static int databox_Cmp(void *hash, void *data1, void *data2)
{
    LDATA_S *pstData1 = data1;
    LDATA_S *pstData2 = data2;

    return MEM_Cmp(pstData1->data, pstData1->len,
            pstData2->data, pstData2->len);
}

int DataBox_Init(BOX_S *box, CUCKOO_HASH_NODE_S *table,
        UINT bucket_num, UINT bucket_depth)
{
    int ret;
    ret = CuckooHash_Init(&box->hash, table, bucket_num, bucket_depth);
    if (ret != 0) {
        return ret;
    }

    CuckooHash_SetIndexFunc(&box->hash, databox_Index);
    CuckooHash_SetCmpFunc(&box->hash, databox_Cmp);

    return 0;
}

