/*================================================================
*   Created by LiXingang
*   Description: linux kernel的桩文件
        在用户态模拟内核API
*
================================================================*/
#ifndef _LINUX_KERNEL_PILE_H
#define _LINUX_KERNEL_PILE_H

#include "utl/asm_utl.h"
#include "utl/atomic_utl.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define __force
#define cpu_to_be16 htobe16
#define cpu_to_be32 htobe32
#define cpu_to_be64 htobe64
#define cpu_to_le16 htole16
#define cpu_to_le32 htole32
#define cpu_to_le64 htole64


typedef U8 u8;
typedef U16 u16;
typedef U32 u32;
typedef U64 u64;
typedef S32 s32;
typedef S64 s64;

static inline u64 div64_u64_rem(u64 dividend, u64 divisor, u64 *remainder)
{
	*remainder = dividend % divisor;
	return dividend / divisor;
}

static inline u64 div64_u64(u64 dividend, u64 divisor)
{
	return dividend / divisor;
}

# define do_div(n, base) ({						\
	unsigned int __base = (base);					\
	unsigned int __rem;						\
	__rem = ((unsigned long long)(n)) % __base;			\
	(n) = ((unsigned long long)(n)) / __base;			\
	__rem;								\
})

static inline void atomic64_add(s64 i, atomic64_t *v)
{
    ATOM_FETCH_ADD(&v->counter, i);
}

static inline s64 atomic64_fetch_add(long i, atomic64_t *v)
{
    return ATOM_FETCH_ADD(&v->counter, i);
}

static inline int atomic_fetch_add(int i, atomic_t *v)
{
    return ATOM_FETCH_ADD(&v->counter, i);
}

static inline void atomic_and(int i, atomic_t *v)
{
    ATOM_FETCH_AND(&v->counter, i);
}

static inline void atomic64_and(long i, atomic64_t *v)
{
    ATOM_FETCH_AND(&v->counter, i);
}

static inline int atomic_fetch_and(int i, atomic_t *v)
{
    return ATOM_FETCH_AND(&v->counter, i);
}

static inline s64 atomic64_fetch_and(s64 i, atomic64_t *v)
{
    return ATOM_FETCH_AND(&v->counter, i);
}

static inline void atomic_or(int i, atomic_t *v)
{
    ATOM_FETCH_OR(&v->counter, i);
}

static inline void atomic64_or(s64 i, atomic64_t *v)
{
    ATOM_FETCH_OR(&v->counter, i);
}

static inline int atomic_fetch_or(int i, atomic_t *v)
{
    return ATOM_FETCH_OR(&v->counter, i);
}

static inline s64 atomic64_fetch_or(s64 i, atomic64_t *v)
{
    return ATOM_FETCH_OR(&v->counter, i);
}

static inline void atomic_xor(int i, atomic_t *v)
{
    ATOM_FETCH_XOR(&v->counter, i);
}

static inline void atomic64_xor(s64 i, atomic64_t *v)
{
    ATOM_FETCH_XOR(&v->counter, i);
}

static inline int atomic_fetch_xor(int i, atomic_t *v)
{
    return ATOM_FETCH_XOR(&v->counter, i);
}

static inline s64 atomic64_fetch_xor(s64 i, atomic64_t *v)
{
    return ATOM_FETCH_XOR(&v->counter, i);
}

static inline int atomic_xchg(atomic_t *v, int new_val)
{
    return ATOM_FETCH_SET(&v->counter, new_val);
}

static inline long long atomic64_xchg(atomic64_t *v, long long new_val)
{
    return ATOM_FETCH_SET(&v->counter, new_val);
}

static inline int atomic_cmpxchg(atomic_t *v, int expected, int new_val)
{
    return ATOM_BOOL_COMP_SWAP(&v->counter, &expected, new_val);
}

static inline long long atomic64_cmpxchg(atomic64_t *v, long long expected, long long new_val)
{
    return ATOM_BOOL_COMP_SWAP(&v->counter, &expected, new_val);
}

#ifdef __cplusplus
}
#endif
#endif 
