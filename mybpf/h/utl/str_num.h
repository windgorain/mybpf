/*================================================================
*   Created by LiXingang
*   Description: 字符串和id映射查询表
*
================================================================*/
#ifndef _STR_NUM_H
#define _STR_NUM_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    char *str;
    int id;
}STR_ID_S;

static inline int StrID_Str2ID(STR_ID_S *nodes, char *str)
{
    STR_ID_S *n = nodes;

    if ((! nodes) || (! str)) {
        return -1;
    }

    while (n->str) {
        if (strcmp(n->str, str) == 0) {
            return n->id;
        }
        n ++;
    }

    return -1;
}

static inline char * StrID_ID2Str(STR_ID_S *nodes, int id)
{
    STR_ID_S *n = nodes;

    while(n->str) {
        if (n->id == id) {
            return n->str;
        }
        n ++;
    }

    return NULL;
}

#ifdef __cplusplus
}
#endif
#endif 
