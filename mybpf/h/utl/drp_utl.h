/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2014-7-24
* Description: deep replace   深度替换
* History:     
******************************************************************************/

#ifndef __DRP_UTL_H_
#define __DRP_UTL_H_

#include "utl/mbuf_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 

typedef VOID* DRP_HANDLE;
typedef VOID* DRP_FILE;
 
typedef BS_STATUS (*PF_DRP_SOURCE_FUNC)(IN DRP_HANDLE hDrp, IN LSTR_S *pstKey, IN VOID *pDrpCtx, IN HANDLE hUserHandle, IN HANDLE hUserHandle2);

typedef struct
{
    DLL_NODE_S stLinkNode;

    CHAR *pcKey;
    UINT uiKeyLen;
 
    PF_DRP_SOURCE_FUNC pfSourceFunc;
    HANDLE hUserHandle2;
}DRP_NODE_S;


#if 1
DRP_HANDLE DRP_Create(IN CHAR *pcStartTag, IN CHAR *pcEndTag);
VOID DRP_Destory(IN DRP_HANDLE hDrp);
BS_STATUS DRP_Set(IN DRP_HANDLE hDrp, IN CHAR *pcKey, IN PF_DRP_SOURCE_FUNC pfFunc, IN HANDLE hUserHandle2);
DRP_NODE_S * DRP_Find(IN DRP_HANDLE hDrp, IN CHAR *pcKey, IN UINT uiKeyLen);
BS_STATUS DRP_CtxOutput(IN VOID *pDrpCtx, IN void *data, IN UINT uiDataLen);
static inline BS_STATUS DRP_CtxOutputString(IN VOID *pDrpCtx, IN CHAR *pcString)
{
    return DRP_CtxOutput(pDrpCtx, pcString, strlen(pcString));
}
#endif

#if 1
CHAR * DRP_FileGetETag(IN DRP_HANDLE hDrp, IN CHAR *pcFile);
DRP_FILE DRP_FileOpen(IN DRP_HANDLE hDrp, IN CHAR *pcFile, IN HANDLE hUserHandle);
VOID DRP_FileClose(IN DRP_FILE hFile);

INT DRP_FileRead(IN DRP_FILE hFile, OUT UCHAR *pucData, IN UINT uiDataLen);
BOOL_T DRP_FileEOF(IN DRP_FILE hFile);
UINT64 DRP_FileLength(IN DRP_FILE hFile);
#endif

#ifdef __cplusplus
    }
#endif 

#endif 


