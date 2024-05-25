/*================================================================
*   Created by LiXingang
*   Description: 无锁队列
*
================================================================*/
#ifndef _RING_UTL_H
#define _RING_UTL_H

#ifdef IN_UNIXLIKE
#ifdef __x86_64__
#include <emmintrin.h>
#endif
#endif

#ifdef IN_WINDOWS
#include <winnt.h>
#endif

#include "utl/cache_def.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define __IS_SP 1
#define __IS_MP 0
#define __IS_SC 1
#define __IS_MC 0

enum {
    RING_QUEUE_FIXED = 0, 
    RING_QUEUE_VARIABLE   
};


#define RING_ENQUEUE_PTRS(r, ring_start, prod_head, obj_table, n, obj_type) \
    do { \
        unsigned int i; \
        const uint32_t size = (r)->size; \
        uint32_t idx = prod_head & (r)->mask; \
        obj_type *ring = (obj_type *)ring_start; \
        if (likely(idx + n < size)) { \
            for (i = 0; i < (n & ((~(unsigned)0x3))); i+=4, idx+=4) { \
                ring[idx] = obj_table[i]; \
                ring[idx+1] = obj_table[i+1]; \
                ring[idx+2] = obj_table[i+2]; \
                ring[idx+3] = obj_table[i+3]; \
            } \
            switch (n & 0x3) { \
                case 3: \
                        ring[idx++] = obj_table[i++];  \
                case 2: \
                        ring[idx++] = obj_table[i++];  \
                case 1: \
                        ring[idx++] = obj_table[i++]; \
            } \
        } else { \
            for (i = 0; idx < size; i++, idx++)\
            ring[idx] = obj_table[i]; \
            for (idx = 0; i < n; i++, idx++) \
            ring[idx] = obj_table[i]; \
        } \
    } while (0)


#define RING_DEQUEUE_PTRS(r, ring_start, cons_head, obj_table, n, obj_type) \
    do { \
        unsigned int i; \
        uint32_t idx = cons_head & (r)->mask; \
        const uint32_t size = (r)->size; \
        obj_type *ring = (obj_type *)ring_start; \
        if (likely(idx + n < size)) { \
            for (i = 0; i < (n & (~(unsigned)0x3)); i+=4, idx+=4) {\
                obj_table[i] = ring[idx]; \
                obj_table[i+1] = ring[idx+1]; \
                obj_table[i+2] = ring[idx+2]; \
                obj_table[i+3] = ring[idx+3]; \
            } \
            switch (n & 0x3) { \
                case 3: \
                        obj_table[i++] = ring[idx++];  \
                case 2: \
                        obj_table[i++] = ring[idx++];  \
                case 1: \
                        obj_table[i++] = ring[idx++]; \
            } \
        } else { \
            for (i = 0; idx < size; i++, idx++) \
            obj_table[i] = ring[idx]; \
            for (idx = 0; i < n; i++, idx++) \
            obj_table[i] = ring[idx]; \
        } \
    } while (0)


typedef struct {
    volatile UINT head;  
    volatile UINT tail;  
    UINT single: 1;      
}RING_HEADTAIL_S;

typedef struct {
    UINT size;           
    UINT mask;           
    UINT capacity;       

	char pad0 CACHE_ALIGNED; 

	
	RING_HEADTAIL_S prod CACHE_ALIGNED;
	char pad1 CACHE_ALIGNED; 

	
	RING_HEADTAIL_S cons CACHE_ALIGNED;
	char pad2 CACHE_ALIGNED; 
    void *data[0];
}RING_S;

RING_S * RING_Create(UINT capacity, int is_single_prod, int is_single_cons);

static inline UINT RING_MoveProdHead(RING_S *r, UINT is_sp, UINT n,
        int behavior, UINT *old_head, UINT *new_head, UINT *free_entries)
{
	const uint32_t capacity = r->capacity;
	uint32_t cons_tail;
	unsigned int max = n;
	int success;

	do {
		
		n = max;
        *old_head = ATOM_GET(&r->prod.head);
        
        ATOM_BARRIER();
		cons_tail = ATOM_GET(&r->cons.tail);
		*free_entries = (capacity + cons_tail - *old_head);

		
		if (unlikely(n > *free_entries))
			n = (behavior == RING_QUEUE_FIXED) ?  0 : *free_entries;

		if (n == 0)
			return 0;

		*new_head = *old_head + n;
		if (is_sp)
			r->prod.head = *new_head, success = 1;
		else
			success = ATOM_BOOL_COMP_SWAP(&r->prod.head, old_head, *new_head);
	} while (unlikely(success == 0));

	return n;
}

