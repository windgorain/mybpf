/*================================================================
*   Created by LiXingang
*   Description: id table. 将ID 到 Data的映射关系放到tbl中
*
================================================================*/
#ifndef _IDTBL_UTL_H
#define _IDTBL_UTL_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef void (*PF_IDTBL_FREE)(void *data, void *ud);

typedef int (*PF_IDTBL_Add)(void *id_tbl, UINT64 id, void *data);
typedef int (*PF_IDTBL_Check_Add)(void **id_tbl, UINT64 id, void *data);
typedef void* (*PF_IDTBL_Get)(void *id_tbl, UINT64 id);
typedef void* (*PF_IDTBL_Del)(void *id_tbl, UINT64 id);
typedef void (*PF_IDTBL_Reset)(void *id_tbl, PF_IDTBL_FREE free_func, void *ud);
typedef void (*PF_IDTBL_Destroy)(void *id_tbl, PF_IDTBL_FREE free_func, void *ud);

typedef struct {
    PF_IDTBL_Add add_func;
    PF_IDTBL_Get get_func;
    PF_IDTBL_Del del_func;
    PF_IDTBL_Reset reset_func;
    PF_IDTBL_Destroy destroy_func;
}IDTBL_FUNC_S;

typedef struct {
    IDTBL_FUNC_S *funcs;
}IDTBL_S;

IDTBL_S * IDTBL_ArrayCreate(UINT max_id);
IDTBL_S * IDTBL_HashCreate(UINT bucket_num);
IDTBL_S * IDTBL_AvlCreate(UINT max_id);

static inline int IDTBL_Add(IDTBL_S *id_tbl, UINT64 id, void *data)
{
    return id_tbl->funcs->add_func(id_tbl, id, data);
}

static inline void * IDTBL_Get(IDTBL_S *id_tbl, UINT64 id)
{
    return id_tbl->funcs->get_func(id_tbl, id);
}

static inline void * IDTBL_Del(IDTBL_S *id_tbl, UINT64 id)
{
    return id_tbl->funcs->del_func(id_tbl, id);
}

static inline void IDTBL_Reset(IDTBL_S *id_tbl, PF_IDTBL_FREE free_func, void *ud)
{
    id_tbl->funcs->reset_func(id_tbl, free_func, ud);
}

static inline void IDTBL_Destroy(IDTBL_S *id_tbl, PF_IDTBL_FREE free_func, void *ud)
{
    id_tbl->funcs->destroy_func(id_tbl, free_func, ud);
}

#ifdef __cplusplus
}
#endif
#endif 
