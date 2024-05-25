/*================================================================
*   Created by LiXingang
*   Description: 计算check sum
*
================================================================*/
#ifndef _CSUM_UTL_H
#define _CSUM_UTL_H
#ifdef __cplusplus
extern "C"
{
#endif

static inline UINT _csum_get_16b_sum(USHORT * data, int len)
{
    UINT checksum = 0x0;

    while (len > 1) {
        checksum += *data;
        data ++;    
        len -= 2;
    }

    if (len) {
        checksum += *((unsigned char*)(void*)data);
    }

    return checksum;
}


static inline USHORT CSUM_IpHeader(USHORT *data, int ip_header_len)
{
    UINT checksum;

    checksum = _csum_get_16b_sum(data, ip_header_len);
    checksum = (checksum >> 16) + (checksum & 0xffff);
    checksum += (checksum>>16);

    return (USHORT)(~checksum);
}

#ifdef __cplusplus
}
#endif
#endif 
