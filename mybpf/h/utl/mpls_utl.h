/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _MPLS_UTL_H
#define _MPLS_UTL_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {

#if BS_BIG_ENDIAN
    UINT label: 20;
    UINT exp: 3;
    UINT bottom: 1;
    UINT ttl: 8;
#else
    UINT ttl: 8;
    UINT bottom: 1;
    UINT exp: 3;
    UINT label: 20;
#endif
}MPLS_HEAD_S;

#ifdef __cplusplus
}
#endif
#endif 
