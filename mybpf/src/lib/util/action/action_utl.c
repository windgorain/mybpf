/******************************************************************************
* Copyright (C), LiXingang
* Author:      LiXingang  Version: 1.0  Date: 2014-10-21
* Description: 
* History:     
******************************************************************************/
#include "bs.h"

#include "utl/lstr_utl.h"
#include "utl/action_utl.h"

static USHORT action_Find
(
    IN ACTION_S *pstActionTbl,
    IN CHAR *pcActionString,
    IN UINT uiActionStringLen
)
{
    UINT index = 0;
    LSTR_S stString;
    ACTION_S *act;

    stString.pcData = pcActionString;
    stString.uiLen = uiActionStringLen;

    LSTR_Strim(&stString, " \t\n\r", &stString);

    if (stString.uiLen == 0) {
        return ACTION_INDEX_INVALID;
    }

    for (act=pstActionTbl; act->pcActionString != NULL; act++,index++) {
        if ((act->uiStringLen == stString.uiLen)
            && (0 == LSTR_StrCmp(&stString, act->pcActionString))) {
            return index;
        }
    }

    BS_DBGASSERT(0);

    return ACTION_INDEX_INVALID;
}

VOID ACTION_Compile(ACTION_S *pstActionTbl, CHAR *pcActions, OUT ACTION_INDEX_S *pstActionIndex)
{
    CHAR *pcAction;
    CHAR *pcActionNext;
    CHAR *pcEnd;
    UINT uiActionStringLen;
    USHORT usIndex;
    UINT uiPos = 0;

    pcAction = pcActions;

    while (pcAction != NULL)
    {
        if (uiPos >= ACTION_INDEX_NUM_MAX)
        {
            BS_DBGASSERT(0);
        }
        
        pcEnd = strchr(pcAction, ',');
        if (NULL == pcEnd)
        {
            uiActionStringLen = strlen(pcAction);
            pcActionNext = NULL;
        }
        else
        {
            uiActionStringLen = pcEnd - pcAction;
            pcActionNext = pcEnd + 1;
        }

        usIndex = action_Find(pstActionTbl, pcAction, uiActionStringLen);
        if (usIndex != ACTION_INDEX_INVALID) {
            pstActionIndex->ausActionIndex[uiPos] = usIndex;
            uiPos ++;
        }

        pcAction = pcActionNext;
    }

    if (uiPos < ACTION_INDEX_NUM_MAX)
    {
        pstActionIndex->ausActionIndex[uiPos] = ACTION_INDEX_INVALID;
    }
}

VOID ACTION_RunFast
(
    IN ACTION_S *pstActionTbl,
    IN ACTION_INDEX_S *pstActionIndex,
    IN VOID *pUserContext
)
{
    UINT i;
    USHORT usIndex;

    for (i=0; i<ACTION_INDEX_NUM_MAX; i++)
    {
        usIndex = pstActionIndex->ausActionIndex[i];
        if (usIndex == ACTION_INDEX_INVALID)
        {
            break;
        }

        pstActionTbl[usIndex].pfAction(pUserContext);
    }
}

VOID ACTION_Run
(
    IN ACTION_S *pstActionTbl,
    IN UINT uiActionTblCount,
    IN CHAR *pcActions,
    IN VOID *pUserContext
)
{
    
    ACTION_INDEX_S stActionIndex;

    ACTION_Compile(pstActionTbl, pcActions, &stActionIndex);

    ACTION_RunFast(pstActionTbl, &stActionIndex, pUserContext);
}


