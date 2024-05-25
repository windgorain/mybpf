/*================================================================
* Author：LiXingang. Data: 2019.08.12
* Description: 
*
================================================================*/
#ifndef _LIST_DL_H
#define _LIST_DL_H
#ifdef __cplusplus
extern "C"
{
#endif


typedef struct tagDL_NODE
{
    struct tagDL_NODE*  pstNext;  
    struct tagDL_NODE** ppstPre;  
} DL_NODE_S;

#define DL_ENTRY(ptr, type, member) (container_of(ptr, type, member))

#define DL_NODE_FROM_PPRE(ppstPre)  (container_of(ppstPre, DL_NODE_S, pstNext))

#define DL_ENTRY_FROM_PPRE(ppstPre, type, member) \
    DL_ENTRY(DL_NODE_FROM_PPRE(ppstPre), type, member)

typedef struct tagDL_HEAD
{
    DL_NODE_S* pstFirst; 
} DL_HEAD_S;

static inline VOID DL_Init(IN DL_HEAD_S* pstList);
static inline VOID DL_NodeInit(IN DL_NODE_S* pstNode);
static inline BOOL_T DL_IsEmpty(IN const DL_HEAD_S* pstList);
static inline DL_NODE_S* DL_First(IN const DL_HEAD_S* pstList);
static inline DL_NODE_S* DL_Next(IN const DL_NODE_S* pstNode);
static inline DL_NODE_S* DL_Prev(IN const DL_NODE_S* pstNode);
static inline DL_NODE_S* DL_DelHead(IN const DL_HEAD_S* pstList);
static inline VOID DL_AddAfter(IN DL_NODE_S* pstPrev, IN DL_NODE_S* pstInst);
static inline VOID DL_AddAfterPtr (IN DL_NODE_S **ppstPre, IN DL_NODE_S  *pstInst);
static inline VOID DL_AddBefore(IN DL_NODE_S* pstNext, IN DL_NODE_S* pstInst);
static inline VOID DL_Append(IN DL_HEAD_S* pstDstList, IN DL_HEAD_S* pstSrcList);
static inline VOID DL_FreeAll(IN DL_HEAD_S *pstList, IN VOID (*pfFree)(VOID *));

static inline VOID DL_Init(IN DL_HEAD_S* pstList)
{
    pstList->pstFirst = (DL_NODE_S*)NULL;
    return;
}

static inline VOID DL_NodeInit(IN DL_NODE_S* pstNode)
{
    pstNode->pstNext = (DL_NODE_S*)NULL;
    pstNode->ppstPre = (DL_NODE_S**)NULL;
}

static inline BOOL_T DL_IsEmpty(IN const DL_HEAD_S* pstList)
{
    return (pstList->pstFirst == NULL);
}

static inline DL_NODE_S* DL_First(IN const DL_HEAD_S* pstList)
{
    return (pstList->pstFirst);
}

static inline DL_NODE_S* DL_Next(IN const DL_NODE_S* pstNode)
{
    if (NULL == pstNode) {
        return NULL;
    }
    return pstNode->pstNext;
}

static inline DL_NODE_S* DL_Prev(IN const DL_NODE_S* pstNode)
{
    return DL_NODE_FROM_PPRE(pstNode->ppstPre);
}

static inline void DL_Del(INOUT DL_NODE_S* pstNode)
{
    if (pstNode->ppstPre) {
        *pstNode->ppstPre = pstNode->pstNext;
    }
    if (pstNode->pstNext) {
        pstNode->pstNext->ppstPre = pstNode->ppstPre;
    }

    return;
}

static inline void DL_AddHead(DL_HEAD_S* pstList, DL_NODE_S* pstNode)
{
    pstNode->ppstPre = &pstList->pstFirst;
    pstNode->pstNext = pstList->pstFirst;
    if (pstNode->pstNext) {
        pstNode->pstNext->ppstPre = &pstNode->pstNext;
    }
    pstList->pstFirst = pstNode;
    return;
}

static inline DL_NODE_S* DL_DelHead(IN const DL_HEAD_S* pstList)
{
    DL_NODE_S* pstNode = DL_First(pstList);
    if (NULL != pstNode)
    {
        DL_Del(pstNode);
    }

    return pstNode;
}

static inline VOID DL_AddAfter(IN DL_NODE_S* pstPrev, IN DL_NODE_S* pstInst)
{
    pstInst->ppstPre = &pstPrev->pstNext;
    pstInst->pstNext = pstPrev->pstNext;
    pstPrev->pstNext = pstInst;
    if (NULL != pstInst->pstNext)
    {
        pstInst->pstNext->ppstPre = &pstInst->pstNext;
    }

    return;
}

static inline VOID DL_AddAfterPtr (IN DL_NODE_S **ppstPre, IN DL_NODE_S  *pstInst)
{
    pstInst->ppstPre = ppstPre;
    pstInst->pstNext = *ppstPre;
    *ppstPre = pstInst;
    if (NULL != pstInst->pstNext)
    {
        pstInst->pstNext->ppstPre = &pstInst->pstNext;
    }
    return;
}

static inline VOID DL_AddBefore(IN DL_NODE_S* pstNext, IN DL_NODE_S* pstInst)
{
    pstInst->ppstPre = pstNext->ppstPre;
    pstInst->pstNext = pstNext;
    if (NULL != pstInst->ppstPre)
    {
        *pstInst->ppstPre = pstInst;
    }
    pstInst->pstNext->ppstPre = &pstInst->pstNext;

    return;
}


#define DL_FOREACH(pstList, pstNode) \
    for ((pstNode) = DL_First((pstList)); \
         NULL != (pstNode); \
         (pstNode) = DL_Next(pstNode))

#define DL_FOREACH_SAFE(pstList, pstNode, pstNext) \
    for ((pstNode) = DL_First((pstList)), (pstNext) = DL_Next(pstNode); \
        (NULL != (pstNode)); \
        (pstNode) = (pstNext), (pstNext) = DL_Next(pstNode))

#define DL_FOREACH_PREVPTR(pstList, pstNode, ppstPre) \
    for ((pstNode) = DL_First(pstList), (ppstPre) = &((pstList)->pstFirst); \
         NULL != (pstNode); \
         (ppstPre) = &((pstNode)->pstNext), (pstNode) = DL_Next(pstNode))

#define DL_ENTRY_FIRST(pstList, type, member) \
    (DL_IsEmpty(pstList) ? NULL : DL_ENTRY(DL_First(pstList), type, member))

#define DL_ENTRY_NEXT(pstEntry, member) \
    (NULL == (pstEntry) ? NULL : \
       (NULL == DL_Next(&((pstEntry)->member)) ? NULL : \
          DL_ENTRY(DL_Next(&((pstEntry)->member)), typeof(*(pstEntry)), member)))

#define DL_ENTRY_PREV(pstEntry, member) \
    (NULL == (pstEntry) ? NULL : \
       (NULL == DL_Prev(&((pstEntry)->member)) ? NULL : \
           DL_ENTRY(DL_Prev(&((pstEntry)->member)), typeof(*(pstEntry)), member)))

#define DL_FOREACH_ENTRY(pstList, pstEntry, member) \
    for ((pstEntry) = DL_ENTRY_FIRST(pstList, typeof(*(pstEntry)), member); \
         NULL != (pstEntry); \
         (pstEntry) = DL_ENTRY_NEXT(pstEntry, member))

#define DL_FOREACH_ENTRY_SAFE(pstList, pstEntry, pstNextEntry, member) \
    for ((pstEntry) = DL_ENTRY_FIRST(pstList, typeof(*(pstEntry)), member); \
         (NULL != (pstEntry)) && \
         ({(pstNextEntry) = DL_ENTRY_NEXT(pstEntry, member); BOOL_TRUE;}); \
         (pstEntry) = (pstNextEntry))

#define DL_FOREACH_ENTRY_PREVPTR(pstList, pstEntry, ppstPre, member) \
    for ((pstEntry) = DL_ENTRY_FIRST(pstList, typeof(*(pstEntry)), member), \
          (ppstPre) = &((pstList)->pstFirst); \
          NULL != (pstEntry); \
          (VOID)({(ppstPre) = &((pstEntry)->member.pstNext); \
                   (pstEntry) = DL_ENTRY_NEXT(pstEntry, member);}))


static inline VOID DL_Append(IN DL_HEAD_S* pstDstList, INOUT DL_HEAD_S* pstSrcList)
{
    DL_NODE_S *pstNode, **ppstPre;

    if (BOOL_TRUE != DL_IsEmpty (pstSrcList)) {
        
        DL_FOREACH_PREVPTR (pstDstList, pstNode, ppstPre) {
            ; 
        }
        
        *ppstPre = pstSrcList->pstFirst;
        pstSrcList->pstFirst->ppstPre = ppstPre;
        DL_Init(pstSrcList);
    }
    return;
}

static inline VOID DL_FreeAll(IN DL_HEAD_S *pstList, IN VOID (*pfFree)(VOID *))
{
    DL_NODE_S *pstCurNode;
    DL_NODE_S *pstNextNode;

    
    DL_FOREACH_SAFE(pstList, pstCurNode, pstNextNode)
    {
        pfFree(pstCurNode);
    }

    DL_Init(pstList);
    return;
}


#ifdef __cplusplus
}
#endif
#endif 
