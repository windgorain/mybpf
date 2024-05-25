/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _CONSISTENT_HASH_DEF_H
#define _CONSISTENT_HASH_DEF_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    UINT hash;
    UINT data;
}CONSISTENT_HASH_NODE_S;

typedef struct {
    int max;
    int count;
    CONSISTENT_HASH_NODE_S nodes[0];
}CONSISTENT_HASH_S;

static inline int ConsistentHash_Match(CONSISTENT_HASH_S *ctrl, UINT key)
{
    int left, right;
    int mid;
    int found = 0;
    CONSISTENT_HASH_NODE_S *nodes = ctrl->nodes;

    if ((!ctrl) || (ctrl->count <= 0)) {
        return -1;
    }

    left = 0;
    right = ctrl->count - 1;

    while (left <= right) {
        mid = (left+right) >> 1;
        if (key == nodes[mid].hash) {
            found = mid;
            break;
        } else if (key <= nodes[mid].hash) {
            found = mid;
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    return found;
}

#ifdef __cplusplus
}
#endif
#endif
