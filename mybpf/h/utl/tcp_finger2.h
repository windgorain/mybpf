/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _TCP_FINGER2_H
#define _TCP_FINGER2_H
#include "utl/tcp_utl.h"
#ifdef __cplusplus
extern "C"
{
#endif

#define TCP_FINGER2_MAX_FINGER_LEN 127
#define TCP_FINGER2_MAX_OS_LEN     512

typedef struct {
    MAP_HANDLE map;
}TCP_FINGER2_S;

typedef struct {
    char finger[TCP_FINGER2_MAX_FINGER_LEN + 1];
    char os[TCP_FINGER2_MAX_OS_LEN + 1];
    int id;
}TCP_FINGER2_NODE_S;

BS_STATUS TcpFinger2_Init(TCP_FINGER2_S *tcp_finger);
void TcpFinger2_Fin(TCP_FINGER2_S *tcp_finger);
BS_STATUS TcpFinger2_AddFinger(TCP_FINGER2_S *tcp_finger,
        char *finger, char *os, int id);
char * TcpFinger2_BuildFinger(TCP_HEAD_S *tcpheader, char *finger);
TCP_FINGER2_NODE_S * TcpFinger2_Search(TCP_FINGER2_S *tcp_finger, char *finger);
TCP_FINGER2_NODE_S * TcpFinger2_Match(TCP_FINGER2_S *tcp_finger, TCP_HEAD_S *tcpheader);
BS_STATUS TcpFinger2_ReadFingerFile(TCP_FINGER2_S *tcp_finger, char *filepath, UINT base_id);

#ifdef __cplusplus
}
#endif
#endif 
