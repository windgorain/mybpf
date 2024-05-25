/*================================================================
*   Created by LiXingang: 2018.12.11
*   Description: 
*
================================================================*/
#ifndef _RSA_UTL_H
#define _RSA_UTL_H

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/evp.h>

#ifdef __cplusplus
extern "C"
{
#endif

EVP_PKEY * RSA_DftPrivateKey();
EVP_PKEY * RSA_DftPublicKey();

EVP_PKEY * EVP_BuildKey(int type, UINT bits);
EVP_PKEY * RSA_BuildKey(UINT bits);
EVP_PKEY * DSA_BuildKey(UINT bits);
EVP_PKEY * EC_BuildKey(UINT bits);

int RSA_PublicEncrypt(IN EVP_PKEY *key, IN void *in, int in_size, OUT void *out, int out_size);
int RSA_PrivateDecrypt(IN EVP_PKEY *key, IN void *in, int in_size, OUT void *out, int out_size);
int RSA_PrivateEncrypt(IN EVP_PKEY *key, IN void *in, int in_size, OUT void *out, int out_size);
int RSA_PublicDecrypt(IN EVP_PKEY *key, IN void *in, int in_size, OUT void *out, int out_size);

#ifdef __cplusplus
}
#endif
#endif 
