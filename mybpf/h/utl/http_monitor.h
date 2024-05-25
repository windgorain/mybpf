/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _HTTP_MONITOR_H
#define _HTTP_MONITOR_H
#include "utl/http_lib.h"
#ifdef __cplusplus
extern "C"
{
#endif

enum {
    HTTP_MONITOR_STATE_HEAD = 0,
    HTTP_MONITOR_STATE_BODY,
    HTTP_MONITOR_STATE_ERR,
};

typedef enum {
    HTTP_MONITOR_DATA_HEAD = 0,
    HTTP_MONITOR_DATA_BODY,
    HTTP_MONITOR_DATA_END,
    HTTP_MONITOR_DATA_ERR
}HTTP_MONITOR_DATA_TYPE_E;

typedef int (*PF_HTTP_MONITOR_OUTPUT)(void *http_monitor,
        int output_type, UCHAR *data, int len, void *ud);

typedef struct {
    UCHAR type;
    UCHAR state;
    UINT64 offset; 
    HTTP_HEAD_PARSER head_parser;
    HTTP_BODY_PARSER body_parser;
    PF_HTTP_MONITOR_OUTPUT output;
    void *ud;
}HTTP_MONITOR_S;

int HttpMonitor_Init(HTTP_MONITOR_S *ctrl, HTTP_SOURCE_E type);
int HttpMonitor_Reset(HTTP_MONITOR_S *ctrl);
void HttpMonitor_Final(HTTP_MONITOR_S *ctrl);
int HttpMonitor_Input(HTTP_MONITOR_S *ctrl, UINT64 offset, UCHAR *data,
        int len, PF_HTTP_MONITOR_OUTPUT output, void *ud);

int HttpMonitor_ForceProcHead(HTTP_MONITOR_S *ctrl, UINT64 offset, UCHAR *data,
        int len, PF_HTTP_MONITOR_OUTPUT output, void *ud);

#ifdef __cplusplus
}
#endif
#endif 
