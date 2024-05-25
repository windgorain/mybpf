/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2008-3-1
* Description: 
* History:     
******************************************************************************/

#ifndef __SS_UTL_H_
#define __SS_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 



typedef struct
{
    UINT aulSundaySkipTable[256];
}SUNDAY_SKIP_TABLE_S;


VOID Sunday_ComplexPatt(IN UCHAR *pucPatt, IN UINT ulPattLen, OUT SUNDAY_SKIP_TABLE_S *pstSkipTb);


UCHAR * Sunday_SearchFast(IN UCHAR *pucData, IN UINT ulDataLen, IN UCHAR *pucPatt, IN UINT ulPattLen, IN SUNDAY_SKIP_TABLE_S *pstSkipTb);


UCHAR *Sunday_Search(IN UCHAR *pucData, IN UINT ulDataLen, IN UCHAR *pucPatt, IN UINT ulPattLen);



#ifdef __cplusplus
    }
#endif 

#endif 


