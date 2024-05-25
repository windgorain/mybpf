/*================================================================
*   Created by LiXingang
*   Description: 存放字符串的盒子
*
================================================================*/
#include "bs.h"
#include "utl/cuckoo_hash.h"
#include "utl/jhash_utl.h"
#include "utl/file_utl.h"
#include "utl/box_utl.h"

static UINT strbox_StringIndex(void *hash, void *data)
{
    return JHASH_GeneralBuffer(data, strlen(data), 0);
}

static int strbox_StringCmp(void *hash, void *data1, void *data2)
{
    return strcmp(data1, data2);
}

int StrBox_Init(BOX_S *box, CUCKOO_HASH_NODE_S *table, 
        UINT bucket_num, UINT bucket_depth)
{
    int ret;
    ret = CuckooHash_Init(&box->hash, table, bucket_num, bucket_depth);
    if (ret != 0) {
        return ret;
    }

    CuckooHash_SetIndexFunc(&box->hash, strbox_StringIndex);
    CuckooHash_SetCmpFunc(&box->hash, strbox_StringCmp);

    return 0;
}

void StrBox_Show(BOX_S *box)
{
    int index = -1;
    while((index = Box_GetNext(box, index)) >= 0) {
        printf("%-10d %s\r\n", index, (char*)box->hash.table[index].data);
    }
}

