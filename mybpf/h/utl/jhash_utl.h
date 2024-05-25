/******************************************************************************
* Copyright (C), 2000-2006,  Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2017-6-2
* Description: 
* History:     
******************************************************************************/

#ifndef __JHASH_UTL_H_
#define __JHASH_UTL_H_

#include "utl/num_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 

#define JHASH_INITVAL		0xdeadbeef

#define JHASH_MIX(a, b, c)    { \
	a -= c;  a ^= NUM_Rol32(c, 4);  c += b;	\
	b -= a;  b ^= NUM_Rol32(a, 6);  a += c;	\
	c -= b;  c ^= NUM_Rol32(b, 8);  b += a;	\
	a -= c;  a ^= NUM_Rol32(c, 16); c += b;	\
	b -= a;  b ^= NUM_Rol32(a, 19); a += c;	\
	c -= b;  c ^= NUM_Rol32(b, 4);  b += a;	\
}

#define JHASH_FINAL(a, b, c)  { \
	c ^= b; c -= NUM_Rol32(b, 14);		\
	a ^= c; a -= NUM_Rol32(c, 11);		\
	b ^= a; b -= NUM_Rol32(a, 25);		\
	c ^= b; c -= NUM_Rol32(b, 16);		\
	a ^= c; a -= NUM_Rol32(c, 4);		\
	b ^= a; b -= NUM_Rol32(a, 14);		\
	c ^= b; c -= NUM_Rol32(b, 24);		\
}

static inline UINT JHASH_GeneralBuffer(const void *key, UINT length, UINT initval)
{
	UINT a, b, c;
	const UCHAR *k = key;
    UINT *d;

	a = b = c = JHASH_INITVAL + length + initval;

	while (length > 12) {
        d = (void*)k;
		a += d[0];
		b += d[1];
		c += d[2];
		JHASH_MIX(a, b, c);
		length -= 12;
		k += 12;
	}

	switch (length) {
	case 12: c += (UINT)k[11]<<24; fallthrough;
	case 11: c += (UINT)k[10]<<16; fallthrough;
	case 10: c += (UINT)k[9]<<8; fallthrough;
	case 9:  c += k[8]; fallthrough;
	case 8:  b += (UINT)k[7]<<24; fallthrough;
	case 7:  b += (UINT)k[6]<<16; fallthrough;
	case 6:  b += (UINT)k[5]<<8; fallthrough;
	case 5:  b += k[4]; fallthrough;
	case 4:  a += (UINT)k[3]<<24; fallthrough;
	case 3:  a += (UINT)k[2]<<16; fallthrough;
	case 2:  a += (UINT)k[1]<<8; fallthrough;
	case 1:  a += k[0]; JHASH_FINAL(a, b, c); fallthrough;
	case 0:  break;
	}

	return c;
}


static inline UINT JHASH_U32Buffer(const UINT *k, UINT length, UINT initval)
{
	UINT a, b, c;

	a = b = c = JHASH_INITVAL + (length<<2) + initval;

	while (length > 3) {
		a += k[0];
		b += k[1];
		c += k[2];
		JHASH_MIX(a, b, c);
		length -= 3;
		k += 3;
	}

    switch (length) {
        case 3: c += k[2];
        case 2: b += k[1];
        case 1: a += k[0]; JHASH_FINAL(a, b, c);
        case 0:	break;
    }

    return c;
}

static inline UINT JHASH_Buffer(const void *k, UINT len, UINT initval)
{
    if ((len & 0x3) == 0) { 
        return JHASH_U32Buffer(k, len >> 2, initval);
    } else {
        return JHASH_GeneralBuffer(k, len, initval);
    }
}

static inline UINT _jhash_nwords(UINT a, UINT b, UINT c, UINT initval)
{
	a += initval;
	b += initval;
	c += initval;
	JHASH_FINAL(a, b, c);
	return c;
}

static inline UINT JHASH_3Words(UINT a, UINT b, UINT c, UINT initval)
{
	return _jhash_nwords(a, b, c, initval + JHASH_INITVAL + (3 << 2));
}

static inline UINT JHASH_2Words(UINT a, UINT b, UINT initval)
{
	return _jhash_nwords(a, b, 0, initval + JHASH_INITVAL + (2 << 2));
}

static inline UINT JHASH_Word(UINT a, UINT initval)
{
	return _jhash_nwords(a, 0, 0, initval + JHASH_INITVAL + (1 << 2));
}

#ifdef __cplusplus
    }
#endif 

#endif 


