/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _HASH_CALC_H
#define _HASH_CALC_H
#ifdef __cplusplus
extern "C"
{
#endif

static inline UINT BKDRHash(UCHAR *data, int len)
{
    UINT seed = 31;
    UINT hash = 0;
    UCHAR *end = data + len;

    while(data < end) {
        hash = hash * seed + (*data++);
    }

    return hash;
}

static inline UINT DJBHash(const UCHAR *data, int len)
{
    UINT hash = 5381;
    UCHAR *end = (void*)(data + len);

    while(data < end) {
        hash += (hash << 5) + (*data++);
    }

    return hash;
}

static inline UINT SDBMHash(UCHAR *data, int len)
{
    UINT hash = 0;
    UCHAR *end = data + len;

    while(data < end) {
        hash = (*data++) + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}

static inline UINT RSHash(UCHAR *data, int len)
{
    UINT b = 378551;
    UINT a = 63689;
    UINT hash = 0;
    UCHAR *end = data + len;

    while(data < end) {
        hash = hash * a + (*data++);
        a *= b;
    }

    return hash;
}

static inline UINT JSHash(UCHAR *data, int len)
{
    UINT hash = 1315423911;
    UCHAR *end = data + len;

    while(data < end) {
        hash ^= ((hash << 5) + (*data++) + (hash >> 2));
    }

    return hash;
}

static inline UINT ELFHash(UCHAR *data, int len)
{
    UINT hash = 0;
    UINT x = 0;
    UCHAR *end = data + len;

    while(data < end) {
        hash = (hash << 4) + (*data++);
        if ((x = hash & 0xF0000000L) != 0) {
            hash ^= (x >> 24);
            hash &= ~x;
        }
    }

    return hash;
}

static inline UINT APHash(UCHAR *data, int len)
{
    UINT hash = 0;
    int i = 0;
    UCHAR *end = data + len;

    while(data < end) {
        if ((i&1) == 0) {
            hash ^= ((hash << 7) ^ (*data++) ^ (hash >> 3));
        } else {
            hash ^= (~((hash << 11) ^ (*data++) ^ (hash >> 5)));
        }
        i ++;
    }

    return hash;
}

#ifdef __cplusplus
}
#endif
#endif 
