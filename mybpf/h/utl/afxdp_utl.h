/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _AFXDP_UTL_H
#define _AFXDP_UTL_H
#ifdef __cplusplus
extern "C"
{
#endif

#define AFXDP_FLAG_RX 0x1
#define AFXDP_FLAG_TX 0x2

typedef void* AFXDP_HANDLE;

typedef void (*PF_AFXDP_RECV_PKT)(void *pkt, UINT pkt_len, void *ud);

typedef struct {
    UINT frame_size;
    UINT frame_count;
    UINT fq_size;
    UINT cq_size;
    UINT rx_size;
    UINT tx_size;
    UINT head_room;
    UINT xsk_num;
    UINT flag;
    UINT libbpf_flags;
    UINT xdp_flags;
    UINT bind_flags;
}AFXDP_PARAM_S;

AFXDP_HANDLE AFXDP_Create(char *if_name, char *bpf_prog, AFXDP_PARAM_S *p);
void AFXDP_Destroy(AFXDP_HANDLE hAfXdp);
void AFXDP_RecvPkt(AFXDP_HANDLE hAfXdp, UINT batch_size, PF_AFXDP_RECV_PKT func, void *ud);
int AFXDP_SendPkt(AFXDP_HANDLE hAfXdp, void *data, int data_len);

#ifdef __cplusplus
}
#endif
#endif 
