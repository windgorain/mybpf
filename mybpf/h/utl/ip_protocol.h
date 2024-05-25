/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _IP_PROTOCOL_H
#define _IP_PROTOCOL_H
#ifdef __cplusplus
extern "C"
{
#endif

#ifndef IPPROTO_IP          
#define IPPROTO_IP          0
#endif

#ifndef IPPROTO_ICMP        
#define IPPROTO_ICMP        1
#endif

#ifndef IPPROTO_IGMP        
#define IPPROTO_IGMP        2       
#endif

#ifndef IPPROTO_TCP         
#define IPPROTO_TCP         6
#endif

#ifndef IPPROTO_UDP         
#define IPPROTO_UDP         17
#endif


CHAR * IPProtocol_GetName(IN UCHAR ucProtocol);
CHAR * IPProtocol_GetNameExt(IN UCHAR ucProtocol);
int IPProtocol_GetByName(char *protocol_name);

int IPProtocol_NameList2Protocols(INOUT char *protocol_name_list);

#ifdef __cplusplus
}
#endif
#endif 
