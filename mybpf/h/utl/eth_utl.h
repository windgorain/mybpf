
#ifndef __ETH_UTL_H_
#define __ETH_UTL_H_

#include "utl/net.h"
#include "utl/data2hex_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 

#define ETH_MAC_ADDR_STRING_LEN 17

#define ETH_MAC_LEN     14
#define ETH_LLC_LEN     3
#define ETH_SNAP_LEN    5


#define ETH_MIN_TYPE     0x0600
#define ETH_MAX_MTU      0x05DC

#define ETH_IS_PKTTYPE(usPktLenOrType) ((usPktLenOrType) >= ETH_MIN_TYPE)
#define ETH_IS_PKTLEN(usPktLenOrType) ((usPktLenOrType) <= ETH_MAX_MTU)


#define ETHERTYPE_IP         0x0800   
#define ETHERTYPE_IP6        0x86DD   
#define ETHERTYPE_ARP        0x0806   
#define ETHERTYPE_ISIS       0x8000   
#define ETHERTYPE_VLAN       0x8100   
#define ETHERTYPE_ISIS2      0x8870   
#define ETHERTYPE_IP_MPLS    0x8847   
#define ETHERTYPE_OTHER      0xFFFF   


#define PKTFMT_ETHII_ENCAP                    0   
#define PKTFMT_SNAP_ENCAP                     1   
#define PKTFMT_LLC_ENCAP                      2   
#define PKTFMT_8023RAW_ENCAP                  3   
#define PKTFMT_OTHERS                         0xFF 

#define ETH_MIN_PAYLOAD_LEN  46 

#define MAC_ADDR_LEN 6

#define MAC_ADDR_IS_EQUAL(mac1, mac2)  \
    (((mac1)[0] == (mac2)[0]) && ((mac1)[1] == (mac2)[1]) && ((mac1)[2] == (mac2)[2])    \
    && ((mac1)[3] == (mac2)[3]) && ((mac1)[4] == (mac2)[4]) && ((mac1)[5] == (mac2)[5]))

#define MAC_ADDR_COPY(_dst, _src) \
    {   \
        (_dst)[0] = (_src)[0];  \
        (_dst)[1] = (_src)[1];  \
        (_dst)[2] = (_src)[2];  \
        (_dst)[3] = (_src)[3];  \
        (_dst)[4] = (_src)[4];  \
        (_dst)[5] = (_src)[5];  \
    }

#define MAC_ADDR_IS_ZERO(aucMac) (((aucMac)[0] == 0) && ((aucMac)[1] == 0) && ((aucMac)[2] == 0)    \
                                 && ((aucMac)[3] == 0) && ((aucMac)[4] == 0) && ((aucMac)[5] == 0))

#define MAC_ADDR_IS_BROADCAST(_aucMac) (((_aucMac)[0] == 0xff) && ((_aucMac)[1] == 0xff) && ((_aucMac)[2] == 0xff)    \
                                 && ((_aucMac)[3] == 0xff) && ((_aucMac)[4] == 0xff) && ((_aucMac)[5] == 0xff))

#define MAC_ADDR_IS_MULTICAST(aucMac)   ((aucMac)[0] & 0x01)

#define MAC_ADDR_2_UINT(aucMac, uiMac1, uiMac2)   \
    do {    \
        UCHAR *_pucMac = (aucMac);  \
        (uiMac1) = (((((UINT)(_pucMac[0])) << 24) & 0xff000000)   \
                    | ((((UINT)(_pucMac[1])) << 16) & 0xff0000)     \
                    | ((((UINT)(_pucMac[2])) << 8) & 0xff00)        \
                    | (((UINT)(_pucMac[3])) & 0xff));               \
        (uiMac2) = (((((UINT)(_pucMac[4])) << 8) & 0xff00)   \
                    | ((((UINT)_pucMac[5])) & 0xff));     \
    }while(0)

