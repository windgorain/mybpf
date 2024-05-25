/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _BSEARCH_UTL_H
#define _BSEARCH_UTL_H
#ifdef __cplusplus
extern "C"
{
#endif

void * BSEARCH_Do(void* base, int items_num, int ele_size, void *key, PF_CMP_FUNC cmp_func);

#ifdef __cplusplus
}
#endif
#endif 
