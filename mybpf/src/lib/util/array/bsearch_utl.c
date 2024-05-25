/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#include "bs.h"
#include "utl/bsearch_utl.h"

void * BSEARCH_Do(void* base, int items_num, int ele_size, void *key, PF_CMP_FUNC cmp_func)
{
    BS_DBGASSERT(base && items_num > 0 && ele_size > 0);

    int low = 0;
    int high = items_num - 1; 

    while (low <= high) {
        int mid = low + (high - low) / 2;
        char* pmid = (char*)base + mid * ele_size;
        if (cmp_func(key, pmid) < 0) {
            low = mid + 1;
        } else if (cmp_func(key, pmid) > 0) {
            high = mid - 1;
        } else {
            return pmid;
        }
    }

    return NULL;
}

