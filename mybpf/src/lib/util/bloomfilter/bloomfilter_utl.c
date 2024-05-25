#include "bs.h"

#include "utl/bitmap_utl.h"
#include "utl/time_utl.h"
#include "utl/rdtsc_utl.h"
#include "utl/jhash_utl.h"
#include "utl/bloomfilter_utl.h"

static void _bloomfilter_TimeProcess(BLOOM_FILTER_S *pstBloomFilter)
{
    int i;
    UINT64 count;
    UINT64 ts;

    if (! (pstBloomFilter->uiFlag & BLOOMFILTER_AUTO_STEP_ENABLE)) {
        return;
    }

    ts = RDTSC_Get();

    count = (ts - pstBloomFilter->uiOldTs) / RDTSC_HZ;
    pstBloomFilter->uiOldTs = pstBloomFilter->uiOldTs + count * RDTSC_HZ;

    if (count >= pstBloomFilter->uiSettdSteps) {
        BITMAP_Zero(&pstBloomFilter->stBitMap);
    } else {
        for (i=0; i<count; i++) {
            BloomFilter_Step(pstBloomFilter);
        }
    }
}

int BloomFilter_Init(IN BLOOM_FILTER_S *pstBloomFilter, IN UINT uiBitCount)
{
    return BITMAP_Create(&pstBloomFilter->stBitMap, uiBitCount);
}

void BloomFilter_Final(BLOOM_FILTER_S *pstBloomFilter)
{
    BITMAP_Destory(&pstBloomFilter->stBitMap);
}

void BloomFilter_Reset(BLOOM_FILTER_S *pstBloomFilter)
{
    BITMAP_ClrAll(&pstBloomFilter->stBitMap);
}

void BloomFilter_SetStepsToClearAll(BLOOM_FILTER_S *pstBloomFilter, UINT uiSteps)
{
    UINT uiBitNum = BITMAP_GET_BIT_NUM(&pstBloomFilter->stBitMap);
    UINT uiCount;

    uiCount = (uiBitNum + (uiSteps - 1)) / uiSteps;
    if (uiCount == 0) {
        uiCount = 1;
    }

    pstBloomFilter->uiSettdSteps = uiSteps;
    pstBloomFilter->uiStepCount = uiCount;
}

void BloomFilter_SetAutoStep(BLOOM_FILTER_S *pstBloomFilter, int enable)
{
    if (enable) {
        pstBloomFilter->uiFlag |= BLOOMFILTER_AUTO_STEP_ENABLE;
    } else {
        pstBloomFilter->uiFlag &= ~((UINT)BLOOMFILTER_AUTO_STEP_ENABLE);
    }
}

UINT BloomFilter_IsSet(IN BLOOM_FILTER_S *pstBloomFilter, IN VOID *pData, IN UINT uiDataLen)
{
    UINT uiHash;

    uiHash = JHASH_GeneralBuffer(pData, uiDataLen, 0);
    uiHash = (uiHash % pstBloomFilter->stBitMap.ulBitNum);

    return BITMAP_ISSET(&pstBloomFilter->stBitMap, uiHash);
}

void BloomFilter_Set(BLOOM_FILTER_S *pstBloomFilter, void *pData, UINT uiDataLen)
{
    UINT uiHash;

    _bloomfilter_TimeProcess(pstBloomFilter);

    uiHash = JHASH_GeneralBuffer(pData, uiDataLen, 0);
    uiHash = (uiHash % pstBloomFilter->stBitMap.ulBitNum);

    BITMAP_SET(&pstBloomFilter->stBitMap, uiHash);
}


int BloomFilter_TrySet(IN BLOOM_FILTER_S *pstBloomFilter, IN VOID *pData, IN UINT uiDataLen)
{
    UINT uiHash;

    _bloomfilter_TimeProcess(pstBloomFilter);

    uiHash = JHASH_GeneralBuffer(pData, uiDataLen, 0);
    uiHash = (uiHash % pstBloomFilter->stBitMap.ulBitNum);

    if (BITMAP_ISSET(&pstBloomFilter->stBitMap, uiHash)) {
        return BS_ALREADY_EXIST;
    }

    BITMAP_SET(&pstBloomFilter->stBitMap, uiHash);

    return 0;
}

void BloomFilter_Copy(BLOOM_FILTER_S *src, BLOOM_FILTER_S *dst)
{
    BS_DBGASSERT(NULL != src);
    BS_DBGASSERT(NULL != dst);

    BITMAP_Copy(&src->stBitMap, &dst->stBitMap);
}

void BloomFilter_Or(BLOOM_FILTER_S *src1, BLOOM_FILTER_S *src2, BLOOM_FILTER_S *dst)
{
    BS_DBGASSERT(NULL != src1);
    BS_DBGASSERT(NULL != src2);
    BS_DBGASSERT(NULL != dst);

    BITMAP_Or(&src1->stBitMap, &src2->stBitMap, &dst->stBitMap);
}

void BloomFilter_Step(IN BLOOM_FILTER_S *pstBloomFilter)
{
    if (pstBloomFilter->uiStepCount == 0) {
        BITMAP_Zero(&pstBloomFilter->stBitMap);
        return;
    }

    BITMAP_ClrBits(&pstBloomFilter->stBitMap, pstBloomFilter->uiStepNow, pstBloomFilter->uiStepCount);
    pstBloomFilter->uiStepNow += pstBloomFilter->uiStepCount;
    if (pstBloomFilter->uiStepNow >= BITMAP_GET_BIT_NUM(&pstBloomFilter->stBitMap)) {
        pstBloomFilter->uiStepNow = 0;
    }
}

