/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2010-5-6
* Description: 
* History:     
******************************************************************************/

#ifndef __OFBUF_UTL_H_
#define __OFBUF_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

typedef struct
{
    UINT uiBufSize;
    UINT uiDataSize;
    UINT uiOffset;
    UCHAR *pucData;
}_OFBUF_CTRL_S;

static inline HANDLE OFBUF_Create(IN UINT uiSize)
{
    _OFBUF_CTRL_S *pstCtrl;

    pstCtrl = MEM_Malloc(sizeof(_OFBUF_CTRL_S) + uiSize);
    if (NULL == pstCtrl)
    {
        return NULL;
    }
    Mem_Zero(pstCtrl, sizeof(_OFBUF_CTRL_S));

    pstCtrl->uiBufSize = uiSize;
    pstCtrl->pucData = (UCHAR*)(pstCtrl + 1);

    return pstCtrl;
}

static inline VOID OFBUF_Destory(IN HANDLE hOfbuf)
{
    MEM_Free(hOfbuf);
}


static inline UCHAR * OFBUF_GetBuf(IN HANDLE hOfbuf)
{
    _OFBUF_CTRL_S *pstCtrl = (_OFBUF_CTRL_S *)hOfbuf;

    return pstCtrl->pucData;
}

static inline UINT OFBUF_GetBufLen(IN HANDLE hOfbuf)
{
    _OFBUF_CTRL_S *pstCtrl = (_OFBUF_CTRL_S *)hOfbuf;

    return pstCtrl->uiBufSize;
}


static inline UCHAR * OFBUF_GetData(IN HANDLE hOfbuf)
{
    _OFBUF_CTRL_S *pstCtrl = (_OFBUF_CTRL_S *)hOfbuf;

    return pstCtrl->pucData + pstCtrl->uiOffset;
}

static inline UINT OFBUF_GetDataLen(IN HANDLE hOfbuf)
{
    _OFBUF_CTRL_S *pstCtrl = (_OFBUF_CTRL_S *)hOfbuf;

    return pstCtrl->uiDataSize - pstCtrl->uiOffset;
}

static inline VOID OFBUF_SetDataLen(IN HANDLE hOfbuf, IN UINT uiLen)
{
    _OFBUF_CTRL_S *pstCtrl = (_OFBUF_CTRL_S *)hOfbuf;

    pstCtrl->uiDataSize = uiLen;
    pstCtrl->uiOffset = 0;
}

static inline VOID OFBUF_CutHead(IN HANDLE hOfbuf, IN UINT uiHeadLen)
{
    _OFBUF_CTRL_S *pstCtrl = (_OFBUF_CTRL_S *)hOfbuf;

    pstCtrl->uiOffset += uiHeadLen;
}

static inline VOID OFBUF_CutTail(IN HANDLE hOfbuf, IN UINT uiTailLen)
{
    _OFBUF_CTRL_S *pstCtrl = (_OFBUF_CTRL_S *)hOfbuf;

    pstCtrl->uiOffset -= uiTailLen;
}


#ifdef __cplusplus
    }
#endif 

#endif 


