/******************************************************************************
* Copyright (C), LiXingang
* Author:      LiXingang  Date: 2016-7-5
* Description: 
* History:     
******************************************************************************/

#ifndef __IP_LIST_H_
#define __IP_LIST_H_

#ifdef __cplusplus
    extern "C" {
#endif 


typedef struct
{
    DLL_NODE_S stLinkNode;
    UINT uiBeginIp; 
    UINT uiEndIp;  
}IPLIST_NODE_S;

typedef struct
{
    DLL_HEAD_S stList;
}IPLIST_S;

static inline VOID IPList_Init(IN IPLIST_S *pstList)
{
    DLL_INIT(&pstList->stList);
}

VOID IPList_Finit(IN IPLIST_S *pstList);
BS_STATUS IPList_ParseString(IN IPLIST_S *pstList, IN LSTR_S *pstIpStrList);

BOOL_T IPLIst_IsOverlap(IN IPLIST_S *pstList, IN UINT uiStartIP, IN UINT uiEndIP);
BOOL_T IPList_IsIPInTheList(IN IPLIST_S *pstList, IN UINT uiIP);
BS_STATUS IPList_AddRange(IN IPLIST_S *pstList, IN UINT uiBeginIP, IN UINT uiEndIP);
BS_STATUS IPList_DelRange(IN IPLIST_S *pstList, IN UINT uiBeginIP, IN UINT uiEndIP);
BS_STATUS IPList_ModifyRange
(
    IN IPLIST_S *pstList,
    IN UINT uiOldBeginIP,
    IN UINT uiOldEndIP,
    IN UINT uiBeginIP,
    IN UINT uiEndIP
);

VOID IPLIst_Cat(INOUT IPLIST_S *pstListDst, INOUT IPLIST_S *pstListSrc);

VOID IPList_Sort(IN IPLIST_S *pstList);

VOID IPList_Compress(IN IPLIST_S *pstList);

VOID IPList_Continue(IN IPLIST_S *pstList);

BS_STATUS IPList_Range2SubNetList
(
    INOUT IPLIST_S *pstList,
    IN UINT uiBeginIP,
    IN UINT uiEndIP
);

BS_STATUS IPLIst_SplitBySubNet(INOUT IPLIST_S *pstList);
BS_STATUS IPLIst_ToString(IN IPLIST_S *pstList, OUT CHAR *pcString, IN UINT uiSize);

#define IPLIST_SCAN_BEGIN(_pstList, _uiBeginIP, _uiEndIP)   do { \
    IPLIST_NODE_S *_pstNode;    \
    DLL_SCAN(&(_pstList)->stList, _pstNode)  \
    {   \
        (_uiBeginIP) = _pstNode->uiBeginIp; \
        (_uiEndIP) = _pstNode->uiEndIp;

#define IPLIST_SCAN_END()   }}while(0)

#ifdef __cplusplus
    }
#endif 

#endif 


