/*================================================================
* Author：LiXingang
* Description: 
*
================================================================*/
#ifndef _TCP_REASSEMBLE_H
#define _TCP_REASSEMBLE_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef int (*PF_TCP_REASSEMBLE_OUT)(void *pkt);
typedef void* (*PF_TCP_REASSEMBLE_DUP_PKT)(void *pkt);
typedef void (*PF_TCP_REASSEMBLE_FREE_DUPPED_PKT)(void *pkt);

typedef struct {
    PF_TCP_REASSEMBLE_DUP_PKT dup_pkt;
    PF_TCP_REASSEMBLE_FREE_DUPPED_PKT free_dup_pkt;
    USHORT max_segs_per_flow; 
}TCP_REASSEMBLE_S;


typedef struct {
    int sn; 
    DLL_HEAD_S pkts_list;
}TCP_REASSEMBLE_FLOW_S;

typedef struct {
    void *pkt;
    int tcp_payload_len;
    int sn;
    UCHAR flag;
}TCP_REASSEMBLE_PKTINFO_S;

void TcpR_Init(TCP_REASSEMBLE_S *ctrl);
void TcpR_Final(TCP_REASSEMBLE_S *ctrl);

void TcpR_SetDupPkt(TCP_REASSEMBLE_S *ctrl, PF_TCP_REASSEMBLE_DUP_PKT dup_pkt,
        PF_TCP_REASSEMBLE_FREE_DUPPED_PKT free_dup_pkt);
void TcpR_SetFirstSn(TCP_REASSEMBLE_FLOW_S *flow);
void TcpR_InitFlow(TCP_REASSEMBLE_FLOW_S *flow, int init_sn);
void TcpR_ResetFlow(TCP_REASSEMBLE_S *ctrl, TCP_REASSEMBLE_FLOW_S *flow, int init_sn);
void TcpR_FinalFlow(TCP_REASSEMBLE_S *ctrl, TCP_REASSEMBLE_FLOW_S *flow);

int TcpR_Input(TCP_REASSEMBLE_S *ctrl, TCP_REASSEMBLE_FLOW_S *flow,
        TCP_REASSEMBLE_PKTINFO_S *pktinfo, PF_TCP_REASSEMBLE_OUT pkt_out);

void TcpR_Flush(TCP_REASSEMBLE_S *ctrl,
        TCP_REASSEMBLE_FLOW_S *flow, PF_TCP_REASSEMBLE_OUT pkt_out);

#ifdef __cplusplus
}
#endif
#endif 
