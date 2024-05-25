/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2008-9-21
* Description: 
* History:     
******************************************************************************/

#ifndef __PHYMNG_UTL_H_
#define __PHYMNG_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 


typedef struct
{
    UINT ulIfIndex;
    UINT ulPhyId;
}PHYMNG_NODE_S;

typedef VOID (*PF_PHYMNG_WALK_FUNC)(IN HANDLE hPhyMngId, IN PHYMNG_NODE_S *pstNode, IN HANDLE hUserHandle);

HANDLE PHYMNG_Create();
BS_STATUS PHYMNG_Add(IN HANDLE hPhyMngId, IN PHYMNG_NODE_S *pstNode);
BS_STATUS PHYMNG_Del(IN HANDLE hPhyMngId, IN PHYMNG_NODE_S *pstNode);
BS_STATUS PHYMNG_Find(IN HANDLE hPhyMngId, INOUT PHYMNG_NODE_S *pstNode);
VOID PHYMNG_Walk(IN HANDLE hPhyMngId, IN PF_PHYMNG_WALK_FUNC pfWalkFunc, IN HANDLE hUserHandle);

#ifdef __cplusplus
    }
#endif 

#endif 


