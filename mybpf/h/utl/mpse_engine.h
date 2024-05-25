/*================================================================
*   Created by LiXingang
*   Description: 所字符串扫描引擎: m p scan engine
*
================================================================*/
#ifndef _MPSE_ENGINE_H
#define _MPSE_ENGINE_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    DLL_NODE_S link_node;
    char *pattern;
    int rule_id;
    UCHAR nocase; 
    UCHAR pattern_index; 
    int distance; 
    int within;  
    int offset; 
    int depth; 
}MPSE_PATTERN_S;

typedef void* MPSE_HANDLE;

MPSE_HANDLE MPSE_New();
void MPSE_Free(MPSE_HANDLE hMpse);

int MPSE_AddPattern(MPSE_HANDLE hMpse, MPSE_PATTERN_S *pattern);
int MPSE_Compile(MPSE_HANDLE hMpse);
int MPSE_Match(MPSE_HANDLE hMpse, UCHAR *data, int data_len);

#ifdef __cplusplus
}
#endif
#endif 
