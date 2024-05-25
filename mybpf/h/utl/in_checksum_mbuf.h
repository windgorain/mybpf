/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _IN_CHECKSUM_MBUF_H
#define _IN_CHECKSUM_MBUF_H
#ifdef __cplusplus
extern "C"
{
#endif

static inline USHORT IN_Cksum(IN MBUF_S *pstMBuf, IN UINT uiLen )
{
    UCHAR *pucData;
    UINT uiTotleLen = uiLen;
    UINT uiCrcLen;
    UINT uiDataLen;
    USHORT usCkSum = 0;

    MBUF_SCAN_DATABLOCK_BEGIN(pstMBuf, pucData, uiDataLen)
    {
        uiCrcLen = MIN(uiTotleLen, uiDataLen);
        usCkSum = IN_CHKSUM_AddRaw(usCkSum, pucData, uiCrcLen);
        uiTotleLen -= uiCrcLen;
        if (uiTotleLen == 0)
        {
            break;
        }
    }MBUF_SCAN_END();

    return IN_CHKSUM_Wrap(usCkSum);
}

#ifdef __cplusplus
}
#endif
#endif 
