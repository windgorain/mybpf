/******************************************************************
*   Created by LiXingang
*   Description: 
*
******************************************************************/
#ifndef _ULC_HELPER_ID_H
#define _ULC_HELPER_ID_H
#ifdef __cplusplus
extern "C"
{
#endif

enum {
    ULC_ID_MALLOC = 1000001,
    ULC_ID_FREE,
    ULC_ID_KALLOC,
    ULC_ID_KFREE,
    ULC_ID_REALLOC,

    ULC_ID_USLEEP = 1000050,

    ULC_ID_PRINTF = 1000070,
    ULC_ID_PUTS,
    ULC_ID_SPRINTF,
    ULC_ID_FPRINTF,

    ULC_ID_ACCESS = 1000100,

    ULC_ID_FTELL = 1000102,
    ULC_ID_FSEEK,
    ULC_ID_FOPEN,
    ULC_ID_FREAD,
    ULC_ID_FCLOSE,
    ULC_ID_FGETS,
    ULC_ID_FWRITE,

    ULC_ID_STAT = 1000120,

    ULC_ID_TIME = 1000130,

    ULC_ID_SOCKET = 1000200,
    ULC_ID_BIND,
    ULC_ID_CONNECT,
    ULC_ID_LISTEN,
    ULC_ID_ACCEPT,
    ULC_ID_RECV,
    ULC_ID_SEND,
    ULC_ID_CLOSE,
    ULC_ID_SETSOCKOPT,

    ULC_ID_THREAD_CREATE = 1000300,

    ULC_ID_RCU_CALL = 1000352,
    ULC_ID_RCU_LOCK,
    ULC_ID_RCU_UNLOCK,
    ULC_ID_RCU_SYNC,

    ULC_ID_ERRNO = 1000402,
    
    ULC_ID_INIT_TIMER = 1000410,
    ULC_ID_ADD_TIMER,
    ULC_ID_DEL_TIMER,

    ULC_ID_MMAP_MAP = 1000450,
    ULC_ID_MMAP_UNMAP,
    ULC_ID_MMAP_MPROTECT,

    ULC_ID_GET_SYM = 1000470,

    ULC_ID_SET_TRUSTEESHIP = 1000480,
    ULC_ID_GET_TRUSTEESHIP,

    ULC_ID_DO_NOTHING = 1000490,
    ULC_ID_LOCAL_ARCH,
    ULC_ID_SET_HELPER,
    ULC_ID_GET_HELPER,
    ULC_ID_GET_BASE_HELPER,
    ULC_ID_GET_SYS_HELPER,
    ULC_ID_GET_USER_HELPER,
    ULC_ID_MAP_GET_NEXT_KEY,
    ULC_ID_ENV_NAME,
};

#ifdef __cplusplus
}
#endif
#endif 
