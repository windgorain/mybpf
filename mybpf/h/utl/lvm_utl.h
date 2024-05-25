/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2009-6-16
* Description: local var or memeory. 如果申请长度不大，则使用局部变量, 否则申请内存
* History:     
******************************************************************************/

#ifndef __LVM_UTL_H_
#define __LVM_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

#define LVM_LOCAL_VAR_SIZE 256

typedef struct
{
    UCHAR *pucData;
    UCHAR aucLocalVar[LVM_LOCAL_VAR_SIZE];
}LVM_S;

static inline UCHAR * LVM_Malloc(IN LVM_S *pstLvm, IN UINT uiSize)
{
    if (uiSize > LVM_LOCAL_VAR_SIZE)
    {
        pstLvm->pucData = MEM_Malloc(uiSize);
    }
    else
    {
        pstLvm->pucData = pstLvm->aucLocalVar;
    }

    return pstLvm->pucData;
}

static inline VOID LVM_Free(IN LVM_S *pstLvm)
{
    if ((pstLvm->pucData != NULL) && (pstLvm->pucData != pstLvm->aucLocalVar))
    {
        MEM_Free(pstLvm->pucData);
        pstLvm->pucData = NULL;
    }
}

#ifdef __cplusplus
    }
#endif 

#endif 



