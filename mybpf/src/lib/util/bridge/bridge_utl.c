/*================================================================
*   Created by LiXingang
*   Description: bridge 交换机
*
================================================================*/
#include "bs.h"
#include "utl/bridge_utl.h"
#include "utl/mac_table.h"
#include "utl/map_utl.h"
#include "utl/idkey_utl.h"
#include "utl/array_bit.h"

typedef struct BR_CTRL_STRUCT {
    MACTBL_HANDLE mactbl;
    MAP_HANDLE if_map;
    PF_BR_OUTPUT output;
}BR_CTRL_S;;

typedef struct {
    IF_INDEX ifindex;
}BR_MAC_UD_S;

static int _br_if_fwd_each(IN MAP_ELE_S *pstEle, IN VOID *ud)
{
    USER_HANDLE_S *uh = ud;
    BR_CTRL_S *br = uh->ahUserHandle[0];
    void *pkt = uh->ahUserHandle[1];
    void *user_ud = uh->ahUserHandle[2];

    br->output(HANDLE_ULONG(pstEle->pData), pkt, user_ud, BR_OUTPUT_FLAG_BROADCAST);

    return 0;
}

BR_HANDLE BR_Create()
{
    BR_CTRL_S *br = MEM_ZMalloc(sizeof(BR_CTRL_S));
    if (! br) {
        return NULL;
    }

    br->mactbl = MACTBL_CreateInstance(sizeof(BR_MAC_UD_S));
    if (! br->mactbl) {
        BR_Destroy(br);
        return NULL;
    }
    MACTBL_SetOldTick(br->mactbl, 5*60);

    br->if_map = MAP_AvlCreate(NULL);
    if (! br->if_map) {
        BR_Destroy(br);
        return NULL;
    }

    return br;
}

void BR_Destroy(BR_HANDLE br)
{
    if (br->mactbl) {
        MACTBL_DestoryInstance(br->mactbl);
    }

    if (br->if_map) {
        MAP_Destroy(br->if_map, NULL, NULL);
    }

    MEM_Free(br);
}

int BR_AddIf(BR_HANDLE br, IF_INDEX ifindex)
{
    return MAP_Add(br->if_map, ULONG_HANDLE(ifindex), 0, 0, 0);
}

void BR_DelIf(BR_HANDLE br, IF_INDEX ifindex)
{
    MAP_Del(br->if_map, ULONG_HANDLE(ifindex), 0);
}

int BR_PktInput(BR_HANDLE br, IF_INDEX ifindex, BR_PKT_S *pkt, void *ud)
{
    ETH_HEADER_S *eth_hdr = pkt->eth_hdr;
    MAC_NODE_S node;
    BR_MAC_UD_S mac_ud;
    USER_HANDLE_S uh;

    MAC_ADDR_COPY(node.stMac.aucMac, eth_hdr->stSMac.aucMac);
    node.uiFlag = 0;
    node.uiPRI = 0;

    mac_ud.ifindex= ifindex;
    MACTBL_Add(br->mactbl, &node, &mac_ud, MAC_MODE_LEARN);

    MAC_ADDR_COPY(node.stMac.aucMac, eth_hdr->stDMac.aucMac);
    if (0 == MACTBL_Find(br->mactbl, &node, &mac_ud)) {
        br->output(mac_ud.ifindex, pkt, ud, 0);
        return 0;
    }

    uh.ahUserHandle[0] = br;
    uh.ahUserHandle[1] = pkt;
    uh.ahUserHandle[2] = ud;

    MAP_Walk(br->if_map, _br_if_fwd_each, &uh);

    return 0;
}


