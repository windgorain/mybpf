
#ifndef __TCP_UTL_H_
#define __TCP_UTL_H_

#include "utl/net.h"
#include "utl/eth_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 

#define TCP_HEADER_LENGTH 20


#define TCPS_CLOSED        0    
#define TCPS_LISTEN        1    
#define TCPS_SYN_SENT      2    
#define TCPS_SYN_RECEIVED  3    

#define TCPS_ESTABLISHED   4    
#define TCPS_CLOSE_WAIT    5    

#define TCPS_FIN_WAIT_1    6    
#define TCPS_CLOSING       7    
#define TCPS_LAST_ACK      8    

#define TCPS_FIN_WAIT_2    9    
#define TCPS_TIME_WAIT    10    

#define TCP_HEAD_LEN(pstTcpHead) ((((pstTcpHead)->ucHeadLenAndOther & 0xf0) >> 4) * 4)

#define    TCP_FLAG_FIN    0x01
#define    TCP_FLAG_SYN    0x02
#define    TCP_FLAG_RST    0x04
#define    TCP_FLAG_PUSH   0x08
#define    TCP_FLAG_ACK    0x10
#define    TCP_FLAG_URG    0x20
#define    TCP_FLAG_ECE    0x40
#define    TCP_FLAG_CWR    0x80

#define    TCP_URG_PSH_RST_FIN    0x002D
#define    TCP_URG_ACK_PSH_RST_FIN  0x003D
#define    TCP_URG_PSH_SYN_FIN    0x002B
#define    TCP_URG_ACK_PSH_SYN_FIN  0x003B
#define    TCP_URG_PSH_RST_SYN    0x002E
#define    TCP_URG_RST_SYN_FIN    0x0027
#define    TCP_PSH_RST_SYN_FIN    0x000F
#define    TCP_URG_PSH_RST_SYN_FIN  0x002F

#define    TCP_OPT_EOL     0
#define    TCP_OPT_NOP     1
#define    TCP_OPT_MSS     2
#define    TCP_OPT_WIN     3
#define    TCP_OPT_SACK_PERMIT    4
#define    TCP_OPT_SACK    5
#define    TCP_OPT_TIME    8

#define TCP_FLAG_IS_SYN(flag) (((flag) & TCP_FLAG_SYN) && (!((flag) & TCP_FLAG_ACK)))
#define TCP_FLAG_IS_SYNACK(flag) (((flag) & TCP_FLAG_SYN) && ((flag) & TCP_FLAG_ACK))
#define TCP_FLAG_IS_RST(flag) (((flag) & TCP_FLAG_RST) && (!((flag) & TCP_FLAG_ACK)))
#define TCP_FLAG_IS_RSTACK(flag) (((flag) & TCP_FLAG_RST) && ((flag) & TCP_FLAG_ACK))
#define TCP_FLAG_IS_FIN(flag)  ((flag) & TCP_FLAG_FIN)
#define TCP_Get_Window(hdr)    ((hdr)->usWindow)
#define TCP_Set_Window(hdr,val) ((hdr)->usWindow = val)

#pragma pack(1)

typedef struct tagTCP_HEAD_S
{
	USHORT usSrcPort;		
	USHORT usDstPort; 		
	UINT  ulSequence;
	UINT  ulAckSequence;
	UCHAR  ucHeadLenAndOther; 
	UCHAR  ucFlag;            
	USHORT usWindow;
	USHORT usCrc;
	USHORT usUrg;
}TCP_HEAD_S;

#pragma pack()

typedef struct {
    USHORT mss;
    UCHAR  wscale;
    UCHAR  sack;
    UCHAR  nop_count;
    UCHAR  eol_count;
    UINT   timestamp;
    UINT   timestamp_echo;
}TCP_OPT_INFO_S;


USHORT TCP_CheckSum
(
    IN UCHAR *pucBuf,
	IN UINT ulLen,
	IN UCHAR *pucSrcIp,
	IN UCHAR *pucDstIp
);
TCP_HEAD_S * TCP_GetTcpHeader(IN UCHAR *pucData, IN UINT uiDataLen, IN NET_PKT_TYPE_E enPktType);
CHAR * TCP_Header2String(IN VOID *tcp, OUT CHAR *info, IN UINT infosize);
CHAR * TCP_Header2Hex(IN VOID *tcp, OUT CHAR *hex);
void TCP_GetOptInfo(TCP_HEAD_S *tcph, TCP_OPT_INFO_S *opt_info);
int TCP_GetOptString(TCP_HEAD_S *tcph, char *buf, int buf_len);
char * TCP_GetFlagsString(IN UCHAR ucFlags, char *info);
UINT TCP_String2Flag(char *str);
int TCP_LoadIPFile(void *sip_monitor);
char * TCP_GetStatusString(int state);


static inline BOOL_T TCP_IsHeaderEnough(TCP_HEAD_S *tcp_head, int len)
{
    if (len < sizeof(TCP_HEAD_S)) {
        return FALSE;
    }

    if (len < TCP_HEAD_LEN(tcp_head)) {
        return FALSE;
    }

    return TRUE;
}

#ifdef __cplusplus
    }
#endif 

#endif 


