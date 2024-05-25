/*================================================================
* Author：LiXingang. Data: 2019.08.12
* Description: 
*
================================================================*/
#ifndef _LIST_DTQ_H
#define _LIST_DTQ_H
#ifdef __cplusplus
extern "C"
{
#endif


typedef struct tagDTQ_NODE
{
    struct tagDTQ_NODE* pstPrev; 
    struct tagDTQ_NODE* pstNext; 
} DTQ_NODE_S;

#define DTQ_ENTRY(ptr, type, member)    (container_of(ptr, type, member))

typedef struct tagDTQ_HEAD
{
    DTQ_NODE_S stHead;   
} DTQ_HEAD_S;

#define DTQ_HEAD_INIT_VALUE(list)  {(void*)(list), (void*)(list)}

static inline VOID DTQ_Init(IN DTQ_HEAD_S* pstList)
{
    pstList->stHead.pstNext = &pstList->stHead;
    pstList->stHead.pstPrev = &pstList->stHead;
    return;
}

static inline VOID DTQ_NodeInit(IN DTQ_NODE_S* pstNode)
{
    pstNode->pstNext = (DTQ_NODE_S*)NULL;
    pstNode->pstPrev = (DTQ_NODE_S*)NULL;
    return;
}

static inline BOOL_T DTQ_IsEmpty(IN const DTQ_HEAD_S* pstList)
{
    return (pstList->stHead.pstNext == &pstList->stHead);
}

static inline DTQ_NODE_S* DTQ_First(IN const DTQ_HEAD_S* pstList)
{   
    DTQ_NODE_S* pstNode = pstList->stHead.pstNext;
    if(pstNode == &(pstList->stHead)) {
        return NULL;
    }
    return pstNode;
}

static inline DTQ_NODE_S* DTQ_Last(IN const DTQ_HEAD_S* pstList)
{
    DTQ_NODE_S* pstNode = pstList->stHead.pstPrev;
    if(pstNode == &pstList->stHead)
    {
        return NULL;
    }
    return pstNode;
}

static inline BOOL_T DTQ_IsEndOfQ(IN const DTQ_HEAD_S* pstList, IN const DTQ_NODE_S* pstNode)
{
    if (DTQ_IsEmpty(pstList)) {
        return BOOL_TRUE;
    }

    if (NULL == pstNode) {
        return BOOL_TRUE;
    }

    return (pstNode == &pstList->stHead);
}

static inline DTQ_NODE_S* DTQ_Prev(IN const DTQ_NODE_S* pstNode)
{
    return (pstNode->pstPrev);
}

static inline DTQ_NODE_S* DTQ_Next(const DTQ_NODE_S* pstNode)
{
    return (pstNode->pstNext);
}

static inline DTQ_NODE_S* DTQ_GetNext(const DTQ_HEAD_S* pstList, IN const DTQ_NODE_S* pstNode)
{
    DTQ_NODE_S *n = DTQ_Next(pstNode);

    if (DTQ_IsEndOfQ(pstList, n)) {
        return NULL;
    }

    return n;
}

static inline VOID DTQ_AddAfter(IN DTQ_NODE_S* pstPrev, IN DTQ_NODE_S* pstInst)
{
    pstInst->pstPrev = pstPrev;
    pstInst->pstNext = pstPrev->pstNext;
    pstPrev->pstNext = pstInst;
    pstInst->pstNext->pstPrev = pstInst;

    return;
}

static inline VOID DTQ_AddBefore(IN DTQ_NODE_S* pstNext, IN DTQ_NODE_S* pstInst)
{
    pstInst->pstPrev = pstNext->pstPrev;
    pstInst->pstNext = pstNext;
    pstInst->pstPrev->pstNext = pstInst;
    pstInst->pstNext->pstPrev = pstInst;

    return;
}

static inline VOID DTQ_Del(IN const DTQ_NODE_S* pstNode)
{
    pstNode->pstPrev->pstNext = pstNode->pstNext;
    pstNode->pstNext->pstPrev = pstNode->pstPrev;

    return;
}

static inline VOID DTQ_AddHead(IN DTQ_HEAD_S* pstList, IN DTQ_NODE_S* pstNode)
{
    DTQ_AddAfter (&pstList->stHead, pstNode);

    return;
}

static inline DTQ_NODE_S* DTQ_DelHead(IN const DTQ_HEAD_S* pstList)
{
    DTQ_NODE_S* pstNode = DTQ_First(pstList);

    if (DTQ_IsEndOfQ(pstList, pstNode))
    {
        pstNode = (DTQ_NODE_S*)NULL;
    }
    else
    {
        DTQ_Del (pstNode);
    }

    return pstNode;
}

static inline VOID DTQ_AddTail(IN DTQ_HEAD_S* pstList, IN DTQ_NODE_S* pstNode)
{
    DTQ_AddBefore (&pstList->stHead, pstNode);
    return;
}

static inline DTQ_NODE_S* DTQ_DelTail(IN const DTQ_HEAD_S* pstList)
{
    DTQ_NODE_S* pstNode = DTQ_Last (pstList);

    if (DTQ_IsEndOfQ(pstList, pstNode))
    {
        pstNode = (DTQ_NODE_S*)NULL;
    }
    else
    {
        DTQ_Del (pstNode);
    }

    return pstNode;
}


#define DTQ_FOREACH(pstList, pstNode) \
    for ((pstNode) = (pstList)->stHead.pstNext; \
         ((pstNode) != &((pstList)->stHead)); \
         (pstNode) = DTQ_Next(pstNode))

#define DTQ_FOREACH_SAFE(pstList, pstNode, pstNextNode) \
    for ((pstNode) = (pstList)->stHead.pstNext, (pstNextNode) = DTQ_Next(pstNode); \
         pstNode != &((pstList)->stHead); \
         (pstNode) = (pstNextNode), (pstNextNode) = DTQ_Next(pstNode))
         
#define DTQ_FOREACH_REVERSE(pstList, pstNode) \
    for ((pstNode) = DTQ_Last(pstList); \
         (BOOL_TRUE != DTQ_IsEndOfQ(pstList, pstNode)); \
         (pstNode) = DTQ_Prev(pstNode))

#define DTQ_FOREACH_REVERSE_SAFE(pstList, pstNode, pstPrev) \
    for ((pstNode) = DTQ_Last(pstList); \
         (BOOL_TRUE != DTQ_IsEndOfQ(pstList, pstNode)) && \
         ({(pstPrev) = DTQ_Prev(pstNode); BOOL_TRUE;}); \
         (pstNode) = (pstPrev))

#define DTQ_ENTRY_FIRST(pstList, type, member) \
        ({DTQ_NODE_S *pstNode__Tmp__Mx = DTQ_First(pstList); \
         (NULL == pstNode__Tmp__Mx) ? NULL : DTQ_ENTRY(pstNode__Tmp__Mx, type, member);})
#define DTQ_ENTRY_LAST(pstList, type, member) \
        ({DTQ_NODE_S *pstNode__Tmp__Mx = DTQ_Last(pstList); \
         (NULL == pstNode__Tmp__Mx) ? NULL : DTQ_ENTRY(pstNode__Tmp__Mx, type, member);})
#define DTQ_ENTRY_NEXT(pstList, pstEntry, member) \
    (DTQ_IsEndOfQ(pstList, (NULL == (pstEntry) ? NULL : DTQ_Next(&((pstEntry)->member)))) ? \
        NULL : \
        DTQ_ENTRY(DTQ_Next(&((pstEntry)->member)), typeof(*(pstEntry)), member))

#define DTQ_ENTRY_PREV(pstList, pstEntry, member) \
    (DTQ_IsEndOfQ(pstList, (NULL == (pstEntry) ? NULL : DTQ_Prev(&((pstEntry)->member)))) ? \
        NULL : \
        DTQ_ENTRY(DTQ_Prev(&((pstEntry)->member)), typeof(*(pstEntry)), member))

#define DTQ_FOREACH_ENTRY(pstList, pstEntry, member) \
    for ((pstEntry) = DTQ_ENTRY((pstList)->stHead.pstNext, typeof(*(pstEntry)), member); \
         ((&(pstEntry)->member != &(pstList)->stHead) || ({pstEntry = NULL; BOOL_FALSE;})); \
         (pstEntry) = DTQ_ENTRY((pstEntry)->member.pstNext, typeof(*(pstEntry)), member))

#define DTQ_FOREACH_ENTRY_SAFE(pstList, pstEntry, pstNextEntry, member) \
    for ((pstEntry) = DTQ_ENTRY((pstList)->stHead.pstNext, typeof(*(pstEntry)), member); \
         (((&(pstEntry)->member != &(pstList)->stHead) &&\
         ({(pstNextEntry) = DTQ_ENTRY((pstEntry)->member.pstNext, typeof(*(pstEntry)), member); BOOL_TRUE;})) || \
         ({pstEntry = NULL; BOOL_FALSE;})); \
         (pstEntry) = (pstNextEntry))
         
#define DTQ_FOREACH_ENTRY_REVERSE(pstList, pstEntry, member) \
    for ((pstEntry) = DTQ_ENTRY_LAST(pstList, typeof(*(pstEntry)), member); \
         NULL != (pstEntry); \
         (pstEntry) = DTQ_ENTRY_PREV(pstList, pstEntry, member))

#define DTQ_FOREACH_ENTRY_REVERSE_SAFE(pstList, pstEntry, pstPrevEntry, member) \
    for ((pstEntry) = DTQ_ENTRY_LAST(pstList, typeof(*(pstEntry)), member); \
         (NULL != (pstEntry)) && \
         ({(pstPrevEntry) = DTQ_ENTRY_PREV(pstList, pstEntry, member); BOOL_TRUE;}); \
         (pstEntry) = (pstPrevEntry))

static inline VOID DTQ_Append(IN DTQ_HEAD_S* pstDstList, INOUT DTQ_HEAD_S* pstSrcList)
{
    if (BOOL_TRUE != DTQ_IsEmpty (pstSrcList)) {
        pstSrcList->stHead.pstNext->pstPrev = pstDstList->stHead.pstPrev;
        pstSrcList->stHead.pstPrev->pstNext = pstDstList->stHead.pstPrev->pstNext;
        pstDstList->stHead.pstPrev->pstNext = pstSrcList->stHead.pstNext;
        pstDstList->stHead.pstPrev = pstSrcList->stHead.pstPrev;
        DTQ_Init(pstSrcList);
    }
    return;
}

static inline VOID DTQ_FreeAll(IN DTQ_HEAD_S *pstList, IN VOID (*pfFree)(VOID *))
{
    DTQ_NODE_S *pstCurNode;
    DTQ_NODE_S *pstNextNode;

    
    DTQ_FOREACH_SAFE(pstList, pstCurNode, pstNextNode) {
        pfFree(pstCurNode);
    }

    DTQ_Init(pstList);
    return;
}

static inline DTQ_NODE_S * DTQ_Find(DTQ_HEAD_S *pstList, const void *key, PF_CMP_EXT_FUNC cmp_fn, void *ud)
{
    void *node;

    DTQ_FOREACH(pstList, node) {
        if (cmp_fn(key, node, ud) == 0) {
            return node;
        }
    }

    return NULL;
}


#ifdef __cplusplus
}
#endif
#endif 
