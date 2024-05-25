/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _ELF_SEC_H_
#define _ELF_SEC_H_

#ifdef __cplusplus
extern "C" {
#endif

#define ELF_MAX_SECTIONS 128

typedef struct {
    U16 new_sec_id;
    U64 offset;
}ELF_SEC_CHANGE_INFO_S;

typedef struct {
    ELF_SEC_CHANGE_INFO_S c1[ELF_MAX_SECTIONS];
    ELF_SEC_CHANGE_INFO_S c2[ELF_MAX_SECTIONS];
}ELF_SEC_CHANGE_INFO2_S;

typedef struct {
    int sec_id;
    Elf64_Shdr shdrs[ELF_MAX_SECTIONS];
}ELF_MDY_SEC_INFO_S;

typedef struct {
    ELF_MDY_SEC_INFO_S *sec_info;
    ELF_SEC_CHANGE_INFO_S *change1;
    ELF_SEC_CHANGE_INFO_S *change2;
}ELF_SEC_RECORD_S;

#ifdef __cplusplus
}
#endif
#endif
