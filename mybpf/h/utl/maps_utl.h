/*================================================================
*   Created by LiXingang
*   Description: map 合集, 里面有多张map表,用id表示是哪张
*
================================================================*/
#ifndef _MAP_S_H
#define _MAP_S_H
#ifdef __cplusplus
extern "C"
{
#endif

void * MAPS_Create(int map_max_num);
int MAPS_Add(void *maps, int id, void *key, int key_len, void *value, UINT flag);
void * MAPS_Del(void *maps, int id, void *key, int key_len);
void * MAPS_Get(void *maps, int id, void *key, int key_len);

#ifdef __cplusplus
}
#endif
#endif 
