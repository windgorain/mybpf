/*********************************************************
*   Copyright (C) LiXingang
*   Description: 
*
********************************************************/
#ifndef _MYBPF_RELO_H
#define _MYBPF_RELO_H

#include "utl/elf_utl.h"

#ifdef __cplusplus
extern "C"
{
#endif


enum {
    MYBPF_RELO_MAP_BPFMAP = 0,
    MYBPF_RELO_MAP_BSS,
    MYBPF_RELO_MAP_DATA,
    MYBPF_RELO_MAP_RODATA
};

typedef struct {
    int sec_id;
    int type; 
    int offset; 
    int value; 
}MYBPF_RELO_MAP_S;

typedef struct {
    MYBPF_RELO_MAP_S *relo_maps;
    int map_count;
}MYBPF_RELO_MAPS_S;

typedef struct {
    void *bss;
    void *data;
    void **rodatas;
    void *code;
}MYBPF_RELO_DATA_INFO_S;

int MYBPF_RELO_ProgRelo(ELF_S *elf);
int MYBPF_RELO_DataRelo(ELF_S *elf, MYBPF_RELO_DATA_INFO_S *info);
BOOL_T MYBPF_RELO_IsMapUsed(ELF_S *elf, int map_sec_id, int map_offset, int is_global_data);

#ifdef __cplusplus
}
#endif
#endif 
