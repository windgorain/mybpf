/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _MYBPF_UTL_H
#define _MYBPF_UTL_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    U64 p[5];
    U64 bpf_ret;
}MYBPF_PARAM_S;

#ifdef __cplusplus
}
#endif
#endif //MYBPF_UTL_H_
