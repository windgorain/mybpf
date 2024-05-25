/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _LIST_STQ_H
#define _LIST_STQ_H
#ifdef __cplusplus
extern "C"
{
#endif


typedef struct tagSTQ_NODE
{
    struct tagSTQ_NODE* pstNext; 
} STQ_NODE_S;

#define STQ_ENTRY(ptr, type, member)    (container_of(ptr, type, member))

typedef struct tagSTQ_HEAD
{
    STQ_NODE_S *pstFirst;  
    STQ_NODE_S *pstLast;   
} STQ_HEAD_S;



static inline VOID STQ_Init(IN STQ_HEAD_S* pstList)
{
    pstList->pstFirst = (STQ_NODE_S *)NULL;
    pstList->pstLast  = (STQ_NODE_S *)NULL;
    return;
}

static inline VOID STQ_NodeInit(IN STQ_NODE_S* pstNode)
{
    pstNode->pstNext = (STQ_NODE_S *)NULL;
}

static inline BOOL_T STQ_IsEmpty(IN const STQ_HEAD_S* pstList)
{
    return (pstList->pstFirst == NULL);
}

static inline STQ_NODE_S* STQ_First(IN const STQ_HEAD_S* pstList)
{
    return pstList->pstFirst;
}

static inline STQ_NODE_S* STQ_Last(IN const STQ_HEAD_S* pstList)
{
    return pstList->pstLast;
}

static inline STQ_NODE_S* STQ_Next(IN const STQ_NODE_S* pstNode)
{
    if (NULL == pstNode)
    {
        return NULL;
    }

    return pstNode->pstNext;
}

static inline VOID STQ_AddHead(IN STQ_HEAD_S* pstList, IN STQ_NODE_S* pstNode)
{
    pstNode->pstNext = pstList->pstFirst;
    pstList->pstFirst = pstNode;
    if (NULL == pstList->pstLast)
    {
        pstList->pstLast = pstNode;
    }
    return;
}


static inline STQ_NODE_S* STQ_DelHead(STQ_HEAD_S* pstList)
{
    STQ_NODE_S* pstNode = pstList->pstFirst;

    if (NULL != pstNode)
    {
        pstList->pstFirst = pstNode->pstNext;
    }
    if (NULL == pstList->pstFirst)
    {
        pstList->pstLast = (STQ_NODE_S *)NULL;
    }

    return pstNode;
}

static inline VOID STQ_AddTail(STQ_HEAD_S* pstList, STQ_NODE_S* pstNode)
{
    pstNode->pstNext = (STQ_NODE_S *)NULL;
    if (NULL != pstList->pstLast)
    {
        pstList->pstLast->pstNext = pstNode;
        pstList->pstLast = pstNode;
    }
    else
    {
        pstList->pstLast  = pstNode;
        pstList->pstFirst = pstNode;
    }
    return;
}

static inline VOID STQ_AddAfter(IN STQ_HEAD_S* pstList,
                                IN STQ_NODE_S* pstPrev,
                                IN STQ_NODE_S* pstInst)
{
    if (NULL == pstPrev)
    {
        STQ_AddHead (pstList, pstInst);
    }
    else
    {
        pstInst->pstNext = pstPrev->pstNext;
        pstPrev->pstNext = pstInst;
        if (pstList->pstLast == pstPrev)
        {
            pstList->pstLast = pstInst;
        }
    }
    return;
}


static inline STQ_NODE_S* STQ_DelAfter(IN STQ_HEAD_S* pstList,
                                       IN STQ_NODE_S* pstPrev)
{
    STQ_NODE_S* pstNode;

    if (NULL == pstPrev)
    {
        pstNode = STQ_DelHead (pstList);
    }
    else
    {
        pstNode = pstPrev->pstNext;
        if (NULL != pstNode)
        {
            pstPrev->pstNext = pstNode->pstNext;
        }
        if (pstList->pstLast == pstNode)
        {
            pstList->pstLast = pstPrev;
        }
    }

    return pstNode;
}


#define STQ_FOREACH(pstList, pstNode) \
    for((pstNode) = STQ_First(pstList); \
        NULL != (pstNode); \
        (pstNode) = STQ_Next(pstNode))

#define STQ_FOREACH_SAFE(pstList, pstNode, pstNext) \
    for ((pstNode) = STQ_First(pstList), (pstNext) = STQ_Next(pstNode); \
         NULL != (pstNode); \
         (pstNode) = (pstNext), (pstNext) = STQ_Next(pstNode))

#define STQ_FOREACH_PREVPTR(pstList, pstNode, pstPrev) \
    for ((pstNode) = STQ_First(pstList), (pstPrev) = (STQ_NODE_S *)NULL; \
         NULL != (pstNode); \
         (pstPrev) = (pstNode), (pstNode) = STQ_Next(pstNode))

#define STQ_ENTRY_FIRST(pstList, type, member) \
    (STQ_IsEmpty(pstList) ? NULL : STQ_ENTRY(STQ_First(pstList), type, member))

#define STQ_ENTRY_LAST(pstList, type, member) \
    (STQ_IsEmpty(pstList) ? NULL : STQ_ENTRY(STQ_Last(pstList), type, member))

#define STQ_ENTRY_NEXT(pstEntry, member) \
    (NULL == (pstEntry) ? NULL : \
       (NULL == STQ_Next(&((pstEntry)->member)) ? NULL : \
          STQ_ENTRY(STQ_Next(&((pstEntry)->member)), typeof(*(pstEntry)), member)))

#define STQ_FOREACH_ENTRY(pstList, pstEntry, member) \
    for ((pstEntry) = STQ_ENTRY_FIRST(pstList, typeof(*(pstEntry)), member); \
         NULL != (pstEntry); \
         (pstEntry) = STQ_ENTRY_NEXT(pstEntry, member))

#define STQ_FOREACH_ENTRY_SAFE(pstList, pstEntry, pstNextEntry, member) \
    for ((pstEntry) = STQ_ENTRY_FIRST(pstList, typeof(*(pstEntry)), member); \
         (NULL != (pstEntry)) && \
         ({(pstNextEntry) = STQ_ENTRY_NEXT(pstEntry, member); BOOL_TRUE;}); \
         (pstEntry) = (pstNextEntry))

#define STQ_FOREACH_ENTRY_PREVPTR(pstList, pstEntry, pstPrevEntry, member) \
    for ((pstEntry) = STQ_ENTRY_FIRST(pstList, typeof(*(pstEntry)), member), \
            (pstPrevEntry) = NULL; \
          NULL != (pstEntry); \
          (VOID)({(pstPrevEntry) = (pstEntry); \
                   (pstEntry) = STQ_ENTRY_NEXT(pstEntry, member);}))


static inline VOID STQ_Del(IN STQ_HEAD_S* pstList, IN const STQ_NODE_S* pstNode)
{
    STQ_NODE_S *pstPrev, *pstCur;

    STQ_FOREACH_PREVPTR (pstList, pstCur, pstPrev)
    {
        if (pstCur == pstNode)
        {
            (VOID)STQ_DelAfter (pstList, pstPrev);
            break;
        }
    }
    return;
}

static inline VOID STQ_Append(IN STQ_HEAD_S* pstDstList,
                              INOUT STQ_HEAD_S* pstSrcList)
{
    if (! STQ_IsEmpty(pstSrcList))
    {
        if (NULL != pstDstList->pstLast)
        {
            pstDstList->pstLast->pstNext = STQ_First(pstSrcList);
        }
        else
        {
            pstDstList->pstFirst = STQ_First(pstSrcList);
        }
        
        pstDstList->pstLast = STQ_Last(pstSrcList);
        STQ_Init(pstSrcList);
    }
    
    return;
}

static inline VOID STQ_FreeAll(IN STQ_HEAD_S *pstList, IN VOID (*pfFree)(VOID *))
{
    STQ_NODE_S *pstCurNode;
    STQ_NODE_S *pstNextNode;

    
    STQ_FOREACH_SAFE(pstList, pstCurNode, pstNextNode)
    {
        pfFree(pstCurNode);
    }

    STQ_Init(pstList);
    return;
}

#ifdef __cplusplus
}
#endif
#endif 
