/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#include "bs.h"
#include "utl/hash_utl.h"
#include "utl/num_utl.h"
#include "utl/mem_cap.h"

HASH_S * HASH_CreateInstance(void *memcap, UINT bucket_num, PF_HASH_BY_NODE hash_by_node)
{
    HASH_S *pstHashHead;
    UINT i;

    BS_DBGASSERT(NUM_IS2N(bucket_num));

    pstHashHead = MemCap_ZMalloc(memcap, sizeof(HASH_S));
    if (NULL == pstHashHead) {
        return NULL;
    }

    pstHashHead->pstBuckets = MemCap_Malloc(memcap, sizeof(DL_HEAD_S) * bucket_num);
    if (! pstHashHead->pstBuckets) {
        MemCap_Free(memcap, pstHashHead);
        return NULL;
    }

    pstHashHead->mask = bucket_num - 1;
    pstHashHead->hash_by_node = hash_by_node;
    pstHashHead->memcap = memcap;

    for (i=0; i<bucket_num; i++) {
        DL_Init(&pstHashHead->pstBuckets[i]);
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

