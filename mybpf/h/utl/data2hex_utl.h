/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2007-8-9
* Description: 
* History:     
******************************************************************************/

#ifndef __DATA2HEX_UTL_H_
#define __DATA2HEX_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

static inline INT HEX_2_NUM(IN CHAR cHex)
{
    if (cHex >= '0' && cHex <= '9')
    {
        return cHex - '0';
    }
    
    if (cHex >= 'a' && cHex <= 'f')
    {
        return (cHex - 'a') + 10;
    }

    if (cHex >= 'A' && cHex <= 'F')
    {
        return (cHex - 'A') + 10;
    }

    return -1;
}

static inline BS_STATUS HEX_2_UCHAR(IN CHAR *pcHex, OUT UCHAR *puch)
{
    UCHAR chAscii = 0;
    INT i;
    INT iNum;
    
    for (i=0; i<2; i++)
    {
        iNum = HEX_2_NUM(pcHex[i]);
        if (iNum == -1)
        {
            return BS_ERR;
        }

        chAscii = chAscii * 16 + iNum;
    }
    
    *puch = chAscii;

    return BS_OK;
}

void UCHAR_2_HEX(UCHAR c, OUT CHAR *hex);


extern BS_STATUS DH_Data2Hex(IN UCHAR *pucData, IN UINT ulLen, OUT CHAR *pszOutString);

extern BS_STATUS DH_Data2HexString(IN UCHAR *pucData, IN UINT ulLen, OUT CHAR *pszOutString);

extern BS_STATUS DH_Hex2Data(IN CHAR *pszHex, IN UINT ulHexLen, OUT UCHAR *pucData);
extern BS_STATUS DH_HexString2Data(IN CHAR *pszHexString, OUT void *data);
extern BS_STATUS HexStrToByte(const char* source, unsigned char* dest, int sourceLen);


#ifdef __cplusplus
    }
#endif 

#endif 


