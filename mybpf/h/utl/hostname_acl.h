/*================================================================
*   Created by LiXingang: 2018.11.15
*   Description: 
*
================================================================*/
#ifndef _HOSTNAME_ACL_H
#define _HOSTNAME_ACL_H
#include "utl/trie_utl.h"
#ifdef __cplusplus
extern "C"
{
#endif

#define HOSTNAME_ACL_UNDEF  0  
#define HOSTNAME_ACL_PERMIT 1
#define HOSTNAME_ACL_DENY   2
#define HOSTNAME_ACL_BYPASS 3

typedef struct {
    TRIE_HANDLE trie;
}HOSTNAME_ACL_S;

int HostnameACL_Init(HOSTNAME_ACL_S *hostname_acl, char *config_file);
void HostnameACL_Fini(IN HOSTNAME_ACL_S *hostname_acl);
int HostnameACL_Match(HOSTNAME_ACL_S *hostname_acl, char *hostname);

#ifdef __cplusplus
}
#endif
#endif 
