/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _LDATA_UTL_H
#define _LDATA_UTL_H
#ifdef __cplusplus
extern "C"
{
#endif


void LDATA_StrimHead(LDATA_S *data, LDATA_S *rm_chars, OUT LDATA_S *out_data);

void LDATA_StrimTail(LDATA_S *data, LDATA_S *rm_chars, OUT LDATA_S *out_data);

void LDATA_Strim(LDATA_S *data, char *rm_chars, OUT LDATA_S *out_data);

void * LDATA_Strchr(LDATA_S *data, UCHAR ch);

void * LDATA_ReverseChr(LDATA_S *data, UCHAR ch);

void * LDATA_Strstr(LDATA_S *data, LDATA_S *to_find);

void * LDATA_Stristr(LDATA_S *data, LDATA_S *to_find);

void LDATA_MSplit(LDATA_S *in, LDATA_S *split_chars, OUT LDATA_S *out1, OUT LDATA_S *out2);

void LDATA_Split(LDATA_S *in, UCHAR split_char, OUT LDATA_S *out1, OUT LDATA_S *out2);

UINT LDATA_XSplit(LDATA_S *in, UCHAR split_char, OUT LDATA_S *outs, UINT count);

int LDATA_Cmp(LDATA_S *data1, LDATA_S *data2);

INT LDATA_CaseCmp(LDATA_S *data1, LDATA_S *data2);


static inline void * LDATA_BoundsCheck(LDATA_S *d, U64 offset, U64 size)
{
    if (offset + size > d->len || offset + size < offset) {
        return NULL;
    }
    return d->data + offset;
}

static inline void * LLDATA_BoundsCheck(LLDATA_S *d, U64 offset, U64 size)
{
    if (offset + size > d->len || offset + size < offset) {
        return NULL;
    }
    return d->data + offset;
}

#ifdef __cplusplus
}
#endif
#endif 
