/******************************************************************
*   Created by LiXingang
*   Description: 
*
******************************************************************/
#ifndef _MYBPF_IOCTL_DEF_H
#define _MYBPF_IOCTL_DEF_H

#include "utl/mybpf_utl.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    IN void *in_data;   
    OUT void *out_data; 
    IN U32 in_data_len; 
    IN U32 out_data_size; 
}MYBPF_IOCTL_S;

enum {
    SPF_IOCTL_GET_INSTANCE = 0,     
    SPF_IOCTL_INSTANCE_EXIST,       
    SPF_IOCTL_GET_NAMEFUNC,         
    SPF_IOCTL_GET_IDFUNC,           
    SPF_IOCTL_GET_EVOB,             
};

typedef struct {
    char name[MYBPF_NAME_SIZE];
    char filename[256];
}SPF_INSTANCE_S;

typedef struct {
    char name[MYBPF_NAME_SIZE];
    char func_name[MYBPF_NAME_SIZE];
    void *entry;
}SPF_NAMEFUNC_S;

typedef struct {
    U32 id;
    char func_name[MYBPF_NAME_SIZE];
    void *entry;
}SPF_IDFUNC_S;

typedef struct {
    U32 event;
    char func_name[MYBPF_NAME_SIZE];
    void *entry;
}SPF_EVOB_S;

#ifdef __cplusplus
}
#endif
#endif 
