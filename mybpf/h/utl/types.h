/*
* Copyright (C), Xingang.Li
* File name     : azHead.h
* Date          : 2005/2/25
* Description   : 
*/

#ifndef __INCazHead_h
#define __INCazHead_h

#include "utl/int_types.h"
#include "utl/args_def.h"

#ifdef __cplusplus
extern "C" {
#endif 

#ifndef NULL
#define NULL 0
#endif

#define	IN      
#define	OUT     
#define	INOUT   

#ifndef noinline
#define noinline __attribute__((noinline))
#endif

#ifndef ALWAYS_INLINE 
    #ifdef IN_LINUX
        #define ALWAYS_INLINE __always_inline
    #else
        #define ALWAYS_INLINE inline
    #endif
#endif


#ifndef __always_inline 
#define __always_inline inline
#endif

#ifndef CHAR_BIT
#define CHAR_BIT 8
#endif

#ifndef SEC
#define SEC(x)
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

#ifndef MAX
#define MAX(a,b)  ((a)>(b) ? (a) : (b))
#define MIN(a,b)  ((a)<(b) ? (a) : (b))
#endif

#ifndef UINT32_MAX
#define UINT32_MAX (0xffffffff)
#endif

#ifndef STATIC
#define STATIC static
#endif

#define VOID    void
#define VRF_ANY (VRF_INDEX)0

#ifdef IN_WINDOWS
    #define CONSTRUCTOR(f)  \
        static void f(void);  \
        static int __f1(void){f();return 0;}  \
        __pragma(data_seg(".CRT$XIU"))  \
        static int(*__f2) (void) = __f1;  \
        __pragma(data_seg())  \
        static void f(void)

    #define DESTRUCTOR(fin) \
        static void fin(void);  \
        static int __fin1(void){fin();return 0;}  \
        __pragma(data_seg(".CRT$XPU"))  \
        static int(*__fin2) (void) = __fin1;  \
        __pragma(data_seg())  \
        static void fin(void)

    #define PLUG_HIDE
    #define PLUG_API  __declspec(dllexport)
    #define PLUG_HDL    HINSTANCE 
    #define PLUG_LOAD(pcPlugFilePath)                  (PLUG_HDL)LoadLibraryA(pcPlugFilePath)
    #define PLUG_FREE(ulPlugId)                        FreeLibrary(ulPlugId)
    #define PLUG_GET_FUNC_BY_NAME(ulPlugId,pcFuncName) GetProcAddress(ulPlugId, pcFuncName)
    #define THREAD_LOCAL __declspec(thread) 
#endif

#ifdef IN_UNIXLIKE
#define CONSTRUCTOR(_init)  __attribute__((constructor)) static void _init(void)
#define DESTRUCTOR(_final) __attribute__((destructor)) static void _final(void)
#define PLUG_API  __attribute__ ((visibility("default")))
#define PLUG_HIDE __attribute__ ((visibility("hidden"))) 
#define PLUG_HDL void*
#define PLUG_LOAD(pcPlugFilePath)   PLUG_LoadLib(pcPlugFilePath)
#define PLUG_FREE(ulPlugId)         PLUG_UnloadLib(ulPlugId)
#define PLUG_GET_FUNC_BY_NAME(ulPlugId,pcFuncName)    dlsym((PLUG_HDL)ulPlugId,pcFuncName)
#define THREAD_LOCAL __thread
#endif

#define UINT_HANDLE(uiValue)     ((HANDLE)((ULONG)((UINT)(uiValue))))
#define HANDLE_UINT(hValue)     ((UINT)((ULONG)(hValue)))
#define ULONG_HANDLE(uiValue)     ((HANDLE)((ULONG)(uiValue)))
#define HANDLE_ULONG(hValue)     (((ULONG)(hValue)))

#define STR(x)  #x

typedef struct {
    char *pcData;
    UINT uiLen;
}LSTR_S;


typedef struct {
    UCHAR *data;
    UINT len;
}LDATA_S;


typedef struct {
    UCHAR *data; 
    UINT64 len;   
}LLDATA_S;

typedef LLDATA_S FILE_MEM_S;

#define BS_DATA_ZERO(_pstData) do {(_pstData)->pucData = NULL; (_pstData)->uiLen = 0;} while(0)


typedef VOID 		(*VOID_FUNC)(void);
typedef int         (*INT_FUNC)(void);
typedef UINT 		(*UINT_FUNC)(void);
typedef UINT 		(*UINT_FUNC_1)(VOID *pArg1);
typedef UINT 		(*UINT_FUNC_2)(VOID *pArg1, VOID *pArg2);
typedef UINT 		(*UINT_FUNC_3)(VOID *pArg1, VOID *pArg2, VOID *pArg3);
typedef UINT 		(*UINT_FUNC_4)(VOID *pArg1, VOID *pArg2, VOID *pArg3, VOID *pArg4);
typedef UINT 		(*UINT_FUNC_5)(VOID *pArg1, VOID *pArg2, VOID *pArg3, VOID *pArg4, VOID *pArg5);
typedef UINT 		(*UINT_FUNC_6)(VOID *pArg1, VOID *pArg2, VOID *pArg3, VOID *pArg4, VOID *pArg5, VOID *pArg6);
typedef HANDLE 		(*HANDLE_FUNC)(void);
typedef BOOL_T		(*BOOL_FUNC)(void);
typedef int         (*PF_CMP_FUNC)(const void *k, const void *n);
typedef int         (*PF_CMP_EXT_FUNC)(const void *k, const void *n, void *ud);
typedef void        (*PF_DEL_FUNC)(void *n, void *ud);
typedef void        (*PF_WALK_FUNC)(void *n, void *ud);
typedef int         (*PF_PRINT_FUNC)(const char *fmt, ...);

typedef enum{
    
	BS_OK = 0,
	BS_ERR = -1,
	BS_NO_SUCH = -2,
	BS_ALREADY_EXIST = -3,
	BS_BAD_PTR = -4,
	BS_CAN_NOT_OPEN = -5,
	BS_WRONG_FILE = -6,
	BS_NOT_SUPPORT = -7,
	BS_OUT_OF_RANGE = -8,
	BS_TIME_OUT = -9,
	BS_NO_MEMORY = -10,
	BS_NULL_PARA = -11,
	BS_NO_RESOURCE = -12,
	BS_BAD_PARA = -13,
	BS_NO_PERMIT = -14,
	BS_FULL = -15,
	BS_EMPTY = -16,
	BS_PAUSE = -17,
	BS_STOP  = -18,
	BS_CONTINUE = -19,
	BS_NOT_FOUND = -20,
	BS_NOT_COMPLETE = -21,
	BS_CAN_NOT_CONNECT = -22,
	BS_CONFLICT = -23,
	BS_TOO_LONG = -24,
	BS_TOO_SMALL = -25,
	BS_BAD_REQUEST = -26,
	BS_AGAIN = -27,
	BS_CAN_NOT_WRITE = -28,
	BS_NOT_READY = -29,
	BS_PROCESSED= -30,
	BS_PEER_CLOSED = -31,
	BS_NOT_MATCHED = -32,
	BS_VERIFY_FAILED = -33,
	BS_NOT_INIT = -34,
	BS_REF_NOT_ZERO = -35, 
    BS_BUSY = -36,
    BS_PARSE_FAILED = -37,
	BS_REACH_MAX = -38,
    BS_STOLEN = -39,

    
    BS_PRIVATE_BASE = -100
}BS_STATUS;

typedef BS_STATUS   (*BS_STATUS_FUNC)(void);
extern CHAR * ErrInfo_Get(IN BS_STATUS eRet);

#ifndef ERROR_SUCCESS
#define ERROR_SUCCESS BS_OK
#endif

#ifndef ERROR_FAILED
#define ERROR_FAILED BS_ERR
#endif

typedef enum {
	BS_ACTION_UNDEF = 0,
    BS_ACTION_DENY,
    BS_ACTION_PERMIT,
    
    
    BS_ACTION_MAX
}BS_ACTION_E;

typedef enum {
    BS_MATCH = 0,   
    BS_PART_MATCH,  
    BS_NOT_MATCH    
}BS_MATCH_RET_E;

typedef enum
{
    BS_NO_WAIT = 0,
    BS_WAIT
}BS_WAIT_E;

#define BS_WAIT_FOREVER	0

#define BS_OFFSET(type,item) ((ULONG)&(((type *) 0)->item))
#define BS_END_OFFSET(type,item) (BS_OFFSET(type,item) + sizeof(((type *)0)->item))
#define BS_ENTRY(pAddr, item, type) ((type *) ((UCHAR*)(pAddr) - BS_OFFSET (type, item)))


#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif


#ifndef offsetofend
# define offsetofend(TYPE, FIELD) (offsetof(TYPE, FIELD) + sizeof(((TYPE *)0)->FIELD))
#endif

#ifndef container_of
#define container_of(ptr, type, member) ({			\
	const typeof(((type *)0)->member) * __mptr = (ptr);	\
	(type *)((char *)__mptr - offsetof(type, member)); })
