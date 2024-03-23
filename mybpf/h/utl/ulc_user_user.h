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


#undef RETURNI
#define RETURNI(_x, _fmt, ...)  do { \
    printf(_fmt, ##__VA_ARGS__); \
    return(_x); \
} while(0)

#undef RETURN
#define RETURN(x) return(x)

#endif

#ifdef __cplusplus
}
#endif
#endif 
