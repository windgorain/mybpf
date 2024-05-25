/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _MATCH_UTL_H
#define _MATCH_UTL_H
#ifdef __cplusplus
extern "C"
{
#endif

#if 1 

typedef void* MATCH_HANDLE;

typedef BOOL_T (*PF_MATCH_COUNT_CMP)(void *pattern, void *key);

MATCH_HANDLE Match_Create(int max, int pattern_size,
        PF_MATCH_COUNT_CMP pfcmp);
void Match_Destroy(MATCH_HANDLE head);
int Match_SetPattern(MATCH_HANDLE head, int index, void *pattern);
void * Match_GetPattern(MATCH_HANDLE head, int index);
void Match_Enable(MATCH_HANDLE head, int index);
void Match_Disable(MATCH_HANDLE head, int index);
BOOL_T Match_IsEnable(MATCH_HANDLE head, int index);
void Match_SetUD(MATCH_HANDLE head, int index, void *ud);
void * Match_GetUD(MATCH_HANDLE head, int index);

int Match_Do(MATCH_HANDLE head, void *key);
UINT64 Match_GetMatchedCount(MATCH_HANDLE head, int index);
void Match_ResetMatchedCount(MATCH_HANDLE head, int index);
void Match_ResetAllMatchedCount(MATCH_HANDLE head);

#endif


#if 1 
typedef struct {
    UINT sip;
    UINT dip;
    USHORT sport;
    USHORT dport;
    UCHAR protocol;
}IP_MATCH_KEY_S;

typedef struct {
    UINT sip;
    UINT sip_mask;
    UINT dip;
    UINT dip_mask;
    USHORT sport;
    USHORT dport;
    UCHAR protocol;
}IP_MATCH_PATTERN_S;

MATCH_HANDLE IPMatch_Create(UINT max);
void IPMatch_LoadConfig(MATCH_HANDLE head, char *file);
#endif

#if 1 
MATCH_HANDLE UintMatch_Create(UINT max);
#endif

#if 1 
typedef struct {
    void *pkt;
    int len;
}BPF_MATCH_KEY_S;
MATCH_HANDLE BpfMatch_Create(UINT max);
#endif


#ifdef __cplusplus
}
#endif
#endif 

