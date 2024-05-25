/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2010-6-15
* Description: 
* History:     
******************************************************************************/

#ifndef __FCGI_MNG_H_
#define __FCGI_MNG_H_

#ifdef __cplusplus
    extern "C" {
#endif 


typedef struct
{
    CHAR *pszUrlPattern;   

    CHAR *pszFcgiAppFilePath; 
    UINT uiSpwanNum;
    
    CHAR *pszFcgiAppAddress;  
    USHORT usFcgiAppPort;     
}FCGIM_SERVICE_S;

HANDLE FCGIM_CreateInstance();
VOID FCGIM_DeleteInstance(IN HANDLE hFcgiInstance);
BS_STATUS FCGIM_AddService(IN HANDLE hFcgiInstance, IN FCGIM_SERVICE_S *pstService);
FCGIM_SERVICE_S * FCGIM_MatchService(IN HANDLE hFcgiInstance, IN CHAR *pszUrl);


VOID FCGIM_Display(IN HANDLE hFcgiInstance);

#ifdef __cplusplus
    }
#endif 

#endif 


