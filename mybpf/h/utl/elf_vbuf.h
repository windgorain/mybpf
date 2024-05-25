/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _ELF_VBUF_H_
#define _ELF_VBUF_H_

#include "utl/vbuf_utl.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*PF_ELFLIB_WALK_SEC)(LLDATA_S *d, Elf64_Shdr *shdr, void *ud);

int ELFBUF_Write(OUT VBUF_S *vbuf, void *buf, U32 buf_len);
int ELFBUF_WriteEhdr(LLDATA_S *d, OUT VBUF_S *vbuf);
int ELFBUF_WriteRelSecs(LLDATA_S *d, ELF_SEC_CHANGE_INFO_S *change, OUT VBUF_S *vbuf, OUT ELF_MDY_SEC_INFO_S *info);
int ELFBUF_WriteStrtab(LLDATA_S *d, char *add_string, OUT VBUF_S *vbuf, OUT ELF_SEC_RECORD_S *sec_record);
int ELFBUF_WriteSymtab(LLDATA_S *d, ELF_SEC_CHANGE_INFO_S *change, OUT VBUF_S *vbuf, OUT ELF_MDY_SEC_INFO_S *info);
int ELFBUF_AddSym(LLDATA_S *d, Elf64_Sym *sym, OUT VBUF_S *vbuf, OUT ELF_MDY_SEC_INFO_S *info);
int ELFBUF_MergeRelSecs(LLDATA_S *d1, LLDATA_S *d2, ELF_SEC_CHANGE_INFO2_S *change, OUT VBUF_S *vbuf, OUT ELF_MDY_SEC_INFO_S *info);
int ELFBUF_MergeSymtab(LLDATA_S *d1, LLDATA_S *d2, ELF_SEC_CHANGE_INFO2_S *change, OUT VBUF_S *vbuf, OUT ELF_MDY_SEC_INFO_S *info);
int ELFLIB_WriteSecs(LLDATA_S *d, OUT VBUF_S *vbuf, OUT ELF_SEC_RECORD_S *sec_record,
        PF_ELFLIB_WALK_SEC permit_func, void *ud);
int ELFBUF_WriteEnd(OUT VBUF_S *vbuf, ELF_MDY_SEC_INFO_S *info);

#ifdef __cplusplus
}
#endif
#endif 
