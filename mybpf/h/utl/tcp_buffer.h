/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _TCP_BUFFER_H
#define _TCP_BUFFER_H
#include "utl/nap_utl.h"
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    int max_buff_size;
    NAP_HANDLE hNap;
}TCPB_S;

typedef struct {
    UINT64 offset; 
    void *data;
    int len;
    UINT save_offset:16; 
    UINT need_save:1;
    UINT failed:1;
}TCPB_BUF_S;

typedef void (*PF_TCPB_OUTPUT)(TCPB_BUF_S *tcpb_buf, void *ud);

int TCPB_Init(OUT TCPB_S *tcpb, int max_count, int max_buff_size);
void TCPB_Final(TCPB_S *tcpb);
TCPB_S * TCPB_New(int max_count, int max_buff_size);
void TCPB_Delete(TCPB_S *ctrl);
void * TCPB_Find(TCPB_S *tcpb, UINT index);
void * TCPB_Alloc(TCPB_S *tcpb, UINT payload_sn);
void * TCPB_AlocSpec(TCPB_S *tcpb, UINT index, UINT payload_sn);
void TCPB_Free(TCPB_S *tcpb, UINT index);
void TCPB_Alloc_SetPayloadSn(TCPB_S *tcpb, void *tcpb_node, UINT payload_sn);

int TCPB_Input(TCPB_S *tcpb, void *tcpb_node, UINT sn,
        void *payload, int payload_len, PF_TCPB_OUTPUT output, void *ud);
void TCPB_SetSaveOffset(TCPB_BUF_S *tcpbuf, UINT offset);

#ifdef __cplusplus
}
#endif
#endif 
