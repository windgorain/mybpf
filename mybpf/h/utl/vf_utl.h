/******************************************************************************
* Copyright (C) LiXingang
* Author:      LiXingang  Version: 1.0  Date: 2013-1-22
* Description: 
* History:     
******************************************************************************/

#ifndef __VF_UTL_H_
#define __VF_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

#define VF_MAX_NAME_LEN 127

enum {
    VF_EVENT_CREATE_VF = 0,
    VF_EVENT_DESTORY_VF,
    VF_EVENT_TIMER,

    VF_EVENT_MAX
};

#define VF_INVALID_USER_INDEX 0xffffffff
#define VF_INVALID_VF 0

typedef HANDLE VF_HANDLE;

typedef struct
{
    CHAR szVfName[VF_MAX_NAME_LEN + 1];
    UINT uiVFID;
    VOID *apUserData[0];
}VF_P_S;

typedef struct {
    void *memcap;
    UINT uiMaxVD;
    UINT bCreateLock:1;
}VF_PARAM_S;

typedef BS_STATUS (*PF_VF_EVENT_FUNC)(IN UINT uiEvent, IN VF_P_S *param, IN USER_HANDLE_S *pstUserHandle);

VF_HANDLE VF_Create(VF_PARAM_S *p);

UINT VF_RegEventListener
(
    IN VF_HANDLE hVf,
    IN UINT uiPriority,
    IN PF_VF_EVENT_FUNC pfEventFunc,
    IN USER_HANDLE_S *pstUserHandle
);

UINT VF_CreateVF(IN VF_HANDLE hVf, IN CHAR *pcVfName);
VOID VF_DestoryVF(IN VF_HANDLE hVf, IN UINT uiVFID);
VOID VF_SetData(IN VF_HANDLE hVf, IN UINT uiVFID, IN UINT uiUserDataIndex, IN VOID *pData);
VOID * VF_GetData(IN VF_HANDLE hVf, IN UINT uiVFID, IN UINT uiUserDataIndex);
UINT VF_GetIDByName(IN VF_HANDLE hVf, IN CHAR *pcName);
CHAR * VF_GetNameByID(IN VF_HANDLE hVf, IN UINT uiID);
VOID VF_TimerStep(IN VF_HANDLE hVf);
CHAR * VF_GetEventName(IN UINT uiEvent);
UINT VF_GetNext(IN VF_HANDLE hVf, IN UINT uiCurrent);


#ifdef __cplusplus
    }
#endif 

#endif 


