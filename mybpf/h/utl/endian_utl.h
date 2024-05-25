/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2014-8-7
* Description: 
* History:     
******************************************************************************/

#ifndef __ENDIAN_UTL_H_
#define __ENDIAN_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define BS_BIG_ENDIAN 0
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define BS_BIG_ENDIAN 1
#else
#error "Error"
#endif


#if BS_BIG_ENDIAN
#define ntoh3B(x) (x)
#define hton3B(x) (x)
#else
#define ntoh3B(x) ((((x) & 0xff) << 16) | ((x) & 0x00ff00) | (((x) >> 16) & 0xff))
#define hton3B(x) ntoh3B(x)
#endif

#if __BYTE_ORDER == __LITTLE_ENDIAN
static inline int HostIsLitter(void) {
    return 1;
}
#elif __BYTE_ORDER == __BIG_ENDIAN
static inline int HostIsLitter(void) {
    return 0;
}
#else
static inline int HostIsLitter(void)
{
    int a=1;
    if(*((char*)&a)==1) {
        return 1;
    }
    return 0;
}
#endif

static inline USHORT ENDIAN_ChangeOrder16(USHORT data)
{
    return (data >> 8) | (data << 8);
}

static inline UINT ENDIAN_ChangeOrder32(UINT data)
{
    return (data >> 24) | ((data >> 8) & 0xff00) | ((data & 0xff00) << 8) | (data << 24);
}

static inline UINT64 ENDIAN_ChangeOrder64(UINT64 data)
{
    return (data >> 56) | ((data >> 40) & 0xff00) | ((data >> 24) & 0xff0000) | ((data >> 8) & 0xff000000)
        | ((data & 0xff000000) << 8) | ((data & 0xff0000) << 24) | ((data & 0xff00) << 40) | (data << 56);
}

static inline USHORT Host2Litter16(USHORT data)
{
    if (HostIsLitter()) {
        return data;
    }
    return ENDIAN_ChangeOrder16(data);
}

static inline UINT Host2Litter32(UINT data)
{
    if (HostIsLitter()) {
        return data;
    }
    return ENDIAN_ChangeOrder32(data);
}

static inline UINT64 Host2Litter64(UINT64 data)
{
    if (HostIsLitter()) {
        return data;
    }
    return ENDIAN_ChangeOrder64(data);
}

static inline USHORT Litter2Host16(USHORT data)
{
    return Host2Litter16(data);
}

static inline UINT Litter2Host32(UINT data)
{
    return Host2Litter32(data);
}

static inline UINT64 Litter2Host64(UINT64 data)
{
    return Host2Litter64(data);
}

static inline USHORT Host2Big16(USHORT data)
{
    if (HostIsLitter()) {
        return ENDIAN_ChangeOrder16(data);
    }
    return data;
}

static inline UINT Host2Big32(UINT data)
{
    if (HostIsLitter()) {
        return ENDIAN_ChangeOrder32(data);
    }
    return data;
}

static inline UINT64 Host2Big64(UINT64 data)
{
    if (HostIsLitter()) {
        return ENDIAN_ChangeOrder64(data);
    }
    return data;
}

static inline USHORT Big2Host16(USHORT data)
{
    return Host2Big16(data);
}

static inline UINT Big2Host32(UINT data)
{
    return Host2Big32(data);
}

static inline UINT64 Big2Host64(UINT64 data)
{
    return Host2Big64(data);
}


#ifndef htonll
#define htonll(x) Host2Big64(x)
#define ntohll(x) Big2Host64(x)
#endif

#ifdef IN_MAC

#include <libkern/OSByteOrder.h>

#define htobe16(x) OSSwapHostToBigInt16(x)
#define htole16(x) OSSwapHostToLittleInt16(x)
#define be16toh(x) OSSwapBigToHostInt16(x)
#define le16toh(x) OSSwapLittleToHostInt16(x)

#define htobe32(x) OSSwapHostToBigInt32(x)
#define htole32(x) OSSwapHostToLittleInt32(x)
#define be32toh(x) OSSwapBigToHostInt32(x)
#define le32toh(x) OSSwapLittleToHostInt32(x)

#define htobe64(x) OSSwapHostToBigInt64(x)
#define htole64(x) OSSwapHostToLittleInt64(x)
#define be64toh(x) OSSwapBigToHostInt64(x)
#define le64toh(x) OSSwapLittleToHostInt64(x)

#endif

#ifndef htobe16
#define htobe16(x) Host2Big16(x)
#define htole16(x) Host2Litter16(x)
#define htobe32(x) Host2Big32(x)
#define htole32(x) Host2Litter32(x)
#define htobe64(x) Host2Big64(x)
#define htole64(x) Host2Litter64(x)
#endif

#ifdef __cplusplus
    }
#endif 

#endif 


