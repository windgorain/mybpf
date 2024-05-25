/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _IP_TUP_H
#define _IP_TUP_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    USHORT sport;
    USHORT dport;
}L4_PORTS_S;

typedef struct {
    unsigned int sip;
    unsigned int dip;
}IP_2TUPS_S;

typedef struct {
    unsigned int sip;
    unsigned int dip;
    unsigned short sport;
    unsigned short dport;
}IP_4TUPS_S;

#pragma pack(1)
typedef struct {
    unsigned int sip;
    unsigned int dip;
    unsigned char ip_proto;
}IP_3TUPS_S;

typedef struct {
    unsigned int sip;
    unsigned int dip;
    unsigned short sport;
    unsigned short dport;
    unsigned char ip_proto;
}IP_5TUPS_S;
#pragma pack()

#ifdef __cplusplus
}
#endif
#endif 
