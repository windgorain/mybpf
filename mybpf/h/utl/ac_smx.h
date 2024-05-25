/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _AC_SMX_H
#define _AC_SMX_H
#ifdef __cplusplus
extern "C"
{
#endif

#define ALPHABET_SIZE    256

#define ACSM_FAIL_STATE   -1
#define ACSM_INIT_STATE   0


typedef int (*PF_AC_Match)(void *mlist, int offset, void *user_data);

typedef void (*PF_AC_USER_FREE)(void *id);

typedef struct _acsm_userdata
{
    uint32_t ref_count;
    void *id;
}ACSMX_USERDATA_S;

typedef struct _acsm_pattern {

    struct  _acsm_pattern *next;
    unsigned char         *patrn;
    int      n;         
    ACSMX_USERDATA_S *udata;
}ACSMX_PATTERN_S;

typedef struct  {

    int      NextState[ ALPHABET_SIZE ];
    int      FailState;
    ACSMX_PATTERN_S *MatchList;
}ACSMX_STATE_TABLE_S;

typedef struct {
    int acsmMaxStates;
    int acsmNumStates;
    ACSMX_PATTERN_S    * acsmPatterns;
    ACSMX_STATE_TABLE_S * acsmStateTable;
    int   bcSize;
    short bcShift[256];
    int numPatterns;
}ACSMX_S;

ACSMX_S * ACSMX_New();
int ACSMX_AddPattern(ACSMX_S * p, UCHAR *pat, int n, void *id);
int ACSMX_Compile(ACSMX_S * acsm);
int ACSMX_Search(ACSMX_S * acsm, unsigned char *Tx, int n,
                   PF_AC_Match match_func,void *data, int* current_state);
void ACSMX_Free(ACSMX_S * acsm, PF_AC_USER_FREE userfree);

#ifdef __cplusplus
}
#endif
#endif 
