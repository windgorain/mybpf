/*********************************************************
*   Copyright (C), Xingang.Li
*   Author:      Xingang.Li  Version: 1.0
*   Description: 
*
********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "bs.h"
#include "utl/arch_utl.h"
#include "utl/mybpf_utl.h"
#include "utl/mybpf_loader_def.h"
#include "utl/mybpf_spf_def.h"
#include "utl/mybpf_bare.h"
#include "utl/mybpf_main.h"

static MYBPF_BARE_S g_mybpf_bare;
static MYBPF_SPF_S *g_mybpf_spf_ctrl;

static int file_mem(char *filename, void **data)
{
    struct stat st;

    int fd=open(filename, O_RDWR);
    if (fd < 0) {
        printf("Can't open file %s \n", filename);
        return fd;
    }

    int r = fstat(fd, &st);
    if ((r < 0) || (st.st_size <= 0)) {
        close(fd);
        return -1;
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

static int _load_spf_loader(void *data, int len)
{
    MYBPF_PARAM_S p = {0};

    int ret = MYBPF_LoadBare(data, len, NULL, &g_mybpf_bare);
    if (ret < 0) {
        printf("Load loader failed \n");
        return ret;
    }

    p.p[0] = (long)data;
    MYBPF_RunBareMain(&g_mybpf_bare, &p);

    g_mybpf_spf_ctrl = (void*)(long)p.bpf_ret;
    if (! g_mybpf_spf_ctrl) {
        printf("Can't get spf ctrl \n");
        return -1;
    }

    ret = g_mybpf_spf_ctrl->init();
    if (ret < 0) {
        printf("SPF init failed \n");
        return -1;
    }

    g_mybpf_spf_ctrl->config_by_file("/usr/local/mybpf/mybpf.cfg");

    return 0;
}

static int _load_spf_loader_file(char *spf_loader)
{
    void *data;
    int len;

    len = file_mem(spf_loader, &data);
    if (len < 0) {
        printf("Can't open file %s", spf_loader);
        return -1;
    }

    int ret = _load_spf_loader(data, len);

    munmap(data, len);

    return ret;
}

/* bare_spf [spfloader] */
__attribute__((constructor)) int MYBPF_init(void)
{
    char *file = NULL;

    if (ARCH_LocalArch() == ARCH_TYPE_ARM64) {
        file = "/usr/local/mybpf/spf_loader.arm64.bare";
    } else if (ARCH_LocalArch() == ARCH_TYPE_X86_64) {
        file = "/usr/local/mybpf/spf_loader.x64.bare";
    } else {
        fprintf(stderr, "Can't support this arch \n");
        return -1;
    }

    return _load_spf_loader_file(file);
}

U64 MYBPF_Notify(int event_type, MYBPF_PARAM_S *p)
{
    return g_mybpf_spf_ctrl->run(event_type, p);
}


