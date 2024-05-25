/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _DATA_QUE_H
#define _DATA_QUE_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    int len;
    unsigned char data[0];
}DATA_QUE_DATA_S;

typedef struct {
    UCHAR *writer_base;
    UCHAR *reader_base;
    int size;
    int read;
    int write;
}DATA_QUE_S;

void DataQue_InitWriter(DATA_QUE_S *ctrl, void *buf, int size);
void DataQue_InitReader(DATA_QUE_S *ctrl, void *buf);
int DataQue_Push(DATA_QUE_S *ctrl, void *data, int len);
DATA_QUE_DATA_S * DataQue_Get(DATA_QUE_S *ctrl);
void DataQue_Pop(DATA_QUE_S *ctrl, DATA_QUE_DATA_S *node);

#ifdef __cplusplus
}
#endif
#endif 
