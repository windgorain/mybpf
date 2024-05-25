#ifndef _TCP_FINGER_H
#define _TCP_FINGER_H

#include "utl/map_utl.h"

#ifdef __cplusplus
extern "C"
{
#endif


#define TCP_FINGER_MAX_FINGER_LEN 31
#define TCP_FINGER_MAX_OS_LEN     127

typedef struct {
    MAP_HANDLE map;
}TCP_FINGER_S;

typedef struct {
    char finger[TCP_FINGER_MAX_FINGER_LEN + 1];
    char os[TCP_FINGER_MAX_OS_LEN + 1];
    int id;
}TCP_FINGER_NODE_S;

BS_STATUS TcpFinger_Init(TCP_FINGER_S *tcp_finger);
void TcpFinger_Fin(TCP_FINGER_S *tcp_finger);
BS_STATUS TcpFinger_AddFinger(TCP_FINGER_S *tcp_finger,
        char *finger, char *os, int id);
char * TcpFinger_BuildFinger(void *ipheader, void *tcpheader, char *finger);
TCP_FINGER_NODE_S * TcpFinger_Search(TCP_FINGER_S *tcp_finger, char *finger);
TCP_FINGER_NODE_S * TcpFinger_Match(TCP_FINGER_S *tcp_finger,
        void *ipheader, void *tcpheader);
BS_STATUS TcpFinger_ReadFingerFile(TCP_FINGER_S *tcp_finger, char *filepath, UINT base_id);
void TcpFinger_DefaultFinger(char *finger);

#ifdef __cplusplus
}
#endif
#endif 