#endif

#define BS_PRINT_ERR(_fmt, ...) fprintf(stderr, _fmt, ##__VA_ARGS__)

#ifdef IN_DEBUG
#define BS_DBGASSERT(X)  do { \
    if (! (X)) { \
        BackTrace_Print(); \
        assert(0); \
    } \
} while(0)
#else
#define BS_DBGASSERT(X)
#endif

#define DBGASSERT(X) BS_DBGASSERT(X)

#define BS_DBG_OUTPUT(_ulFlag,_ulSwitch,_X)  \
    do {if ((_ulFlag) & (_ulSwitch)){IC_DbgInfo _X;}}while(0)

#ifndef BS_WARNNING
#define BS_WARNNING(X)  \
    do {    \
        PRINTLN_HYELLOW("Warnning:%s(%d): ", __FILE__, __LINE__); \
        printf X;   \
        printf ("\n");    \
    }while(0)
#endif

#ifdef IN_DEBUG
#define BS_DBG_WARNNING(X) BS_WARNNING(X)
#else
#define BS_DBG_WARNNING(X)
#endif

typedef struct {
    HANDLE ahUserHandle[4];
}USER_HANDLE_S;

#ifndef __has_attribute
#define __has_attribute(x) 0
#endif

#if __has_attribute(__fallthrough__)
#define fallthrough                    __attribute__((__fallthrough__))
#else
#define fallthrough                    do {} while (0)  
#endif

#ifdef __cplusplus
}
#endif 
#endif 



