/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _VXLAN_UTL_H
#define _VXLAN_UTL_H
#ifdef __cplusplus
extern "C"
{
#endif

#define VXLAN_PORT 4789
#define ALI_VXLAN_PORT 250
#define VXLAN_GPE_UDP_PORT 4790

#define VXLAN_NEXT_PROTOCOL_IPv4 0x01
#define VXLAN_NEXT_PROTOCOL_IPv6 0x02
#define VXLAN_NEXT_PROTOCOL_ETH  0x03
#define VXLAN_NEXT_PROTOCOL_NSH  0x04

#define VXLAN_FLAG_I  0x08
#define VXLAN_FLAG_P  0x04
#define VXLAN_FLAG_B  0x02
#define VXLAN_FLAG_O  0x01

#define VXLAN_GET_VER(flag) ((flag & 0x30) >> 4)

#pragma pack(1)
typedef struct {
    UCHAR flag;
    UCHAR reserved1;
    UCHAR reserved2;
    UCHAR next_protocol;
#ifdef BS_LITTLE_ENDIAN
    UINT reserved3:8;
    UINT vni:24;
#else
    UINT vni:24;
    UINT reserved3:8;
#endif
}VXLAN_HEAD_S;
#pragma pack()

BOOL_T VXLAN_Valid(IN VXLAN_HEAD_S *vxlan_header);
VXLAN_HEAD_S * VXLAN_GetVxlanHeader(IN void *pkt_buf,
        IN int buf_len, IN NET_PKT_TYPE_E pkt_type);
int VXLAN_GetInnerPktType(IN VXLAN_HEAD_S *vxlan_header, int is_ip_vxlan);
char * VXLAN_Header2String(VXLAN_HEAD_S *vxlan_header,
        OUT CHAR *info, UINT infosize);
void * VXLAN_GetInnerIPPkt(VXLAN_HEAD_S *vxlan_header,
        int buf_len, int is_ip_vxlan);

#ifdef __cplusplus
}
#endif
#endif 
