/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2007-9-7
* Description: 
* History:     
******************************************************************************/

#ifndef __MBUF_UTL_H_
#define __MBUF_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

#include "utl/net.h"
#include "utl/event_utl.h"

#define MBUF_DFT_RESERVED_HEAD_SPACE 200

#define _MBUF_POOL_DFT_CLUSTER_SIZE 2048

#define MBUF_DATA_CONTROL                     0x01
#define MBUF_DATA_DATA                        0x02
#define MBUF_DATA_NAME                        0x03
#define MBUF_DATA_OOBDATA                     0x04


#define MBUF_POOL_NAME_MAX_LEN 32

#define MBUF_GET_DATABLOK_SIZE_BY_MBUF(pstMbuf)  (_MBUF_POOL_DFT_CLUSTER_SIZE)
#define MBUF_TOTAL_DATA_LEN(pstMbuf)             ((pstMbuf)->ulTotalDataLen)
#define MBUF_DATABLOCK_NUM(pstMbuf)              (DLL_COUNT(&(pstMbuf)->stMblkHead))
#define MBUF_GET_FIRST_DATABLOCK(pstMbuf)        ((NULL == (pstMbuf)) ? NULL : DLL_FIRST((pstMbuf)->stMblkHead))
#define MBUF_GET_LAST_DATABLOCK(pstMbuf)         ((NULL == (pstMbuf)) ? NULL : DLL_LAST((pstMbuf)->stMblkHead))
#define MBUF_GET_NEXT_DATABLOCK(pstMbuf,pstMblk) (DLL_NEXT(&(pstMbuf)->stMblkHead,pstMblk))
#define MBUF_CLUSTER_SIZE(pstCluster)            ((pstCluster)->ulSize)
#define MBUF_CLUSTER_REF(pstCluster)             ((pstCluster)->ulRefCnt)
#define MBUF_DATABLOCK_LEN(pstMblk)              ((pstMblk)->ulLen)
#define MBUF_DATABLOCK_HEAD_FREE_LEN(pstMblk)    ((UINT)((pstMblk)->pucData - (pstMblk)->pstCluster->pucData))
#define MBUF_DATABLOCK_TAIL_FREE_LEN(pstMblk)    ((pstMblk)->pstCluster->ulSize - MBUF_DATABLOCK_HEAD_FREE_LEN (pstMblk) - (pstMblk)->ulLen)
#define MBUF_GET_NEXT_MBUF(pstMbuf)              ((MBUF_S*)((pstMbuf)->pstNextMbuf))
#define MBUF_SET_NEXT_MBUF(pstMbuf,_pstNextMbuf)  ((pstMbuf)->pstNextMbuf = (VOID*)(_pstNextMbuf))
#define MBUF_GET_MBUF_TYPE(pstMbuf)              ((pstMbuf)->ucType)

#define MBUF_SCAN_DATABLOCK_BEGIN(_pstMbuf,_pucData,_ulDataLen)  \
    do {    \
        MBUF_MBLK_S *_pstMblk;  \
        DLL_SCAN (&_pstMbuf->stMblkHead, _pstMblk)   \
        {   \
            _pucData = _pstMblk->pucData;    \
            _ulDataLen = _pstMblk->ulLen;   \
            {

#define MBUF_SCAN_END()  \
    }}}while (0)


#define MBUF_CAT_EXT(pstDstMbuf,pstSrcMbuf)  \
    do{ \
        if ((pstDstMbuf) == NULL) \
        {   \
            (pstDstMbuf) = (pstSrcMbuf);    \
        }   \
        else    \
        {   \
            MBUF_NeatCat(pstDstMbuf, pstSrcMbuf);	\
        }   \
    }while(0);

typedef struct structMBUF_CLUSTER_S
{
    UINT           ulSize; 
    UINT           ulRefCnt;
    UCHAR           *pucData;
}MBUF_CLUSTER_S;

