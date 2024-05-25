/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _MMAP_UTL_H
#define _MMAP_UTL_H
#ifdef __cplusplus
extern "C"
{
#endif

void * MMAP_Map(void *buf, int buf_size, int head_size);
void MMAP_Unmap(void *buf, int total_size);

#ifdef __cplusplus
}
#endif
#endif 
