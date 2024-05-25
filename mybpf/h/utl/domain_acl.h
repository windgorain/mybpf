/******************************************************************************
* Copyright (C), 2000-2006,  Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2016-10-14
* Description: 
* History:     
******************************************************************************/

#ifndef __DOMAIN_ACL_H_
#define __DOMAIN_ACL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

#include "utl/address_pool_utl.h"
#include "utl/port_pool_utl.h"
#include "utl/domain_group_utl.h"

typedef HANDLE DOMAINACL_HANDLE;
typedef HANDLE DOMAINACL_LIST_HANDLE;

#define ACL_INVALID_LIST_ID 0
#define ACL_POOL_NAME_LEN_MAX 64

#define DOMAINACL_RULE_ID_MAX 10000000


#define DOMAINACL_KEY_SIP         0x01          
#define DOMAINACL_KEY_DPORT      (0x01 << 1)    
#define DOMAINACL_KEY_POOL_SIP   (0x01 << 2)    
#define DOMAINACL_KEY_DOMAIN     (0x01 << 3)
#define DOMAINACL_KEY_POOL_DOAMIN   (0x01 << 4)    
#define DOMAINACL_KEY_POOL_DPORT (0x01 << 5)    
#define DOMAINACL_KEY_PROTO         (0x01 << 6) 
#define DOMAINACL_KEY_USER_GROUP_SINGLE (0x01 << 7) 
#define DOMAINACL_KEY_USER_GROUP_SET    (0x01 << 8) 


typedef struct
{
    UINT uiIP;
    UINT uiWildcard;
}DOMAINACL_IPKEY_S;

typedef struct
{
    USHORT usBegin;
    USHORT usEnd;
}DOMAINACL_PORTKEY_S;

typedef struct{
    DOMAINACL_IPKEY_S stSIP;
    DOMAINACL_PORTKEY_S stDPort;
    CHAR szDomain[128];
}DOMAINACL_SINGLE_KEY_S;

typedef struct{
    LIST_RULE_LIST_S *pstSipList;
    LIST_RULE_LIST_S *pstDportList;
    LIST_RULE_LIST_S *pstDomainGroup;
}DOMAINACL_POOL_KEY_S;

typedef struct{
    DOMAINACL_POOL_KEY_S pools;
    DOMAINACL_SINGLE_KEY_S single;
} DOMAINACL_KEY_S;

typedef struct
{
    BS_ACTION_E enAction;    

    UINT uiKeyMask;
    DOMAINACL_KEY_S stKey;
    UCHAR ucProto;
}DOMAINACL_RULE_CFG_S;

typedef struct
{
    
    volatile ULONG ulMatchCount;    
    UINT uiLatestMatchTime;         
}DOMAINACL_RULE_STATISTICS_S;

typedef struct
{
    UINT uiKeyMask;

    UINT uiSIP;
    USHORT usDPort;
    UCHAR ucProto;
    CHAR szDomain[128];
    UINT acl_id;
}DOMAINACL_MATCH_INFO_S;

typedef struct
{
    DOMAINACL_RULE_CFG_S stRuleCfg;
    DOMAINACL_RULE_STATISTICS_S stStatistics;
}DOMAINACL_RULE_S;

typedef BOOL_T (*PF_DOMAINACL_RULE_SCAN)(IN UINT uiRuleID, IN DOMAINACL_RULE_S *pstRule, IN VOID *pUserHandle);

DOMAINACL_HANDLE DOMAINACL_Create(void* memcap);
VOID DOMAINACL_Destroy(IN DOMAINACL_HANDLE hIpAcl);
DOMAINACL_LIST_HANDLE DOMAINACL_CreateList(IN DOMAINACL_HANDLE hIpAcl, IN CHAR *pcListName);
VOID DOMAINACL_DestroyList(IN DOMAINACL_HANDLE hIpAcl, IN DOMAINACL_LIST_HANDLE hIpAclList);
UINT DOMAINACL_AddList(IN DOMAINACL_HANDLE hIpAcl, IN CHAR *pcListName);
VOID DOMAINACL_DelList(IN DOMAINACL_HANDLE hIpAcl, IN UINT ulListID);
BS_STATUS DOMAINACL_ReplaceList(IN DOMAINACL_HANDLE hIpAcl, IN UINT uiListID, IN DOMAINACL_LIST_HANDLE hIpAclListNew);

