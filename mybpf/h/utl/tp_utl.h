#ifndef __TP_UTL_H_
#define __TP_UTL_H_

#include "utl/mbuf_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

typedef HANDLE TP_HANDLE;
typedef UINT TP_ID;

typedef struct
{
    HANDLE ahChannelDesc[4];
}TP_CHANNEL_S;

typedef enum
{
    TP_TYPE_PROTOCOL,   /* 协议TP */
    TP_TYPE_CONN,       /* 连接TP */

    TP_TYPE_MAX
}TP_TYPE_E;

typedef enum
{
    TP_OPT_NBIO = 0, /* 非阻塞模式. UINT, 0为阻塞, 1为非阻塞. 默认为阻塞模式 */
    TP_OPT_KEEPALIVE_TIME, /* KeepAlive时间. TP_OPT_KEEP_ALIVE_S. */

    TP_OPT_MAX
}TP_OPT_E;

typedef struct
{
    USHORT usIdle;           /* 触发开始KeepAlive的空闲Tick数. 0表示不触发. */
    USHORT usIntval;         /* KeepAlive的间隔Tick */
    USHORT usMaxProbeCount;  /* 最大的重试次数 */
}TP_OPT_KEEP_ALIVE_S;

typedef struct
{
    TP_TYPE_E eType;
    UINT uiFlag;
    UINT uiProtocolId;   /* 主机序 */
    TP_ID uiLocalTpId;   /* 本地TP_ID, 主机序 */
    TP_ID uiPeerTpId;    /* 对方的TP_ID,主机序 */
    UINT uiStatus;       /* 状态 */
    INT iSn;             /* 主机序, Next Pkt Sn */
    INT iAckSn;          /* 主机序, Next Need Pkt Sn */
    UINT uiSendBufCount;    /* 发送缓冲区中有多少报文 */
    UINT uiRecvBufCount;    /* 接收缓冲区中有多少报文 */
    TP_CHANNEL_S stChannel;
}TP_STATE_S;

typedef BS_STATUS (*PF_TP_SEND_FUNC)(IN TP_HANDLE hTpHandle, IN TP_CHANNEL_S *pstChannel, IN MBUF_S *pstMbuf, IN USER_HANDLE_S *pstUserHandle);

#define TP_EVENT_CONNECT 0x1
#define TP_EVENT_ACCEPT  0x2
#define TP_EVENT_READ    0x4
#define TP_EVENT_WRITE   0x8
#define TP_EVENT_ERR     0x10

typedef BS_STATUS (*PF_TP_EVENT_FUNC)(IN TP_HANDLE hTpHandle, IN TP_ID uiTpId, IN UINT uiEvent, IN USER_HANDLE_S *pstUserHandle);
typedef VOID (*PF_TP_WALK_FUNC)(IN TP_HANDLE hTpHandle, IN TP_ID uiTpId, IN HANDLE hUserHandle);

typedef VOID (*PF_TP_CLOSE_NOTIFY_FUNC)(IN UINT uiTPID, IN HANDLE *phPropertys, IN USER_HANDLE_S *pstUserHandle);

TP_HANDLE TP_Create
(
    IN PF_TP_SEND_FUNC pfSendFunc,
    IN PF_TP_EVENT_FUNC pfEventFunc,
    IN USER_HANDLE_S *pstUserHandle,
    IN UINT uiMaxPropertys /* 支持多少个属性 */
);

BS_STATUS TP_SetDftKeepAlive
(
    IN TP_HANDLE hTpHandle,
    IN USHORT usIdle,
    IN USHORT usIntval,
    IN USHORT usMaxProbeCount
);

VOID TP_Destory(IN TP_HANDLE IN hTpHandle);

BS_STATUS TP_RegCloseNotifyEvent
(
    IN TP_HANDLE hTpHandle,
    IN PF_TP_CLOSE_NOTIFY_FUNC pfFunc,
    IN USER_HANDLE_S *pstUserHandle
);

