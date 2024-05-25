/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2007-8-10
* Description: 
* History:     
******************************************************************************/

#ifndef __PASSWD_UTL_H_
#define __PASSWD_UTL_H_

#include "utl/md5_utl.h"
#include "utl/aes_utl.h"
#include "utl/base64_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 

#if 1
#define PW_BASE64_CIPHER_LEN(ulClearLen) BASE64_LEN(AES_CIPHER_LEN(ulClearLen))
BS_STATUS PW_Base64Encrypt(IN CHAR *szClearText, OUT CHAR *szCipher, IN ULONG ulCipherSize);
BS_STATUS PW_Base64Decrypt(IN CHAR *szCipher, OUT CHAR *szClearText, IN ULONG ulClearSize);
#endif

#if 1
#define PW_HEX_CIPHER_LEN(ulClearLen) AES_CIPHER_LEN((ulClearLen) * 2)
BS_STATUS PW_HexEncrypt(IN CHAR *szClearText, OUT CHAR *szCipher, IN ULONG ulCipherSize);
BS_STATUS PW_HexDecrypt(IN CHAR *szCipher, OUT CHAR *szClearText, IN ULONG ulClearSize);
#endif

#if 1
#define PW_MD5_ENCRYPT_LEN (64)

BS_STATUS PW_Md5Encrypt(IN CHAR *szClearText, OUT CHAR *szCipherText, int cipher_text_size);

BOOL_T PW_Md5Check(IN CHAR *szClearText, IN CHAR *pcCipherText);
#endif

#ifdef __cplusplus
    }
#endif 

#endif 


