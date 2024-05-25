/******************************************************************************
* Copyright (C), 2000-2006,  Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2017-7-4
* Description: 
*   
* History:     
******************************************************************************/

#ifndef __DSTREAM_UTL_H_
#define __DSTREAM_UTL_H_

#include "utl/vbuf_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 

typedef enum
{
    DSTREAM_RECV_STATE_HEAD = 0,    
    DSTREAM_RECV_STATE_DATA         
}DSTREAM_RECV_STATE_E;

typedef struct
{
    VBUF_S stWriteVBuf;
    VBUF_S stReadVBuf;
    UINT uiRemainReadLen;   
    INT iFd;
    UINT bitIsBlock:1;  
    UINT enState:2;     
}DSTREAM_S;

BS_STATUS DSTREAM_Init(IN DSTREAM_S *pstDStream);
VOID DSTREAM_Finit(IN DSTREAM_S *pstDStream);
VOID DSTREAM_SetFd(IN DSTREAM_S *pstDStream, IN INT iFd);
BS_STATUS DSTREAM_Write(IN DSTREAM_S *pstDStream, IN UCHAR *pucData, IN UINT uiDataLen);

BS_STATUS DSTREAM_Read(IN DSTREAM_S *pstDStream);

VBUF_S * DSTREAM_GetData(IN DSTREAM_S *pstDStream);


#ifdef __cplusplus
    }
#endif 

#endif 


