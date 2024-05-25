/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _SEGMENT_BITMAP_H
#define _SEGMENT_BITMAP_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct SBITMAP_ST{
    struct SBITMAP_ST *next;
    UINT offset;
    UINT bitsize; 
    UINT *data;
}SBITMAP_NODE_S;

typedef struct {
    SBITMAP_NODE_S *root;
}SBITMAP_S;

typedef void (*PF_SBITMAP_FREE_DATA)(void * data);
typedef void (*PF_SBITMAP_FREE_NODE)(SBITMAP_NODE_S * node);

int SBITMAP_Init(SBITMAP_S *ctrl);
void SBITMAP_Finit(SBITMAP_S *ctrl, PF_SBITMAP_FREE_NODE free_node_func);
SBITMAP_NODE_S * SBITMAP_FindNode(SBITMAP_S *ctrl, UINT index);
void SBITMAP_AddNode(SBITMAP_S *ctrl, SBITMAP_NODE_S *new_node);
int SBITMAP_Set(SBITMAP_S *ctrl, UINT index);
int SBITMAP_Clr(SBITMAP_S *ctrl, UINT index);
void SBITMAP_Compress(SBITMAP_S *ctrl, PF_SBITMAP_FREE_DATA data_free_func);
int SBITMAP_IsSet(SBITMAP_S *ctrl, UINT index);

#ifdef __cplusplus
}
#endif
#endif 
