/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2014-10-9
* Description: 
* History:     
******************************************************************************/
#include "bs.h"

#include "utl/txt_utl.h"
#include "utl/bit_opt.h"
#include "utl/dfa_utl.h"

static BOOL_T dfa_IsMatchExt(IN UINT uiInputCode, IN UINT uiParttenCode)
{
    BOOL_T bMatch = FALSE;
    UCHAR ucInputCode;

    if (uiInputCode > DFA_CODE_CHAR_MAX)
    {
        return FALSE;
    }

    ucInputCode = (UCHAR)uiInputCode;

    switch (uiParttenCode)
    {
        case DFA_CODE_LWS:
        {
            bMatch = DFA_IsLws(ucInputCode);
            break;
        }

        case DFA_CODE_ALPHA:
        {
            if (isalpha(ucInputCode))
            {
                bMatch = TRUE;
            }

            break;
        }

        case DFA_CODE_NUMBER:
        {
            if (isdigit(ucInputCode))
            {
                bMatch = TRUE;
            }

            break;
        }

        case DFA_CODE_HEX:   
        {
            if (isxdigit(ucInputCode))
            {
                bMatch = TRUE;
            }

            break;
        }

        case DFA_CODE_OTHER:
        {
            bMatch = TRUE;
            break;
        }

        case DFA_CODE_WORD:
        {
            if (isalpha(ucInputCode) || (isdigit(ucInputCode)) || (ucInputCode == '_'))
            {
                bMatch = TRUE;
            }
            break;
        }

        default:
        {
            BS_DBGASSERT(0);
            break;
        }
    }

    return bMatch;
}

static BOOL_T dfa_IsMatch(IN UINT uiInputCode, IN UINT uiParttenCode)
{
    BOOL_T bMatch = FALSE;

    if (uiParttenCode <= DFA_CODE_MAX)
    {
        if (uiInputCode == uiParttenCode)
        {
            bMatch = TRUE;
        }
    }
    else
    {
        bMatch = dfa_IsMatchExt(uiInputCode, uiParttenCode);
    }

    return bMatch;
}

static VOID dfa_ProcessCode
(
    IN DFA_S *pstDfa,
    IN UINT uiInputCode,
    IN DFA_NODE_S *pstDfaNode
)
{
    UINT uiNextState;

    uiNextState = pstDfaNode->uiNextState;

    if (uiNextState == DFA_STATE_SELF)
    {
        uiNextState = pstDfa->uiCurrentState;
    }

    pstDfa->uiOldState = pstDfa->uiCurrentState;
    pstDfa->uiCurrentState = uiNextState;
    pstDfa->uiInputCode = uiInputCode;

    BS_DBG_OUTPUT(pstDfa->uiDbgFlag, DFA_DBG_FLAG_PROCESS,
        ("Input:%d, State %d to %d, action:%s\r\n",
        uiInputCode, pstDfa->uiOldState, uiNextState, TXT_GET_SELF_OR_BLANK(pstDfaNode->pcActions)));

    ACTION_RunFast(pstDfa->pstActions, &pstDfaNode->stActionIndex, (VOID*)pstDfa);
}

VOID DFA_Compile(DFA_TBL_LINE_S *pstDfaLines, ACTION_S *pstActions)
{
    int j;
    DFA_TBL_LINE_S *line;
    DFA_NODE_S *pstNode;

    for (line=pstDfaLines; line->pstDfaNode !=NULL; line++) {
        for (j=0; j<line->uiDfaNodeNum; j++) {
            pstNode = &line->pstDfaNode[j];
            ACTION_Compile(pstActions, pstNode->pcActions, &pstNode->stActionIndex);
        }
    }
}

void DFA_Init(DFA_S *dfa, DFA_TBL_LINE_S *pstDfaLines, ACTION_S *pstActions, UINT uiInitState)
{
    memset(dfa, 0, sizeof(DFA_S));
    dfa->pstDfaLines = pstDfaLines;
    dfa->pstActions = pstActions;
    dfa->uiCurrentState = uiInitState;
}

DFA_HANDLE DFA_Create(DFA_TBL_LINE_S *pstDfaLines, ACTION_S *pstActions, UINT uiInitState)
{
    DFA_S *pstDfa;

    pstDfa = MEM_Malloc(sizeof(DFA_S));
    if (NULL == pstDfa) {
        return NULL;
    }

    DFA_Init(pstDfa, pstDfaLines, pstActions, uiInitState);

    return pstDfa;
}

