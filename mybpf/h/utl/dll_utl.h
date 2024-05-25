/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      lixingang  Version: 1.0  Date: 2007-2-2
* Description: 
* History:     
******************************************************************************/
#ifndef __DLL_H_
#define __DLL_H_

#include "utl/atomic_utl.h"

#ifdef __cplusplus
extern "C" {
#endif 

typedef struct tagDLL_HEAD_S _DLL_HEAD_S;

typedef struct tagDLL_NODE_S {
    struct tagDLL_NODE_S    *prev, *next;
    _DLL_HEAD_S * pstHead;
}DLL_NODE_S;

typedef struct tagDLL_HEAD_S {
    DLL_NODE_S *prev, *next;
    UINT       ulCount;
}DLL_HEAD_S;

#define DLL_HEAD(type)  \
    struct {    \
        type *prev, *next;  \
        UINT ulCount;  \
    }

#define DLL_NODE(type)  \
    struct {    \
        struct type *prev, *next;  \
        _DLL_HEAD_S * pstHead; \
    }

#define DLL_HEAD_INIT_VALUE(pstDllHead)  {(void*)(pstDllHead), (void*)(pstDllHead), 0}

static inline void DLL_INIT(DLL_HEAD_S *pstDllHead) {
    pstDllHead->prev = pstDllHead->next = (void*)pstDllHead;
    pstDllHead->ulCount = 0;
}

#define DLL_NODE_INIT(pstNode) \
        memset((pstNode), 0, sizeof(DLL_NODE_S))

static inline UINT DLL_COUNT(void *dll_head) {
    return ((DLL_HEAD_S*)dll_head)->ulCount;
}

#define DLL_GET_HEAD(pstNode) (((DLL_NODE_S*)(pstNode))->pstHead)

static inline void * DLL_FIRST(DLL_HEAD_S *pstDllHead) {
    if (DLL_COUNT(pstDllHead) == 0) {
        return NULL;
    }

    return pstDllHead->next;
}

#define DLL_LAST(pstDllHead)   ((void*)(DLL_COUNT(pstDllHead) == 0 ? NULL : ((DLL_HEAD_S*)(pstDllHead))->prev))
#define DLL_NEXT(pstDllHead,pstNode) ((void*)((pstNode) == NULL ? DLL_FIRST(pstDllHead) : (((DLL_NODE_S*)(pstNode))->next == (DLL_NODE_S*)pstDllHead ? NULL : ((DLL_NODE_S*)(pstNode))->next)))
#define DLL_PREV(pstDllHead,pstNode) ((void*)((pstNode) == NULL ? NULL : (((DLL_NODE_S*)(pstNode))->prev == (DLL_NODE_S*)pstDllHead ? NULL : ((DLL_NODE_S*)(pstNode))->prev)))

#define DLL_LOOPNEXT(pstDllHead,pstNode) ((void*)((pstNode) == NULL ? DLL_FIRST(pstDllHead) : (((DLL_NODE_S*)(pstNode))->next == (DLL_NODE_S*)pstDllHead ? DLL_FIRST(pstDllHead) : ((DLL_NODE_S*)(pstNode))->next)))

#define DLL_LOOPPREV(pstDllHead,pstNode) ((void*)((pstNode) == NULL ? NULL : (((DLL_NODE_S*)(pstNode))->prev == (DLL_NODE_S*)pstDllHead ? DLL_LAST(pstDllHead) : ((DLL_NODE_S*)(pstNode))->prev)))



#define DLL_ADD_TO_HEAD_RCU(pstDllHead, new_node) do { \
    DLL_NODE_S *pstNewNode = new_node; \
    pstNewNode->pstHead = pstDllHead; \
    pstNewNode->next = pstDllHead->next; \
    pstNewNode->prev = (void*)pstDllHead; \
    ATOM_BARRIER(); \
    pstDllHead->next->prev = pstNewNode; \
    pstDllHead->next = pstNewNode; \
    pstDllHead->ulCount++; \
}while(0)


static inline void DLL_ADD_TO_HEAD(DLL_HEAD_S *pstDllHead, void *new_node) {
    DLL_NODE_S *pstNewNode = new_node;
    pstNewNode->pstHead = pstDllHead;
    pstNewNode->next = pstDllHead->next;
    pstNewNode->prev = (void*)pstDllHead;
    pstDllHead->next->prev = pstNewNode;
    pstDllHead->next = pstNewNode;
    pstDllHead->ulCount++;
}


