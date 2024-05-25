/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _AUTO_PORT_H
#define _AUTO_PORT_H
#ifdef __cplusplus
extern "C"
{
#endif

enum {
    AUTOPORT_TYPE_SET = 0, 
    AUTOPORT_TYPE_INC,     
    AUTOPORT_TYPE_PID,     
    AUTOPORT_TYPE_ADD,     
    AUTOPORT_TYPE_ANY,     
};

typedef struct {
    USHORT port_type;
    USHORT port;
    USHORT v1;
    USHORT v2;
}AUTOPORT_S;


typedef int (*PF_AUTOPORT_OPEN)(USHORT port, void *ud);


int AutoPort_Open(AUTOPORT_S *ap, PF_AUTOPORT_OPEN open_fn, void *ud);

#ifdef __cplusplus
}
#endif
#endif 
