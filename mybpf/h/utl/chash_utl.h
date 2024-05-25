/*================================================================
*   Created by LiXingang
*   Description: 一致性hash
*
================================================================*/
#ifndef _CHASH_UTL_H
#define _CHASH_UTL_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef void* CHASH_HDL;

CHASH_HDL CHASH_Create();
int CHASH_AddNode(CHASH_HDL hCtrl, void *key, int key_len, int vcount, void *node);
int CHASH_DelNode(CHASH_HDL hCtrl, void *key, int key_len, int vcount, void *node);
void * CHASH_GetNode(CHASH_HDL hCtrl, UINT hash);

#ifdef __cplusplus
}
#endif
#endif 
