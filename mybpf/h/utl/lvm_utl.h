/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2009-6-16
* Description: local var or memeory. 如果申请长度不大，则使用局部变量, 否则申请内存
* History:     
******************************************************************************/

#ifndef __LVM_UTL_H_
#define __LVM_UTL_H_

#include "lvm_ext.h"

#ifdef __cplusplus
    extern "C" {
#endif 

static inline U8 * LVM_Malloc(LVM_S *pstLvm, U32 size)
{
    if (size > LVM_LOCAL_VAR_SIZE) {
        pstLvm->data = MEM_Malloc(size);
    } else {
        pstLvm->data = pstLvm->local_var;
    }
    return pstLvm->data;
}

static inline void LVM_Free(LVM_S *pstLvm)
{
    if ((pstLvm->data) && (pstLvm->data != pstLvm->local_var)) {
        MEM_Free(pstLvm->data);
        pstLvm->data = NULL;
    }
}

#ifdef __cplusplus
    }
#endif 

#endif 



