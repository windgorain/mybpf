/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2007-6-3
* Description: 
* History:     
******************************************************************************/


#define RETCODE_FILE_NUM RETCODE_FILE_NUM_CONNPOOL

#include "bs.h"

#include "utl/conn_pool.h"
#include "utl/txt_utl.h"
#include "utl/ssltcp_utl.h"



typedef struct
{
    DLL_NODE_S stDllNode;
    CHAR *pcConnName;
    UINT hConnHandle;
}S_CONN_POOL_NODE;

typedef struct
{
    DLL_HEAD_S stDllHead;
    UINT ulMaxNodeNum;
}S_CONN_POOL_CTRL;


BS_STATUS CONN_POOL_Create(IN UINT ulMaxNodeNum, OUT HANDLE *phHandle)
{
    S_CONN_POOL_CTRL *pstCtrl;
    
    if (phHandle == NULL)
    {
        BS_WARNNING(("Null ptr!"));
        RETURN(BS_NULL_PARA);
    }

    pstCtrl = MEM_Malloc(sizeof(S_CONN_POOL_CTRL));
    if (pstCtrl == NULL)
    {
        RETURN(BS_NO_MEMORY);
    }
    Mem_Zero(pstCtrl, sizeof(S_CONN_POOL_CTRL));

    DLL_INIT(&pstCtrl->stDllHead);
    pstCtrl->ulMaxNodeNum = ulMaxNodeNum;

    *phHandle = pstCtrl;

    return BS_OK;
}

BS_STATUS CONN_POOL_Delete(IN HANDLE hHandle)
{
    S_CONN_POOL_CTRL *pstCtrl = (S_CONN_POOL_CTRL *)hHandle;
    DLL_NODE_S *pstNode, *pstNodeTmp;
    S_CONN_POOL_NODE *pstConnNode;

    if (hHandle == 0)
    {
        BS_WARNNING(("Null ptr!"));
        RETURN(BS_NULL_PARA);
    }

    DLL_SAFE_SCAN(&pstCtrl->stDllHead,pstNode,pstNodeTmp)
    {
        pstConnNode = (void*)pstNode;
        SSLTCP_Close(pstConnNode->hConnHandle);
        DLL_DEL(&pstCtrl->stDllHead, pstNode);
        MEM_Free(pstConnNode);
    }

    MEM_Free(pstCtrl);
    
    return BS_OK;
}


BS_STATUS CONN_POOL_Add(IN HANDLE hHandle, IN CHAR *pcConnName, IN UINT hConnHandle)
{
    S_CONN_POOL_NODE *pstNode;
    UINT ulLen;
    S_CONN_POOL_CTRL *pstCtrl = (S_CONN_POOL_CTRL *)hHandle;

    if ((pcConnName == NULL) || (hHandle == 0))
    {
        BS_WARNNING(("Null ptr!"));
        RETURN(BS_NULL_PARA);
    }

    if (DLL_COUNT(&pstCtrl->stDllHead) >= pstCtrl->ulMaxNodeNum)
    {
        RETURN(BS_FULL);
    }

    ulLen = sizeof(S_CONN_POOL_NODE) + strlen(pcConnName) + 1;
    pstNode = MEM_Malloc(ulLen);
    if (pstNode == NULL)
    {
        RETURN(BS_NO_MEMORY);
    }
    Mem_Zero(pstNode, ulLen);

    pstNode->pcConnName = (CHAR*)(pstNode + 1);
    pstNode->hConnHandle = hConnHandle;
    TXT_StrCpy(pstNode->pcConnName, pcConnName);
    DLL_ADD_TO_HEAD(&pstCtrl->stDllHead, &pstNode->stDllNode);

    return BS_OK;
}

BS_STATUS CONN_POOL_Get(IN HANDLE hHandle, IN CHAR *pcConnName, OUT UINT *phConnHandle)
{
    DLL_NODE_S *pstNode, *pstNodeTmp;
    S_CONN_POOL_NODE *pstConnNode;
    S_CONN_POOL_CTRL *pstCtrl = (S_CONN_POOL_CTRL*) hHandle;

    if ((pcConnName == NULL) || (hHandle == 0) || (phConnHandle == NULL))
    {
        BS_WARNNING(("Null ptr!"));
        RETURN(BS_NULL_PARA);
    }

    DLL_SAFE_SCAN(&pstCtrl->stDllHead,pstNode,pstNodeTmp)
    {
        pstConnNode = (void*)pstNode;
        if (strcmp(pstConnNode->pcConnName, pcConnName) == 0)
        {
            *phConnHandle = pstConnNode->hConnHandle;
            DLL_DEL(&pstCtrl->stDllHead, pstNode);
            MEM_Free(pstConnNode);
            return BS_OK;
        }
    }

    RETURN(BS_NO_SUCH);
}

BS_STATUS CONN_POOL_DelByConnHandle(IN HANDLE hHandle, IN UINT hConnHandle)
{
    DLL_NODE_S *pstNode, *pstNodeTmp;
    S_CONN_POOL_NODE *pstConnNode;
    S_CONN_POOL_CTRL *pstCtrl = (S_CONN_POOL_CTRL *) hHandle;

    if ((hHandle == 0) || (hConnHandle == 0))
    {
        BS_WARNNING (("Null ptr!"));
        RETURN(BS_NULL_PARA);
    }

    DLL_SAFE_SCAN(&pstCtrl->stDllHead,pstNode,pstNodeTmp)
    {
        pstConnNode = (void*)pstNode;
        if (pstConnNode->hConnHandle == hConnHandle)
        {
            DLL_DEL(&pstCtrl->stDllHead, pstNode);
            SSLTCP_Close (hConnHandle);
            MEM_Free (pstConnNode);
            break;
        }
    }

    return BS_OK;
}


