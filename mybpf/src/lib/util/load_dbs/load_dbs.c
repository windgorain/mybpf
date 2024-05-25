/*********************************************************
*   Copyright (C) LiXingang
*   Description: load dynamic bs.so
*
********************************************************/
#include "bs.h"
#include "utl/load_dbs.h"
#include "utl/plug_utl.h"

typedef void (*PF_SYSINFO_SetConfDir)(const char *conf_dir);
typedef int (*PF_LoadBs_Init)(void);

int LOADDBS_Load(const char *filepath, const char *conf_path)
{
    PF_SYSINFO_SetConfDir set_conf_func;
    PF_LoadBs_Init load_bs_func;

	PLUG_HDL ulPlugId = PLUG_LOAD((char*)filepath);
    if (! ulPlugId) {
        RETURN(BS_CAN_NOT_OPEN);
    }

    set_conf_func = PLUG_GET_FUNC_BY_NAME(ulPlugId, "SYSINFO_SetConfDir");
    load_bs_func = PLUG_GET_FUNC_BY_NAME(ulPlugId, "LoadBs_Init");
    if ((! set_conf_func) || (! load_bs_func)) {
        RETURN(BS_NOT_FOUND);
    }

    set_conf_func(conf_path);

    return load_bs_func();
}


