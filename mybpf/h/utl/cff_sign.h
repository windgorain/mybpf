/*================================================================
*   Created by LiXingang: 2018.12.12
*   Description: 
*
================================================================*/
#ifndef _CFF_SIGN_H
#define _CFF_SIGN_H
#include "utl/rsa_utl.h"
#ifdef __cplusplus
extern "C"
{
#endif


#define CFF_SIGN_FLAG_INCLUDE_TAGNAME 0x1 

int CFFSign_PrivateSign(IN CFF_HANDLE hCff, IN char *tag_name, IN void *pri_key, UINT flag);
int CFFSign_PublicVerify(CFF_HANDLE hCff, IN char *tag_name, IN EVP_PKEY *pub_key, UINT flag);
int CFFSign_PublicSign(IN CFF_HANDLE hCff, IN char *tag_name, IN void *pub_key, UINT flag);
int CFFSign_PrivateVerify(CFF_HANDLE hCff, IN char *tag_name, IN void *pri_key, UINT flag);

#ifdef __cplusplus
}
#endif
#endif 
