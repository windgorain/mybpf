/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2016-10-14
* Description: 
* History:     
******************************************************************************/

#ifndef __LIST_RULE_H_
#define __LIST_RULE_H_

#include "utl/rule_list.h"

#ifdef __cplusplus
    extern "C" {
#endif 

#define LIST_RULE_LIST_NAME_SIZE 64


typedef struct {
    NAP_HANDLE hListNap;
    void *memcap;
}LIST_RULE_CTRL_S;

typedef LIST_RULE_CTRL_S* LIST_RULE_HANDLE;

typedef struct {
    char list_name[LIST_RULE_LIST_NAME_SIZE];
    UCHAR default_action;
    UINT list_id;
    UINT uiRefCount; 
    RULE_LIST_S stRuleList;
    union {
        UCHAR user_data[0];
        void *user_handle[0];
    };
}LIST_RULE_LIST_S;

typedef struct {
    LIST_RULE_LIST_S *pstListRule;
}LIST_RULE_HEAD_S;

typedef void (*PF_LIST_RULE_WALK_LIST_FUNC)(LIST_RULE_LIST_S *list, void *ud);
typedef void (*PF_LIST_RULE_WALK_RULE_FUNC)(LIST_RULE_LIST_S *list, RULE_NODE_S *rule, void *ud);

BS_STATUS ListRule_Init(INOUT LIST_RULE_CTRL_S *pstCtx, void *memcap);
void ListRule_Finit(INOUT LIST_RULE_CTRL_S *pstCtx, PF_RULE_FREE pfFunc, VOID *pUserHandle);
LIST_RULE_HANDLE ListRule_Create(void *memcap);
VOID ListRule_Destroy(IN LIST_RULE_HANDLE hListRule, IN PF_RULE_FREE pfFunc, IN VOID *pUserHandle);
void ListRule_Reset(IN LIST_RULE_HANDLE hListRule, IN PF_RULE_FREE pfFunc, IN VOID *pUserHandle);

LIST_RULE_LIST_S * ListRule_CreateList(IN LIST_RULE_HANDLE hListRule, IN CHAR *list_name, int user_data_size);
VOID ListRule_DestroyList(IN LIST_RULE_HANDLE hListRule, LIST_RULE_LIST_S *pstList, IN PF_RULE_FREE pfFunc, IN VOID *uh);
UINT ListRule_AttachList(LIST_RULE_HANDLE ctx, LIST_RULE_LIST_S *list);
LIST_RULE_LIST_S * ListRule_DetachList(LIST_RULE_HANDLE ctx, UINT list_id);
UINT ListRule_AddList(IN LIST_RULE_HANDLE hListRule, IN CHAR *list_name, int user_data_size);
void ListRule_DelList(LIST_RULE_HANDLE ctx, LIST_RULE_LIST_S *list, PF_RULE_FREE pfFunc, void *ud);
void ListRule_DelListID(LIST_RULE_HANDLE hListRule, UINT uiListID, PF_RULE_FREE pfFunc, void *ud);
int ListRule_ReplaceList(LIST_RULE_HANDLE hListRule, UINT uiListID, PF_RULE_FREE pfFunc, void *ud, LIST_RULE_LIST_S *pstListNew);

BS_STATUS ListRule_IncListRef(IN LIST_RULE_HANDLE hListRule, IN UINT uiListID);
BS_STATUS ListRule_DecListRef(IN LIST_RULE_HANDLE hListRule, IN UINT uiListID);
UINT ListRule_GetListRef(IN LIST_RULE_HANDLE hListRule, IN UINT uiListID);
BOOL_T ListRule_IsAnyListRefed(IN LIST_RULE_HANDLE ctx);
BS_ACTION_E ListRule_GetDefaultActionByID(IN LIST_RULE_HANDLE hListRule, IN UINT uiListID);
BS_STATUS ListRule_SetDefaultActionByID(IN LIST_RULE_HANDLE hListRule, IN UINT uiListID, BS_ACTION_E enAciton);
UINT ListRule_GetListIDByName(IN LIST_RULE_HANDLE hListRule, IN CHAR *pcListName);
LIST_RULE_LIST_S* ListRule_GetListByID(IN LIST_RULE_HANDLE hListRule, IN UINT uiListID);
LIST_RULE_LIST_S* ListRule_GetListByName(IN LIST_RULE_HANDLE hListRule, IN CHAR *pcListName);
LIST_RULE_HEAD_S * ListRule_GetNextList(IN LIST_RULE_HANDLE ctx, IN LIST_RULE_HEAD_S *curr);
UINT ListRule_GetNextListID(IN LIST_RULE_HANDLE hListRule, IN UINT ulCurrentListID);
CHAR * ListRule_GetListNameByID(IN LIST_RULE_HANDLE hListRule, IN UINT ulListID);

BS_STATUS ListRule_AddRule2List(LIST_RULE_LIST_S *pstList, IN UINT uiRuleID, IN RULE_NODE_S *pstRule);
int ListRule_AddRule(IN LIST_RULE_HANDLE hCtx, IN UINT uiListID, IN UINT uiRuleID, IN RULE_NODE_S *pstRule);
RULE_NODE_S * ListRule_DelRule(IN LIST_RULE_HANDLE hCtx, IN UINT uiListID, IN UINT uiRuleID);
RULE_NODE_S * ListRule_GetRule(IN LIST_RULE_HANDLE hCtx, IN UINT uiListID, IN UINT uiRuleID);
RULE_NODE_S *ListRule_GetLastRule(IN LIST_RULE_HANDLE hCtx, IN UINT uiListID);
BS_STATUS ListRule_IncreaseID(IN LIST_RULE_HANDLE hCtx, IN UINT uiListID, IN UINT uiStart, IN UINT uiEnd, IN UINT uiStep);

BS_STATUS ListRule_MoveRule(LIST_RULE_HANDLE hCtx, UINT uiListID, UINT uiOldRuleID, UINT uiNewRuleID);
RULE_NODE_S * ListRule_GetNextRule(LIST_RULE_HANDLE hCtx, UINT uiListID, UINT uiCurrentRuleID);
BS_STATUS ListRule_ResetID(IN LIST_RULE_HANDLE hCtx, IN UINT uiListID, IN UINT uiStep);

void ListRule_WalkList(LIST_RULE_HANDLE hCtx, PF_LIST_RULE_WALK_LIST_FUNC walk_list, void *ud);
void ListRule_WalkRule(LIST_RULE_LIST_S *list, PF_LIST_RULE_WALK_RULE_FUNC walk_rule, void *ud);
void ListRule_Walk(LIST_RULE_HANDLE hCtx, PF_LIST_RULE_WALK_LIST_FUNC walk_list,
        PF_LIST_RULE_WALK_RULE_FUNC walk_rule, void *ud);

static inline void * ListRule_GetMemcap(LIST_RULE_HANDLE ctx) {
    return ctx->memcap;
}

static inline void * ListRule_GetUserHandle(IN LIST_RULE_LIST_S *list, int index)
{
    return list->user_handle[index];
}


#ifdef __cplusplus
    }
#endif 

#endif 


