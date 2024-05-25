/******************************************************************
*   Created by LiXingang
*   Description: 
*
******************************************************************/
#ifndef _ARGS_DEF_H
#define _ARGS_DEF_H
#ifdef __cplusplus
extern "C"
{
#endif


#define _BS_ARG_N(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,N,...) N
#define BS_ARG_COUNT(...) _BS_ARG_N(0,##__VA_ARGS__,10,9,8,7,6,5,4,3,2,1,0)


#define _BS_ARG_GET1(a0,a1,...) (a1)
#define _BS_ARG_GET2(a0,a1,a2,...) (a2)
#define _BS_ARG_GET3(a0,a1,a2,a3,...) (a3)
#define _BS_ARG_GET4(a0,a1,a2,a3,a4,...) (a4)
#define _BS_ARG_GET5(a0,a1,a2,a3,a4,a5,...) (a5)
#define _BS_ARG_GET6(a0,a1,a2,a3,a4,a5,a6,...) (a6)
#define _BS_ARG_GET7(a0,a1,a2,a3,a4,a5,a6,a7,...) (a7)
#define _BS_ARG_GET8(a0,a1,a2,a3,a4,a5,a6,a7,a8,...) (a8)
#define _BS_ARG_GET9(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,...) (a9)
#define _BS_ARG_GET10(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,...) (a10)
#define BS_ARG_GET(N,...) _BS_ARG_GET##N(0,##__VA_ARGS__,0,0,0,0,0,0,0,0,0,0)

#ifdef __cplusplus
}
#endif
#endif 
