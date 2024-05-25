/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#include "bs.h"
#include "utl/hash_utl.h"
#include "utl/num_utl.h"
#include "utl/mem_cap.h"

HASH_S * HASH_CreateInstance(void *memcap, IN UINT ulHashBucketNum, IN PF_HASH_INDEX_FUNC pfFunc)
{
    HASH_S *pstHashHead;
    UINT i;

    BS_DBGASSERT(NUM_IS2N(ulHashBucketNum));

    pstHashHead = MemCap_ZMalloc(memcap, sizeof(HASH_S));
    if (NULL == pstHashHead) {
        return NULL;
    }

    pstHashHead->pstBuckets = MemCap_Malloc(memcap, sizeof(DLL_HEAD_S) * ulHashBucketNum);
    if (! pstHashHead->pstBuckets) {
        MemCap_Free(memcap, pstHashHead);
        return NULL;
    }

    pstHashHead->mask = ulHashBucketNum - 1;
    pstHashHead->pfHashIndexFunc = pfFunc;
    pstHashHead->memcap = memcap;

    for (i=0; i<ulHashBucketNum; i++) {
        DLL_INIT(&pstHashHead->pstBuckets[i]);
    }

    return pstHashHead;
}

VOID HASH_DestoryInstance(IN HASH_S * hHashId)
{
    HASH_S *pstHashCtrl = (HASH_S*)hHashId;

    BS_DBGASSERT(0 != hHashId);

    if (pstHashCtrl->pstBuckets) {
        MemCap_Free(pstHashCtrl->memcap, pstHashCtrl->pstBuckets);
    }

    MemCap_Free(pstHashCtrl->memcap, pstHashCtrl);
}

