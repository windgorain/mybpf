/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _QSORT_UTL_H
#define _QSORT_UTL_H
#ifdef __cplusplus
extern "C"
{
#endif

void QSORT_Do(void *base, int num, int width, PF_CMP_FUNC cmp_func);

#ifdef __cplusplus
}
#endif
#endif 
