/*================================================================
*   Created by LiXingang
*   Description: 插件管理
*
================================================================*/
#ifndef _PLUG_MGR_H
#define _PLUG_MGR_H
#include "utl/cff_utl.h"
#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
    PLUG_STAGE_PLUG_LOAD = 1,      
    PLUG_STAGE_PLUG_LOADED,        
    PLUG_STAGE_CMD_REG0,           
    PLUG_STAGE_CMD_REGED0,         
    PLUG_STAGE_CMD_REG,            
    PLUG_STAGE_CMD_REGED,          
    PLUG_STAGE_CFG0_LOAD,          
    PLUG_STAGE_CFG0_LOADED,        
    PLUG_STAGE_CFG_LOAD,           
    PLUG_STAGE_CFG_LOADED,         
    PLUG_STAGE_RUNNING,            
    PLUG_STAGE_STOP,               
    PLUG_STAGE_CMD_UNREG,          
    PLUG_STAGE_PLUG_UNLOAD         
}PLUG_STAGE_E;

typedef int (*PF_PLUG_STAGE)(int stage, void *env);


#define PLUG_MAIN \
    PLUG_API int PlugMain (char *file, char *conf_path, char *save_path)  \
    {   \
        LOCAL_INFO_SetHost(file);    \
        LOCAL_INFO_SetConfPath(conf_path);    \
        LOCAL_INFO_SetSavePath(save_path);    \
        return 0; \
    }


typedef struct {
    DLL_NODE_S link_node; 
    char *plug_name;
    char *filename;
    PLUG_HDL hPlug;
}PLUG_MGR_NODE_S;

typedef struct {
    DLL_HEAD_S plug_list;
    int stage;
}PLUG_MGR_S;

int PlugMgr_Init(PLUG_MGR_S *plug_mgr);
int PlugMgr_LoadByCfgFile(PLUG_MGR_S *plug_mgr, char *cfg_file);
int PlugMgr_LoadManual(PLUG_MGR_S *mgr, char *ini_file, char *tag);
int PlugMgr_Unload(PLUG_MGR_S *plug_mgr, char *ini_file, char *tag);
PLUG_MGR_NODE_S * PlugMgr_Find(PLUG_MGR_S *mgr, char *plug_name);
PLUG_MGR_NODE_S * PlugMgr_Next(PLUG_MGR_S *mgr, PLUG_MGR_NODE_S *curr);
int PlugMgr_GetStage(PLUG_MGR_S *mgr);
char * PlugMgr_GetSavePathByEnv(void *env);

#ifdef __cplusplus
}
#endif
#endif 
