#ifndef _BLOOMFILTER_UTL_H
#define _BLOOMFILTER_UTL_H

#include "utl/bitmap_utl.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define BLOOMFILTER_AUTO_STEP_ENABLE 0x1

typedef struct {
    BITMAP_S stBitMap;
    UINT uiFlag;
    UINT uiStepCount; 
    UINT uiStepNow;
    UINT uiSettdSteps;
    UINT64 uiOldTs;
}BLOOM_FILTER_S;

int BloomFilter_Init(IN BLOOM_FILTER_S *pstBloomFilter, IN UINT uiBitCount);
void BloomFilter_Final(BLOOM_FILTER_S *pstBloomFilter);
void BloomFilter_Reset(BLOOM_FILTER_S *pstBloomFilter);
void BloomFilter_SetStepsToClearAll(BLOOM_FILTER_S *pstBloomFilter, UINT uiSteps);
void BloomFilter_SetAutoStep(BLOOM_FILTER_S *pstBloomFilter, int enable);
UINT BloomFilter_IsSet(IN BLOOM_FILTER_S *pstBloomFilter, IN VOID *pData, IN UINT uiDataLen);
void BloomFilter_Set(BLOOM_FILTER_S *pstBloomFilter, void *pData, UINT uiDataLen);

int BloomFilter_TrySet(IN BLOOM_FILTER_S *pstBloomFilter, IN VOID *pData, IN UINT uiDataLen);
void BloomFilter_Copy(BLOOM_FILTER_S *src, BLOOM_FILTER_S *dst);
void BloomFilter_Or(BLOOM_FILTER_S *src1, BLOOM_FILTER_S *src2, BLOOM_FILTER_S *dst);
void BloomFilter_Step(IN BLOOM_FILTER_S *pstBloomFilter);

#ifdef __cplusplus
}
#endif
#endif 