#define DLL_ADD_RCU(pstDllHead, new_node) do { \
    DLL_NODE_S *_pstNewNode = (new_node); \
    _pstNewNode->pstHead = (pstDllHead); \
    _pstNewNode->next = (void*)(pstDllHead); \
    _pstNewNode->prev = (pstDllHead)->prev; \
    ATOM_BARRIER(); \
    (pstDllHead)->prev->next = _pstNewNode; \
    (pstDllHead)->prev = _pstNewNode; \
    (pstDllHead)->ulCount++; \
}while(0)


static inline void DLL_ADD(DLL_HEAD_S *pstDllHead, void *new_node) {
    DLL_NODE_S *pstNewNode = new_node;
    pstNewNode->pstHead = pstDllHead;
    pstNewNode->next = (void*)pstDllHead;
    pstNewNode->prev = pstDllHead->prev;
    pstDllHead->prev->next = pstNewNode;
    pstDllHead->prev = pstNewNode;
    pstDllHead->ulCount++;
}

#define DLL_INSERT_BEFORE_RCU(pstDllHead,pstNewNode,pstNode)                    \
    do{                                                                         \
        if (NULL == pstNode) {                                                  \
            DLL_ADD_RCU(pstDllHead, pstNewNode);                                \
        } else {                                                                \
            ((DLL_NODE_S*)(pstNewNode))->pstHead = pstDllHead;                  \
            ((DLL_NODE_S*)(pstNewNode))->next = ((DLL_NODE_S*)(pstNode));       \
            ((DLL_NODE_S*)(pstNewNode))->prev = ((DLL_NODE_S*)(pstNode))->prev; \
            ATOM_BARRIER();                                                     \
            ((DLL_NODE_S*)(pstNode))->prev->next = ((DLL_NODE_S*)(pstNewNode)); \
            ((DLL_NODE_S*)(pstNode))->prev = ((DLL_NODE_S*)(pstNewNode));       \
            ((DLL_HEAD_S*)(pstDllHead))->ulCount++;                             \
        }                                                                       \
    }while(0)

#define DLL_INSERT_BEFORE(pstDllHead,pstNewNode,pstNode)                        \
    do{                                                                         \
        if (NULL == pstNode) {                                                  \
            DLL_ADD (pstDllHead, pstNewNode);                                   \
        } else {                                                                \
            ((DLL_NODE_S*)(pstNewNode))->pstHead = pstDllHead;                  \
            ((DLL_NODE_S*)(pstNewNode))->next = ((DLL_NODE_S*)(pstNode));       \
            ((DLL_NODE_S*)(pstNewNode))->prev = ((DLL_NODE_S*)(pstNode))->prev; \
            ((DLL_NODE_S*)(pstNode))->prev->next = ((DLL_NODE_S*)(pstNewNode)); \
            ((DLL_NODE_S*)(pstNode))->prev = ((DLL_NODE_S*)(pstNewNode));        \
            ((DLL_HEAD_S*)(pstDllHead))->ulCount++;                              \
        }                                                                        \
    }while(0)

#define DLL_INSERT_AFTER(pstDllHead,pstNewNode,pstNode) \
    do {DLL_INSERT_BEFORE (pstDllHead,pstNewNode, DLL_NEXT(pstDllHead, pstNode));} while(0)

#define DLL_INSERT_AFTER_RCU(pstDllHead,pstNewNode,pstNode) \
    do {DLL_INSERT_BEFORE_RCU(pstDllHead,pstNewNode, DLL_NEXT(pstDllHead, pstNode));} while(0)

static inline void DLL_DEL(DLL_HEAD_S *pstDllHead, void *del_node) {
    DLL_NODE_S *pstDelNode = del_node;
    pstDelNode->prev->next = pstDelNode->next;
    pstDelNode->next->prev = pstDelNode->prev;
    pstDelNode->pstHead = NULL;
    pstDllHead->ulCount--;
}


