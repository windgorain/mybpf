/*================================================================
*   Created by LiXingang
*   Description: segment x bitmap: x表示bit个数,如1024, 256等
*
================================================================*/
#include "bs.h"
#include "utl/mem_utl.h"
#include "utl/segment_bitmap.h"
#include "utl/segmentx_bitmap.h"

static void sxbitmap_FreeNode(SBITMAP_NODE_S * node)
{
    if (node->data != NULL) {
        MEM_Free(node->data);
    }

    MEM_Free(node);
}

int SXBitmap_Init(SXBITMAP_S *ctrl, UINT bitsize)
{
    BS_DBGASSERT((bitsize & 31) ==0);
    SBITMAP_Init(&ctrl->sbitmap);
    ctrl->bitsize = bitsize;

    return 0;
}

void SXBitmap_Finit(SXBITMAP_S *ctrl)
{
    SBITMAP_Finit(&ctrl->sbitmap, sxbitmap_FreeNode);
}

int SXBitmap_Set(SXBITMAP_S *ctrl, UINT index)
{
    SBITMAP_NODE_S *node;

    node = SBITMAP_FindNode(&ctrl->sbitmap, index);
    if (NULL == node) {
        node = MEM_ZMalloc(sizeof(SBITMAP_NODE_S));
        if (node == NULL) {
            RETURN(BS_NO_MEMORY);
        }
        node->data = MEM_ZMalloc(ctrl->bitsize/8);
        if (node->data == NULL) {
            MEM_Free(node);
            RETURN(BS_NO_MEMORY);
        }
        node->offset = index/ctrl->bitsize*ctrl->bitsize;
        node->bitsize = ctrl->bitsize;

        SBITMAP_AddNode(&ctrl->sbitmap, node);
    }

    return SBITMAP_Set(&ctrl->sbitmap, index);
}

void SXBitmap_Clr(SXBITMAP_S *ctrl, UINT index)
{
    SBITMAP_Clr(&ctrl->sbitmap, index);
}
