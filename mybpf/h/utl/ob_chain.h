/******************************************************************************
* Copyright (C) LiXingang
* Author:      LiXingang  Version: 1.0  Date: 2012-11-3
* Description: Observer chain
* History:     
******************************************************************************/

#ifndef __OB_CHAIN_H_
#define __OB_CHAIN_H_

#ifdef __cplusplus
    extern "C" {
#endif 

typedef DLL_HEAD_S OB_CHAIN_S;
#define OB_CHAIN_HEAD_INIT_VALUE DLL_HEAD_INIT_VALUE
#define OB_CHAIN_INIT            DLL_INIT

typedef struct
{
    DLL_NODE_S    stDllNode;   
    UINT uiPri;                
    void *pfFunc;
    USER_HANDLE_S stUserHandle;
}OB_CHAIN_NODE_S;

#define OB_CHAIN_SCAN_BEGIN(_pstObChainHead,_pfFunc,_pstUserHandle)   \
    do{     \
        OB_CHAIN_NODE_S *_pstNode; \
        DLL_SCAN ((_pstObChainHead), _pstNode)  { \
            (_pfFunc) = (VOID*) _pstNode->pfFunc;  \
            (_pstUserHandle) = &(_pstNode->stUserHandle);

#define OB_CHAIN_SCAN_END()  }}while(0)

#define OB_CHAIN_NOTIFY(_pstObChainHead, _func_type, ...) \
    do { \
        _func_type _pfFunc;    \
        USER_HANDLE_S *_pstUserHandle;   \
        OB_CHAIN_SCAN_BEGIN(_pstObChainHead, _pfFunc, _pstUserHandle) { \
            _pfFunc(__VA_ARGS__, _pstUserHandle);    \
        }OB_CHAIN_SCAN_END();    \
    }while(0)

BS_STATUS OB_CHAIN_AddWithPri
(
    IN OB_CHAIN_S *pstHead,
    IN UINT uiPri,
    IN void *pfFunc,
    IN USER_HANDLE_S *pstUserHandle
);
BS_STATUS OB_CHAIN_Add(IN OB_CHAIN_S *pstHead, IN void *pfFunc, IN USER_HANDLE_S *pstUserHandle);
BS_STATUS OB_CHAIN_Del(IN OB_CHAIN_S *pstHead, IN void *pfFunc);
BS_STATUS OB_CHAIN_DelAll(IN OB_CHAIN_S *pstHead);

#ifdef __cplusplus
    }
#endif 

#endif 