typedef struct structMBUF_MBLK_S
{
    DLL_NODE(structMBUF_MBLK_S) stMbufBlkLinkNode;  
    UCHAR            *pucData;           
    UINT            ulLen;      
    MBUF_CLUSTER_S   *pstCluster;
}MBUF_MBLK_S;

#if 1

typedef VOID (*PF_MBUF_USER_CONTEXT_FREE)(IN HANDLE hUserContext);
typedef BS_STATUS (*PF_MBUF_USER_CONTEXT_DUP)(IN HANDLE hUserContext, OUT HANDLE *phUserContext);

typedef struct
{
    VOID *pUserContextData;
    PF_MBUF_USER_CONTEXT_FREE pfFreeFunc;
    PF_MBUF_USER_CONTEXT_DUP pfDupFunc;     
}MBUF_USER_CONTEXT_S;

#define MBUF_GET_USER_CONTEXT_DATA(pstMbuf) ((pstMbuf)->stUserContext.pUserContextData)
#define MBUF_GET_USER_CONTEXT(pstMbuf) (&((pstMbuf)->stUserContext))


#endif

#if 1

#define MBUF_GET_RECV_IF_INDEX(pstMbuf) ((pstMbuf)->stTag.stIfInfo.uiRecvIfIndex)
#define MBUF_SET_RECV_IF_INDEX(pstMbuf, uiIfIndex) ((pstMbuf)->stTag.stIfInfo.uiRecvIfIndex = (uiIfIndex))
#define MBUF_GET_SEND_IF_INDEX(pstMbuf) ((pstMbuf)->stTag.stIfInfo.uiSendIfIndex)
#define MBUF_SET_SEND_IF_INDEX(pstMbuf, uiIfIndex) ((pstMbuf)->stTag.stIfInfo.uiSendIfIndex = (uiIfIndex))
#define MBUF_GET_NEXT_HOP(pstMbuf) ((pstMbuf)->stTag.stIfInfo.uiNextHop)
#define MBUF_SET_NEXT_HOP(pstMbuf, _uiNextHop) ((pstMbuf)->stTag.stIfInfo.uiNextHop = (_uiNextHop))

typedef struct
{
    UINT uiRecvIfIndex;
    UINT uiSendIfIndex;
    UINT uiNextHop;         
}MBUF_IF_INFO_S;

#endif

#if 1 




#define MBUF_L2_FLAG_SRC_MAC                  0x01      
#define MBUF_L2_FLAG_DST_MAC                  0x02      
#define MBUF_L2_FLAG_ENCAPED                  0x04      
#define MBUF_L2_FLAG_ALL                      0xffff


typedef struct tagMBufEthernetHdr
{
    USHORT usL2Type;                 
    USHORT usMarkFlag;               
    UCHAR  aucSrcAddr[6];            
    UCHAR  aucDstAddr[6];            
    UCHAR  ucPktFmt;                 
} MBUF_ETHERNET_HDR_S;


#define MBUF_LINK_TYPE_X25                    1
#define MBUF_LINK_TYPE_ETHERNET               2
#define MBUF_LINK_TYPE_FRAME_RELAY            3
#define MBUF_LINK_TYPE_ATM                    4
#define MBUF_LINK_TYPE_PPP                    5
#define MBUF_LINK_TYPE_DOT11                  6
#define MBUF_LINK_TYPE_CHDLC                  7
#define MBUF_LINK_TYPE_LAPB                   8


typedef struct
{
    UCHAR  ucLinkType;             
    UCHAR  ucLinkHeadSize;

    union
    {
        MBUF_ETHERNET_HDR_S stEthHdr;
    }unL2Hdr;
}MBUF_LINK_INFO_S;
#endif

#if 1 


#define IP_PKT_HOST                 0x00000001   
#define IP_PKT_HOSTSENDPKT          0x00000002   
#define IP_PKT_FORWARDPKT           0x00000004   
#define IP_PKT_IIFBRAODCAST         0x00000008   
#define IP_PKT_OIFBRAODCAST         0x00000010   
#define IP_PKT_MULTICAST            0x00000020   
#define IP_PKT_ETHBCAST             0x00000040   
#define IP_PKT_ETHMCAST             0x00000080   
#define IP_PKT_SRCROUTE             0x00000100   
#define IP_PKT_FILLTAG              0x00000200   


