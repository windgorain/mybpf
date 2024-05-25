/******************************************************************************
* Copyright (C), LiXingang
* Author:      LiXingang  Date: 2016-7-14
* Description: 
* History:     
******************************************************************************/

#ifndef __SIF_UTL_H_
#define __SIF_UTL_H_

#include "utl/if_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 

#define SIF_FLAG_LOCK 0x1 

BS_STATUS SIF_Init(IN UINT uiFlag);
BS_STATUS SIF_RegEvent(IN PF_IF_EVENT_FUNC pfEventFunc, IN USER_HANDLE_S *pstUserHandle);
BS_STATUS SIF_SetProtoType(IN CHAR *pcProtoType, IN IF_PROTO_PARAM_S *pstParam);
IF_PROTO_PARAM_S * SIF_GetProtoType(IN CHAR *pcProtoType);
BS_STATUS SIF_SetLinkType(IN CHAR *pcLinkType, IN IF_LINK_PARAM_S *pstParam);
IF_LINK_PARAM_S * SIF_GetLinkType(IN CHAR *pcLinkType);
BS_STATUS SIF_SetPhyType(IN CHAR *pcPhyType, IN IF_PHY_PARAM_S *pstParam);
IF_PHY_PARAM_S * SIF_GetPhyType(IN CHAR *pcPhyType);
UINT SIF_AddIfType(IN CHAR *pcIfType, IN IF_TYPE_PARAM_S *pstParam);
CHAR * SIF_GetTypeNameByType(IN UINT uiType);
UINT SIF_GetIfTypeFlag(IN CHAR *pcIfType);
IF_INDEX SIF_CreateIf(IN CHAR *pcIfType);
IF_INDEX SIF_CreateIfByName(IN CHAR *pcIfType, IN CHAR *pcIfName);
VOID SIF_DeleteIf(IN IF_INDEX ifIndex);
CHAR * SIF_GetIfName(IN IF_INDEX ifIndex, OUT CHAR szIfName[IF_MAX_NAME_LEN + 1]);
BS_STATUS SIF_Ioctl(IN IF_INDEX ifIndex, IN IF_IOCTL_CMD_E enCmd, IN HANDLE hData);
BS_STATUS SIF_PhyIoctl(IN IF_INDEX ifIndex, IN IF_PHY_IOCTL_CMD_E enCmd, IN HANDLE hData);
IF_INDEX SIF_GetIfIndex(IN CHAR *pcIfName);
BS_STATUS SIF_PhyOutput(IN IF_INDEX ifIndex, IN MBUF_S *pstMbuf);
BS_STATUS SIF_LinkInput(IN IF_INDEX ifIndex, IN MBUF_S *pstMbuf);
BS_STATUS SIF_LinkOutput (IN IF_INDEX ifIndex, IN MBUF_S *pstMbuf, IN USHORT usProtoType);
BS_STATUS SIF_ProtoInput(IN IF_INDEX ifIndex, IN MBUF_S *pstMbuf, IN USHORT usProtoType);
UINT SIF_AllocUserDataIndex();
BS_STATUS SIF_SetUserData(IN IF_INDEX ifIndex, IN UINT uiIndex, IN HANDLE hData);
BS_STATUS SIF_GetUserData(IN IF_INDEX ifIndex, IN UINT uiIndex, IN HANDLE *phData);
IF_INDEX SIF_GetNext(IN IF_INDEX ifIndexCurrent);
BS_STATUS SIF_RegPktProcesser
(
    IN IF_INDEX ifIndex,
    IN IF_PKT_PROCESSER_TYPE_E enType,
    IN UINT uiPri,  
    IN PF_IF_PKT_PORCESSER_FUNC pfFunc
);

#ifdef __cplusplus
    }
#endif 

#endif 


