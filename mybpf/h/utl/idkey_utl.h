/*================================================================
*   Created by LiXingang
*   Description:  根据ID/KEY管理数据
*      根据ID查找data
*      根据key查找data
*      ID/KEY互相查找
*
================================================================*/
#ifndef _IDKEY_UTL_H
#define _IDKEY_UTL_H

#include "utl/idmgr_utl.h"
#include "utl/map_utl.h"
#ifdef __cplusplus
extern "C"
{
#endif

#define IDKEY_FLAG_ID_MAP 0x1 
#define IDKEY_FLAG_KEY_MAP 0x1 

#define IDKEY_NODE_FLAG_DUP_KEY    0x1 

typedef HANDLE IDKEY_HDL;

typedef struct {
    void *key;
    int key_len;
}IDKEY_KL_S;

typedef void (*PF_IDKEY_FREE_FUNC)(void *data, void *ud);
typedef void (*PF_IDKEY_WALK_FUNC)(IDKEY_KL_S *kl, INT64 id, void *data, void *ud);


typedef void (*PF_IDKEY_Destroy)(IDKEY_HDL hCtrl, PF_IDKEY_FREE_FUNC free_func, void *ud);
typedef void (*PF_IDKEY_Reset)(IDKEY_HDL hCtrl, PF_IDKEY_FREE_FUNC free_func, void *ud);
typedef void (*PF_IDKEY_SetCapacity)(IDKEY_HDL hCtrl, INT64 capacity);
typedef int (*PF_IDKEY_SET)(IDKEY_HDL hCtrl, INT64 id, void *key, int key_len, void *data, UINT flag);
typedef INT64 (*PF_IDKEY_Add)(IDKEY_HDL hCtrl, void *key, int key_len, void *data, UINT flag);
typedef void * (*PF_IDKEY_DelByKey)(IDKEY_HDL hCtrl, void *key, int key_len);
typedef void * (*PF_IDKEY_DelByID)(IDKEY_HDL hCtrl, INT64 id);
typedef INT64 (*PF_IDKEY_GetIDByKey)(IDKEY_HDL hCtrl, void *key, int key_len);
typedef IDKEY_KL_S * (*PF_IDKEY_GetKeyByID)(IDKEY_HDL hCtrl, INT64 id);
typedef void * (*PF_IDKEY_GetDataByID)(IDKEY_HDL hCtrl, INT64 id);
typedef void * (*PF_IDKEY_GetDataByKey)(IDKEY_HDL hCtrl, void *key, int key_len);
typedef void (*PF_IDKEY_Walk)(IDKEY_HDL hCtrl, PF_IDKEY_WALK_FUNC walk_func, void *ud);

typedef struct {
    PF_IDKEY_Destroy destroy;
    PF_IDKEY_Reset reset;
    PF_IDKEY_SetCapacity set_capacity;
    PF_IDKEY_SET set;
    PF_IDKEY_Add add;
    PF_IDKEY_DelByKey del_by_key;
    PF_IDKEY_DelByID del_by_id;
    PF_IDKEY_GetIDByKey get_id_by_key;
    PF_IDKEY_GetKeyByID get_key_by_id;
    PF_IDKEY_GetDataByID get_data_by_id;
    PF_IDKEY_GetDataByKey get_data_by_key;
    PF_IDKEY_Walk walk;
}IDKEY_FUNC_S;

typedef struct {
    IDKEY_FUNC_S *funcs;
}IDKEY_S;

typedef struct {
    INT64 id;
    UINT flag;
    void *data;
    IDKEY_KL_S kl;
}IDKEY_NODE_S;

typedef struct {
    IDKEY_FUNC_S *funcs;
    void *memcap;
	INT64 id_to_use;
    MAP_HANDLE id_tbl;
    MAP_HANDLE key_tbl;
}IDKEY_IMPL_S;

typedef struct {
    void *memcap;
    int bucket_num;
    UINT flag;
}IDKEY_PARAM_S;


IDKEY_S * IDKEY_BaseCreate(IDKEY_PARAM_S *p);


IDKEY_S * IDKEY_WrapperIdmgrCreate(IDKEY_S *impl, IDMGR_S *idmgr);

static inline void IDKEY_Destroy(IDKEY_S *ctrl, PF_IDKEY_FREE_FUNC free_func, void *ud)
{
    ctrl->funcs->destroy(ctrl, free_func, ud);
}

static inline void IDKEY_Reset(IDKEY_S *ctrl, PF_IDKEY_FREE_FUNC free_func, void *ud)
{
    ctrl->funcs->reset(ctrl, free_func, ud);
}

static inline void IDKEY_SetCapacity(IDKEY_S *ctrl, INT64 capacity)
{
    ctrl->funcs->set_capacity(ctrl, capacity);
}

static inline int IDKEY_Set(IDKEY_S *ctrl, INT64 id, void *key, int key_len, void *data, UINT flag)
{
    return ctrl->funcs->set(ctrl, id, key, key_len, data, flag);
}

static inline INT64 IDKEY_Add(IDKEY_S *ctrl, void *key, int key_len, void *data, UINT flag)
{
    return ctrl->funcs->add(ctrl, key, key_len, data, flag);
}


static inline void * IDKEY_DelByKey(IDKEY_S *ctrl, void *key, int key_len)
{
    return ctrl->funcs->del_by_key(ctrl, key, key_len);
}


static inline void * IDKEY_DelByID(IDKEY_S *ctrl, INT64 id)
{
    return ctrl->funcs->del_by_id(ctrl, id);
}

static inline INT64 IDKEY_GetIDByKey(IDKEY_S *ctrl, void *key, int key_len)
{
    return ctrl->funcs->get_id_by_key(ctrl, key, key_len);
}

static inline IDKEY_KL_S * IDKEY_GetKeyByID(IDKEY_S *ctrl, INT64 id)
{
    return ctrl->funcs->get_key_by_id(ctrl, id);
}

static inline void * IDKEY_GetDataByID(IDKEY_S *ctrl, INT64 id)
{
    return ctrl->funcs->get_data_by_id(ctrl, id);
}

static inline void * IDKEY_GetDataByKey(IDKEY_S *ctrl, void *key, int key_len)
{
    return ctrl->funcs->get_data_by_key(ctrl, key, key_len);
}

static inline void IDKEY_Walk(IDKEY_S *ctrl, PF_IDKEY_WALK_FUNC walk_func, void *ud)
{
    ctrl->funcs->walk(ctrl, walk_func, ud);
}

#ifdef __cplusplus
}
#endif
#endif 