static inline UINT RING_MoveConsHead(RING_S *r, int is_sc, UINT n,
        int behavior, UINT *old_head, UINT *new_head, UINT *entries)
{
	UINT max = n;
	UINT prod_tail;
	int success;

	
	*old_head = __atomic_load_n(&r->cons.head, __ATOMIC_RELAXED);
	do {
		n = max;
		__atomic_thread_fence(__ATOMIC_ACQUIRE);
		prod_tail = __atomic_load_n(&r->prod.tail, __ATOMIC_ACQUIRE);
		*entries = (prod_tail - *old_head);

		if (n > *entries)
			n = (behavior == RING_QUEUE_FIXED) ? 0 : *entries;

		if (unlikely(n == 0))
			return 0;

		*new_head = *old_head + n;
		if (is_sc)
			r->cons.head = *new_head, success = 1;
		else
			success = __atomic_compare_exchange_n(&r->cons.head,
							old_head, *new_head,
							0, __ATOMIC_RELAXED,
							__ATOMIC_RELAXED);
	} while (unlikely(success == 0));
	return n;
}


static inline void RING_UpdateTail(RING_HEADTAIL_S *ht, uint32_t old_val,
        uint32_t new_val, uint32_t single)
{
	
	if (!single)
		while (unlikely(ht->tail != old_val)) {
#ifdef __x86_64__
			_mm_pause();
#endif
        }

	__atomic_store_n(&ht->tail, new_val, __ATOMIC_RELEASE);
}

static inline UINT RING_DoEnqueue(RING_S *r, void **obj_table,
		 UINT n, int behavior, UINT is_sp, UINT *free_space)
{
	uint32_t prod_head, prod_next;
	uint32_t free_entries;

	n = RING_MoveProdHead(r, is_sp, n, behavior,
			&prod_head, &prod_next, &free_entries);
	if (n == 0)
		goto end;

	RING_ENQUEUE_PTRS(r, &r[1], prod_head, obj_table, n, void *);

	RING_UpdateTail(&r->prod, prod_head, prod_next, is_sp);
end:
	if (free_space != NULL)
		*free_space = free_entries - n;
	return n;
}

static inline UINT RING_DoDequeue(RING_S *r, void **obj_table,
		 UINT n, int behavior, UINT is_sc, UINT *available)
{
	uint32_t cons_head, cons_next;
	uint32_t entries;

	n = RING_MoveConsHead(r, (int)is_sc, n, behavior,
			&cons_head, &cons_next, &entries);
	if (n == 0)
		goto end;

	RING_DEQUEUE_PTRS(r, &r[1], cons_head, obj_table, n, void *);

	RING_UpdateTail(&r->cons, cons_head, cons_next, is_sc);

end:
	if (available != NULL)
		*available = entries - n;
	return n;
}

static inline UINT RING_MpEnqueueBulk(RING_S *r, void **obj_table,
			 UINT n, UINT *free_space)
{
	return RING_DoEnqueue(r, obj_table, n, RING_QUEUE_FIXED,
			__IS_MP, free_space);
}

static inline UINT RING_SpEnqueueBulk(RING_S *r, void **obj_table,
			 UINT n, UINT *free_space)
{
	return RING_DoEnqueue(r, obj_table, n, RING_QUEUE_FIXED,
			__IS_SP, free_space);
}

static inline UINT RING_EnqueueBulk(RING_S *r, void **obj_table,
		      UINT n, UINT *free_space)
{
	return RING_DoEnqueue(r, obj_table, n, RING_QUEUE_FIXED,
			r->prod.single, free_space);
}

static inline int RING_MpEnqueue(RING_S *r, void *obj)
{
	return RING_MpEnqueueBulk(r, &obj, 1, NULL) ? 0 : -ENOBUFS;
}

