/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2007-7-31
* Description: 
* History:     
******************************************************************************/

#ifndef __RINGBUF_H_
#define __RINGBUF_H_

#ifdef __cplusplus
    extern "C" {
#endif 

extern BS_STATUS RBUF_Create(IN UINT ulBufLen, OUT HANDLE *phHandle);
extern BS_STATUS RBUF_Delete(IN HANDLE hHandle);

extern BS_STATUS RBUF_WriteForce(IN HANDLE hHandle, IN UCHAR *pucBuf, IN UINT ulBufLen);

extern BS_STATUS RBUF_Write(IN HANDLE hHandle, IN UCHAR *pucBuf, IN UINT ulBufLen, OUT UINT *pulWriteLen);
extern BS_STATUS RBUF_ReadNoDel(IN HANDLE hHandle, OUT UCHAR **ppucData, OUT UINT *pulDataLen);
extern BS_STATUS RBUF_Read(IN HANDLE hHandle, OUT UCHAR **ppucData, OUT UINT *pulDataLen);

extern BS_STATUS RBUF_GetContinueWritePtr(IN HANDLE hHandle, OUT UCHAR **ppucWritePtr, OUT UINT *pulContinueMemLen);

extern VOID RBUF_MoveWriteIndex(IN HANDLE hHandle, IN INT lIndexMovOffset);

extern VOID RBUF_MoveReadIndex(IN HANDLE hHandle, IN INT lIndexMovOffset);
extern BOOL_T RBUF_IsFull(IN HANDLE hHandle);
extern BOOL_T RBUF_IsEmpty(IN HANDLE hHandle);



extern HANDLE RArray_Create(IN UINT ulRowsCount, IN UINT ulColCount);
extern void RArray_Reset(IN HANDLE hHandle);
extern BS_STATUS RArray_Delete(IN HANDLE hHandle);
extern BS_STATUS RArray_WriteForce(IN HANDLE hHandle, IN UCHAR *pucData, IN UINT ulDataLen);
extern BS_STATUS RArray_Write(IN HANDLE hHandle, IN UCHAR *pucData, IN UINT ulDataLen);
extern BS_STATUS RArray_ReadNoDel(IN HANDLE hHandle, OUT UCHAR **ppucData, OUT UINT *pulDataLen);
extern BS_STATUS RArray_Read(IN HANDLE hHandle, OUT UCHAR **ppucData, OUT UINT *pulDataLen);
extern BS_STATUS RArray_ReadIndex(IN HANDLE hHandle, IN UINT ulIndex, OUT UCHAR **ppucData, OUT UINT *pulDataLen);
extern BS_STATUS RArray_ReadReversedIndex(IN HANDLE hHandle, IN UINT ulIndex, OUT UCHAR **ppucData, OUT UINT *pulDataLen);

#ifdef __cplusplus
    }
#endif 

#endif 