typedef union tagPacketInfo
{
    struct tagIpTcpUdpPort
    {
        USHORT usSourcePort;
        USHORT usDestinationPort;
    }stTcpUdpPort;

    struct tagIcmpInfo
    {
        UCHAR ucIcmpType;
        UCHAR ucIcmpCode;
        USHORT usIcmpID;
    }stIcmpInfo;
}MBUF_PACKET_INFO_U;


typedef struct tagMBufIpHdr        
{
    UINT uiSrcIp;              
    UINT uiDstIp;              
    UINT uiNextHop;            
    UINT uiIpPktType;          
    UINT uiCacheFlag;          
    
    MBUF_PACKET_INFO_U unPacketInfo;
    
    UCHAR ucTos;                 
    UCHAR ucProtocol;            
    UCHAR ucIsFragment:1;        
    UCHAR ucIsFirstFrag:1;       
    UCHAR ucTCPFlags:6;          
} MBUF_IP_HDR_S;
#endif

#if 1   
typedef struct tag_MbufVrf
{
    VRF_INDEX vrfIndexRawIn;
    VRF_INDEX vrfIndexIn;
    VRF_INDEX vrfIndexOut;
}MBUF_VRF_S;

#endif

typedef struct
{
    
    MBUF_IF_INFO_S stIfInfo;

    
    MBUF_VRF_S stVrf;

    
    union {
        MBUF_LINK_INFO_S stLinkInfo;
    }unL2Hdr;

    
    union {
        MBUF_IP_HDR_S stIpHdr;
    }unL3Hdr;
    
}MBUF_TAG_S;

typedef struct structMBUF_S
{
    struct structMbuf_s *pstNextMbuf;
    DLL_HEAD_S          stMblkHead;
    UINT                ulTotalDataLen;
    UCHAR               ucType;

    
    MBUF_TAG_S stTag;

    
    MBUF_USER_CONTEXT_S stUserContext;    
}MBUF_S;

typedef struct tagMBufQueue
{
    MBUF_S * pstHeadMBuf;
    MBUF_S * pstTailMBuf;
    UINT uiCurrentLength;
    UINT uiMaxLength;
}MBUF_QUE_S;

#define MBUF_QUE_INIT(_pstMBufQue, _uiMaxLength)\
{\
    (_pstMBufQue)->pstHeadMBuf = NULL;\
    (_pstMBufQue)->pstTailMBuf = NULL;\
    (_pstMBufQue)->uiCurrentLength = 0;\
    (_pstMBufQue)->uiMaxLength = (_uiMaxLength);\
}

#define MBUF_QUE_IS_EMPTY(_pstMBufQue) ((_pstMBufQue)->uiCurrentLength == 0)
#define MBUF_QUE_IS_FULL(_pstMBufQue) ((_pstMBufQue)->uiCurrentLength >= (_pstMBufQue)->uiMaxLength)
#define MBUF_QUE_GET_COUNT(_pstMBufQue) ((_pstMBufQue)->uiCurrentLength)


#define MBUF_QUE_PUSH(_pstMbufQue, _pstMbuf)    \
    do {    \
        MBUF_SET_NEXT_MBUF(_pstMbuf, NULL);     \
        if ((_pstMbufQue)->pstTailMBuf == NULL)     \
        {   \
            (_pstMbufQue)->pstHeadMBuf = (_pstMbuf);    \
        }   \
        else    \
        {   \
            MBUF_SET_NEXT_MBUF((_pstMbufQue)->pstTailMBuf, _pstMbuf);   \
        }   \
        (_pstMbufQue)->pstTailMBuf = (_pstMbuf);    \
        (_pstMbufQue)->uiCurrentLength++;  \
    }while(0)

