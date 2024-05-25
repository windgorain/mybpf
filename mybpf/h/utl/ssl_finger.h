#ifndef _SSL_FINGER_H
#define _SSL_FINGER_H

#include "utl/map_utl.h"
#include "utl/ssl_decode.h"
#include "utl/cjson.h"
#include "utl/lpm_utl.h"

#ifdef __cplusplus
extern "C"
{
#endif


#define SSL_FINGER_MAX_FINGER_LEN 1023

typedef struct {
    MAP_HANDLE map;
    LPM_S lpm;
}SSL_FINGER_S;

typedef struct {
    char str_repr[SSL_FINGER_MAX_FINGER_LEN + 1];
    cJSON* json_handle;
    int id;
}SSL_FINGER_NODE_S;

BS_STATUS SSLFinger_Init(SSL_FINGER_S *tcp_finger);
void SSLFinger_Fin(SSL_FINGER_S *tcp_finger);
BS_STATUS SSLFinger_AddFinger(SSL_FINGER_S *ssl_finger, cJSON *json, int id);
int SSLFinger_BuildFingerByClientHello(SSL_CLIENT_HELLO_INFO_S *client_hello, char *finger);
int SSLFinger_BuildFinger(void *ipheader, void *tcpheader, char *finger);
SSL_FINGER_NODE_S * SSLFinger_Match(SSL_FINGER_S*ssl_finger,
        char *str_repr, int length);
BS_STATUS SSLFinger_ParseFingerFile(SSL_FINGER_S* ssl_finger, char *file, UINT base_id);
BS_STATUS SSLFinger_LoadASNFile(SSL_FINGER_S *ctrl, char *file);
cJSON * SSLFinger_Analysis(SSL_FINGER_S *ctrl, SSL_FINGER_NODE_S *node,
        UINT ip, USHORT port, char *hostname, OUT double *out_score);

#ifdef __cplusplus
}
#endif
#endif 
