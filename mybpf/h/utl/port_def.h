/*================================================================
*   Created by LiXingang
*   Description: 常见端口类型定义
*
================================================================*/
#ifndef _PORT_DEF_H
#define _PORT_DEF_H
#ifdef __cplusplus
extern "C"
{
#endif

#define PORT_IS_HTTP(port) \
    (((port) == 80) || ((port) == 8081) || ((port) == 8080) || (port == 3128))
#define PORT_IS_HTTPS(port) ((port == 443) || (port == 8443))
#define PORT_IS_SSL(port)  ((port == 443) || (port == 465) || (port == 993) || (port == 995))

#ifdef __cplusplus
}
#endif
#endif 