#define MBUF_QUE_POP(_pstMBufQue, _pstMBuf)\
    do{ \
        (_pstMBuf) = (_pstMBufQue)->pstHeadMBuf;    \
        if((_pstMBufQue)->pstHeadMBuf != NULL)  \
        {   \
            (_pstMBufQue)->pstHeadMBuf = MBUF_GET_NEXT_MBUF((_pstMBufQue)->pstHeadMBuf);    \
            if((_pstMBufQue)->pstHeadMBuf == NULL)  \
            {   \
                (_pstMBufQue)->pstTailMBuf = NULL;  \
            }   \
            (_pstMBufQue)->uiCurrentLength--;  \
        }   \
    }while(0)

#define MBUF_QUE_PEEK(_pstMBufQue) ((_pstMBufQue)->pstHeadMBuf)

#define MBUF_QUE_FREE_ALL(_pstMBufQue)  \
    for(;;)   \
    {   \
        MBUF_S * _pstMbuf;  \
        MBUF_QUE_POP(_pstMBufQue, _pstMbuf);    \
        if (_pstMbuf == NULL)   \
        {   \
            break;  \
        }   \
        MBUF_Free(_pstMbuf);    \
    }

typedef struct structMBUF_ITOR_S
{
    MBUF_MBLK_S *pstMblk;
    UINT        ulOffset;
}MBUF_ITOR_S;

extern MBUF_CLUSTER_S * MBUF_CreateCluster();
extern VOID MBUF_FreeCluster(IN MBUF_CLUSTER_S *pstCluster);
extern MBUF_S * MBUF_Create (IN UCHAR ucType, IN UINT ulHeadSpaceLen);
extern VOID MBUF_Free (IN MBUF_S *pstMbuf);

extern BS_STATUS _MBUF_Compress (IN MBUF_S *pstMbuf);
extern BS_STATUS MBUF_ApppendBlock (IN MBUF_S *pstMbuf, IN UINT ulLen);
extern BS_STATUS MBUF_PrependBlock (IN MBUF_S *pstMbuf, IN UINT ulLen);
extern BS_STATUS MBUF_CutHead (IN MBUF_S *pstMbuf, IN UINT ulCutLen);
extern BS_STATUS MBUF_CutTail (IN MBUF_S *pstMbuf, IN UINT ulCutLen);
extern BS_STATUS MBUF_CutPart (IN MBUF_S *pstMbuf, IN UINT ulOffset, IN UINT ulCutLen);
extern VOID MBUF_Neat (IN MBUF_S *pstMbuf);
extern BS_STATUS _MBUF_MakeContinue (IN MBUF_S * pstMbuf, IN UINT ulLen);
extern BS_STATUS MBUF_CopyFromMbufToBuf (IN MBUF_S *pstMbuf, IN UINT ulOffset, IN UINT ulLen, OUT void *buf);
extern BS_STATUS MBUF_InsertBuf(IN MBUF_S *pstMbuf, IN UINT ulOffset, IN void *buf, IN UINT ulLen);
extern MBUF_S * MBUF_Fragment (IN MBUF_S *pstMbuf, IN UINT ulLen);
extern MBUF_S * MBUF_CreateByCopyBuf
(
    IN UINT ulReserveHeadSpace,
    IN void *pBuf,
    IN UINT ulLen,
    IN UCHAR ucType
);
extern MBUF_MBLK_S * MBUF_MblkFragment (IN MBUF_MBLK_S *pstMblk, IN UINT ulLen);

extern MBUF_MBLK_S * MBUF_CreateMblkByCopyBuf (IN void *buf, IN UINT ulLen, IN UINT ulReservrdHeadSpace);
extern BS_STATUS MBUF_CopyFromBufToMbuf (IN MBUF_S *pstMbuf, IN UINT ulOffset, IN void *buf, IN UINT ulLen);
extern MBUF_MBLK_S * MBUF_FindMblk (IN MBUF_S *pstMbuf, IN UINT ulOffset, OUT UINT *pulOffsetInThisMblk);
extern MBUF_S * MBUF_ReferenceCopy (IN MBUF_S *pstMbuf, IN UINT ulOffset, IN UINT ulLen);
extern MBUF_S * MBUF_RawCopy (IN MBUF_S *pstMbuf, IN UINT ulOffset, IN UINT ulLen, IN UINT ulReservedHeadSpace);
extern MBUF_S * MBUF_CreateByCluster
(
    IN MBUF_CLUSTER_S *pstCluster,
    IN UINT ulReserveHeadSpace,
    IN UINT ulLen,
    IN UCHAR ucType
);

