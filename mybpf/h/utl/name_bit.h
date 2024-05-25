/******************************************************************************
* Copyright (C), 2000-2006,  Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2016-11-28
* Description: 
* History:     
******************************************************************************/

#ifndef __NAME_BIT_H_
#define __NAME_BIT_H_

#ifdef __cplusplus
    extern "C" {
#endif 

typedef struct
{
    CHAR *pcName;
    UINT uiBit;
}NAME_BIT_S;


UINT NameBit_GetBitByName(IN NAME_BIT_S *pstNameBits, IN CHAR *pcName);


#ifdef __cplusplus
    }
#endif 

#endif 


