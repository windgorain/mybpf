/*================================================================
*   Created by LiXingang
*   Description: 封装文件监视器
*
================================================================*/
#ifndef _INOTIFY_UTL_H
#define _INOTIFY_UTL_H
#include "bs.h"
#ifdef __cplusplus
extern "C"
{
#endif

#ifdef IN_LINUX

#include <sys/inotify.h>

#else

#define IN_NONBLOCK
#define IN_CLOEXEC

#define IN_ACCESS (1) 
#define IN_ATTRIB (1<<1) 
#define IN_CLOSE_WRITE (1<<2)  
#define IN_CLOSE_NOWRITE (1<<3) 
#define IN_CREATE (1<<4)  
#define IN_DELETE (1<<5) 
#define IN_DELETE_SELF (1<<6) 
#define IN_MODIFY (1<<7) 
#define IN_MOVE_SELF (1<<8) 
#define IN_MOVED_FROM (1<<9) 
#define IN_MOVED_TO (1<<10) 
#define IN_OPEN (1<<11)  
#define IN_ALL_EVENTS (0xfff)


#define IN_DONT_FOLLOW (1<<12) 
#define IN_MASK_ADD (1<<13) 
#define IN_ONESHOT (1<<14) 
#define IN_ONLYDIR (1<<15) 


#define IN_IGNORED (1<<16) 
#define IN_ISDIR (1<<17) 
#define IN_Q_OVERFLOW (1<<18) 
#define IN_UMOUNT (1<<19) 

struct inotify_event {
   int      wd;       
   uint32_t mask;     
   uint32_t cookie;   
   uint32_t len;      
   char     name[];   
};

#endif

#define INOTIFY_EVENT_SIZE sizeof(struct inotify_event)

int INOTIFY_Init(int flag);
int INOTIFY_AddWatch(int fd, IN char *pathname, UINT mask);
int INOTIFY_DelWatch(int fd, int wd);

#ifdef __cplusplus
}
#endif
#endif 
