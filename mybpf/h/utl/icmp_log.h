/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _ICMP_LOG_H
#define _ICMP_LOG_H
#include "utl/log_utl.h"
#include "utl/bloomfilter_utl.h"
#ifdef __cplusplus
extern "C"
{
#endif

#define ICMP_LOG_ENABLE(log) (log.log_enable == 1)

typedef struct {
    UINT log_enable:1;
    UINT log_all_pkt:1;
    UINT ip_header_hex:1;
    UINT icmp_header_hex:1;
    UINT icmp_payload_hex:1;
    char *config_base_dir;
    LOG_UTL_S log_utl;
    BLOOM_FILTER_S bloomfilter;
    UINT bloomfilter_size;
}ICMP_LOG_S;

void ICMPLOG_Init(ICMP_LOG_S *config, char * conf_base_dir, char *log_base_dir);
BS_STATUS ICMPLOG_ParseConfig(ICMP_LOG_S *config, char *conf_string);
void ICMPLOG_Input(ICMP_LOG_S *config, VOID *ippkt, UINT pktlen, NET_PKT_TYPE_E pkt_type);

#ifdef __cplusplus
}
#endif
#endif 
