/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _UNAME_UTL_H
#define _UNAME_UTL_H

#include <sys/utsname.h>

#ifdef __cplusplus
    extern "C" {
#endif 

#ifndef KERNEL_VERSION
#define KERNEL_VERSION(a, b, c) (((a) << 16) + ((b) << 8) + ((c) > 255 ? 255 : (c)))
#endif

int UNAME_GetInfo(OUT struct utsname *buff);
unsigned int UNAME_GetKernelVersion();

#ifdef __cplusplus
    }
#endif 

#endif 
