#ifndef PKT_SENT_H
#define PKT_SENT_H

#include "bs.h"

#ifdef  USE_PACKET_INJECT 

typedef struct {
	ip_t* net;
	eth_t* link;
	char* device;
	int	  state;
} Pkt_Send;

#endif
int pkt_send_init(void *handle);
int pkt_send_inject(void *handle, const char* buf, uint32_t len);
int pkt_send_uinit(void *handle);

#endif
