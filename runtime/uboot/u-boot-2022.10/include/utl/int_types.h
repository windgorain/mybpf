/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _INT_TYPES_H_
#define _INT_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef short		        SHORT;
typedef unsigned short		USHORT;
typedef unsigned int 		UINT; /* 32 bits */
typedef unsigned long long  UINT64;
typedef long long           INT64;
typedef int 				INT; /* 32 bits */
typedef unsigned char 		UCHAR;
typedef char			    CHAR;
typedef char                BOOL_T;
typedef unsigned long       ULONG;  /* 变长的类型,32位系统上4个字节,64位系统8个字节 */
typedef long                LONG;   /* 变长的类型,32位系统上4个字节,64位系统8个字节 */
typedef UINT                VRF_INDEX;

typedef unsigned char       U8;
typedef unsigned short      U16;
typedef unsigned int        U32;
typedef unsigned long long  U64;
typedef char S8;
typedef short S16;
typedef int S32;
typedef long long S64;

typedef void *              HANDLE;

#ifdef __cplusplus
}
#endif
#endif //INT_TYPES_H_
