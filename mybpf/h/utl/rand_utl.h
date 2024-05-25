/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2009-4-3
* Description: 
* History:     
******************************************************************************/

#ifndef __RAND_UTL_H_
#define __RAND_UTL_H_

UINT RAND_GetNonZero(void);


unsigned long RAND_GetRandom(void);


VOID RAND_Entropy(IN UINT uiEntropy);


void RAND_Mem(OUT UCHAR *buf, int len);

static inline void RAND_Init(void)
{
    srand(time(NULL));
}

static inline UINT RAND_Get(void)
{
    return rand();
}

static inline UINT RAND_FastGet(UINT *seedp)
{
    *seedp = (*seedp) * 1103515245 + 12345;
    return *seedp;
}

static inline U64 RAND_FastGet64(U64 *seedp)
{
    *seedp = (*seedp) * 2862933555777941757ULL + 1;
    return *seedp;
}

#endif 


