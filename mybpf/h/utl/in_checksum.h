/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2007-5-13
* Description: 
* History:     
******************************************************************************/

#ifndef __IN_CHECKSUM_H_
#define __IN_CHECKSUM_H_

#ifdef __cplusplus
    extern "C" {
#endif 


USHORT IN_CHKSUM_AddRaw(IN USHORT usCurrentSum, IN UCHAR *pucData, IN UINT uiDataLen);
USHORT IN_CHKSUM_DelRaw(IN USHORT usCurrentSum, IN UCHAR *pucData, IN UINT uiDataLen);

static inline USHORT IN_CHKSUM_AddRawWord(IN USHORT usCurrentSum, IN USHORT usWord)
{
    UINT uiCheckSum = usCurrentSum;

    uiCheckSum += usWord;

    
    if (uiCheckSum & 0xffff0000) {
        uiCheckSum = (uiCheckSum & 0xFFFF) + ((uiCheckSum >> 16) & 0xFFFF);
    }

    return (USHORT) uiCheckSum;
}


static inline USHORT IN_CHKSUM_Raw(IN UCHAR *pucData, IN UINT uiDataLen)
{
    return IN_CHKSUM_AddRaw(0, pucData, uiDataLen);
}


static inline USHORT IN_CHKSUM_Wrap(IN USHORT usRawCheckSum)
{
    USHORT usSum = ~usRawCheckSum;
	return htons(usSum);
}


static inline USHORT IN_CHKSUM_UnWrap(IN USHORT usCheckSum)
{
    USHORT usSum = ~usCheckSum;
	return htons(usSum);
}


static inline USHORT IN_CHKSUM_CheckSum(IN UCHAR *pucData, IN UINT uiDataLen)
{
    return IN_CHKSUM_Wrap(IN_CHKSUM_Raw(pucData, uiDataLen));
}



static inline U16 IN_CHKSUM_Change(U16 *olddata, int oldcount, U16 *newdata, int newcount, U16 old_checksum)
{
    UINT sum;
    int i;

    sum = (~old_checksum) & 0xffff;

    for (i=0; i<oldcount; i++) {
        sum += (~olddata[i] & 0xffff);
    }
    for (i=0; i<newcount; i++) {
        sum += newdata[i];
    }

    sum = (sum & 0xffff) + (sum >> 16);
    sum = (sum & 0xffff) + (sum >> 16);

    if (sum == 0xffff) {
        return sum;
    }

    return (~sum) & 0xffff;
}

#ifdef __cplusplus
    }
#endif 

#endif 

