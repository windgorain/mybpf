/******************************************************************************
* Copyright (C), LiXingang
* Author:      LiXingang  Date: 2016-5-23
* Description: 
* History:     
******************************************************************************/

#ifndef __ORG_TREE_H_
#define __ORG_TREE_H_

#ifdef __cplusplus
    extern "C" {
#endif 

typedef HANDLE ORG_TREE_HANDLE;

typedef int (*PF_OrgTree_Walk_Func)(IN ORG_TREE_HANDLE hUserTree, IN UINT64 uiID, IN HANDLE hUserHandle);

VOID OrgTree_Destroy(IN ORG_TREE_HANDLE hOrgTree);

#ifdef __cplusplus
    }
#endif 

#endif 