#define DLL_DEL_BEFORE(pstDllHead, pstNode, freefunc) \
    do { \
        DLL_NODE_S* pstTemp = DLL_LOOPPREV(pstDllHead, pstNode); \
        DLL_DEL(pstDllHead, pstTemp); \
        freefunc(pstTemp); \
    } while(0)


#define DLL_SCAN(pstDllHead,pstNode)    \
    for (pstNode = DLL_FIRST(pstDllHead); \
        pstNode != NULL; \
        pstNode = DLL_NEXT(pstDllHead,pstNode))

#define DLL_SAFE_SCAN(pstDllHead,pstNode,pstNodeTmp)    \
    for (pstNode = DLL_FIRST(pstDllHead), pstNodeTmp = DLL_NEXT(pstDllHead,pstNode); \
        pstNode != NULL; \
        pstNode = pstNodeTmp, pstNodeTmp = DLL_NEXT(pstDllHead,pstNode))

#define DLL_SCAN_REVERSE(pstDllHead,pstNode)    \
    for (pstNode = DLL_LAST (pstDllHead); \
        pstNode != NULL; \
        pstNode = DLL_PREV (pstDllHead,pstNode))

#define DLL_SAFE_SCAN_REVERSE(pstDllHead,pstNode,pstNodeTmp)    \
    for (pstNode = DLL_LAST (pstDllHead), pstNodeTmp = DLL_PREV (pstDllHead,pstNode); \
        pstNode != NULL; \
        pstNode = pstNodeTmp, pstNodeTmp = DLL_PREV (pstDllHead,pstNode))

#define DLL_WALK(pstDllHead,func,handle)    \
    do{ \
        DLL_NODE_S *pstNode,*pstNodeTmp; \
        DLL_SAFE_SCAN(pstDllHead,pstNode,pstNodeTmp) \
        { \
            func(pstNode,handle); \
        } \
    }while(0)

#define DLL_FREE(pstDllHead,freeFunc)    \
    do{ \
        DLL_NODE_S *_pstNode;   \
        while((_pstNode = DLL_FIRST(pstDllHead))) \
        {   \
            DLL_DEL(pstDllHead, _pstNode);  \
            freeFunc(_pstNode); \
        }   \
        DLL_INIT(pstDllHead);   \
    }while(0)

#define DLL_CAT(pstDllHeadDst, pstDllHeadSrc)   \
    DLL_Cat((DLL_HEAD_S *)(pstDllHeadDst), (DLL_HEAD_S*)(pstDllHeadSrc))

#define DLL_IN_LIST(pstNode)  (DLL_GET_HEAD(pstNode) != NULL)

typedef int (*PF_DLL_CMP_FUNC)(DLL_NODE_S *pstNode1, DLL_NODE_S *pstNode2, void *ud);

static inline void * DLL_Get (IN DLL_HEAD_S *pstDllHead)
{
    DLL_NODE_S *pstDllNode;

    pstDllNode = (DLL_NODE_S*) DLL_FIRST (pstDllHead);
    if (NULL != pstDllNode) {
        DLL_DEL(pstDllHead,pstDllNode);
    }

    return (void*) pstDllNode;
}

static inline void DLL_DelIfInList(IN DLL_NODE_S *pstNode)
{
    if (DLL_GET_HEAD(pstNode)) {
        DLL_DEL(DLL_GET_HEAD(pstNode), pstNode);
    }
}

extern void DLL_Sort(IN DLL_HEAD_S *pstDllHead, IN PF_DLL_CMP_FUNC pfFunc, IN HANDLE hUserHandle);

extern int DLL_UniqueSortAdd(DLL_HEAD_S *head, DLL_NODE_S *node, PF_DLL_CMP_FUNC cmp_func, void *user_data);

extern void DLL_SortAdd
(
    IN DLL_HEAD_S *pstDllHead,
    IN DLL_NODE_S *pstNewNode,
    IN PF_DLL_CMP_FUNC pfFunc,
    IN HANDLE hUserHandle
);

extern void DLL_Cat (IN DLL_HEAD_S *pstDllHeadDst, IN DLL_HEAD_S *pstDllHeadSrc);

extern void * DLL_Find(IN DLL_HEAD_S *pstDllHead, IN PF_DLL_CMP_FUNC pfCmpFunc, IN void *pstNodeToFind, IN HANDLE hUserHandle);

#ifdef __cplusplus
}
#endif 

#endif 

