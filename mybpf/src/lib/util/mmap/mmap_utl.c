/*********************************************************
*   Copyright (C) LiXingang
*   Description: 
*
********************************************************/
#include "bs.h"

static void * _mmap_create(int size)
{
    UCHAR *code = NULL;

    code = mmap(0, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (code == MAP_FAILED) {
        return NULL;
    }

    return code;
}


void * MMAP_Map(void *buf, int buf_size, int head_size)
{
    UCHAR *code = NULL;
    int total_size = buf_size + head_size;

    code = _mmap_create(total_size);
    if (code)  {
        memcpy(code + head_size, buf, buf_size);
    }

    return code;
}

void MMAP_Unmap(void *buf, int size)
{
    munmap(buf, size);
}

int MMAP_Mprotect(void *buf, int size, int flag)
{
    return mprotect(buf, size, flag);
}

void * MMAP_MapExe(void *buf, int size)
{
    void *code = MMAP_Map(buf, size, size);
    if (! code) {
        return NULL;
    }

    if (mprotect(code, size, PROT_READ | PROT_EXEC) < 0) {
        MMAP_Unmap(code, size);
        return NULL;
    }

    return code;
}
