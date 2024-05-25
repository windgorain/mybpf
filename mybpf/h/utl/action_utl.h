/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2014-10-21
* Description: 
* History:     
******************************************************************************/

#ifndef __ACTION_UTL_H_
#define __ACTION_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

typedef VOID (*PF_ACTION_FUNC)(IN VOID *pUserContext);

typedef struct
{
    CHAR *pcActionString;
    UINT uiStringLen;
    PF_ACTION_FUNC pfAction;
}ACTION_S;

#define ACTION_LINE(pcString,pfAction) {(pcString), sizeof(pcString)-1, pfAction}
#define ACTION_END {NULL, 0, NULL}

#define ACTION_INDEX_NUM_MAX 32
#define ACTION_INDEX_INVALID 0xffff

typedef struct
{
    USHORT ausActionIndex[ACTION_INDEX_NUM_MAX];
}ACTION_INDEX_S;

VOID ACTION_Compile(ACTION_S *pstActionTbl, CHAR *pcActions, OUT ACTION_INDEX_S *pstActionIndex);

VOID ACTION_RunFast
(
    IN ACTION_S *pstActionTbl,
    IN ACTION_INDEX_S *pstActionIndex,
    IN VOID *pUserContext
);

VOID ACTION_Run
(
    IN ACTION_S *pstActionTbl,
    IN UINT uiActionTblCount,
    IN CHAR *pcActions,
    IN VOID *pUserContext
);

#ifdef __cplusplus
    }
#endif 

#endif 


