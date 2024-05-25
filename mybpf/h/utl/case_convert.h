/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _CASE_CONVERT_H
#define _CASE_CONVERT_H
#ifdef __cplusplus
extern "C"
{
#endif

void CaseConvert_Init();
void CaseConvert_ToUpper(UCHAR *dst, UCHAR *src, int len);
void CaseConvert_ToLower(UCHAR *dst, UCHAR *src, int len);

#ifdef __cplusplus
}
#endif
#endif 
