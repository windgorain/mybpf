/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#include "bs.h"
#include "utl/file_func.h"


S64 FILE_GetFpSize(FILE *fp)
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

int FILE_Mem(char *filename, FILE_MEM_S *m)
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


int FILE_MemTo(IN CHAR *pszFilePath, OUT void *buf, int buf_size)
{
    FILE *fp;
    S64 filesize;
    int read_len;
    char mode[] = "rb";

    fp = fopen(pszFilePath, mode);
    if (NULL == fp) {
        RETURN(BS_CAN_NOT_OPEN);
    }

    filesize = FILE_GetFpSize(fp);
    read_len = MIN(buf_size, filesize);

    if (read_len != fread(buf, 1, read_len, fp)) {
        fclose(fp);
        RETURN(BS_ERR);
    }

    fclose(fp);

    return read_len;
}

int FILE_WriteFile(char *filename, void *data, U32 data_len)
{
    FILE *fp = NULL;
    int ret = 0;
    char mode[] = "wb+";

    fp = fopen(filename, mode);
    if (! fp) {
        RETURNI(BS_ERR, "Can't open file %s", filename);
    }

    if (data_len) {
        ret = fwrite(data, 1, data_len, fp);
    }

    fclose(fp);

    if (ret < 0) {
        RETURNI(BS_CAN_NOT_WRITE, "Can't wiret to file %s", filename);
    }

    return 0;
}


char * FILE_GetFileNameFromPath(char *pszPath)
{
    UINT ulLen;

    BS_DBGASSERT(NULL != pszPath);

    ulLen = strlen(pszPath);

    while (ulLen > 0) {
        if ((pszPath[ulLen - 1] == '\\') || (pszPath[ulLen - 1] == '/')) {
            return pszPath + ulLen;
        }
        ulLen --;
    }

    return pszPath;
}


char * FILE_GetExternNameFromPath(char *pszPath, U32 uiPathLen)
{
    UINT ulLen = uiPathLen;

    BS_DBGASSERT(NULL != pszPath);

    while (ulLen > 0) {
        if (pszPath[ulLen - 1] == '.') {
            return pszPath + ulLen;
        }
        ulLen --;
    }

    return NULL;
}


int FILE_GetFileNameWithOutExtFromPath(char *pszPath, OUT LSTR_S *pstFileNameWithOutExt)
{
    CHAR *pcFileName;
    CHAR *pcExtName;

    BS_DBGASSERT(NULL != pszPath);

    pcFileName = FILE_GetFileNameFromPath(pszPath);
    if (NULL == pcFileName) {
        return BS_ERR;
    }

    pstFileNameWithOutExt->pcData = pcFileName;

    pcExtName = FILE_GetExternNameFromPath(pcFileName, strlen(pcFileName));
    if (NULL == pcExtName) {
        pstFileNameWithOutExt->uiLen = strlen(pcFileName);
    } else {
        pstFileNameWithOutExt->uiLen = ((pcExtName - pcFileName) - 1);
    }

    return BS_OK;
}

