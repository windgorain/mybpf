/******************************************************************************
* Copyright (C), LiXingang
* Author:      lixingang  Version: 1.0  Date: 2012-2-8
* Description: 缓冲器. 缓冲到满或者Flush时触发回调函数调用.
* History:     
******************************************************************************/

#include "bs.h"

#include "utl/txt_utl.h"
#include "utl/data2hex_utl.h"
#include "utl/buffer_utl.h"

static inline VOID buffer_FlushBuffer(IN BUFFER_S *pstBuffer)
{
    UINT uiDataLen;

    uiDataLen = pstBuffer->uiDataLen;
    pstBuffer->uiDataLen = 0;
    if (NULL != pstBuffer->pfFunc) {
        pstBuffer->pfFunc(pstBuffer->buf, uiDataLen, &pstBuffer->ud);
    }
}

void BUFFER_Init(BUFFER_S *pstBuffer)
{
    memset(pstBuffer, 0, sizeof(BUFFER_S));
}

void BUFFER_Fini(BUFFER_S *pstBuffer)
{
    if (NULL == pstBuffer) {
        return;
    }

    if (pstBuffer->buf_alloced) {
        MEM_Free(pstBuffer->buf);
        pstBuffer->buf = NULL;
        pstBuffer->buf_alloced = 0;
    }
}

void BUFFER_AttachBuf(BUFFER_S *pstBuffer, void *buf, UINT buf_size)
{
    if (pstBuffer->buf_alloced) {
        MEM_Free(pstBuffer->buf);
    }

    pstBuffer->buf = buf;
    pstBuffer->uiBufferSize = buf_size;
}

int BUFFER_AllocBuf(BUFFER_S *pstBuffer, UINT buf_size)
{
    pstBuffer->buf = MEM_Malloc(buf_size + 1);  
    if (! pstBuffer->buf) {
        RETURN(BS_NO_MEMORY);
    }

    pstBuffer->uiBufferSize = buf_size;
    pstBuffer->buf_alloced = 1;

    return 0;
}

void BUFFER_SetOutputFunc(BUFFER_S *pstBuffer, PF_BUFFER_OUT_FUNC func, USER_HANDLE_S *ud)
{
    pstBuffer->pfFunc = func;
    if (ud) {
        pstBuffer->ud = *ud;
    }
}

int BUFFER_Write(BUFFER_S *pstBuffer, void *pData, UINT uiDataLen)
{
    if (! pstBuffer) {
        return BS_ERR;
    }

    if (pstBuffer->uiDataLen > 0) {
        if (uiDataLen + pstBuffer->uiDataLen >= pstBuffer->uiBufferSize) {
            buffer_FlushBuffer(pstBuffer);
        }
    }

    if (uiDataLen >= pstBuffer->uiBufferSize) {
        pstBuffer->pfFunc(pData, uiDataLen, &pstBuffer->ud);
    } else {
        memcpy(pstBuffer->buf + pstBuffer->uiDataLen, pData, uiDataLen);
        pstBuffer->uiDataLen += uiDataLen;
    }

    return BS_OK;
}

void BUFFER_WriteLn(BUFFER_S *pstBuffer, char *data, int len)
{
    if (len > 0) {
        BUFFER_Write(pstBuffer, data, len);
    }
    BUFFER_Write(pstBuffer, "\n", 1);
}

int BUFFER_WriteString(BUFFER_S *pstBuffer, char *pcData)
{
    BS_DBGASSERT(NULL != pcData);
    return BUFFER_Write(pstBuffer, pcData, strlen(pcData));
}

void BUFFER_WriteStringLn(BUFFER_S *pstBuffer, char *str)
{
    BUFFER_WriteLn(pstBuffer, str, strlen(str));
}

void BUFFER_WriteByHex(BUFFER_S *pstBuffer, void *data, int len)
{
    char hex[1024 + 1];
    UCHAR *tmp = data;
    int left_len = len;
    int hex_data_len;

    while (left_len > 0) {
        hex_data_len = MIN(left_len, sizeof(hex)/2);
        DH_Data2HexString(tmp, hex_data_len, hex);
        BUFFER_Write(pstBuffer, hex, hex_data_len *2);
        left_len -= hex_data_len;
        tmp += hex_data_len;
    }
}

static VOID _buffer_Print(char *pcMsg, BUFFER_S *pstBuffer)
{
     BUFFER_Write(pstBuffer, pcMsg, strlen(pcMsg));
}

int BUFFER_Print(BUFFER_S *pstBuffer, char *fmt, ...)
{
    if (! pstBuffer) {
        return BS_ERR;
    }

    TXT_ARGS_PRINT(_buffer_Print, pstBuffer);

    return BS_OK;
}

BS_STATUS BUFFER_Flush(BUFFER_S *pstBuffer)
{
    if (! pstBuffer) {
        return BS_ERR;
    }

    if (pstBuffer->uiDataLen > 0) {
        buffer_FlushBuffer(pstBuffer);
    }

    return BS_OK;
}

