/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _RULE_LIST_H
#define _RULE_LIST_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "utl/dll_utl.h"
typedef struct {
    DLL_HEAD_S stRuleList;
}RULE_LIST_S;

typedef struct {
    DLL_NODE_S stLinkNode;
    UINT uiRuleID;
}RULE_NODE_S;

#define RULE_ID_INVALID 0xffffffff

typedef VOID (*PF_RULE_FREE)(IN void *pstRule, IN VOID *pUserHandle);
typedef BOOL_T (*PF_RULE_SCAN)(IN void *pstRule, IN VOID *pUserHandle);
typedef int (*PF_RULE_CMP)(RULE_NODE_S *pstNode1, RULE_NODE_S *pstNode2);

void RuleList_Init(RULE_LIST_S * rulelist);
void RuleList_Finit(RULE_LIST_S *pstList, PF_RULE_FREE pfFunc, IN VOID *pUserHandle);
void RuleList_ScanRule(RULE_LIST_S *pstList, PF_RULE_SCAN pfFunc, IN VOID *pUserHandle);
void RuleList_Add(RULE_LIST_S * pstList, UINT uiRuleID, IN RULE_NODE_S *pstRule);
void RuleList_DelByNode(RULE_LIST_S *pstList, RULE_NODE_S *pstRule);
RULE_NODE_S * RuleList_Del(RULE_LIST_S *pstList, IN UINT uiRuleID);
void RuleList_ResetRuleID(RULE_LIST_S *pstList, IN UINT step);
RULE_NODE_S * RuleList_FindRule(RULE_LIST_S *pstList, PF_RULE_CMP pfCmp, RULE_NODE_S *pstToFind);
RULE_NODE_S * RuleList_GetRule(RULE_LIST_S *pstList, IN UINT uiRuleID);
RULE_NODE_S * RuleList_GetLastRule(RULE_LIST_S *pstList);
BS_STATUS RuleList_IncreaseID(RULE_LIST_S *pstList, IN UINT uiStart, IN UINT uiEnd, IN UINT uiStep);

BS_STATUS RuleList_MoveRule(RULE_LIST_S *pstList, UINT uiOldRuleID, UINT uiNewRuleID);
RULE_NODE_S * RuleList_GetNextByNode(RULE_LIST_S *pstList, RULE_NODE_S *pstCurr);
RULE_NODE_S * RuleList_GetNextByID(RULE_LIST_S *pstList, UINT uiCurrentRuleID);
UINT RuleList_Count(RULE_LIST_S *pstList);
UINT RuleList_ResetID(RULE_LIST_S* pstList, UINT uiStep);
static inline UINT Rule_GetIDByNode(IN RULE_NODE_S *pstNode) {
    return pstNode->uiRuleID;
}


#ifdef __cplusplus
}
#endif
#endif 
