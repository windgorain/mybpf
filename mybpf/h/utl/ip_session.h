/*================================================================
* Author：LiXingang. Data: 2019.07.25
* Description：
*
================================================================*/
#ifndef _IP_SESSION_H
#define _IP_SESSION_H

#include "utl/ob_utl.h"
#include "utl/box_utl.h"
#include "utl/vclock_utl.h"
#include "utl/ip_utl.h"
#include "utl/tcp_utl.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define IP_SESSION_MAX_EXTS_NUM 32

#define IP_SESSION_EVENT_BEFORE_ADD 1
#define IP_SESSION_EVENT_AFTER_ADD  2
#define IP_SESSION_EVENT_BEFORE_DEL 3
#define IP_SESSION_EVENT_AFTER_DEL 4

#define IP_SESSION_STATE_UDP (TCPS_TIME_WAIT+1)
#define IP_SESSION_STATE_ICMP (TCPS_TIME_WAIT+2)
#define IP_SESSION_STATE_MAX (TCPS_TIME_WAIT+3)

#define IP_SESSION_C_POS(_ip_sess) ((_ip_sess)->client_index)
#define IP_SESSION_S_POS(_ip_sess) (((_ip_sess)->client_index + 1) & 1)

typedef struct {
    IP_TUP_KEY_S key; 
    UINT key_hash;
    UINT client_index:1; 
    UINT state:4;
    VCLOCK_NODE_S vclock_timer;
}IP_SESSION_S;

typedef struct {
    IP_SESSION_S *session[65536];
}IP_SESSION_CACHE_S;

typedef struct {
    BOX_S ip_tup_box;
    OB_LIST_S ob_list;
    VCLOCK_INSTANCE_S vclock;
    UINT timeout_tick[IP_SESSION_STATE_MAX];
    IP_SESSION_CACHE_S *find_cache;
}IP_SESSION_CTRL_S;

typedef UINT (*PF_IPSESSION_OB)(void *ob, IP_SESSION_S *session, UINT event);

static inline UCHAR IPSession_GetFamily(IP_SESSION_S *sess)
{
    return sess->key.family;
}

static inline void * IPSession_GetClientIP(IP_SESSION_S *sess)
{
    return &sess->key.ip[sess->client_index];
}

static inline void * IPSession_GetServerIP(IP_SESSION_S *sess)
{
    return &sess->key.ip[IP_SESSION_S_POS(sess)];
}

static inline USHORT IPSession_GetClientPort(IP_SESSION_S *sess)
{
    return sess->key.port[sess->client_index];
}

static inline USHORT IPSession_GetServerPort(IP_SESSION_S *sess)
{
    return sess->key.port[IP_SESSION_S_POS(sess)];
}

static inline BOOL_T IPSession_IsClient(IP_SESSION_S *sess, void *cip, USHORT cport)
{
    BOOL_T ret = FALSE;

    if (IPSession_GetClientPort(sess) != cport) {
        return FALSE;
    }

    if (sess->key.family == AF_INET) {
        if (sess->key.ip[sess->client_index].ip4 == *(UINT*)cip) {
            ret = TRUE;
        }
    } else if (sess->key.family == AF_INET6) {
        if (0 == IP6_ADDR_CMP(&sess->key.ip[sess->client_index], cip)) {
            ret = TRUE;
        }
    } else {
        BS_DBGASSERT(0);
    }

    return ret;
}

int IPSession_Init(IP_SESSION_CTRL_S *ctrl, CUCKOO_HASH_NODE_S *hash_table,
        UINT bucket_num, UINT bucket_depth);
void IPSession_Fini(IP_SESSION_CTRL_S *ctrl);
void IPSession_SetTimeoutSteps(IP_SESSION_CTRL_S *ctrl, int state, UINT tick);
IP_SESSION_CTRL_S * IPSession_Create(UINT bucket_num, UINT bucket_depth);
void IPSession_Destroy(IP_SESSION_CTRL_S *ctrl);
int IPSession_CacheEnable(IP_SESSION_CTRL_S *ctrl);
void IPSession_RegOb(IP_SESSION_CTRL_S *ctrl, OB_S *ob);
void IPSession_UnregOb(IP_SESSION_CTRL_S *ctrl, OB_S *ob);

int IPSession_KeyInit(IP_TUP_KEY_S *key, UCHAR family, UCHAR protocol,
        UINT *client_ip, UINT *server_ip, USHORT client_port, USHORT server_port, UINT vnet_id);
void IPSession_SessInit(IP_SESSION_S *session, UCHAR family, UCHAR protocol,
        UINT *client_ip, UINT *server_ip, USHORT client_port, USHORT server_port, UINT vnet_id);
int IPSession_Add(IP_SESSION_CTRL_S *ctrl, IP_SESSION_S *session);
int IPSession_Find(IP_SESSION_CTRL_S *ctrl, IP_TUP_KEY_S *key);
IP_SESSION_S * IPSession_FindSession(IP_SESSION_CTRL_S *ctrl, IP_TUP_KEY_S *key);
IP_SESSION_S * IPSession_Del(IP_SESSION_CTRL_S *ctrl, IP_TUP_KEY_S *key);
IP_SESSION_S * IPSession_DelByIndex(IP_SESSION_CTRL_S *ctrl, int index);
IP_SESSION_S * IPSession_GetSession(IP_SESSION_CTRL_S *ctrl, int index);
int IPSession_GetNext(IP_SESSION_CTRL_S *ctrl, int index);
int IPSession_Count(IP_SESSION_CTRL_S *ctrl);
void IPSession_TimeStep(IP_SESSION_CTRL_S *ctrl);
void IPSession_Refresh(IP_SESSION_CTRL_S *ctrl, IP_SESSION_S *session);
void IPSession_SetState(IP_SESSION_CTRL_S *ctrl, IP_SESSION_S *session, int state);

#ifdef __cplusplus
}
#endif
#endif 