VOID DFA_Destory(IN DFA_HANDLE hDfa)
{
    DFA_S *pstDfa = hDfa;

    MEM_Free(pstDfa);
}

VOID DFA_SetDbgFlag(IN DFA_HANDLE hDfa, IN UINT uiDbgFlag)
{
    DFA_S *pstDfa = hDfa;

    pstDfa->uiDbgFlag |= uiDbgFlag;
}

VOID DFA_ClrDbgFlag(IN DFA_HANDLE hDfa, IN UINT uiDbgFlag)
{
    DFA_S *pstDfa = hDfa;

    BIT_CLR(pstDfa->uiDbgFlag, uiDbgFlag);
}

VOID DFA_Input(IN DFA_HANDLE hDfa, IN UINT uiInputCode)
{
    DFA_S *pstDfa = hDfa;
    UINT uiIndex;
    DFA_NODE_S *pstDfaNode;
    UINT uiDfaNodeNum;

    BS_DBG_OUTPUT(pstDfa->uiDbgFlag, DFA_DBG_FLAG_PROCESS, ("Input:%c\r\n", uiInputCode));

    pstDfaNode = pstDfa->pstDfaLines[pstDfa->uiCurrentState].pstDfaNode;
    uiDfaNodeNum = pstDfa->pstDfaLines[pstDfa->uiCurrentState].uiDfaNodeNum;

    for (uiIndex = 0; uiIndex<uiDfaNodeNum; uiIndex++)
    {
        if (TRUE == dfa_IsMatch(uiInputCode, pstDfaNode[uiIndex].uiCode))
        {
            dfa_ProcessCode(pstDfa, uiInputCode, &pstDfaNode[uiIndex]);
            break;
        }
    }
}

VOID DFA_InputChar(IN DFA_HANDLE hDfa, IN CHAR cInputCode)
{
    DFA_S *pstDfa = hDfa;

    DFA_Input(pstDfa, (UINT)(UCHAR)cInputCode);
}

VOID DFA_Edge(IN DFA_HANDLE hDfa)
{
    DFA_S *pstDfa = hDfa;

    DFA_Input(pstDfa, DFA_CODE_EDGE);
}

VOID DFA_End(IN DFA_HANDLE hDfa)
{
    DFA_S *pstDfa = hDfa;

    DFA_Input(pstDfa, DFA_CODE_END);
}

VOID DFA_SetState(IN DFA_HANDLE hDfa, IN UINT uiNewState)
{
    DFA_S *pstDfa = hDfa;

    BS_DBG_OUTPUT(pstDfa->uiDbgFlag, DFA_DBG_FLAG_PROCESS, ("Set state from %d to %d\r\n", pstDfa->uiCurrentState, uiNewState));

    pstDfa->uiCurrentState = uiNewState;
}

UINT DFA_GetInputCode(IN DFA_HANDLE hDfa)
{
    DFA_S *pstDfa = hDfa;

    return pstDfa->uiInputCode;
}

UINT DFA_GetOldState(IN DFA_HANDLE hDfa)
{
    DFA_S *pstDfa = hDfa;

    return pstDfa->uiOldState;
}

VOID DFA_SetUserData(IN DFA_HANDLE hDfa, IN VOID *pData)
{
    DFA_S *pstDfa = hDfa;

    pstDfa->pUserData= pData;

    return;
}

VOID * DFA_GetUserData(IN DFA_HANDLE hDfa)
{
    DFA_S *pstDfa = hDfa;

    return pstDfa->pUserData;
}

BOOL_T DFA_IsWord(IN UCHAR ucInputCode)
{
    BOOL_T bRet = FALSE;
    
    if (isalpha(ucInputCode) || (isdigit(ucInputCode)) || (ucInputCode == '_'))
    {
        bRet = TRUE;
    }

    return bRet;
}

BOOL_T DFA_IsLws(IN UCHAR ucInputCode)
{
    if ((ucInputCode == ' ')
        || (ucInputCode == '\t')
        || (ucInputCode == '\r')
        || (ucInputCode == '\n')
        || (ucInputCode == '\f')
        || (ucInputCode == '\v'))
    {
        return TRUE;
    }

    return FALSE;
}

