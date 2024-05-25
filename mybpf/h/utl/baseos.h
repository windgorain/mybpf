/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      lixingang  Version: 1.0  Date: 2007-2-9
* Description: 
* History:     
******************************************************************************/

#ifndef __BASEOS_H_
#define __BASEOS_H_

#ifdef __cplusplus
    extern "C" {
#endif 

#ifdef IN_WINDOWS
    #include <share.h>
    #include <tchar.h>
    #include <winsock2.h>
    #include <windows.h>
    #include <process.h>
    #include <io.h>
    #include <conio.h>
    #include <winioctl.h>
    #include <Iphlpapi.h>
    #include <assert.h>
    #include <signal.h>
    #include <direct.h>
    #include <errno.h>
    #include <sys/utime.h>

    #define random rand
    #define srandom srand
    #define RANDOM_MAX (RAND_MAX)

    typedef unsigned int socklen_t;

    #ifndef inline
        #define inline __inline
    #endif

    #define mkdir(path,mode)     _mkdir(path)

    #ifndef rmdir
        #define rmdir     _rmdir
    #endif

    #define stricmp  _stricmp
    #define strnicmp _strnicmp
    #define sscanf sscanf_s
    #define access _access
    #define chdir _chdir
    #define getch _getch

#endif

#ifdef IN_LINUX
#ifndef __USE_GNU
    #define __USE_GNU 1
#endif
    #include <sched.h>
#endif

#ifdef IN_UNIXLIKE
    #include   <stdio.h>
    #include   <stdbool.h>
    #include   <netinet/in.h>
    #include   <netinet/tcp.h>
    #include   <netdb.h>  
    #include   <sys/types.h>  
    #include   <sys/socket.h>  
    #include   <sys/times.h>
    #include   <sys/time.h>
    #include   <arpa/inet.h>
    #include   <unistd.h>  
    #include   <sys/ioctl.h>  
    #include   <sys/select.h>  
    #include   <errno.h>   
    #include   <sys/mman.h>
#ifndef IN_MAC
    #include   <asm/ioctls.h>
#endif
    #include   <dirent.h>
    #include   <dlfcn.h>
    #include   <signal.h>
    #include   <pthread.h>
    #include   <utime.h>
    #include   <ctype.h>
    #include   <assert.h>
    #include   <fcntl.h>
    #include   <string.h>

    #define SOCKET int
    #define ETIMEOUT ETIMEDOUT
    #define WINAPI 
    #define APITBL_Connect connect

    #define stricmp  strcasecmp
    #define strnicmp strncasecmp

#ifndef closesocket 
    #define closesocket close
#endif

    #define DeleteFile(x)  remove(x)

    #define Sleep(x)    usleep((x)*1000)
#endif

#if !defined(__GNUC__) || (__GNUC__ == 2 && __GNUC_MINOR__ < 96)
#define __builtin_expect(x, expected_value) (x)
#endif

#ifndef likely
#define likely(x)	__builtin_expect((x),1)
#define unlikely(x)	__builtin_expect((x),0)
#endif


#ifdef __cplusplus
    }
#endif 

#endif 


