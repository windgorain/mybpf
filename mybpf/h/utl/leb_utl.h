/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _LEB_UTL_H
#define _LEB_UTL_H
#ifdef __cplusplus
extern "C"
{
#endif


UINT64 LEB_Read(UCHAR *bytes, UINT *pos);


UINT64 LEB_ReadSigned(UCHAR *bytes, UINT *pos);



#ifdef __cplusplus
}
#endif
#endif 
