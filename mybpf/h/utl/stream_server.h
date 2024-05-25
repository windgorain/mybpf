/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _STREAM_SERVER_H
#define _STREAM_SERVER_H
#include "utl/vbuf_utl.h"
#ifdef __cplusplus
extern "C"
{
#endif

enum {
    STREAM_SERVER_EVENT_NEW_CONN = 0,
    STREAM_SERVER_EVENT_RECV_MSG,
    STREAM_SERVER_EVENT_CLOSE_CONN
};

enum {
    STREAM_SERVER_TYPE_TCP = 0,
    STREAM_SERVER_TYPE_PIPE,
};

typedef struct {
    int fd;
    UINT send_finish: 1;
    void *cfg; 
    void *ud[4];
    VBUF_S recv_vbuf;
    VBUF_S send_vbuf;
}STREAM_CONN_S;


typedef int (*PF_STREAM_SERVER_EVENT)(void *cfg, STREAM_CONN_S *conn, UINT event);

typedef struct {
    UINT enable: 1;
    UINT type: 2;
    int listen_fd;
    char *pipe_name; 
    MYPOLL_HANDLE mypoll;
    PF_STREAM_SERVER_EVENT event_func;
}STREAM_SERVER_S;

int StreamServer_Start(STREAM_SERVER_S *cfg);
void StreamServer_Stop(STREAM_SERVER_S *cfg);
int StreamServer_Send(STREAM_CONN_S *conn, void *msg, int len);
void StreamServer_SendFinish(STREAM_CONN_S *conn);

#ifdef __cplusplus
}
#endif
#endif 
