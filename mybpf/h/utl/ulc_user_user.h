/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _ULC_USER_USER_H_
#define _ULC_USER_USER_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef IN_ULC_USER

#define BARE_Print(_fmt, ...) do { \
    char _msg[] = _fmt; \
    ulc_sys_printf(_msg, ##__VA_ARGS__); \
}while(0)


#ifdef IN_ULC_BARE
#define printf(_fmt, ...) BARE_Print(_fmt, ##__VA_ARGS__)
#endif

#undef RETURNI
#ifdef IN_ULC_BARE
#define RETURNI(_x, _fmt, ...)  do { \
    char _msg[] = _fmt"\n"; \
    ulc_sys_printf(_msg, ##__VA_ARGS__); \
    return(_x); \
} while(0)
#else
#define RETURNI(_x, _fmt, ...)  do { \
    printf(_fmt"\n", ##__VA_ARGS__); \
    return(_x); \
} while(0)
#endif

#undef RETURN
#define RETURN(x) return(x)

#endif

#ifdef __cplusplus
}
#endif
#endif 
