/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _ELF_LIB_H
#define _ELF_LIB_H

#include "utl/elf_sec.h"
#include "utl/elf_vbuf.h"
#include "utl/elf_prog.h"
#include "utl/elf_merge.h"
#include "utl/elf_reduce.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef EM_BPF
#define	EM_X86_64	62
#define	EM_AARCH64	183
#define	EM_BPF      247
#endif

int ELFLIB_CheckHeader(LLDATA_S *d);
Elf64_Ehdr * ELFLIB_GetHeader(LLDATA_S *d);
int ELFLIB_CountSection(LLDATA_S *d);
Elf64_Shdr * ELFLIB_GetSecByID(LLDATA_S *d, U32 id);
int ELFLIB_GetSecID(LLDATA_S *d, Elf64_Shdr *shdr);
int ELFLIB_GetSecIDByName(LLDATA_S *d, char *sec_name);
void * ELFLIB_GetSecData(LLDATA_S *d, Elf64_Shdr *shdr);
Elf64_Shdr * ELFLIB_GetNextSec(LLDATA_S *d, Elf64_Shdr *cur);
Elf64_Shdr * ELFLIB_GetNextTypeSec(LLDATA_S *d, U32 type, Elf64_Shdr *cur);
int ELFLIB_GetTypeSecCount(LLDATA_S *d, U32 type);
Elf64_Shdr * ELFLIB_GetTypeSec(LLDATA_S *d, U32 type, int index);
Elf64_Shdr * ELFLIB_GetTypeSec(LLDATA_S *d, U32 type, int index);
Elf64_Sym * ELFLIB_GetSymByRel(LLDATA_S *d, Elf64_Rel *rel);
Elf64_Shdr * ELFLIB_GetTargetSecByRel(LLDATA_S *d, Elf64_Rel *rel);
Elf64_Shdr * ELFLIB_GetRelSecByMdySecID(LLDATA_S *d, int mdy_sec_id);
Elf64_Rel * ELFLIB_GetRelByMdy(LLDATA_S *d, int mdy_sec_id, U64 mdy_offset);
Elf64_Rel * ELFLIB_GetRelByMdyRange(LLDATA_S *d, int mdy_sec_id, U64 mdy_begin, U64 mdy_end);
Elf64_Rel * ELFLIB_GetRelByTarget(LLDATA_S *d, int target_sec_id, U64 target_offset);
Elf64_Rel * ELFLIB_GetRelBySymID(LLDATA_S *d, U32 sym_id);
Elf64_Shdr * ELFLIB_GetMdySecByRelSec(LLDATA_S *d, Elf64_Shdr *rel_shdr);
Elf64_Shdr * ELFLIB_GetStrtabSec(LLDATA_S *d);
Elf64_Shdr * ELFLIB_GetSymSec(LLDATA_S *d);
Elf64_Shdr * ELFLIB_GetSecBySym(LLDATA_S *d, Elf64_Sym *sym);
int ELFLIB_GetSymCount(Elf64_Shdr *sym_shdr);
char * ELFLIB_GetSymName(LLDATA_S *d, Elf64_Sym *sym);
int ELFLIB_GetSymID(LLDATA_S *d, Elf64_Sym *sym);
Elf64_Sym * ELFLIB_GetSymByID(LLDATA_S *d, U32 id);
Elf64_Sym * ELFLIB_GetSymByName(LLDATA_S *d, char *sym_name);
Elf64_Sym * ELFLIB_GetLocalSymByName(LLDATA_S *d, char *sym_name);
char * ELFLIB_GetStr(LLDATA_S *d, U64 off);
char * ELFLIB_GetSecName(LLDATA_S *d, Elf64_Shdr *shdr);
Elf64_Shdr * ELFLIB_GetSecByName(LLDATA_S *d, char *sec_name);
BOOL_T ELFLIB_IsTextSection(LLDATA_S *d, Elf64_Shdr *shdr);
BOOL_T ELFLIB_IsTextSecID(LLDATA_S *d, U32 sec_id);
BOOL_T ELFLIB_IsProgSection(Elf64_Shdr *shdr);
BOOL_T ELFLIB_IsBssSection(LLDATA_S *d, Elf64_Shdr *shdr);
BOOL_T ELFLIB_IsDataSection(LLDATA_S *d, Elf64_Shdr *shdr);
BOOL_T ELFLIB_IsRoDataSection(LLDATA_S *d, Elf64_Shdr *shdr);
BOOL_T ELFLIB_IsGlobalDataSection(LLDATA_S *d, Elf64_Shdr *shdr);
int ELFLIB_GetRoDataCount(LLDATA_S *d);
int ELFLIB_GetRoDataID(LLDATA_S *d, Elf64_Shdr *shdr);
Elf64_Shdr * ELFLIB_GetNextRoDataSec(LLDATA_S *d, Elf64_Shdr *shdr);
U64 ELFLIB_GetBssSize(LLDATA_S *d);
int ELFLIB_SecSymbolCount(LLDATA_S *d, int sec_id, int type);
Elf64_Sym * ELFLIB_GetSecSymByOffset(LLDATA_S *d, int sec_id, U64 offset);
char * ELFLIB_GetSecSymNameByOffset(LLDATA_S *d, int sec_id, U64 offset);
Elf64_Sym * ELFLIB_GetSecSymByID(LLDATA_S *d, int sec_id, int type, int index);
char * ELFLIB_GetSecSymNameByID(LLDATA_S *d, int sec_id, int type, int index);
void * ELFLIB_GetTargetBySym(LLDATA_S *d, Elf64_Sym *sym);
U64 ELFLIB_GetProgSecOffset(LLDATA_S *d, Elf64_Shdr *shdr);
U64 ELFLIB_GetProgsSize(LLDATA_S *d);
int ELFLIB_GetFuncCount(LLDATA_S *d);
int ELFLIB_GetFuncInfo(LLDATA_S *d, OUT ELF_PROG_INFO_S *funcs, int max_prog_count);
void ELFLIB_FixSecID(ELF_SEC_CHANGE_INFO_S *info, int old_sec_id, int new_sec_id, U64 offset);
void ELFLIB_SetRelLink(LLDATA_S *d, int new_symtab_id);
void ELFLIB_SetEhdrStrtabID(void *data, U32 sec_id);

#ifdef __cplusplus
}
#endif
#endif 
