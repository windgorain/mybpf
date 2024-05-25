/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _SKIP_LIST_H
#define _SKIP_LIST_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef HANDLE SKL_HDL;

SKL_HDL SKL_Create(int max_level);
void SKL_Reset(SKL_HDL hSkl);
void SKL_Destroy(SKL_HDL hSkl);
void * SKL_GetFirst(SKL_HDL hSkl);
void * SKL_GetLast(SKL_HDL hSkl);

void * SKL_GetInLeft(SKL_HDL hSkl, UINT key);

void * SKL_GetInRight(SKL_HDL hSkl, UINT key);

void * SKL_GetLeft(SKL_HDL hSkl, UINT key);

void * SKL_GetRight(SKL_HDL hSkl, UINT key);
void * SKL_Search(SKL_HDL hSkl, UINT key);
int SKL_Insert(SKL_HDL hSkl, UINT key, void *value);
void * SKL_Delete(SKL_HDL hSkl, UINT key);

#ifdef __cplusplus
}
#endif
#endif 
