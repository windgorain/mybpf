/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _ELF_UTL_H
#define _ELF_UTL_H

#include <libelf.h>
#include <gelf.h>
#include "utl/elf_def.h"
#include "utl/elf_lib.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define ELF_DBG_PROCESS 0x1

typedef LLDATA_S ELF_S;

typedef struct {
    int sec_id;
    char *shname;
    Elf64_Shdr *shdr;
	void *data;
}ELF_SECTION_S;

#define ELF_MAX_GLOBAL_DATA_SEC_NUM 32

typedef struct {
    UINT sec_count: 8; 
    UINT rodata_count: 8;
    UINT have_bss:1;
    UINT have_data:1;
    ELF_SECTION_S bss_sec;
    ELF_SECTION_S data_sec;
    ELF_SECTION_S rodata_sec[ELF_MAX_GLOBAL_DATA_SEC_NUM];
}ELF_GLOBAL_DATA_S;

int ELF_Open(char *path, OUT ELF_S *elf);
void ELF_Close(ELF_S *elf);

void * ELF_GetNextSection(ELF_S *elf, void *iter, OUT ELF_SECTION_S *sec);
void * ELF_GetNextTypeSection(ELF_S *elf, int type, void *iter, OUT ELF_SECTION_S *sec);
int ELF_GetSecIDByName(ELF_S *elf, char *sec_name);
int ELF_GetSecByID(ELF_S *elf, int id, OUT ELF_SECTION_S *sec);
int ELF_GetSecByName(ELF_S *elf, char *sec_name, OUT ELF_SECTION_S *sec);
Elf64_Sym * ELF_GetSymbolByID(ELF_S *elf, int id);
char * ELF_GetSymbolName(ELF_S *elf, Elf64_Sym * sym);

BOOL_T ELF_IsProgSection(ELF_SECTION_S *sec);
BOOL_T ELF_IsDataSection(ELF_SECTION_S *sec);

int ELF_GetGlobalData(ELF_S *elf, OUT ELF_GLOBAL_DATA_S *global_data);

int ELF_CopyProgs(ELF_S *elf, OUT void *mem, int mem_size);
void * ELF_DupProgs(ELF_S *elf);
int ELF_GetSecProgsInfoCount(ELF_PROG_INFO_S *info, int prog_count, char *sec_name);

typedef int (*PF_ELF_WALK_PROG)(void *data, ELF_PROG_INFO_S *info, void *ud);
int ELF_WalkProgs(ELF_S *elf, PF_ELF_WALK_PROG walk_func, void *ud);

#ifdef __cplusplus
}
#endif
#endif 
