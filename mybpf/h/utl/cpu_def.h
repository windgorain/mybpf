/*================================================================
*   Created by LiXingang
*   Description: 一些cpu的宏定义可以查看
*        https:
*
================================================================*/
#ifndef _CPU_DEF_H
#define _CPU_DEF_H
#ifdef __cplusplus
extern "C"
{
#endif

#if ((defined __x86_64__) || (defined __amd64__) || (defined __i386__))
    #define __X86__
#endif

#if ((defined __arm__) || (defined __aarch64__))
    #define __ARM__
    #ifdef __aarch64__
        #define __ARM64__
    #else
        #define __ARM32__
    #endif
#endif

#ifdef __cplusplus
}
#endif
#endif 
