/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _SPRINTF_CACHE_H
#define _SPRINTF_CACHE_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct tagSPRINTF_CACHE * SPRINTF_CACHE_HDL;

SPRINTF_CACHE_HDL SprintfCache_Create();
void SprintfCache_Destroy(SPRINTF_CACHE_HDL hdl);
int SprintfCache_Sprintf(SPRINTF_CACHE_HDL hdl, char *buf, int size, char *fmt, va_list args);

#ifdef __cplusplus
}
#endif
#endif 
