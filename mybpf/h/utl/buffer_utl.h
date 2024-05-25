/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2007-7-9
* Description: 
* History:     
******************************************************************************/

#ifndef __BUFFER_UTL_H_
#define __BUFFER_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

typedef void (*PF_BUFFER_OUT_FUNC)(void *data, UINT len, USER_HANDLE_S *ud);

typedef struct
{
    PF_BUFFER_OUT_FUNC pfFunc;
    USER_HANDLE_S ud;
    UINT uiBufferSize;
    UINT uiDataLen;
    UINT buf_alloced: 1;
    char *buf;
}BUFFER_S;

void BUFFER_Init(BUFFER_S *pstBuffer);
void BUFFER_Fini(BUFFER_S *pstBuffer);

void BUFFER_AttachBuf(BUFFER_S *pstBuffer, void *buf, UINT buf_size);
int BUFFER_AllocBuf(BUFFER_S *pstBuffer, UINT buf_size);
void BUFFER_SetOutputFunc(BUFFER_S *pstBuffer, PF_BUFFER_OUT_FUNC func, USER_HANDLE_S *ud);

int BUFFER_Write(BUFFER_S *pstBuffer, void *data, UINT data_len);
int BUFFER_WriteString(BUFFER_S *pstBuffer, char *data);
void BUFFER_WriteByHex(BUFFER_S *pstBuffer, void *data, int len);
int BUFFER_Print(BUFFER_S *pstBuffer, char *fmt, ...);
int BUFFER_Flush(BUFFER_S *pstBuffer);

static inline void * BUFFER_GetData(BUFFER_S *pstBuffer)
{
    return pstBuffer->buf;
}

static inline UINT BUFFER_GetDataLen(BUFFER_S *pstBuffer)
{
    return pstBuffer->uiDataLen;
}


#ifdef __cplusplus
    }
#endif 

#endif 



