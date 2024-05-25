/*================================================================
*   Created by LiXingang: 2018.12.11
*   Description: 
*
================================================================*/
#ifndef _LICENSE_UTL_H
#define _LICENSE_UTL_H
#include "utl/cff_utl.h"
#include "utl/rsa_utl.h"
#ifdef __cplusplus
extern "C"
{
#endif

typedef void (*PF_LICENSE_PRINT_FUNC)(IN char *str, IN void *user_data);

int LICENSE_Init(IN CFF_HANDLE hCff);
BS_STATUS LICENSE_X_SetModule(IN CFF_HANDLE hCff, IN char *lic_index, IN char *module);
BS_STATUS LICENSE_X_SetHostID(IN CFF_HANDLE hCff, IN char *lic_index, IN char *id);
BS_STATUS LICENSE_X_EnableFeature(IN CFF_HANDLE hCff, IN char *lic_index, IN char *feature);
BS_STATUS LICENSE_X_SetFeatureLimition(IN CFF_HANDLE hCff, IN char *lic_index, IN char *feature, IN UINT limit);
BS_STATUS LICENSE_X_SetCreateTime(IN CFF_HANDLE hCff, IN char *lic_index);
BS_STATUS LICENSE_X_SetExpire(IN CFF_HANDLE hCff, IN char *lic_index, IN UINT64 expire);
BS_STATUS LICENSE_X_SetVendor(IN CFF_HANDLE hCff, IN char *lic_index, IN char *vendor);
BS_STATUS LICENSE_X_SetExpireFromNow(IN CFF_HANDLE hCff, IN char *lic_index, IN UINT64 seconds_from_now);
BS_STATUS LICENSE_X_SetKeyValue(IN CFF_HANDLE hCff, IN char *lic_index, IN char *key, IN char *value);

int LICENSE_X_Sign(IN CFF_HANDLE hCff, IN char *lic_index, IN void *pri_key);

int LICENSE_X_VerifySignature(IN CFF_HANDLE hCff, IN char *lic_index, IN EVP_PKEY *pub_key);
BOOL_T LICENSE_X_CheckHostID(IN CFF_HANDLE hCff, IN char *lic_index, IN char *hostid);
char * LICENSE_X_GetModule(IN CFF_HANDLE hCff, IN char *lic_index);
char * LICENSE_X_GetHostID(IN CFF_HANDLE hCff, IN char *lic_index);
char *LICENSE_X_GetVendor(IN CFF_HANDLE hCff, IN char *lic_index);

BOOL_T LICENSE_X_IsEnabled(IN CFF_HANDLE hCff, IN char *lic_index, IN char *feature);

typedef enum {
    LICENSE_VERIFY_OK = 0,
    LICENSE_VERIFY_MODULE_NOT_EXIST = -1,
    LICENSE_VERIFY_MODULE_NOT_MATCH = -2,
    LICENSE_VERIFY_EXPIRED=-3,
    LICENSE_VERIFY_SIGNATURE_FAILED=-4
}LICENSE_VERIFY_RET;

LICENSE_VERIFY_RET LICENSE_X_Verify(CFF_HANDLE hCff, char *lic_index, char *module, EVP_PKEY *pub_key);

UINT LICENSE_X_GetLimition(IN CFF_HANDLE hCff, IN char *lic_index, IN char *feature);
UINT64 LICENSE_X_GetCreateTime(IN CFF_HANDLE hCff, IN char *lic_index);
UINT64 LICENSE_X_GetExpireTime(IN CFF_HANDLE hCff, IN char *lic_index);
char * LICENSE_X_GetDescription(IN CFF_HANDLE hCff, IN char *lic_index);
char * LICENSE_X_GetTip(IN CFF_HANDLE hCff, IN char *lic_index);

BOOL_T LICENSE_X_IsExpired(IN CFF_HANDLE hCff, IN char *lic_index);
char * LICENSE_X_GetKeyValue(IN CFF_HANDLE hCff, IN char *lic_index, IN char *key);


char * LICENSE_VerifyResultInfo(IN int result);
BOOL_T LICENSE_IsEnabled(CFF_HANDLE hCff, char *module, char *hostid, char *feature, EVP_PKEY *pub_key);
void LICENSE_ShowTip(IN CFF_HANDLE hCff, IN PF_LICENSE_PRINT_FUNC print_func, IN void *user_data);

#ifdef __cplusplus
}
#endif
#endif 
