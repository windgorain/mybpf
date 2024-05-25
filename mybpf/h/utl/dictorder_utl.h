/*================================================================
*   Created by LiXingang
*   Description: 字典序
*
================================================================*/
#ifndef _DICTORDER_UTL_H
#define _DICTORDER_UTL_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef int (*PF_DICTORDER_CMP)(void *node1, void *node2, void *ud);
typedef int (*PF_DICTORDER_WALK_FUNC)(void *node, void *ud);


static inline void * DictOrder_ArrayGetFirst(void *data, int node_count,
        int node_size, PF_DICTORDER_CMP cmp_func, void *ud)
{
    int i;

    if (node_count <= 0) {
        return NULL;
    }

    void *min = data;
    UCHAR *tmp = data;
    tmp += node_size;

    for (i=1; i<node_count; i++) {
        if (cmp_func(min, tmp, ud) > 0) {
            min = tmp;
        }
        tmp += node_size;
    }

    return min;
}


static inline void * DictOrder_ArrayGetNext(void *data, int node_count,
        int node_size, void *curr, PF_DICTORDER_CMP cmp_func, void *ud)
{
    if (! curr) {
        return DictOrder_ArrayGetFirst(data, node_count, node_size, cmp_func, ud);
    }

    UCHAR *tmp = data;
    int i;
    int ret;
    void *next = NULL;

    for (i=0; i<node_count; i++) {
        ret = cmp_func(tmp, curr, ud);
        if ((ret == 0) && (tmp > (UCHAR*)curr)) {
            
            return tmp;
        }
        if (ret > 0) {
            if ((next == NULL) || (cmp_func(tmp, next, ud) < 0)) {
                next = tmp;
            }
        }
        tmp += node_size;
    }

    return next;
}

static inline void DictOrder_ArrayScan(void *data, int node_count, int node_size,
        PF_DICTORDER_CMP cmp_func, PF_DICTORDER_WALK_FUNC walk_func, void *ud)
{
    void *next = NULL;

    while ((next = DictOrder_ArrayGetNext(data, node_count, node_size, next, cmp_func, ud))) {
        walk_func(next, ud);
    }
}

#ifdef __cplusplus
}
#endif
#endif 
