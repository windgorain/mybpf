/*********************************************************
*   Copyright (C) LiXingang
*   Description: 
*
********************************************************/
#include "bs.h"
#include <sys/mman.h>

static void * _mmap_create(int size)
{
    UCHAR *code = NULL;

    code = mmap(0, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (code == MAP_FAILED) {
        return NULL;
    }

    return code;
}

S64 MMAP_MapFile(char *filename, OUT void **data)
{
    int fd;
    fd=open(filename, O_RDWR);
    if (fd < 0) {
        printf("Can't open file %s \n", filename);
        return fd;
    }

    struct stat st;
    int r = fstat(fd, &st);
    if (r < 0) {
        close(fd);
        return r;
    }

    void *p = mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0);    
    if ((p == NULL) || (p==(void*)-1)) {
        printf("Can't mmap file %s \n", filename);
        close(fd);
        return -1;
    }

    *data = p;

    close(fd);

    return st.st_size;
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
