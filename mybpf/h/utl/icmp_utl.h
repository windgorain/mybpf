
#ifndef __ICMP_UTL_H_
#define __ICMP_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

#define ICMP_MINLEN 8               
#define ICMP_TSLEN  (8 + 3 * sizeof (UINT))   
#define ICMP_MASKLEN    12              
#define ICMP_ADVLENMIN  (8 + sizeof (IP_HEAD_S) + 8)    
#define ICMP_ADVLEN(p)  (8 + ((int)(p)->icmp_ip.ip_hl << 2) + 8)


#define ICMP_TYPE_ECHO_REPLY   0
#define ICMP_TYPE_ECHO_REQUEST 8
#define ICMP_TYPE_DST_UR       1

#pragma pack(1)

typedef struct
{
    USHORT usIcmpId;
    USHORT usSn;
}ICMP_ECHO_S;


typedef struct
{
    UCHAR ucType;
    UCHAR ucCode;
    USHORT usCheckSum;
}ICMP_HEAD_S;

typedef struct
{
    ICMP_HEAD_S stIcmpHeader;
    ICMP_ECHO_S stEcho;
}ICMP_ECHO_HEAD_S;


struct icmp_ra_addr
{
    UINT ira_addr;
    UINT ira_preference;
};

typedef struct icmp
{
    UCHAR    icmp_type;        
    UCHAR    icmp_code;        
    USHORT   icmp_cksum;        
    union {
        UCHAR ih_pptr;            
        UINT ih_gwaddr;    
        struct ih_idseq {
            USHORT    icd_id;
            USHORT    icd_seq;
        } ih_idseq;
        int ih_void;

        
        struct ih_pmtu {
            USHORT ipm_void;
            USHORT ipm_nextmtu;
        } ih_pmtu;

        
        struct tagOrgDataLen
        {
            UCHAR ucUsed1;
            UCHAR ucOrgDataLen;
            USHORT usUsed2;
        } ih_stOrgDLen;
        
        struct ih_rtradv {
            UCHAR irt_num_addrs;
            UCHAR irt_wpa;
            USHORT irt_lifetime;
        } ih_rtradv;
    } icmp_hun;
#define icmp_pptr    icmp_hun.ih_pptr
#define icmp_gwaddr    icmp_hun.ih_gwaddr
#define icmp_id        icmp_hun.ih_idseq.icd_id
#define icmp_seq    icmp_hun.ih_idseq.icd_seq
#define icmp_void    icmp_hun.ih_void
#define icmp_pmvoid    icmp_hun.ih_pmtu.ipm_void
#define icmp_nextmtu    icmp_hun.ih_pmtu.ipm_nextmtu
#define icmp_ncOrgDLen    icmp_hun.ih_stOrgDLen.ucOrgDataLen
#define icmp_num_addrs    icmp_hun.ih_rtradv.irt_num_addrs
#define icmp_wpa    icmp_hun.ih_rtradv.irt_wpa
#define icmp_lifetime    icmp_hun.ih_rtradv.irt_lifetime

    union {
        struct id_ts {            
            UINT its_otime;    
            UINT its_rtime;    
            UINT its_ttime;    
        } id_ts;
        struct id_ip  {
            IP_HEAD_S idi_ip;
            
        } id_ip;
        struct icmp_ra_addr id_radv;
        UINT id_mask;
        char    id_data[1];
    } icmp_dun;
#define icmp_otime    icmp_dun.id_ts.its_otime
#define icmp_rtime    icmp_dun.id_ts.its_rtime
#define icmp_ttime    icmp_dun.id_ts.its_ttime
#define icmp_ip        icmp_dun.id_ip.idi_ip
#define icmp_radv    icmp_dun.id_radv
#define icmp_mask    icmp_dun.id_mask
#define icmp_data    icmp_dun.id_data
}ICMP_S;

#pragma pack()


USHORT ICMP_CheckSum (IN UCHAR *pucBuf, IN UINT ulLen);
ICMP_HEAD_S * ICMP_GetIcmpHeader(IN VOID *pucData, IN UINT uiDataLen, IN NET_PKT_TYPE_E enPktType);
ICMP_ECHO_HEAD_S * ICMP_GetEchoHeader(void *data, UINT datalen, NET_PKT_TYPE_E enPktType);

#ifdef __cplusplus
    }
#endif 

#endif 


