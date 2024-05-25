/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _RP_UTL_H
#define _RP_UTL_H
#include "utl/mime_utl.h"
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    char tag_start; 
    char tag_stop; 
}RP_TAG_S;

int RP_Do(RP_TAG_S *tag, MIME_HANDLE kv, char *in_str, char *out_str, int out_str_size);

#ifdef __cplusplus
}
#endif
#endif 
