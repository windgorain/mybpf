/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _LUA_FILE_MGR_H
#define _LUA_FILE_MGR_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef enum {
    LUA_FM_PARAM_TYPE_INVALID = 0,
    LUA_FM_PARAM_TYPE_INT,
    LUA_FM_PARAM_TYPE_STRING,
}LUA_FM_PARAM_TYPE_E;

typedef struct {
    DLL_NODE_S link_node;
    char *filename;
}LUA_FILE_S;

typedef struct {
    DLL_HEAD_S list;
}LUA_FM_S;

typedef struct {
    int type;  
    char *name;
    void *value;
}LUA_PARAM_S;

#define LUA_FM_MAX_PARAM 32

typedef struct {
    char *func;
    USHORT param_count;
    LUA_PARAM_S params[LUA_FM_MAX_PARAM];
}LUA_ENV_S;

static inline void LuaFM_EnvInit(LUA_ENV_S *env)
{
    env->param_count = 0;
}

int LuaFM_Init(LUA_FM_S * lfm);
int luaFM_Add(LUA_FM_S *lfm, LUA_FILE_S *luaf);
void luaFM_Del(LUA_FM_S *lfm, LUA_FILE_S *luaf);
LUA_FILE_S * LuaFM_Find(LUA_FM_S *lfm, char *file);
int LuaFM_MallocAndAdd(LUA_FM_S *lfm, char *file);
int LuaFM_DelAndFree(LUA_FM_S *lfm, char *file);
LUA_FILE_S * LuaFM_Next(LUA_FM_S *lfm, LUA_FILE_S *curr);
int LuaFM_EnvAddParam(LUA_ENV_S *env, int type, char *name, void *value);
int LuaFM_Call(LUA_FM_S *lfm, LUA_ENV_S *env);

#ifdef __cplusplus
}
#endif
#endif 
