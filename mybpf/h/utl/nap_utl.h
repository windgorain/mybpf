/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2014-11-16
* Description: node id pool
* History:     
******************************************************************************/

#ifndef __NAP_UTL_H_
#define __NAP_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

#define NAP_INVALID_ID    0 
#define NAP_INVALID_INDEX ((UINT)(INT)-1)

typedef VOID* NAP_HANDLE;

typedef enum
{
    NAP_TYPE_ARRAY = 0,   
    NAP_TYPE_PTR_ARRAY,   
    NAP_TYPE_HASH,        
    NAP_TYPE_AVL,         
}NAP_TYPE_E;

typedef struct {
    void *memcap;    
    NAP_TYPE_E enType;
    UINT uiMaxNum;   
    UINT uiNodeSize;
}NAP_PARAM_S;

NAP_HANDLE NAP_Create(NAP_PARAM_S *p);


BS_STATUS NAP_EnableSeq(HANDLE hNAPHandle, UINT ulSeqMask, UINT uiSeqCount);
UINT NAP_GetNodeSize(IN NAP_HANDLE hNapHandle);

VOID * NAP_Alloc(IN NAP_HANDLE hNapHandle);
VOID * NAP_ZAlloc(IN NAP_HANDLE hNapHandle);

VOID * NAP_AllocByIndex(NAP_HANDLE hNapHandle, UINT uiSpecIndex);
VOID * NAP_ZAllocByIndex(IN NAP_HANDLE hNapHandle, IN UINT uiSpecIndex);

VOID * NAP_AllocByID(IN NAP_HANDLE hNapHandle, IN UINT ulSpecID);
VOID * NAP_ZAllocByID(IN NAP_HANDLE hNapHandle, IN UINT ulSpecID);
VOID NAP_Free(IN NAP_HANDLE hNapHandle, IN VOID *pstNode);
VOID NAP_FreeByID(IN NAP_HANDLE hNapHandle, IN UINT ulID);
VOID NAP_FreeByIndex(IN NAP_HANDLE hNapHandle, IN UINT index);
VOID NAP_FreeAll(IN NAP_HANDLE hNapHandle);
VOID * NAP_GetNodeByID(IN NAP_HANDLE hNAPHandle, IN UINT ulID);
VOID * NAP_GetNodeByIndex(IN NAP_HANDLE hNAPHandle, IN UINT uiIndex);
UINT NAP_GetIDByNode(IN NAP_HANDLE hNAPHandle, IN VOID *pstNode);
UINT NAP_GetIDByIndex(IN NAP_HANDLE hNAPHandle, UINT index);
UINT NAP_GetIndexByID(IN NAP_HANDLE hNAPHandle, UINT id);
UINT NAP_GetIndexByNode(IN NAP_HANDLE hNAPHandle, void *node);
VOID NAP_Destory(IN NAP_HANDLE hNAPHandle);
void * NAP_GetMemCap(NAP_HANDLE hNAPHandle);

UINT NAP_GetNextIndex(IN NAP_HANDLE hNAPHandle, IN UINT uiCurrentIndex);
UINT NAP_GetNextID(IN NAP_HANDLE hNAPHandle, IN UINT ulCurrentID);
UINT NAP_GetCount(IN NAP_HANDLE hNAPHandle);

#ifdef __cplusplus
    }
#endif 

#endif 


