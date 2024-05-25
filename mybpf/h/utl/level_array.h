/*================================================================
*   Created: 2018.12.09 LiXingang All rights reserved.
*   Description: 
*
================================================================*/
#ifndef __LEVEL_ARRAY_H_
#define __LEVEL_ARRAY_H_
#ifdef __cplusplus
extern "C"
{
#endif

#define LEVEL_ARRAY_NODE_CAPACITY 1024

typedef struct {
    UINT level:5; 
    UINT reserved:27;
    void * data[LEVEL_ARRAY_NODE_CAPACITY];
}LEVEL_ARRAY_NODE_S;

typedef struct {
    UINT total_count;
    LEVEL_ARRAY_NODE_S *root;
}LEVEL_ARRAY_S;

int LevelArray_Init(IN LEVEL_ARRAY_S *level_array);
void LevelArray_Fini(IN LEVEL_ARRAY_S *level_array);
int LevelArray_Set(IN LEVEL_ARRAY_S *level_array, IN UINT index, IN void *data);

#ifdef __cplusplus
}
#endif
#endif 
