/*================================================================
*   Created by LiXingang: 2018.12.12
*   Description: 
*
================================================================*/
#ifndef _CPU_UTL_H
#define _CPU_UTL_H
#ifdef __cplusplus
extern "C"
{
#endif

int CPU_HighestFunction();
int CPU_GetID (char *id, int max);
int CPU_GetCompany(char *company, int size);
int CPU_GetBrand(char *cBrand, int size);
int CPU_GetBaseParam(char *baseparam, int size);
unsigned int CPU_GetModel();


#ifdef __cplusplus
}
#endif
#endif 
