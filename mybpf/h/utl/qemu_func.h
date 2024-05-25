/*********************************************************
*   Copyright (C) LiXingang
*   Description: 
*
********************************************************/
#ifndef _QEMU_FUNC_H
#define _QEMU_FUNC_H
#ifdef __cplusplus
extern "C"
{
#endif

static inline char * QEMU_Bdf2String(USHORT bdf, OUT char *buf, int buf_size)
{
    snprintf(buf, buf_size, "%02x:%02x.%u", bdf >> 8, (bdf & 0xff) >> 3, bdf & 7);
    return buf;
}


static inline int QEMU_String2Bdf(char *bdf_string)
{
    char *strs[2];
    UINT bus = 0;
    UCHAR dev = 0;
    UCHAR func = 0;
    char *df_string;

    int d_num = TXT_StrToToken(bdf_string, (char*)":", strs, 2);
    if (d_num == 2) {
        bus = TXT_Str2Ui(strs[0]);
        df_string = strs[1];
    } else {
        df_string = strs[0];
    }

    d_num = TXT_StrToToken(df_string, (char*)".", strs, 2);
    if (d_num < 2) {
        return -1;
    }

    dev = TXT_Str2Ui(strs[0]);
    func = TXT_Str2Ui(strs[1]);

    return (bus << 8) | (dev << 3) | func;
}

#ifdef __cplusplus
}
#endif
#endif 
