/*================================================================
*   Created：LiXingang All rights reserved.
*   Description：
*
================================================================*/
#ifndef __LPM_UTL_H_
#define __LPM_UTL_H_

#include "utl/free_list.h"
#include "utl/map_utl.h"

#ifdef __cplusplus
extern "C"
{
#endif

enum {
    LPM_ENTRY_STATE_FREE = 0, 
    LPM_ENTRY_STATE_INVALID,  
    LPM_ENTRY_STATE_VALID,    
    LPM_ENTRY_STATE_GROUP     
};

typedef struct {
    UINT state:2;
    UINT depth:6;
    UINT nexthop:24;
}LPM_ENTRY_S;

typedef struct {
    UINT state:2;
    UINT depth:6;
    UINT reserved:24;
    UINT nexthop;
}LPM32B_ENTRY_S;

typedef struct {
    UINT state:2;
    UINT depth:6;
    UINT reserved:24;
    UINT64 nexthop;
}LPM64B_ENTRY_S;

typedef int (*PF_LPM_WALK_CB)(UINT ip, int depth, UINT64 nexthop, void *ud);

typedef void (*PF_LPM_Reset)(void *lpm);
typedef void (*PF_LPM_Final)(void *lpm);
typedef int (*PF_LPM_SetLevel)(void *lpm, int level, int first_bit_num);
typedef int (*PF_LPM_Add)(void *lpm, UINT ip, UCHAR depth, UINT64 nexthop);
typedef int (*PF_LPM_Del)(void *lpm, UINT ip, UCHAR depth, UCHAR new_depth, UINT64 new_nexthop);
typedef int (*PF_LPM_Lookup)(void *lpm, UINT ip, UINT64 *next_hop);
typedef void (*PF_LPM_Walk)(void *lpm, PF_LPM_WALK_CB walk_func, void *ud);

typedef struct {
    PF_LPM_Reset reset_func;
    PF_LPM_Final final_func;
    PF_LPM_SetLevel set_level_func;
    PF_LPM_Add add_func;
    PF_LPM_Del del_func;
    PF_LPM_Lookup lookup_func;
    PF_LPM_Walk walk_func;
}LPM_FUNC_S;

typedef struct {
    UCHAR bit_num[32];
    UINT level: 8;
    UINT array_alloced: 1;
    UINT reserved: 23;
    UINT array_size;
    FREE_LIST_S free_list;
    void *memcap;
    MAP_HANDLE recording_map;
    LPM_FUNC_S *funcs;
    void *array; 
}LPM_S;


int LPM_Init(IN LPM_S *lpm, IN UINT array_size, IN LPM_ENTRY_S *array );


int LPM32B_Init(IN LPM_S *lpm, IN UINT array_size, IN LPM32B_ENTRY_S *array );


int LPM64B_Init(IN LPM_S *lpm, IN UINT array_size, IN LPM64B_ENTRY_S *array );


int DLPM64B_Init(IN LPM_S *lpm, void *memcap);

int LPM_EnableRecording(IN LPM_S *lpm);
int LPM_Add(IN LPM_S *lpm, UINT ip, UCHAR depth, UINT64 nexthop);
void LPM_Reset(LPM_S *lpm);
void LPM_Final(LPM_S *lpm);
int LPM_Del(IN LPM_S *lpm, UINT ip, UCHAR depth, UCHAR new_depth, UINT64 new_nexthop);
int LPM_FindRecording(LPM_S *lpm, UINT ip, UCHAR depth, OUT UINT64 *nexthop);
int LPM_WalkRecording(LPM_S *lpm, PF_LPM_WALK_CB walk_func, void *ud);

static inline int LPM_SetLevel(LPM_S *lpm, int level, int first_bit_num) {
    return lpm->funcs->set_level_func(lpm, level, first_bit_num);
}


static inline int LPM_Lookup(LPM_S *lpm, UINT ip, UINT64 *next_hop)
{
    return lpm->funcs->lookup_func(lpm, ip, next_hop);
}

static inline void LPM_Walk(LPM_S *lpm, PF_LPM_WALK_CB walk_func, void *ud)
{
    return lpm->funcs->walk_func(lpm, walk_func, ud);
}

#ifdef __cplusplus
}
#endif
#endif 
