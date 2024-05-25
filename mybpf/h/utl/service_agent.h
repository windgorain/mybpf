/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _SERVICE_AGENT_H
#define _SERVICE_AGENT_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef void (*PF_SERVICE_AGENT_DATA_OB)(void *service_agent, int fd,
        void *data, int len);

#define SERVICE_AGENT_EVENT_NEW  1
#define SERVICE_AGENT_EVENT_FREE 2

typedef int (*PF_SERVICE_AGENT_EV_OB)(void *service_agent, int fd, int event);

typedef struct {
    void *my_poll;
    int listen_fd;
    void *ud;
    PF_SERVICE_AGENT_DATA_OB data_func;
    PF_SERVICE_AGENT_EV_OB event_func;
}SERVICE_AGENT_S;

int ServiceAgent_Init(SERVICE_AGENT_S *ctrl, void *mypller);
int ServiceAgent_OpenTcp(SERVICE_AGENT_S *ctrl, USHORT port);
void ServiceAgent_SetEventOb(SERVICE_AGENT_S *ctrl, PF_SERVICE_AGENT_EV_OB fn);
void ServiceAgent_SetDataOb(SERVICE_AGENT_S *ctrl, PF_SERVICE_AGENT_DATA_OB fn);

void ServiceAgent_SetFdUd(SERVICE_AGENT_S *ctrl, int fd, void *fd_ud);
void * ServiceAgent_GetFdUd(SERVICE_AGENT_S *ctrl, int fd);
void ServiceAgent_Close(SERVICE_AGENT_S *ctrl, int fd);

#ifdef __cplusplus
}
#endif
#endif 
