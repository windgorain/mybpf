/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _STUB_BS_H
#define _STUB_BS_H
#ifdef __cplusplus
extern "C"
{
#endif

#ifdef USE_BS

#define SSHOW_Add(x,f,l) _sshow_Add(x,f,l)
#define SSHOW_Del(x) _sshow_Del(x)
#define THREAD_Create(name,param,func,ud) _thread_create(name,param,func,ud)
#define THREAD_RegOb(ob) _thread_reg_ob(ob)

#else

#define SSHOW_Add(x,f,l)
#define SSHOW_Del(x)
#define THREAD_Create(name,param,func,ud) ThreadNamed_Create(name,param,func,ud)
#define THREAD_RegOb(ob) ThreadNamed_RegOb(ob)

#endif

#ifdef __cplusplus
}
#endif
#endif 
