/*================================================================
*   Created by LiXingang
*   Description: 带空洞的不连续buf
*
================================================================*/
#include "bs.h"
#include "utl/list_dtq.h"
#include "utl/hole_buf.h"

int HoleBuf_Init(HOLE_BUF_S *hole_buf)
{
    Mem_Zero(hole_buf, sizeof(HOLE_BUF_S));
    return 0;
}

int HoleBuf_Add(HOLE_BUF_S *hole_buf, HOLE_BUF_NODE_S *node)
{
    DTQ_NODE_S *link_node;
    HOLE_BUF_NODE_S *tmp;

    DTQ_FOREACH(&hole_buf->list, link_node) {
        tmp = container_of(link_node, HOLE_BUF_NODE_S, link_node);
        if (node->offset < tmp->offset) {
            break;
        }
    }

    if (DTQ_IsEndOfQ(&hole_buf->list, link_node)) {
        DTQ_AddTail(&hole_buf->list, &node->link_node);
    } else {
        DTQ_AddBefore(link_node, &node->link_node);
    }

    return 0;
}

void HoleBuf_Del(HOLE_BUF_S *hole_buf, HOLE_BUF_NODE_S *node)
{
    DTQ_Del(&node->link_node);
}
