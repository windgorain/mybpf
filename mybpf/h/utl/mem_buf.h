#ifndef MEM_BUF_H
#define MEM_BUF_H

#include <stdbool.h>
#include "utl/types.h"

typedef struct _membuf_t {
	
	char *buf;
	int32_t sz;
	uint32_t ctrl;
	
	struct _membuf_t *next;
} membuf_t;



#define membuf_ctrl_bit(buf, bit) (!!(buf->ctrl & bit_value(typeof(buf->ctrl), bit)))

#define MEMBUF_INT (membuf_t){buf = NULL, sz = 0, ctrl = 0, next = NULL}

membuf_t *_membuf_new(void *buf, int32_t sz, membuf_t *next, bool malloc_buffer);

#define membuf_new(__buf, __sz, __next)   \
	_membuf_new((void *)(__buf), (int32_t)(__sz), __next, true)

#define MEMBUF_NEW(__buf, __sz)     membuf_new(__buf, __sz, NULL, NULL)

membuf_t *membuf_new_alloc(int32_t sz, membuf_t *next);
#define MEMBUF_NEW_ALLOC(__sz)  membuf_new_alloc(__sz, NULL, NULL)

#define membuf_new_copy(__buf, __sz, __next)   \
	_membuf_new((void *)(__buf), (int32_t)(__sz), __next, true)
    
membuf_t *_membuf_new_copy_merge(membuf_t *buf_list, char *append_str, int32_t append_len, bool free_origin_list);
#define membuf_new_merge(__buf_list,__buf, __sz) _membuf_new_copy_merge(__buf_list, __buf, __sz, true)



#define MEMBUF_ONESZ(__lb)  ((__lb)->buf ? (__lb)->sz : 0)
static inline int32_t membuf_size(membuf_t *lb)
{
	int32_t sz = 0;
	while (lb) {
		sz += MEMBUF_ONESZ(lb);
		lb = lb->next;
	}

	return sz;
}

#define MEMBUF_SIZE(__lb)   \
	((__lb && (__lb)->next == NULL) ? MEMBUF_ONESZ(__lb) : membuf_size(__lb))


#define free_with_func(__des_func, __ptr, args...)                              \
	do {                                                                        \
		if (__ptr) {                                                             \
			__des_func(__ptr, ##args);                                          \
			__ptr = NULL;                                                       \
		}                                                                       \
	} while (0)


typedef void (*lb_prefree_func_t)(membuf_t *lb, void *arg);
void membuf_free_fh(membuf_t *lb, lb_prefree_func_t lb_prefree_func, void *arg);

#define membuf_free(p_lb)       free_with_func(membuf_free_fh, p_lb, NULL, NULL)

#endif