#define UINT_2_MAC_ADDR(uiMac1, uiMac2, aucMac)   \
    do {\
        UCHAR *_pucMac = (aucMac);  \
        _pucMac[0] = ((uiMac1) >> 24) & 0xff;  \
        _pucMac[1] = ((uiMac1) >> 16) & 0xff;  \
        _pucMac[2] = ((uiMac1) >> 8) & 0xff;  \
        _pucMac[3] = (uiMac1) & 0xff;  \
        _pucMac[4] = ((uiMac2) >> 8) & 0xff;  \
        _pucMac[5] = ((uiMac2)) & 0xff;  \
    }while(0)


#define MAC_ADDR_2_STRING(aucMac, pcString, cSplit) \
    do { \
        CHAR *_pcString = (pcString);  \
        UINT _i;  \
        for (_i=0; _i<6; _i++)  {   \
            UCHAR_2_HEX((aucMac)[_i], _pcString);    \
            _pcString[2] = (cSplit);    \
            _pcString += 3; \
        }   \
    }while(0)


#define STRING_2_MAC_ADDR(pcString, aucMac) \
    do { \
        CHAR *_pcString = (pcString);  \
        UINT _i;  \
        for (_i=0; _i<6; _i++)  {   \
            HEX_2_UCHAR(_pcString, &((aucMac)[_i]));   \
            _pcString += 3; \
        }   \
    }while(0)

#define ETH_P_ALL  0x0003  
#define ETH_P_IP   0x0800    
#define ETH_P_ARP  0x0806    
#define ETH_P_IP6  0x86DD    
#define ETH_P_MPLS 0x8847    



#define ETH_INVALID_VLAN_ID 0
#define ETH_MIN_VLAN_ID 1
#define ETH_MAX_VLAN_ID 4094


#pragma pack(1)


typedef struct tagETH_HEAD_S
{
    MAC_ADDR_S stDMac;
    MAC_ADDR_S stSMac;
    USHORT usProto;
}ETH_HEADER_S;


typedef struct tagETH_VLAN_HEAD_S {
    USHORT usVlanTci;   
    USHORT usProto;     
}ETH_VLAN_HEAD_S;

typedef struct vlanETH_HEAD_S
{
    MAC_ADDR_S stDMac;
    MAC_ADDR_S stSMac;
    uint16_t   tpid;
    uint16_t   priority:3;
    uint16_t   cfi:1;
    uint16_t   vlan_id:12;    
    USHORT     usProto;
}VLAN_ETH_HEADER_S;

typedef struct
{
    UCHAR      ucDSAP;                                  
    UCHAR      ucSSAP;                                  
    UCHAR      ucCtrl;                                      
}ETH_LLC_S;

#define ETH_SNAPORI_LEN         3
typedef struct
{
    UCHAR      aucORI[ETH_SNAPORI_LEN];
    USHORT     usType;
}ETH_SNAP_S;


typedef struct
{
    ETH_HEADER_S stHeader;
    ETH_LLC_S stLlc;
    ETH_SNAP_S stSnap;
}ETH_SNAPENCAP_S;

#pragma pack()

typedef struct
{
    USHORT usHeadLen;
    UCHAR ucPktFmt;
    UCHAR ucReserved;
    USHORT usType;          
    USHORT usPktLenOrType;  
    USHORT usVlanId;        
}ETH_PKT_INFO_S;


BS_STATUS ETH_GetEthHeadInfo(IN UCHAR *pucData, IN UINT uiDataLen, OUT ETH_PKT_INFO_S *pstHeadInfo);
VOID ETH_Mac2String(IN UCHAR *pucMac, IN CHAR cSplit, OUT CHAR szMacString[ETH_MAC_ADDR_STRING_LEN + 1]);
BS_STATUS ETH_String2Mac(IN CHAR *pcMacString, OUT UCHAR *pucMac);

static inline BS_STATUS ETH_Get_Eth_SrcMacString(IN UCHAR *pucData, IN UINT uiDataLen, char* buf) 
{
    if (uiDataLen < sizeof(ETH_HEADER_S)) return BS_ERR;

    ETH_HEADER_S *pstHeader = (ETH_HEADER_S*)pucData;

    ETH_Mac2String((UCHAR*)(pstHeader->stSMac.aucMac), ':', buf);

    return BS_OK;
}

#ifdef __cplusplus
    }
#endif 

#endif 



