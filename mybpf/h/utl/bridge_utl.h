/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _BRIDGE_UTL_H
#define _BRIDGE_UTL_H

#include "utl/if_utl.h"
#include "utl/eth_utl.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct BR_CTRL_STRUCT * BR_HANDLE;

#define BR_OUTPUT_FLAG_BROADCAST 0x1

typedef struct {
    ETH_HEADER_S *eth_hdr;
    int pkt_len;
}BR_PKT_S;

typedef void (*PF_BR_OUTPUT)(IF_INDEX ifindex, BR_PKT_S *pkt, void *ud, UINT flag);

BR_HANDLE BR_Create();
void BR_Destroy(BR_HANDLE br);
int BR_AddIf(BR_HANDLE br, IF_INDEX ifindex);
void BR_DelIf(BR_HANDLE br, IF_INDEX ifindex);
int BR_PktInput(BR_HANDLE br, IF_INDEX ifindex, BR_PKT_S *pkt, void *ud);

#ifdef __cplusplus
}
#endif
#endif 
