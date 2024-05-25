/*================================================================
*   Created by LiXingang
*   Description: 使用宏定义定义很多个元素
*
================================================================*/
#ifndef _MDEF_UTL_H
#define _MDEF_UTL_H
#ifdef __cplusplus
extern "C"
{
#endif

#define MDEF_10_FUNCS(prefix, id) \
    MDEF_DEF_FUNC(prefix##0, (id)*10) \
    MDEF_DEF_FUNC(prefix##1, (id)*10+1) \
    MDEF_DEF_FUNC(prefix##2, (id)*10+2) \
    MDEF_DEF_FUNC(prefix##3, (id)*10+3) \
    MDEF_DEF_FUNC(prefix##4, (id)*10+4) \
    MDEF_DEF_FUNC(prefix##5, (id)*10+5) \
    MDEF_DEF_FUNC(prefix##6, (id)*10+6) \
    MDEF_DEF_FUNC(prefix##7, (id)*10+7) \
    MDEF_DEF_FUNC(prefix##8, (id)*10+8) \
    MDEF_DEF_FUNC(prefix##9, (id)*10+9)

#define MDEF_100_FUNCS(prefix, id) \
    MDEF_10_FUNCS(prefix##0, (id)*10) \
    MDEF_10_FUNCS(prefix##1, (id)*10+1) \
    MDEF_10_FUNCS(prefix##2, (id)*10+2) \
    MDEF_10_FUNCS(prefix##3, (id)*10+3) \
    MDEF_10_FUNCS(prefix##4, (id)*10+4) \
    MDEF_10_FUNCS(prefix##5, (id)*10+5) \
    MDEF_10_FUNCS(prefix##6, (id)*10+6) \
    MDEF_10_FUNCS(prefix##7, (id)*10+7) \
    MDEF_10_FUNCS(prefix##8, (id)*10+8) \
    MDEF_10_FUNCS(prefix##9, (id)*10+9)

#define MDEF_1000_FUNCS \
    MDEF_100_FUNCS(0, 0) \
    MDEF_100_FUNCS(1, 1) \
    MDEF_100_FUNCS(2, 2) \
    MDEF_100_FUNCS(3, 3) \
    MDEF_100_FUNCS(4, 4) \
    MDEF_100_FUNCS(5, 5) \
    MDEF_100_FUNCS(6, 6) \
    MDEF_100_FUNCS(7, 7) \
    MDEF_100_FUNCS(8, 8) \
    MDEF_100_FUNCS(9, 9)

#ifdef __cplusplus
}
#endif
#endif 
