/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _DOMAIN_DUBIOUS_H
#define _DOMAIN_DUBIOUS_H
#include "utl/trie_utl.h"
#ifdef __cplusplus
extern "C"
{
#endif

enum {
    DOMAIN_DUBIOUS_ADD_WHITE = 1, 
    DOMAIN_DUBIOUS_ADD_BLACK,     
};

enum {
    DOMAIN_DUBIOUS_RESULT_OK = 0, 
    DOMAIN_DUBIOUS_RESULT_DGA,    
    DOMAIN_DUBIOUS_RESULT_BLACK,  
};

typedef struct {
    TRIE_HANDLE domain_name_trie;
}DOMAIN_DUBIOUS_S;

int DomainDubious_Init(DOMAIN_DUBIOUS_S *ctrl);



int DomainDubious_Add(DOMAIN_DUBIOUS_S *ctrl, char *domain_name, int domain_name_len, int type);
int DomainDubious_LoadFile(DOMAIN_DUBIOUS_S *ctrl, char *file, int type);


int DomainDubious_Check(DOMAIN_DUBIOUS_S *ctrl, char *domain_name, int domain_name_len);

#ifdef __cplusplus
}
#endif
#endif 