extern INT MBUF_Find (IN MBUF_S *pstMbuf, IN UINT ulStartOffset, IN UCHAR *pMemToFind, IN UINT ulMemLen);
extern INT MBUF_FindByte(IN MBUF_S *pstMbuf, IN UINT ulStartOffset, IN UCHAR ucByteToFind);

extern INT MBUF_NFind (IN MBUF_S *pstMbuf, IN UINT ulStartOffset, IN UINT ulLen, IN UCHAR *pMemToFind, IN UINT ulMemLen);
extern BS_STATUS MBUF_AddCluster (IN MBUF_S *pstMbufDst, IN MBUF_CLUSTER_S *pstCluster, IN UINT ulOffset, IN UINT ulDataLen);
extern BS_STATUS MBUF_Set(IN MBUF_S *pstMbuf, IN UCHAR ucToSet, IN UINT ulOffset, IN UINT ulSetLen);
extern UCHAR * MBUF_GetContinueMem(IN MBUF_S *pstMbuf, IN UINT ulOffset, IN UINT ulLen, OUT HANDLE *phMemHandle);
extern void* MBUF_GetContinueMemRaw(IN MBUF_S *pstMbuf, IN UINT ulOffset, IN UINT ulLen, OUT HANDLE *phMemHandle);
extern VOID MBUF_FreeContinueMem(IN HANDLE hMemHandle);
extern UCHAR * MBUF_GetChar (IN MBUF_S *pstMbuf, IN UINT ulOffset, OUT MBUF_ITOR_S *pstItor);
extern UCHAR * MBUF_GetNextChar (IN MBUF_S *pstMbuf, INOUT MBUF_ITOR_S *pstItor);

extern UINT MBUF_DelBlankSideByIndex(IN MBUF_S *pstMbuf, IN UINT ulStartOffset, INOUT UINT *pulOffset);
extern VOID MBUF_PrintAsString(IN MBUF_S *pstMbuf);
extern VOID MBUF_PrintAsHex(IN MBUF_S *pstMbuf);

extern BS_STATUS MBUF_Append(IN MBUF_S *pstMbuf, IN UINT ulLen);

extern BS_STATUS MBUF_AppendData(IN MBUF_S *pstMbuf, IN UCHAR *pucData, IN UINT uiDataLen);

extern BS_STATUS MBUF_Prepend (IN MBUF_S *pstMbuf, IN UINT ulLen);

extern BS_STATUS MBUF_PrependData(IN MBUF_S *pstMbuf, IN UCHAR *pucData, IN UINT uiDataLen);

static inline BS_STATUS MBUF_MakeContinue (IN MBUF_S * pstMbuf, IN UINT ulLen)
{
    MBUF_MBLK_S *pstMblk;

    pstMblk = (MBUF_MBLK_S *)DLL_FIRST (&pstMbuf->stMblkHead);

    if ((pstMblk != NULL) && (MBUF_DATABLOCK_LEN (pstMblk) >= ulLen))
    {
        return BS_OK;
    }

    return _MBUF_MakeContinue (pstMbuf, ulLen);
}


static inline BS_STATUS MBUF_Compress (IN MBUF_S * pstMbuf)
{
    BS_DBGASSERT(NULL != pstMbuf);
    
    if (MBUF_DATABLOCK_NUM(pstMbuf) <= 1)
    {
        return BS_OK;
    }

    return _MBUF_Compress(pstMbuf);
}