BS_STATUS DOMAINACL_AddListRef(IN DOMAINACL_HANDLE hIpAcl, IN UINT uiListID);
BS_STATUS DOMAINACL_DelListRef(IN DOMAINACL_HANDLE hIpAcl, IN UINT uiListID);
UINT DOMAINACL_ListGetRef(IN DOMAINACL_HANDLE hIpAcl, IN UINT uiListID);
UINT DOMAINACL_GetListByName(IN DOMAINACL_HANDLE hIpAcl, IN CHAR *pcListName);
UINT DOMAINACL_GetNextListID(IN DOMAINACL_HANDLE hIpAcl, IN UINT uiCurrentListID);
CHAR * DOMAINACL_GetListNameByID(IN DOMAINACL_HANDLE hIpAcl, IN UINT ulListID);
BS_ACTION_E DOMAINACL_GetDefaultActionByID(IN DOMAINACL_HANDLE hIpAcl, IN UINT uiListID);
BS_STATUS DOMAINACL_SetDefaultActionByID(IN DOMAINACL_HANDLE hIpAcl, IN UINT uiListID, BS_ACTION_E enAction);

BS_STATUS DOMAINACL_AddRule2List(IN DOMAINACL_HANDLE hIpAcl, IN DOMAINACL_LIST_HANDLE hIpAclList, 
                             IN UINT uiRuleID, IN DOMAINACL_RULE_CFG_S *pstRuleCfg);
BS_STATUS DOMAINACL_AddRule(IN DOMAINACL_HANDLE hIpAcl, IN UINT uiListID, IN UINT uiRuleID, IN DOMAINACL_RULE_CFG_S *pstRuleCfg);
VOID DOMAINACL_DelRule(IN DOMAINACL_HANDLE hIpAcl, IN UINT uiListID, IN UINT uiRuleID);
VOID DOMAINACL_UpdateRule(IN DOMAINACL_RULE_S *pstRule, IN DOMAINACL_RULE_CFG_S *pstRuleCfg);
DOMAINACL_RULE_S * DOMAINACL_GetRule(IN DOMAINACL_HANDLE hIpAcl, IN UINT uiListID, IN UINT uiRuleID);
UINT DOMAINACL_GetLastRuleID(IN DOMAINACL_HANDLE hIpAcl, IN UINT uiListID);
BS_STATUS DOMAINACL_MoveRule(IN DOMAINACL_HANDLE hIpAcl, IN UINT uiListID, IN UINT uiOldRuleID, IN UINT uiNewRuleID);
BS_STATUS DOMAINACL_RebaseID(IN DOMAINACL_HANDLE hIpAcl, IN UINT uiListID, IN UINT uiStep);
BS_STATUS  DOMAINACL_IncreaseID(IN DOMAINACL_HANDLE hIpAcl, IN UINT uiListID, IN UINT uiStart, IN UINT uiEnd, IN UINT uiStep);
UINT DOMAINACL_GetNextRuleID(IN DOMAINACL_HANDLE hIpAcl, IN UINT uiListID, IN UINT uiCurrentRuleID);
void DOMAINACL_IncPoolReferedNumber(INOUT DOMAINACL_POOL_KEY_S *pstPoolKeys);
void DOMAINACL_DecPoolReferedNumber(INOUT DOMAINACL_POOL_KEY_S *pstPoolKeys);
VOID DOMAINACL_ScanRule(IN DOMAINACL_HANDLE hIpAcl, IN UINT uiListID, PF_DOMAINACL_RULE_SCAN pfFunc, IN VOID *pUserHandle);
BS_ACTION_E DOMAINACL_Match
(
    IN DOMAINACL_HANDLE hIpAcl,
    IN UINT uiListID,
    IN DOMAINACL_MATCH_INFO_S *pstMatchInfo
);
void DOMAINACL_Reset(DOMAINACL_HANDLE hDomainAcl);


#ifdef __cplusplus
    }
#endif 

#endif 


