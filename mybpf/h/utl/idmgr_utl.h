/*================================================================
*   Created by LiXingang
*   Description: ID管理器
*
================================================================*/
#ifndef _IDMGR_UTL_H
#define _IDMGR_UTL_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef void (*PF_IDMGR_Destroy)(void *tbl);
typedef INT64 (*PF_IDMGR_Alloc)(void *tbl);
typedef int (*PF_IDMGR_Set)(void *tbl, INT64 id);
typedef void (*PF_IDMGR_Free)(void *tbl, INT64 id);
typedef BOOL_T (*PF_IDMGR_IsExist)(void *tbl, INT64 id);
typedef INT64 (*PF_IDMGR_GetNext)(void *tbl, INT64 curr);
typedef void (*PF_IDMGR_Reset)(void *tbl);

typedef struct {
    PF_IDMGR_Destroy destroy_func;
    PF_IDMGR_Alloc alloc_func;
    PF_IDMGR_Set set_func;
    PF_IDMGR_Free free_func;
    PF_IDMGR_IsExist isexist_func;
    PF_IDMGR_GetNext getnext_func;
    PF_IDMGR_Reset reset_func;
}IDMGR_FUNC_S;

typedef struct {
    IDMGR_FUNC_S *funcs;
}IDMGR_S;

IDMGR_S * IDMGR_BitmapCreate(INT64 max_index);

static inline void IDMGR_Destroy(IDMGR_S *tbl)
{
    tbl->funcs->destroy_func(tbl);
}

static inline INT64 IDMGR_Alloc(IDMGR_S *tbl)
{
    return tbl->funcs->alloc_func(tbl);
}

static inline int IDMGR_Set(IDMGR_S *tbl, INT64 id)
{
    return tbl->funcs->set_func(tbl, id);
}

static inline void IDMGR_Free(IDMGR_S *tbl, INT64 id)
{
    tbl->funcs->free_func(tbl, id);
}

static inline BOOL_T IDMGR_IsExist(IDMGR_S *tbl, INT64 id)
{
    return tbl->funcs->isexist_func(tbl, id);
}

static inline INT64 IDMGR_GetNext(IDMGR_S *tbl, INT64 curr)
{
    return tbl->funcs->getnext_func(tbl, curr);
}

static inline void IDMGR_Reset(IDMGR_S *tbl)
{
    tbl->funcs->reset_func(tbl);
}

#ifdef __cplusplus
}
#endif
#endif 
