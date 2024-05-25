/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2007-8-28
* Description: 
* History:     
******************************************************************************/

#ifndef __VNIC_LIB_H_
#define __VNIC_LIB_H_

#ifdef __cplusplus
    extern "C" {
#endif 

#define VNIC_MAX_MTU      1500
#define VNIC_MAX_NAME_LEN 256

typedef HANDLE VNIC_HANDLE;

extern VNIC_HANDLE VNIC_Create (IN UCHAR *pcVnicFilePath);
extern BS_STATUS VNIC_Delete (IN VNIC_HANDLE hVnic);
extern BS_STATUS VNIC_SetAdapterGuid(IN VNIC_HANDLE hVnic, IN CHAR *pcGuid);
extern CHAR * VNIC_GetAdapterGuid(IN VNIC_HANDLE hVnic);


extern BS_STATUS VNIC_Read
(
    IN VNIC_HANDLE hVnic, 
    IN BS_WAIT_E eMode,     
    IN UINT ulTime,
    OUT UCHAR *pucData, 
    IN UINT ulDataLen, 
    OUT UINT *pulReadLen
);
extern BS_STATUS VNIC_Write (IN VNIC_HANDLE hVnic, OUT UCHAR *pucData, IN UINT ulDataLen, OUT UINT *pulWriteLen);
extern BS_STATUS VNIC_Signale (IN VNIC_HANDLE hVnic);
extern BS_STATUS VNIC_Ioctl
(
    IN VNIC_HANDLE hVnic,
    IN UINT ulCmd,
    IN UCHAR *pucInBuf,
    IN UINT ulInBufLen,
    OUT UCHAR *pucOutBuf,
    IN UINT ulOutBufLen,
    OUT UINT *puiOutBufLen
);

extern BS_STATUS VNIC_AddIP (IN VNIC_HANDLE hVnic, IN UINT ulIp, IN UINT ulMask);

BS_STATUS VNIC_GetIP(IN VNIC_HANDLE hVnic, OUT UINT *puiIp, OUT UINT *puiMask);
BS_STATUS VNIC_AddDns(IN VNIC_HANDLE hVnic, IN UINT uiDns, IN UINT uiIndex);
BS_STATUS VNIC_SetDns(IN VNIC_HANDLE hVnic, IN UINT uiDns);
BS_STATUS VNIC_DelDns(IN VNIC_HANDLE hVnic, IN UINT uiDns);
BS_STATUS VNIC_GetAdapterIndex(IN VNIC_HANDLE hVnic, OUT UINT *puiIndex);


#ifdef __cplusplus
    }
#endif 

#endif 


