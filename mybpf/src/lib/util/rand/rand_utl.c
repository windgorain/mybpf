/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2012-8-1
* Description: 
* History:     
******************************************************************************/
#include "bs.h"

#include "utl/time_utl.h"
#include "utl/rand_utl.h"

UINT RAND_GetNonZero(void)
{
    UINT uiRand;

    do {
        uiRand = RAND_Get();
    }while (uiRand == 0);

    return uiRand;
}

VOID RAND_Entropy(IN UINT uiEntropy)
{
    UINT seed = RAND_Get() + uiEntropy;
    srand(seed);
}


void RAND_Mem(OUT UCHAR *buf, int len)
{
    int i;

    for (i=0; i<len; i++) {
        buf[i] = RAND_Get();
    }
}

#ifdef IN_UNIXLIKE

unsigned long RAND_GetRandom(void)
{
	int fd=0;
	unsigned long ul_num = 0;

	fd=open("/dev/urandom",O_RDONLY);
	if(fd<0) {
        return RAND_Get();
	}

	int ret=read(fd,&ul_num,sizeof(ul_num));
	if(ret<0){
		close(fd);
        return RAND_Get();
	}

	close(fd);

	return ul_num;
}
#else
unsigned long RAND_GetRandom(void)
{
    return RAND_Get();
}
#endif

static void rand_init(void)
{
    RAND_Init();
}

CONSTRUCTOR(init) {
    rand_init();
}

