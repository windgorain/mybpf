
#ifndef __UDP_UTL_H_
#define __UDP_UTL_H_

#include "net.h"

#ifdef __cplusplus
    extern "C" {
#endif 


typedef struct 
{
	USHORT usSrcPort;		
	USHORT usDstPort; 		
	USHORT usDataLength; 	
	USHORT usCrc;			
}UDP_HEAD_S;

USHORT UDP_CheckSum
(
    IN UCHAR *pucBuf,
	IN UINT ulLen,
	IN UCHAR *pucSrcIp,
	IN UCHAR *pucDstIp
);

UDP_HEAD_S * UDP_GetUDPHeader(IN VOID *pucData, IN UINT uiDataLen, IN NET_PKT_TYPE_E enPktType);

CHAR * UDP_Header2String(IN VOID *udp, OUT CHAR *info, IN UINT infosize);


static inline BOOL_T UDP_IsHeaderEnough(UDP_HEAD_S *udp_head, int len)
{
    if (len < sizeof(UDP_HEAD_S)) {
        return FALSE;
    }

    return TRUE;
}

#ifdef __cplusplus
    }
#endif 

#endif 


