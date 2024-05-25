/******************************************************************************
* Copyright (C), LiXingang
* Author:      LiXingang  Version: 1.0  Date: 2015-6-23
* Description: 
* History:     
******************************************************************************/

#ifndef __CFF_INNER_H_
#define __CFF_INNER_H_

#include "utl/mkv_utl.h"
#include "utl/file_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 

typedef VOID (*PF_CFF_SAVE_CB)(IN CHAR *buf, IN VOID *user_data);
typedef BS_STATUS (*PF_CFF_Save)(IN CFF_HANDLE hCff, IN PF_CFF_SAVE_CB pfFunc, IN VOID *user_data);

typedef struct
{
    PF_CFF_Save pfSave;
}_CFF_FUNC_TBL_S;

typedef struct
{
    _CFF_FUNC_TBL_S *pstFuncTbl;
    MKV_MARK_S stCfgRoot;  
    UINT uiFlag;
    CHAR *pcFileName;
    FILE_MEM_S file_mem;
    CHAR *pcFileContent;
}_CFF_S;

_CFF_S * _cff_Open(IN CHAR *pcFileName, IN UINT uiFlag);
_CFF_S * _cff_OpenBuf(IN CHAR *buf, IN UINT flag);
BOOL_T _ccf_IsSort(IN _CFF_S *pstCff);

#ifdef __cplusplus
    }
#endif 

#endif 


