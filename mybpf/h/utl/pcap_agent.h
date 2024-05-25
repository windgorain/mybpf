#ifndef __PCAP_AGENT_H_
#define __PCAP_AGENT_H_

#include "utl/pktcap_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 

#define PCAP_AGENT_INDEX_INVALID 0xFFFFFFFF


#define PCAP_AGENT_DBG_FLAG_PACKET 0x1


typedef HANDLE PCAP_AGENT_HANDLE;


typedef VOID (*PF_PCAP_AGENT_PKT_INPUT_FUNC)(IN UCHAR *pucData, IN PKTCAP_PKT_INFO_S *pstPktInfo, IN USER_HANDLE_S *pstUserHandle);


PCAP_AGENT_HANDLE PCAP_AGENT_CreateInstance(IN UINT uiMaxAgentNum);

UINT PCAP_AGENT_GetAFreeIndex(IN PCAP_AGENT_HANDLE hPcapAgent);
BS_STATUS PCAP_AGENT_SetNdis
(
    IN PCAP_AGENT_HANDLE hPcapAgent,
    IN UINT uiIndex,
    IN CHAR *pcNdisName
);
UINT PCAP_AGENT_FindIndexByNdisName(IN PCAP_AGENT_HANDLE hPcapAgent, IN CHAR *pcNdisName);
UINT PCAP_AGENT_FindIndexByNdisName(IN PCAP_AGENT_HANDLE hPcapAgent, IN CHAR *pcNdisName);
CHAR * PCAP_AGENT_GetNdisName
(
    IN PCAP_AGENT_HANDLE hPcapAgent,
    IN UINT uiIndex
);
BS_STATUS PCAP_AGENT_Start
(
    IN PCAP_AGENT_HANDLE hPcapAgent,
    IN UINT uiIndex
);
BS_STATUS PCAP_AGENT_Stop
(
    IN PCAP_AGENT_HANDLE hPcapAgent,
    IN UINT uiIndex
);
BOOL_T PCAP_AGENT_IsStart
(
    IN PCAP_AGENT_HANDLE hPcapAgent,
    IN UINT uiIndex
);
BS_STATUS PCAP_AGENT_RegService
(
    IN PCAP_AGENT_HANDLE hPcapAgent,
    IN UINT uiIndex,
    IN USHORT usProtoType,
    IN PF_PCAP_AGENT_PKT_INPUT_FUNC pfFunc,
    IN USER_HANDLE_S *pstUserHandle
);
VOID PCAP_AGENT_UnregService
(
    IN PCAP_AGENT_HANDLE hPcapAgent,
    IN UINT uiIndex,
    IN USHORT usProtoType,
    IN PF_PCAP_AGENT_PKT_INPUT_FUNC pfFunc
);
BS_STATUS PCAP_AGENT_SendPkt
(
    IN PCAP_AGENT_HANDLE hPcapAgent,
    IN UINT uiIndex,
    IN MBUF_S *pstMbuf
);

VOID PCAP_AGENT_SetDbgFlag(IN UINT uiFlag);
VOID PCAP_AGENT_ClrDbgFlag(IN UINT uiFlag);
VOID PCAP_AGENT_DbgCmd(IN CHAR *pcModuleName, IN CHAR *pcFlagName);
VOID PCAP_AGENT_NoDbgCmd(IN CHAR *pcModuleName, IN CHAR *pcFlagName);

#ifdef __cplusplus
    }
#endif 

#endif 

