/*********************************************************
*   Copyright (C) LiXingang
*   Description: 
*
********************************************************/
#ifndef _ELF_DEF_H
#define _ELF_DEF_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    char *sec_name;
    char *func_name;
    UINT sec_offset; 
    UINT func_offset; 
    UINT func_size; 
    int  sec_id; 
}ELF_PROG_INFO_S;

#ifdef __cplusplus
}
#endif
#endif 
