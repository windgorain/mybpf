/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#include "bs.h"
#include "utl/file_func.h"
#include "utl/file_utl.h"
#include "utl/vbuf_utl.h"


int VBUF_WriteFile(char *filename, VBUF_S *vbuf)
{
    return FILE_WriteFile(filename, VBUF_GetData(vbuf), VBUF_GetDataLength(vbuf));
}


int VBUF_ReadFP(void *fp, OUT VBUF_S *vbuf)
{
    S64 filesize;
    void *data;

    filesize = FILE_GetFpSize(fp);
    if (filesize < 0) {
        RETURN(BS_ERR);
    }

    VBUF_ClearData(vbuf);

    if (VBUF_ExpandTo(vbuf, filesize) < 0) {
        RETURN(BS_ERR);
    }

    VBUF_SetDataLength(vbuf, filesize);
    data = VBUF_GetData(vbuf);

    if (filesize != fread(data, 1, filesize, fp)) {
        RETURN(BS_ERR);
    }

    return 0;    
}


int VBUF_ReadFile(char *filename, OUT VBUF_S *vbuf)
{
    FILE *fp;
    int ret;

    fp = FILE_Open(filename, FALSE, "rb");
    if (! fp) {
        RETURN(BS_CAN_NOT_OPEN);
    }

    ret = VBUF_ReadFP(fp, vbuf);

    FILE_Close(fp);

    return ret;
}