static inline BS_STATUS MBUF_Cat (IN MBUF_S *pstMbufDst, IN MBUF_S *pstMbufSrc)
{
    BS_DBGASSERT (NULL != pstMbufDst);
    BS_DBGASSERT (NULL != pstMbufSrc);

    DLL_CAT (&pstMbufDst->stMblkHead, &pstMbufSrc->stMblkHead);
    MBUF_TOTAL_DATA_LEN (pstMbufDst) += MBUF_TOTAL_DATA_LEN (pstMbufSrc);
    MBUF_Free (pstMbufSrc);

    return BS_OK;
}

static inline BS_STATUS MBUF_NeatCat (IN MBUF_S *pstMbufDst, IN MBUF_S *pstMbufSrc)
{
    MBUF_Cat (pstMbufDst, pstMbufSrc);
    MBUF_Neat (pstMbufDst);

    return BS_OK;
}

static inline VOID * MBUF_MTOD (IN MBUF_S *pstMbuf)
{
    MBUF_MBLK_S *pstMblk;
    
    BS_DBGASSERT (NULL != pstMbuf);

    pstMblk = (MBUF_MBLK_S *)DLL_FIRST (&pstMbuf->stMblkHead);

    return ((pstMblk == NULL) ? NULL : pstMblk->pucData);
}

static inline BS_STATUS MBUF_AddTailData(INOUT MBUF_S *pstMbuf, IN UCHAR *pucData, IN UINT uiDataLen)
{
    return MBUF_CopyFromBufToMbuf(pstMbuf, MBUF_TOTAL_DATA_LEN(pstMbuf), pucData, uiDataLen);
}

static inline BS_STATUS MBUF_AddTailString(INOUT MBUF_S *pstMbuf, IN CHAR *pcString)
{
    return MBUF_CopyFromBufToMbuf(pstMbuf, MBUF_TOTAL_DATA_LEN(pstMbuf), (UCHAR*)pcString, strlen(pcString));
}

#if 1  
static inline USHORT MBUF_GET_ETH_MARKFLAG (IN MBUF_S *pstMBuf)
{
    return pstMBuf->stTag.unL2Hdr.stLinkInfo.unL2Hdr.stEthHdr.usMarkFlag;
}
static inline VOID MBUF_SET_ETH_MARKFLAG (IN MBUF_S *pstMBuf, IN USHORT usFlag)
{
    pstMBuf->stTag.unL2Hdr.stLinkInfo.unL2Hdr.stEthHdr.usMarkFlag |= usFlag;
    pstMBuf->stTag.unL2Hdr.stLinkInfo.ucLinkType = MBUF_LINK_TYPE_ETHERNET;
    return;
}
static inline VOID MBUF_CLEAR_ETH_MARKFLAG (IN MBUF_S *pstMBuf, IN USHORT usFlag)
{
    pstMBuf->stTag.unL2Hdr.stLinkInfo.unL2Hdr.stEthHdr.usMarkFlag  &= (USHORT)(~usFlag);
    return;
}


static inline VOID MBUF_SET_ETH_L2TYPE (IN MBUF_S *pstMBuf, IN USHORT usL2Protocol)
{
    pstMBuf->stTag.unL2Hdr.stLinkInfo.unL2Hdr.stEthHdr.usL2Type = usL2Protocol;
    return;
}
static inline USHORT MBUF_GET_ETH_L2TYPE (IN MBUF_S *pstMBuf)
{
    return pstMBuf->stTag.unL2Hdr.stLinkInfo.unL2Hdr.stEthHdr.usL2Type;
}


static inline VOID MBUF_SET_ETH_PKTFMT (IN MBUF_S * pstMBuf, IN UCHAR ucPacketFmt)
{
    pstMBuf->stTag.unL2Hdr.stLinkInfo.unL2Hdr.stEthHdr.ucPktFmt = ucPacketFmt;
    return;
}
static inline UCHAR MBUF_GET_ETH_PKTFMT (IN MBUF_S * pstMBuf) 
{
    return pstMBuf->stTag.unL2Hdr.stLinkInfo.unL2Hdr.stEthHdr.ucPktFmt;
}


