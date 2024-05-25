/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2007-8-18
* Description: 
* History:     
******************************************************************************/

#ifndef __BIT_OPT_H_
#define __BIT_OPT_H_

#ifdef __cplusplus
    extern "C" {
#endif 

#ifdef IN_WINDOWS
#define	BIT_ROTATE(a,n)	(_lrotr(a,n))   
#elif defined(__GNUC__) && __GNUC__>=2 
#if defined(__i386) || defined(__i386__) || defined(__x86_64) || defined(__x86_64__)
#define BIT_ROTATE(a,n)	({ register unsigned int ret;	\
				asm ("rorl %1,%0"	\
					: "=r"(ret)	\
					: "I"(n),"0"(a)	\
					: "cc");	\
			   ret;				\
			})
#endif
#endif

#ifndef BIT_ROTATE
#define	BIT_ROTATE(a,n)	(((a)>>(n))+((a)<<(32-(n))))
#endif


#define BIT_ISSET(ulFlag, ulBits)  (((ulFlag) & (ulBits)) != 0)

#define BIT_MATCH(ulFlag, ulBits)  (((ulFlag) & (ulBits)) == (ulBits))

#define BIT_SET(ulFlag, ulBits)  ((ulFlag) |= (ulBits))

#define BIT_CLR(ulFlag, ulBits)  ((ulFlag) &= ~(ulBits))

#define BIT_TEST(flag, bits) BIT_ISSET(flag, bits)
#define BIT_RESET(flag, bits) BIT_CLR(flag, bits)


#define BIT_BUILD_RANGE(begin, end)  ((0xffffffff >> (31 - ((end) - (begin)))) << (begin))


#define BIT_BUILD_OFF(off, size)  BIT_BUILD_RANGE(off, (size)-1)


#define BIT_TURN(flag, bits) ((flag) ^= (bits))


#define BIT_SETTO(ulFlag, ulBitMask, ulToBits) ((ulFlag) = (((ulFlag) & (~(ulBitMask))) | ((ulToBits) & (ulBitMask))))


#define BIT_OFF_SETTO(ulFlag, mask, v, off) BIT_SETTO(ulFlag, (mask)<<(off), (v)<<(off))


#define BIT_RANGE_SETTO(ulFlag, begin, end, v) BIT_SETTO(ulFlag, BIT_BUILD_RANGE(begin, end), (v)<<(begin))


#define BIT_GET_OFF(v, off, size) (((v) >> off) & BIT_BUILD_OFF(0, size))


#define BIT_ONLY_LAST(a) ((a) & (-(a)))


static inline UINT BIT_Count1(UINT u)
{
    u = (u & 0x55555555) + ((u >> 1) & 0x55555555);
    u = (u & 0x33333333) + ((u >> 2) & 0x33333333);
    u = (u & 0x0F0F0F0F) + ((u >> 4) & 0x0F0F0F0F);
    u = (u & 0x00FF00FF) + ((u >> 8) & 0x00FF00FF);
    u = (u & 0x0000FFFF) + ((u >> 16) & 0x0000FFFF);
    return u;
}


typedef struct {
    U32 off;
    U32 size;
}BIT_DESC_S;


static inline int BIT_GetLowIndex(UINT num)
{
    int i;

    for (i=0; i<32; i++) {
        if (num & (1 << i)) { 
            return i;
        }
    }

    return -1;
}


static inline int BIT_GetHighIndex(UINT num)
{
    int i;

    for (i=31; i>=0; i--) {
        if (num & (1 << i)) { 
            return i;
        }
    }

    return -1;
}


static inline int BIT_GetLowIndex64(U64 num)
{
    int i;

    for (i=0; i<64; i++) {
        if (num & (1 << i)) { 
            return i;
        }
    }

    return -1;
}


static inline int BIT_GetHighIndex64(U64 num)
{
    int i;

    for (i=63; i>=0; i--) {
        if (num & (1 << i)) { 
            return i;
        }
    }

    return -1;
}

int BIT_GetHighIndexFrom(UINT num, UINT from );
char * BIT_SPrint(U32 v, U32 off, U32 size, OUT char *buf);
void BIT_Print(U32 v, U32 off, U32 size, PF_PRINT_FUNC func);
int BIT_XSPrint(U32 v, BIT_DESC_S *desc, int desc_num, OUT char *buf, U32 buf_size);
U8 BIT_ChangeOrder(U8 v);
UINT BIT_GetCount1(UINT u);

#ifdef __cplusplus
        }
#endif 

#endif 


