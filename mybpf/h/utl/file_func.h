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


static inline S64 FILE_GetFpSize(FILE *fp)
{
    S64 cur = ftell(fp);
    if (cur < 0) {
        return -1;
    }

    if (fseek(fp, 0, SEEK_END) < 0) {
        return -1;
    }

    S64 size = ftell(fp);

    fseek(fp, cur, SEEK_SET);

    return size;
}

static inline void FILE_FreeMem(FILE_MEM_S *m)
{
    if (m && m->data) {
        MEM_Free(m->data);
        m->data = NULL;
        m->len = 0;
    }
}

static inline int FILE_Mem(char *filename, FILE_MEM_S *m)
{
    FILE *fp;
    S64 filesize;
    char mode[] = "rb";

    m->data = NULL;
    m->len = 0;

    fp = fopen(filename, mode);
    if (! fp) {
        goto _ERR;
    }

    filesize = FILE_GetFpSize(fp);
    if (filesize < 0) {
        goto _ERR;
    }

    m->len = filesize;
    m->data = MEM_Malloc(filesize);
    if (! m->data) {
        goto _ERR;
    }

    if (filesize != fread(m->data, 1, filesize, fp)) {
        goto _ERR;
    }

    fclose(fp);

    return 0;

_ERR:
    if (fp) {
        fclose(fp);
    }
    if (m->data) {
        FILE_FreeMem(m);
    }
    return -1;
}


#ifdef __cplusplus
}
#endif
#endif 
