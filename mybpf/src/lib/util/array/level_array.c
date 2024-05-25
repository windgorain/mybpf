/*================================================================
*   Created：2018.12.09 LiXingang All rights reserved.
*   Description: 分级数组
*                一开始只有一级,随着数目增多,变为两级,再增多变为三级...
================================================================*/
#include "bs.h"
#include "utl/mem_utl.h"
#include "utl/bitmap_utl.h"
#include "utl/level_array.h"

int LevelArray_Init(IN LEVEL_ARRAY_S *level_array)
{
    memset(level_array, 0, sizeof(LEVEL_ARRAY_S));
    level_array->root = MEM_Malloc(sizeof(LEVEL_ARRAY_NODE_S));
    if (NULL == level_array->root) {
        RETURN(BS_NO_MEMORY);
    }

    level_array->root->level = 0;

    return 0;
}

static void levelarray_Fini(IN LEVEL_ARRAY_NODE_S *node)
{
    int i;

    if (node->level != 0) {
        for (i=0; i<LEVEL_ARRAY_NODE_CAPACITY; i++) {
            if (node->data[i] != NULL) {
                levelarray_Fini(node->data[i]);
            }
        }
    }

    MEM_Free(node);

    return;
}

void LevelArray_Fini(IN LEVEL_ARRAY_S *level_array)
{
    levelarray_Fini(level_array->root);
    memset(level_array, 0, sizeof(LEVEL_ARRAY_S));
}

static int levelarray_Index2Level(IN UINT index)
{
    int level = 0;

    while(1) {
        if (index < LEVEL_ARRAY_NODE_CAPACITY) {
            return level;
        }
        index = index/LEVEL_ARRAY_NODE_CAPACITY;
        level ++;
    }
}

static int levelarray_Expand(IN LEVEL_ARRAY_S *level_array, IN UINT index)
{
    int need_level = levelarray_Index2Level(index);
    LEVEL_ARRAY_NODE_S *node;

    while (need_level > level_array->root->level) {
        node = MEM_Malloc(sizeof(LEVEL_ARRAY_NODE_S));
        if (node == NULL) {
            RETURN(BS_NO_MEMORY);
        }
        node->level = level_array->root->level + 1;
        node->data[0] = level_array->root;
        level_array->root = node;
    }

    return BS_OK;
}

static int levelarray_GetOffset(IN int level, IN UINT index)
{
    int i;

    
    for (i=0; i<level; i++) {
        index /= LEVEL_ARRAY_NODE_CAPACITY;
    }

    
    return index % LEVEL_ARRAY_NODE_CAPACITY;
}

static int levelarray_Set(IN LEVEL_ARRAY_NODE_S *node, IN UINT index, IN void *data)
{
    int offset;
    LEVEL_ARRAY_NODE_S *next_node;

    offset = levelarray_GetOffset(node->level, index);

    if (node->level == 0) {
        node->data[offset] = data;
        return BS_OK;
    }

    if (node->data[offset] == NULL) {
        next_node = MEM_Malloc(sizeof(LEVEL_ARRAY_NODE_S));
        if (next_node == NULL) {
            RETURN(BS_NO_MEMORY);
        }
        next_node->level = node->level - 1;
        node->data[offset] = next_node;
    }

    return levelarray_Set(node->data[offset], index, data);
}

int LevelArray_Set(IN LEVEL_ARRAY_S *level_array, IN UINT index, IN void *data)
{
    BS_STATUS eRet;

    eRet = levelarray_Expand(level_array, index);
    if (BS_OK != eRet) {
        return eRet;
    }

    levelarray_Set(level_array->root, index, data);

    return 0;
}