static inline int RING_SpEnqueue(RING_S *r, void *obj)
{
	return RING_SpEnqueueBulk(r, &obj, 1, NULL) ? 0 : -ENOBUFS;
}

static inline int RING_Enqueue(RING_S *r, void *obj)
{
	return RING_EnqueueBulk(r, &obj, 1, NULL) ? 0 : -ENOBUFS;
}

static inline UINT RING_McDequeueBulk(RING_S *r, void **obj_table,
		UINT n, UINT *available)
{
	return RING_DoDequeue(r, obj_table, n, RING_QUEUE_FIXED,
			__IS_MC, available);
}

static inline UINT RING_ScDequeueBulk(RING_S *r, void **obj_table,
		UINT n, UINT *available)
{
	return RING_DoDequeue(r, obj_table, n, RING_QUEUE_FIXED,
			__IS_SC, available);
}

static inline UINT RING_DequeueBulk(RING_S *r, void **obj_table,
        UINT n, UINT *available)
{
	return RING_DoDequeue(r, obj_table, n, RING_QUEUE_FIXED,
				r->cons.single, available);
}

static inline int RING_McDequeue(RING_S *r, void **obj_p)
{
	return RING_McDequeueBulk(r, obj_p, 1, NULL)  ? 0 : -ENOENT;
}

static inline int RING_ScDequeue(RING_S *r, void **obj_p)
{
	return RING_ScDequeueBulk(r, obj_p, 1, NULL) ? 0 : -ENOENT;
}

static inline int RING_Dequeue(RING_S *r, void **obj_p)
{
	return RING_DequeueBulk(r, obj_p, 1, NULL) ? 0 : -ENOENT;
}

static inline UINT RING_Count(RING_S *r)
{
	uint32_t prod_tail = r->prod.tail;
	uint32_t cons_tail = r->cons.tail;
	uint32_t count = (prod_tail - cons_tail) & r->mask;
	return (count > r->capacity) ? r->capacity : count;
}

static inline UINT RING_FreeCount(RING_S *r)
{
	return r->capacity - RING_Count(r);
}

static inline int RING_Full(RING_S *r)
{
	return RING_FreeCount(r) == 0;
}

static inline int RING_Empty(RING_S *r)
{
	return RING_Count(r) == 0;
}

static inline UINT RING_GetSize(RING_S *r)
{
	return r->size;
}

static inline UINT RING_GetCapacity(RING_S *r)
{
	return r->capacity;
}

static inline UINT RING_MpEnqueueBurst(RING_S *r, void **obj_table,
			 UINT n, UINT *free_space)
{
	return RING_DoEnqueue(r, obj_table, n,
			RING_QUEUE_VARIABLE, __IS_MP, free_space);
}

static inline UINT RING_SpEnqueueBurst(RING_S *r, void **obj_table,
			 UINT n, UINT *free_space)
{
	return RING_DoEnqueue(r, obj_table, n,
			RING_QUEUE_VARIABLE, __IS_SP, free_space);
}

static inline UINT RING_EnqueueBurst(RING_S *r, void **obj_table,
		      UINT n, UINT *free_space)
{
	return RING_DoEnqueue(r, obj_table, n, RING_QUEUE_VARIABLE,
			r->prod.single, free_space);
}

static inline UINT RING_McDequeueBurst(RING_S *r, void **obj_table,
		UINT n, UINT *available)
{
	return RING_DoDequeue(r, obj_table, n,
			RING_QUEUE_VARIABLE, __IS_MC, available);
}

static inline UINT RING_ScDequeueBurst(RING_S *r, void **obj_table,
		UINT n, UINT *available)
{
	return RING_DoDequeue(r, obj_table, n,
			RING_QUEUE_VARIABLE, __IS_SC, available);
}

static inline UINT RING_DequeueBurst(RING_S *r, void **obj_table,
		UINT n, UINT *available)
{
	return RING_DoDequeue(r, obj_table, n, RING_QUEUE_VARIABLE,
            r->cons.single, available);
}

#ifdef __cplusplus
}
#endif
#endif 
