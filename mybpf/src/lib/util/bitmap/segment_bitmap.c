/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#include "bs.h"
#include "utl/segment_bitmap.h"

int SBITMAP_Init(SBITMAP_S *ctrl)
{
    memset(ctrl, 0, sizeof(SBITMAP_S));

    return 0;
}

void SBITMAP_Finit(SBITMAP_S *ctrl, PF_SBITMAP_FREE_NODE free_node_func)
{
    SBITMAP_NODE_S *node = ctrl->root;
    SBITMAP_NODE_S *next = ctrl->root;

    while (node != NULL) {
        next = node->next;
        free_node_func(node);
        node = next;
    }
}

SBITMAP_NODE_S * SBITMAP_FindNode(SBITMAP_S *ctrl, UINT index)
{
    SBITMAP_NODE_S *node = ctrl->root;
    SBITMAP_NODE_S *find = NULL;

    while (node) {
        if (index < node->offset) {
            break;
        }
        if ((index >= node->offset) && (index < node->offset + node->bitsize)) {
            find = node;
            break;
        }
        node = node->next;
    }

    return find;
}

void SBITMAP_AddNode(SBITMAP_S *ctrl, SBITMAP_NODE_S *new_node)
{
    SBITMAP_NODE_S *node = ctrl->root;

    BS_DBGASSERT((new_node->bitsize & 31) ==0);

    if (node == NULL) {
        ctrl->root = new_node;
        return;
    }

    if (new_node->offset < node->offset) {
        new_node->next = ctrl->root;
        ctrl->root = new_node;
        return;
    }

    while (node->next) {
        if (new_node->offset < node->next->offset) {
            break;
        }
        node = node->next;
    }

    new_node->next = node->next;
    node->next = new_node;

    return;
}

int SBITMAP_Set(SBITMAP_S *ctrl, UINT index)
{
    SBITMAP_NODE_S *node = SBITMAP_FindNode(ctrl, index);
    UINT tmpindex;

    if (node == NULL) {
        RETURN(BS_NOT_FOUND);
    }

    tmpindex = index - node->offset;

    node->data[tmpindex/32] |= ((UINT)1 << (tmpindex & 0x1f));

    return BS_OK;
}

int SBITMAP_Clr(SBITMAP_S *ctrl, UINT index)
{
    SBITMAP_NODE_S *node = SBITMAP_FindNode(ctrl, index);
    UINT tmpindex;

    if (node == NULL) {
        RETURN(BS_NOT_FOUND);
    }

    tmpindex = index - node->offset;

    node->data[tmpindex/32] &= ~((UINT)1 << (tmpindex & 0x1f));

    return BS_OK;
}

int SBITMAP_IsSet(SBITMAP_S *ctrl, UINT index)
{
    SBITMAP_NODE_S *node = SBITMAP_FindNode(ctrl, index);
    UINT tmpindex;

    if (node == NULL) {
        return 0;
    }

    tmpindex = index - node->offset;

    if (node->data[tmpindex/32] & ((UINT)1 << (tmpindex & 0x1f))) {
        return 1;
    }

    return 0;
}
