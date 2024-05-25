/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _LIST_SL_H
#define _LIST_SL_H
#ifdef __cplusplus
extern "C"
{
#endif


typedef struct tagSL_NODE
{
    struct tagSL_NODE* pstNext;  
} SL_NODE_S;

#define SL_ENTRY(ptr, type, member) (container_of(ptr, type, member))

typedef struct tagSL_HEAD
{
    SL_NODE_S* pstFirst;
} SL_HEAD_S;

static inline VOID SL_Init(IN SL_HEAD_S* pstList);
static inline VOID SL_NodeInit(IN SL_NODE_S* pstNode);
static inline BOOL_T SL_IsEmpty(IN const SL_HEAD_S* pstList);
static inline SL_NODE_S* SL_First(IN const SL_HEAD_S* pstList);
static inline SL_NODE_S* SL_Next(IN const SL_NODE_S* pstNode);
static inline VOID SL_AddHead(IN SL_HEAD_S* pstList, IN SL_NODE_S* pstNode);
static inline SL_NODE_S* SL_DelHead(IN SL_HEAD_S* pstList);
static inline VOID SL_AddAfter(IN SL_HEAD_S* pstList,
                               IN SL_NODE_S* pstPrev,
                               IN SL_NODE_S* pstInst);
static inline SL_NODE_S* SL_DelAfter(IN SL_HEAD_S* pstList,
                                     IN SL_NODE_S* pstPrev);
static inline VOID SL_Del(IN SL_HEAD_S* pstList, IN const SL_NODE_S* pstNode);
static inline VOID SL_Append(IN SL_HEAD_S* pstDstList, IN SL_HEAD_S* pstSrcList);
static inline VOID SL_FreeAll(IN SL_HEAD_S *pstList, IN VOID (*pfFree)(VOID *));

static inline VOID SL_Init(IN SL_HEAD_S* pstList)
{
    pstList->pstFirst = (SL_NODE_S *)NULL;
    return;
}

static inline VOID SL_NodeInit(OUT SL_NODE_S* pstNode)
{
    pstNode->pstNext = (SL_NODE_S *)NULL;
}

static inline BOOL_T SL_IsEmpty(IN const SL_HEAD_S* pstList)
{
    return (pstList->pstFirst == NULL);
}

static inline SL_NODE_S* SL_First(IN const SL_HEAD_S* pstList)
{
    return (pstList->pstFirst);
}

static inline SL_NODE_S* SL_Next(IN const SL_NODE_S* pstNode)
{
    if (NULL == pstNode)
    {
        return NULL;
    }

    return (pstNode->pstNext);
}

static inline VOID SL_AddHead(IN SL_HEAD_S* pstList, IN SL_NODE_S* pstNode)
{
    pstNode->pstNext  = pstList->pstFirst;
    pstList->pstFirst = pstNode;
    return;
}


static inline SL_NODE_S* SL_DelHead(IN SL_HEAD_S* pstList)
{
    SL_NODE_S* pstNode = pstList->pstFirst;

    if (NULL != pstNode)
    {
        pstList->pstFirst = pstNode->pstNext;
    }

    return pstNode;
}

static inline VOID SL_AddAfter(IN SL_HEAD_S* pstList,
                               IN SL_NODE_S* pstPrev,
                               IN SL_NODE_S* pstInst)
{
    if (NULL == pstPrev)
    {
        SL_AddHead (pstList, pstInst);
    }
    else
    {
        pstInst->pstNext = pstPrev->pstNext;
        pstPrev->pstNext = pstInst;
    }
    return;
}


static inline SL_NODE_S* SL_DelAfter(IN SL_HEAD_S* pstList,
                                     IN SL_NODE_S* pstPrev)
{
    SL_NODE_S* pstNode;

    if (NULL == pstPrev)
    {
        pstNode = SL_DelHead (pstList);
    }
    else
    {
        pstNode = pstPrev->pstNext;
        if (NULL != pstNode)
        {
            pstPrev->pstNext = pstNode->pstNext;
        }
    }

    return pstNode;
}


#define SL_FOREACH(pstList, pstNode) \
    for ((pstNode) = SL_First(pstList); \
         NULL != (pstNode); \
         (pstNode) = SL_Next(pstNode))

#define SL_FOREACH_SAFE(pstList, pstNode, pstNext) \
    for ((pstNode) = SL_First((pstList)), (pstNext) = SL_Next(pstNode); \
         (NULL != (pstNode)); \
         (pstNode) = (pstNext), (pstNext) = SL_Next(pstNode))

#define SL_FOREACH_PREVPTR(pstList, pstNode, pstPrev) \
    for ((pstNode) = SL_First(pstList), (pstPrev) = (SL_NODE_S *)NULL; \
         NULL != (pstNode); \
         (pstPrev) = (pstNode), (pstNode) = SL_Next(pstNode))

#define SL_ENTRY_FIRST(pstList, type, member) \
    (SL_IsEmpty(pstList) ? NULL : SL_ENTRY(SL_First(pstList), type, member))

#define SL_ENTRY_NEXT(pstEntry, member) \
    (NULL == (pstEntry) ? NULL : \
        (NULL == SL_Next(&((pstEntry)->member))) ? NULL : \
           SL_ENTRY(SL_Next(&((pstEntry)->member)), typeof(*(pstEntry)), member))

#define SL_FOREACH_ENTRY(pstList, pstEntry, member) \
    for ((pstEntry) = SL_ENTRY_FIRST(pstList, typeof(*(pstEntry)), member); \
          NULL != (pstEntry); \
         (pstEntry) = SL_ENTRY_NEXT(pstEntry, member))

#define SL_FOREACH_ENTRY_SAFE(pstList, pstEntry, pstNextEntry, member) \
    for ((pstEntry) = SL_ENTRY_FIRST(pstList, typeof(*(pstEntry)), member); \
         (NULL != (pstEntry)) && \
         ({(pstNextEntry) = SL_ENTRY_NEXT(pstEntry, member); BOOL_TRUE;}); \
         (pstEntry) = (pstNextEntry))

#define SL_FOREACH_ENTRY_PREVPTR(pstList, pstEntry, pstPrevEntry, member) \
    for ((pstEntry) = SL_ENTRY_FIRST(pstList, typeof(*(pstEntry)), member), \
            (pstPrevEntry) = NULL; \
          NULL != (pstEntry); \
          (VOID)({(pstPrevEntry) = (pstEntry); \
                   (pstEntry) = SL_ENTRY_NEXT(pstEntry, member);}))


static inline VOID SL_Del(IN SL_HEAD_S* pstList, IN const SL_NODE_S* pstNode)
{
    SL_NODE_S *pstCur, *pstPrev;

    SL_FOREACH_PREVPTR (pstList, pstCur, pstPrev)
    {
        if (pstCur == pstNode)
        {
            (VOID) SL_DelAfter(pstList, pstPrev);
            break;
        }
    }
    return;
}

static inline VOID SL_Append(IN SL_HEAD_S* pstDstList, INOUT SL_HEAD_S* pstSrcList)
{
    SL_NODE_S *pstNode, *pstPrev;

    if (BOOL_TRUE != SL_IsEmpty(pstSrcList))
    {
        SL_FOREACH_PREVPTR(pstDstList, pstNode, pstPrev)
        {
            ; 
        }
        
        if (NULL == pstPrev)
        {
            pstDstList->pstFirst = SL_First(pstSrcList);
        }
        else
        {
            pstPrev->pstNext = SL_First(pstSrcList);
        }

        SL_Init(pstSrcList);
    }
    
    return;
}

static inline VOID SL_FreeAll(IN SL_HEAD_S *pstList, IN VOID (*pfFree)(VOID *))
{
    SL_NODE_S *pstCurNode;
    SL_NODE_S *pstNextNode;

    
    SL_FOREACH_SAFE(pstList, pstCurNode, pstNextNode)
    {
        pfFree(pstCurNode);
    }

    SL_Init(pstList);
    return;
}



#ifdef __cplusplus
}
#endif
#endif 
