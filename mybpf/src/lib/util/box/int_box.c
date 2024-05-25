/*================================================================
* Author：LiXingang.
* Description: 放置int的box
*
================================================================*/
#include "bs.h"
#include "utl/box_utl.h"

static UINT intbox_Index(void *hash, void *data)
{
    return HANDLE_UINT(data);
}

static int intbox_Cmp(void *hash, void *data1, void *data2)
{
    int d1, d2;

    d1 = HANDLE_UINT(data1);
    d2 = HANDLE_UINT(data2);

    return d1 - d2;
}

int IntBox_Init(BOX_S *box, CUCKOO_HASH_NODE_S *table,
        UINT bucket_num, UINT bucket_depth)
{
    int ret;
    ret = CuckooHash_Init(&box->hash, table, bucket_num, bucket_depth);
    if (ret != 0) {
        return ret;
    }

    CuckooHash_SetIndexFunc(&box->hash, intbox_Index);
    CuckooHash_SetCmpFunc(&box->hash, intbox_Cmp);

    return 0;
}

void IntBox_Show(BOX_S *box)
{
    int index = -1;
    while((index = Box_GetNext(box, index)) >= 0) {
        printf("%-10d %d\r\n", index, (int)HANDLE_UINT(box->hash.table[index].data));
    }
}

