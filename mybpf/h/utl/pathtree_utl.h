/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2010-1-14
* Description: 
* History:     
******************************************************************************/

#ifndef __PATHTREE_UTL_H_
#define __PATHTREE_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

#include "utl/tree_utl.h"

typedef struct
{
    TREE_NODE_S stTreeNode;
    CHAR *pszDirName;
    VOID *pUserHandle;
}PATHTREE_NODE_S;

#define PATHTREE_IS_ROOT(pstNode) TREE_IS_ROOT(&((pstNode)->stTreeNode))
#define PATHTREE_IS_LEAF(pstNode) TREE_IS_LEAF(&((pstNode)->stTreeNode))
#define PATHTREE_DIR_NAME(pstNode) ((pstNode)->pszDirName)
#define PATHTREE_USER_HANDLE(pstNode) ((pstNode)->pUserHandle)


typedef int (*PF_PathTree_DepthWalkNode)(IN PATHTREE_NODE_S *pstNode, IN UINT ulDeepth, IN BOOL_T bIsBack, IN VOID * pUserHandle);
typedef VOID (*PF_PathTree_DestoryNode)(IN PATHTREE_NODE_S *pstNode, IN VOID * pUserHandle);


PATHTREE_NODE_S * PathTree_Create();

VOID PathTree_Destory
(
    IN PATHTREE_NODE_S *pstRoot,
    IN PF_PathTree_DestoryNode pfFunc,
    IN VOID *pUserHandle
);

BS_STATUS PathTree_AddPath(IN PATHTREE_NODE_S *pstRoot, IN CHAR *pszPath, IN VOID * pUserHandle);

int PathTree_DepthBackWalk(PATHTREE_NODE_S *pstRoot, PF_PathTree_DepthWalkNode pfFunc, VOID * pUserHandle);


#ifdef __cplusplus
    }
#endif 

#endif 


