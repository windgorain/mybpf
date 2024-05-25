/*================================================================
*   Created：LiXingang All rights reserved.
*   Description：
*
================================================================*/
#ifndef __TUN_UTL_H_
#define __TUN_UTL_H_

#ifdef IN_WINDOWS
#include "utl/vnic_lib.h"
#include "utl/vnic_tap.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef IN_WINDOWS
#define TUN_FD HANDLE
#define TUN_FD_IS_VALID(_fd) ((_fd) != NULL)
#endif

#ifdef IN_UNIXLIKE
#define TUN_FD int
#define TUN_FD_IS_VALID(_fd) ((_fd) >= 0)
#endif

#define TUN_TYPE_TUN 0
#define TUN_TYPE_TAP 1


TUN_FD TUN_Open(char *dev_name, int dev_name_size);
TUN_FD TAP_Open(char *dev_name, int dev_name_size);
int TUN_MQUE_Open(INOUT char *dev_name, int dev_name_size, IN int que_num, OUT int *fds);
int TUN_SetNonblock(TUN_FD fd);
int TUN_Read(IN TUN_FD fd, OUT void *buf, IN int buf_size);
int TUN_Write(IN TUN_FD fd, IN void *buf, IN int len);

#ifdef __cplusplus
}
#endif
#endif 
