

#ifndef __PKTCAP_UTL_H_
#define __PKTCAP_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

#define PKTCAP_NDIS_NAME_MAX_LEN 127

#define PKTCAP_FLAG_PROMISCUOUS 0x1 

typedef HANDLE PKTCAP_NDIS_HANDLE; 
typedef HANDLE PKTCAP_NIDS_INFO_HANDLE; 

typedef struct
{
    struct timeval ts; 
    UINT uiPktRawLen; 
    UINT uiPktCaptureLen; 
}PKTCAP_PKT_INFO_S;

typedef VOID (*PF_PKT_CAP_PACKET_IN_FUNC)(IN UCHAR *pucPktData, IN PKTCAP_PKT_INFO_S *pstPktInfo, IN HANDLE hUserHandle);

#define PKTCAP_NDIS_MAX_ADDR_NUM 16 

typedef struct
{
    UINT uiAddr;
    UINT uiMask;
}PKTCAP_NDIS_ADDR_S;

typedef struct
{
    PKTCAP_NDIS_ADDR_S astAddr[PKTCAP_NDIS_MAX_ADDR_NUM];    
}PKTCAP_NDIS_INFO_S;


PKTCAP_NIDS_INFO_HANDLE PKTCAP_GetNdisInfoList();

PKTCAP_NIDS_INFO_HANDLE PKTCAP_FindNdisByNameInNdisInfoList(IN PKTCAP_NIDS_INFO_HANDLE hNdisInfoList, IN CHAR *pcName);

PKTCAP_NIDS_INFO_HANDLE PKTCAP_GetNextNdisInfo(IN PKTCAP_NIDS_INFO_HANDLE hCurrentNdisInfo);

CHAR * PKTCAP_GetNdisInfoDesc(IN PKTCAP_NIDS_INFO_HANDLE hNdisInfo);

CHAR * PKTCAP_GetNdisInfoName(IN PKTCAP_NIDS_INFO_HANDLE hNdisInfo);

VOID PKTCAP_GetNdisInfoString(IN PKTCAP_NIDS_INFO_HANDLE hNdisInfo, IN UINT uiSize, OUT CHAR *pcString);

BOOL_T PKTCAP_IsNdisExist(IN CHAR *pcName);


VOID PKTCAP_PrintfNdisInfo(IN PKTCAP_NIDS_INFO_HANDLE hNdisInfo);

#define PKTCAP_INVALID_INDEX 0xffffffff

UINT PKTCAP_GetNdisIndexByName(IN PKTCAP_NIDS_INFO_HANDLE hNdisInfo, IN CHAR *pcNdisName);


CHAR * PKTCAP_GetNdisNameByIndex(IN PKTCAP_NIDS_INFO_HANDLE hNdisInfo, IN UINT uiIndex);

BS_STATUS PKTCAP_GetNdisInfoByName
(
    IN CHAR *pcNdisName,
    OUT PKTCAP_NDIS_INFO_S *pstInfo
);
BS_STATUS PKTCAP_GetNdisInfoByNameEx
(
    IN PKTCAP_NIDS_INFO_HANDLE hNdisInfo,
    IN CHAR *pcNdisName,
    OUT PKTCAP_NDIS_INFO_S *pstInfo
);

VOID PKTCAP_FreeNdisInfoList(IN PKTCAP_NIDS_INFO_HANDLE hNdisInfo);


PKTCAP_NDIS_HANDLE PKTCAP_OpenNdis(IN CHAR *pcNdisName, IN UINT uiFlag, IN UINT uiTimeOutTime);

INT PKTCAP_GetDataLinkTypeByName(IN CHAR *pcDevName);

INT PKTCAP_GetDataLinkType(IN PKTCAP_NDIS_HANDLE hNdisHandle);

BOOL_T PKTCAP_IsEthDataLink(IN INT iDataLink);


VOID PKTCAP_CloseNdis(IN PKTCAP_NDIS_HANDLE hNdisHandle);


BOOL_T PKTCAP_SendPacket
(
    IN PKTCAP_NDIS_HANDLE hNdisHandle,
    IN UCHAR *pucPkt,
    IN UINT uiPktLen
);


BOOL_T PKTCAP_SetFilter(IN PKTCAP_NDIS_HANDLE hNdisHandle, IN CHAR* pcFilter);


INT PKTCAP_Dispatch
(
    IN PKTCAP_NDIS_HANDLE hNdisHandle,
    IN PF_PKT_CAP_PACKET_IN_FUNC pfFunc,
    IN HANDLE hUserHandle
);


INT PKTCAP_Loop
(
    IN PKTCAP_NDIS_HANDLE hNdisHandle,
    IN PF_PKT_CAP_PACKET_IN_FUNC pfFunc,
    IN HANDLE hUserHandle
);

VOID PKTCAP_BreakLoop(IN PKTCAP_NDIS_HANDLE hNdisHandle);

#ifdef __cplusplus
    }
#endif 

#endif 

