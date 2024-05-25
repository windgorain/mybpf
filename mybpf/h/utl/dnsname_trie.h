/*================================================================
*   Created by LiXingang：2018.11.07
*   Description：
*
================================================================*/
#ifndef _DNSNAME_TRIE_H
#define _DNSNAME_TRIE_H
#include "utl/trie_utl.h"
#ifdef __cplusplus
extern "C"
{
#endif

int DnsNameTrie_InsertExt(TRIE_HANDLE trie_ctrl, char *hostname, int len, PF_TRIE_UD_SET ud_set, void *uh);
int DnsNameTrie_Insert(TRIE_HANDLE trie_ctrl, char *hostname, int len, void *ud);
TRIE_COMMON_S * DnsNameTrie_MatchNode(TRIE_HANDLE trie, char *hostname, UINT len, int match_type);
void * DnsNameTrie_Match(TRIE_HANDLE trie, char *hostname, UINT len, int match_type);

#ifdef __cplusplus
}
#endif
#endif 
