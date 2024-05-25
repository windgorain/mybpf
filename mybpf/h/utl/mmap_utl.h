/*********************************************************
*   Copyright (C) LiXingang
*   Description: 
*
********************************************************/
#ifndef _MMAP_UTL_H
#define _MMAP_UTL_H

#ifdef __cplusplus
extern "C"
{
#endif

S64 MMAP_MapFile(char *filename, OUT void **data);
void * MMAP_Map(void *buf, int buf_size, int head_size);
void MMAP_Unmap(void *buf, int total_size);
int MMAP_Mprotect(void *buf, int size, int flag);
void * MMAP_MapExe(void *buf, int size);

#ifdef __cplusplus
}
#endif
#endif 
