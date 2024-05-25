/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2009-4-13
* Description: 
* History:     
******************************************************************************/

#ifndef __RPC_UTL_H_
#define __RPC_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 


#define RPC_MSG_TYPE_REQUEST    0
#define RPC_MSG_TYPE_RESPONSE   1
#define RPC_MSG_TYPE_ERRINFO    2


#define RPC_PARAM_TYPE_VOID         0   
#define RPC_PARAM_TYPE_UINT32        1
#define RPC_PARAM_TYPE_STRING       2


#define RPC_RETURN_TYPE_VOID        0
#define RPC_RETURN_TYPE_UINT32       1
#define RPC_RETURN_TYPE_STRING      2
#define RPC_RETURN_TYPE_BOOL        3


#define _RPC_MAX_PARAM_NUM  16


typedef struct
{
    UCHAR ucType;           
    UCHAR ucIsMemMalloc;    
    UCHAR ucReserved1;
    UCHAR ucReserved2;
    UINT ulParamLen;       
    UCHAR *pucParam;        
}RPC_PARAM_NODE_S;


typedef struct
{
    UCHAR ucMsgType;            
    UCHAR ucReturnType;         
    UCHAR ucIsMemMalloc;        
    UCHAR ucReserved1;
    UINT ulRpcHeadDataLen;        
    UCHAR *pucRpcHeadDataValue;         
    UINT ulParamNum;           
    RPC_PARAM_NODE_S astParams[_RPC_MAX_PARAM_NUM];
}RPC_MSG_S;

RPC_MSG_S * RPC_CreateMsg(IN UCHAR ucMsgType);
VOID RPC_FreeMsg(IN RPC_MSG_S * pstMsg);
UCHAR RPC_GetMsgType(IN RPC_MSG_S *pstMsg);
BS_STATUS RPC_SetHeadDataValue(IN RPC_MSG_S *pstMsg, IN VOID *pData, IN UINT ulDataLen);
UCHAR * RPC_GetHeadDataValue(IN RPC_MSG_S *pstMsg);
BS_STATUS RPC_SetReturnType(IN RPC_MSG_S *pstMsg, IN UCHAR ucType);
UCHAR RPC_GetReturnType(IN RPC_MSG_S *pstMsg);
UINT RPC_GetParamNum(IN RPC_MSG_S *pstMsg);
BS_STATUS RPC_AddParam(IN RPC_MSG_S *pstMsg, IN UCHAR ucParamType, IN UINT ulParamLen, IN VOID *pParam);
BS_STATUS RPC_GetParamByIndex
(
    IN RPC_MSG_S *pstMsg,
    IN UINT ulIndex,
    OUT UCHAR *pucParamType,
    OUT UINT *pulParamLen,
    OUT VOID **ppParam
);
UCHAR * RPC_CreateDataByMsg(IN RPC_MSG_S *pstMsg);
RPC_MSG_S * RPC_CreateMsgByData(IN UCHAR *pucData);
UINT RPC_GetTotalSizeOfData(IN UCHAR *pucData);
VOID RPC_FreeData(IN UCHAR *pucData);



HANDLE RPCC_UDP_Create(IN UINT ulIp, IN USHORT usPort);

RPC_MSG_S * RPCC_Send(IN HANDLE hRpcHandle, IN RPC_MSG_S *pstRpcMsg);


#ifdef __cplusplus
    }
#endif 

#endif 


