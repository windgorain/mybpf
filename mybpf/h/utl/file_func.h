/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _FILE_FUNC_H_
#define _FILE_FUNC_H_

#ifdef __cplusplus
extern "C" {
#endif

S64 FILE_GetFpSize(FILE *fp);
int FILE_Mem(char *filename, FILE_MEM_S *m);
int FILE_MemTo(IN CHAR *pszFilePath, OUT void *buf, int buf_size);
int FILE_WriteFile(char *filename, void *data, U32 data_len);

static inline void FILE_FreeMem(FILE_MEM_S *m)
{
    if (m && m->data) {
        MEM_Free(m->data);
        m->data = NULL;
        m->len = 0;
    }
}


#ifdef __cplusplus
}
#endif
#endif 
