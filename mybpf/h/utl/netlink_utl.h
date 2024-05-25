/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _NETLINK_UTL_H
#define _NETLINK_UTL_H
#include <linux/netlink.h>
#include <linux/genetlink.h>
#ifdef __cplusplus
extern "C"
{
#endif

#define NETLINK_GEN_NAME "my_nl_gen"

enum{  
    NETLINK_GEN_C_UNSPEC,  
    NETLINK_GEN_C_CMD,

    NETLINK_GEN_C_MAX
};  

typedef struct netlink_msg_st {
    struct nlmsghdr hdr;
    struct genlmsghdr g;  
    int msg_type;
    int reply_size; 
    void *reply_ptr;
    char data[0];
}NETLINK_MSG_S;

typedef struct {
    int nl_fd;
    int gen_family_id;
}NETLINK_S;

typedef int (*PF_NETLINK_DO)(NETLINK_S *nl, int cmd, void *data, int data_len);

void NetLink_Init(NETLINK_S *nl);
int NetLink_Open(NETLINK_S *nl, char *nl_gen_name);
void NetLink_Close(NETLINK_S *nl);
int NetLink_SendMsg(NETLINK_S *nl, unsigned int msg_type, void *data, int datalen, void *recv_buf, int buf_size);
int NetLink_DoExt(char *nl_name, PF_NETLINK_DO do_func, int cmd, void *data, int data_len);
int NetLink_Do(PF_NETLINK_DO do_func, int cmd, void *data, int data_len);

#ifdef __cplusplus
}
#endif
#endif 
