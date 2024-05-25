/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _HOLE_BUF_H
#define _HOLE_BUF_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    DTQ_NODE_S link_node;
    void *buf;
    unsigned int len;
    unsigned int offset;
}HOLE_BUF_NODE_S;

typedef struct {
    DTQ_HEAD_S list;
}HOLE_BUF_S;

int HoleBuf_Init(HOLE_BUF_S *hole_buf);
int HoleBuf_Add(HOLE_BUF_S *hole_buf, HOLE_BUF_NODE_S *node);




#ifdef __cplusplus
}
#endif
#endif 