VOID TP_Walk(IN TP_HANDLE hTpHandle, IN PF_TP_WALK_FUNC pfFunc, IN HANDLE hUserHandle);

#define TP_DBG_FLAG_PKT 0x1

VOID TP_SetDbgFlag(IN TP_HANDLE hTpHandle, IN UINT uiDbgFlag);
VOID TP_ClrDbgFlag(IN TP_HANDLE hTpHandle, IN UINT uiDbgFlag);

TP_ID TP_Socket
(
    IN TP_HANDLE hTpHandle,
    IN TP_TYPE_E eType
);

BS_STATUS TP_SetOpt
(
    IN TP_HANDLE hTpHandle,
    IN TP_ID uiTpId,
    IN TP_OPT_E enOpt,
    IN VOID *pValue
);

BS_STATUS TP_SetProperty
(
    IN TP_HANDLE hTpHandle,
    IN TP_ID uiTpId,
    IN UINT uiIndex,
    IN HANDLE hValue
);

HANDLE TP_GetProperty
(
    IN TP_HANDLE hTpHandle,
    IN TP_ID uiTpId,
    IN UINT uiIndex
);

BS_STATUS TP_Bind
(
    IN TP_HANDLE hTpHandle,
    IN TP_ID uiTpId,
    IN UINT uiProtocolId /* 主机序 */
);

BS_STATUS TP_Listen
(
    IN TP_HANDLE hTpHandle,
    IN TP_ID uiTpId
);

BS_STATUS TP_Close
(
    IN TP_HANDLE hTpHandle,
    IN TP_ID uiTpId
);

BS_STATUS TP_Connect
(
    IN TP_HANDLE hTpHandle,
    IN TP_ID uiTpId,
    IN UINT uiProtocolId,
    IN TP_CHANNEL_S *pstChannel
);

BS_STATUS TP_Accept
(
    IN TP_HANDLE hTpHandle,
    IN TP_ID uiTpId,
    OUT TP_ID *puiAcceptTpId
);

TP_CHANNEL_S * TP_GetChannel
(
    IN TP_HANDLE hTpHandle,
    IN TP_ID uiTpId
);

BS_STATUS TP_SendMbuf
(
    IN TP_HANDLE hTpHandle,
    IN TP_ID uiTpId,
    IN MBUF_S *pstMbuf
);

BS_STATUS TP_SendData
(
    IN TP_HANDLE hTpHandle,
    IN TP_ID uiTpId,
    IN UCHAR *pucData,
    IN UINT uiDataLen
);

MBUF_S * TP_RecvMbuf
(
    IN TP_HANDLE hTpHandle,
    IN TP_ID uiTpId
);

BS_STATUS TP_RecvData
(
    IN TP_HANDLE hTpHandle,
    IN TP_ID uiTpId,
    INOUT UCHAR *pucData,
    IN UINT uiMaxDataLen,
    OUT UINT *puiReadLen
);

BS_STATUS TP_PktInput
(
    IN TP_HANDLE hTpHandle,
    IN MBUF_S *pstMbuf,
    IN TP_CHANNEL_S *pstChannel
);

BOOL_T TP_IsSynPkt(IN MBUF_S *pstMbuf);

BOOL_T TP_NeedAck(IN MBUF_S *pstMbuf);

MBUF_S * TP_PktBuildRstByPkt(IN MBUF_S *pstMbuf);

BS_STATUS TP_TimerStep(IN TP_HANDLE hTpHandle);

BS_STATUS TP_GetState
(
    IN TP_HANDLE hTpHandle,
    IN TP_ID uiTpId,
    OUT TP_STATE_S *pstState
);

CHAR * TP_GetStatusString(IN UINT uiStatus);

VOID TP_TiggerKeepAlive(IN TP_HANDLE hTpHandle, IN TP_ID uiTpId);


#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif /* __TP_UTL_H_ */


