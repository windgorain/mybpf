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
typedef unsigned int 		UINT; 
typedef unsigned long long  UINT64;
typedef long long           INT64;
typedef int 				INT; 
typedef unsigned char 		UCHAR;
typedef char			    CHAR;
typedef char                BOOL_T;
typedef unsigned long       ULONG;  
typedef long                LONG;   
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

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef BOOL_TRUE
#define BOOL_TRUE TRUE
#endif

#ifndef BOOL_FALSE
#define BOOL_FALSE FALSE
#endif

#ifdef __cplusplus
}
#endif
#endif 
