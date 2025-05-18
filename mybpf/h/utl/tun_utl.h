/*================================================================
*   Created：LiXingang All rights reserved.
*   Description：
*
================================================================*/
#ifndef __TUN_UTL_H_
#define __TUN_UTL_H_

#include "utl/net.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef IN_WINDOWS
#define TUN_FD HANDLE
#define TUN_INVALID_FD (NULL)
#define TUN_FD_VALID(_fd) ((_fd) != NULL)
#define TUN_FD_INVALID(_fd) ((_fd) == NULL)
#define TUN_SET_FD_INVALID(_fd) ((_fd) = NULL)
#endif

#ifdef IN_UNIXLIKE
#define TUN_FD int
#define TUN_INVALID_FD (-1)
#define TUN_FD_VALID(_fd) ((_fd) >= 0)
#define TUN_FD_INVALID(_fd) ((_fd) < 0)
#define TUN_SET_FD_INVALID(_fd) ((_fd) = -1)
#endif

#define TUN_TYPE_TUN 0
#define TUN_TYPE_TAP 1

#define TUN_DEV_NAME_SIZE 64


TUN_FD TUN_Open(char *dev_name, int dev_name_size);
TUN_FD TAP_Open(char *dev_name, int dev_name_size);
void TUN_Close(TUN_FD fd);
int TUN_MQUE_Open(INOUT char *dev_name, int dev_name_size, IN int que_num, OUT int *fds);
int TUN_SetNonblock(TUN_FD fd);
int TUN_Read(IN TUN_FD fd, OUT void *buf, IN int buf_size);
int TUN_Write(IN TUN_FD fd, IN void *buf, IN int len);
int TAP_GetMacAddr(TUN_FD fd, OUT MAC_ADDR_S *mac);
int TUN_SetMediaStatus(TUN_FD fd, INOUT int *status);
int TUN_GetIPAddr(TUN_FD fd, OUT U32 *ip, OUT U32 *mask);

#ifdef __cplusplus
}
#endif
#endif 