static inline MBUF_ETHERNET_HDR_S * MBUF_GET_ETHERNET_HDR (IN MBUF_S * pstMBuf)
{
    return (&(pstMBuf->stTag.unL2Hdr.stLinkInfo.unL2Hdr.stEthHdr));
}


static inline UCHAR *MBUF_GET_SOURCEMAC(IN MBUF_S * pstMBuf)
{
    return (pstMBuf->stTag.unL2Hdr.stLinkInfo.unL2Hdr.stEthHdr.aucSrcAddr);
}
static inline UCHAR *MBUF_GET_DESTMAC(IN MBUF_S * pstMBuf)
{
    return (pstMBuf->stTag.unL2Hdr.stLinkInfo.unL2Hdr.stEthHdr.aucDstAddr);
}

static inline VOID MBUF_SET_SOURCEMAC(IN MBUF_S * pstMBuf, IN UCHAR* pucSrcMac)
{
    UCHAR *pucMacAddr = pstMBuf->stTag.unL2Hdr.stLinkInfo.unL2Hdr.stEthHdr.aucSrcAddr;

    *((USHORT *)(VOID *)pucMacAddr) = *((USHORT *)(VOID *)pucSrcMac);
    *((USHORT *)(VOID *)pucMacAddr + 1) = *((USHORT *)(VOID *)pucSrcMac + 1);
    *((USHORT *)(VOID *)pucMacAddr + 2) = *((USHORT *)(VOID *)pucSrcMac + 2);

    return;
}
static inline VOID MBUF_SET_DESTMAC(IN MBUF_S * pstMBuf,IN UCHAR* pucDstMac)
{
    UCHAR *pucMacAddr = pstMBuf->stTag.unL2Hdr.stLinkInfo.unL2Hdr.stEthHdr.aucDstAddr;

    *((USHORT *)(VOID *)pucMacAddr) = *((USHORT *)(VOID *)pucDstMac);
    *((USHORT *)(VOID *)pucMacAddr + 1) = *((USHORT *)(VOID *)pucDstMac + 1);
    *((USHORT *)(VOID *)pucMacAddr + 2) = *((USHORT *)(VOID *)pucDstMac + 2);

    return;
}
#endif

#if 1   
static inline UINT MBUF_GET_IP_PKTTYPE (IN MBUF_S *pstMBuf)
{
    return pstMBuf->stTag.unL3Hdr.stIpHdr.uiIpPktType;
}
#endif

#if 1   


static inline VOID MBUF_SET_RAWINVPNID (IN MBUF_S *pstMBuf, IN VRF_INDEX vrfIndexRawIn)
{
    pstMBuf->stTag.stVrf.vrfIndexRawIn = vrfIndexRawIn;
    return;
}

static inline VRF_INDEX MBUF_GET_RAWINVPNID (IN MBUF_S *pstMBuf)
{
    return (pstMBuf->stTag.stVrf.vrfIndexRawIn);
}


static inline VOID MBUF_SET_INVPNID (IN MBUF_S *pstMBuf, IN VRF_INDEX vrfIndexIn)
{
    pstMBuf->stTag.stVrf.vrfIndexIn = vrfIndexIn;
    return;
}

static inline VRF_INDEX MBUF_GET_INVPNID (IN MBUF_S *pstMBuf)
{
    return (pstMBuf->stTag.stVrf.vrfIndexIn);
}


static inline VOID MBUF_SET_OUTVPNID (IN MBUF_S *pstMBuf, IN VRF_INDEX vrfIndexOut)
{
    pstMBuf->stTag.stVrf.vrfIndexOut = vrfIndexOut;
    return;
}

static inline VRF_INDEX MBUF_GET_OUTVPNID (IN MBUF_S *pstMBuf)
{
    return (pstMBuf->stTag.stVrf.vrfIndexOut);
}
#endif

#ifdef __cplusplus
    }
#endif 

#endif 


