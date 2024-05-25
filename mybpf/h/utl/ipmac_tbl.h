/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2009-5-4
* Description: 
* History:     
******************************************************************************/

#ifndef __IPMAC_TBL_H_
#define __IPMAC_TBL_H_

#include "utl/hash_utl.h"
#include "utl/eth_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 

typedef HANDLE IPMAC_HANDLE;

typedef struct
{
    HASH_NODE_S stHashNode;
    UINT uiIp;
    MAC_ADDR_S stMac;
    UINT uiUsrContext;
}IPMAC_TBL_NODE_S;

typedef VOID  (*PF_IPMAC_TBL_WALK_FUNC)(IN IPMAC_TBL_NODE_S *pstNode, IN VOID * pUserHandle);

IPMAC_HANDLE IPMAC_TBL_CreateInstance();
VOID IPMAC_TBL_DelInstance(IN IPMAC_HANDLE hInstance);
IPMAC_TBL_NODE_S * IPMAC_TBL_Find(IN IPMAC_HANDLE hInstance, IN UINT uiIp);
IPMAC_TBL_NODE_S * IPMAC_TBL_Add(IN IPMAC_HANDLE hInstance, IN UINT uiIp, IN MAC_ADDR_S *pstMac);
VOID IPMAC_TBL_Del(IN IPMAC_HANDLE hInstance, IN UINT uiIp);
VOID IPMAC_TBL_Walk(IN IPMAC_HANDLE hInstance, IN PF_IPMAC_TBL_WALK_FUNC pfFunc, IN VOID *pUserHandle);

#ifdef __cplusplus
    }
#endif 

#endif 


