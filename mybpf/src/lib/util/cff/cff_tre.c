/******************************************************************************
* Copyright (C), LiXingang
* Author:      LiXingang  Version: 1.0  Date: 2015-6-24
* Description: 
* History:     
******************************************************************************/
#include "bs.h"

#include "utl/txt_utl.h"
#include "utl/cff_utl.h"

#include "cff_inner.h"

typedef enum
{
    CFF_TRE_LINE_TYPE_SECTION = 0,
    CFF_TRE_LINE_TYPE_PROP
}CFF_TRE_LINE_TYPE_E;

static CFF_TRE_LINE_TYPE_E cff_tre_GetLineType(IN CHAR *pucLine)
{
    if (*pucLine == '[')
    {
        return CFF_TRE_LINE_TYPE_SECTION;
    }

    return CFF_TRE_LINE_TYPE_PROP;
}

static BOOL_T cff_tre_CheckLineValid(CHAR *pucLine)
{
    BS_DBGASSERT(NULL != pucLine);

    if (*pucLine == '[')
    {
        if (strchr(pucLine, ']') == NULL)
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }

    if (strchr(pucLine, '=') == NULL)
    {
        return FALSE;
    }

    return TRUE;
}

static UINT cff_tre_GetSectionLineLevel(CHAR *pszLine)
{
    CHAR *pcSplit;
    UINT ulLevel;

    pcSplit = strchr(pszLine, ']');
    if (NULL == pcSplit)
    {
        BS_DBGASSERT(0);
        return 0;
    }

    pcSplit ++;

    if (pcSplit[0] == '\0')
    {
        return 1;
    }

    if (BS_OK != TXT_Atoui(pcSplit, &ulLevel))
    {
        BS_DBGASSERT(0);
        return 0;
    }

    return ulLevel;
}

static VOID cff_tre_ProcPropLine(IN _CFF_S *pstCff, IN MKV_MARK_S *pstSection, IN CHAR *pcLine)
{
    CHAR *pcProp = pcLine;
    CHAR *pcValue = NULL;
    CHAR *pcSplit;
    BOOL_T bSort = FALSE;

    if (NULL == pstSection)
    {
        return;
    }

    pcSplit = strchr(pcLine, '=');
    if (NULL == pcSplit)
    {
        pcValue = "";
    }
    else
    {
        *pcSplit = '\0';
        pcValue = pcSplit + 1;
    }

    TXT_StrimAndMove(pcProp);
    TXT_StrimAndMove(pcValue);

    if (pcProp[0] == '\0')
    {
        return;
    }

    if (pstCff->uiFlag & CFF_FLAG_SORT)
    {
        bSort = TRUE;
    }

    MKV_AddNewKey2Mark(pstSection, pcProp, pcValue, FALSE, bSort);
}

static MKV_MARK_S * cff_tre_ProcSectionLine(IN _CFF_S *pstCff, IN CHAR *pcLine)
{
    CHAR *pcSplit;
    UINT uiLevel;
    MKV_MARK_S *pstSecCurrent;

    
    uiLevel = cff_tre_GetSectionLineLevel(pcLine);
    if (0 == uiLevel)
    {
        return NULL;
    }

    
    pcLine++;
    pcSplit = strchr(pcLine, ']');
    BS_DBGASSERT(NULL != pcSplit);
    *pcSplit = '\0';

    TXT_StrimAndMove(pcLine);
    if (pcLine[0] == '\0')
    {
        return NULL;
    }

    pstSecCurrent = MKV_GetLastMarkOfLevel(&pstCff->stCfgRoot, uiLevel - 1);
    if (NULL == pstSecCurrent)
    {
        return NULL;
    }

    return MKV_AddMark2Mark(pstSecCurrent, pcLine, FALSE);
}

static VOID cff_tre_Init(IN _CFF_S *pstCff)
{
    CHAR *pcLine;
    UINT uiLineLen;
    MKV_MARK_S *pstSecCurrent = NULL;
    
    TXT_SCAN_LINE_BEGIN(pstCff->pcFileContent, pcLine, uiLineLen)
    {
        pcLine[uiLineLen] = '\0';
        TXT_StrimAndMove(pcLine);

        if (pcLine[0] == '#') {
            continue;
        }

        if ((pcLine[0] == '\0') || (TRUE != cff_tre_CheckLineValid(pcLine)))
        {
            continue;
        }

        if (CFF_TRE_LINE_TYPE_SECTION == cff_tre_GetLineType(pcLine))
        {
            pstSecCurrent = cff_tre_ProcSectionLine(pstCff, pcLine);
        }
        else
        {
            cff_tre_ProcPropLine(pstCff, pstSecCurrent, pcLine);
        }
    }TXT_SCAN_LINE_END();

    if (pstCff->uiFlag & CFF_FLAG_SORT)
    {
        MKV_SortMark(&pstCff->stCfgRoot);
    }
}

static VOID cff_tre_SaveTag(IN MKV_MARK_S *pstMark, IN UINT uiLevle, IN PF_CFF_SAVE_CB pfFunc, IN VOID *pUserData)
{
    MKV_KEY_S  *pstKey;
    MKV_MARK_S  *pstSecTmp;
    UINT i;
    char buf[1024];

    if (uiLevle == 1) {
        snprintf(buf, sizeof(buf), "[%s]\r\n", pstMark->pucMarkName);
        pfFunc(buf, pUserData);
    } else {
        for (i=1; i<uiLevle; i++) {
            pfFunc("   ", pUserData);
        }
        snprintf(buf, sizeof(buf), "[%s]%d\r\n", pstMark->pucMarkName, uiLevle);
        pfFunc(buf, pUserData);
    }

    DLL_SCAN(&pstMark->stKeyValueDllHead, pstKey)
    {
        for (i=1; i<uiLevle; i++) {
            pfFunc("   ", pUserData);
        }

        pfFunc(pstKey->pucKeyName, pUserData);
        pfFunc("=", pUserData);
        pfFunc(pstKey->pucKeyValue, pUserData);
        pfFunc("\r\n", pUserData);
    }

    DLL_SCAN(&pstMark->stSectionDllHead, pstSecTmp) {
        cff_tre_SaveTag(pstSecTmp, uiLevle + 1, pfFunc, pUserData);
    }

    return;
}

static BS_STATUS cff_tre_Save(IN CFF_HANDLE hCff, IN PF_CFF_SAVE_CB pfFunc, IN VOID *pUserData)
{
    _CFF_S *pstCff = hCff;
    MKV_MARK_S  *pstMark;

    DLL_SCAN(&pstCff->stCfgRoot.stSectionDllHead, pstMark) {
        cff_tre_SaveTag(pstMark, 1, pfFunc, pUserData);
    }

    return BS_OK;
}

static _CFF_FUNC_TBL_S g_stCffTreFuncTbl = 
{
    cff_tre_Save
};

CFF_HANDLE CFF_TRE_Open(IN CHAR *pcFilePath, IN UINT uiFlag)
{
    _CFF_S *pstCff;

    pstCff = _cff_Open(pcFilePath, uiFlag);
    if (NULL == pstCff)
    {
        return NULL;
    }

    pstCff->pstFuncTbl = &g_stCffTreFuncTbl;

    cff_tre_Init(pstCff);

    return pstCff;
}

CFF_HANDLE CFF_TRE_OpenBuf(IN CHAR *buf, IN UINT uiFlag)
{
    _CFF_S *pstCff;

    pstCff = _cff_OpenBuf(buf, uiFlag);
    if (NULL == pstCff) {
        return NULL;
    }

    pstCff->pstFuncTbl = &g_stCffTreFuncTbl;

    cff_tre_Init(pstCff);

    return pstCff;
}

VOID CFF_TRE_SetAs(IN CFF_HANDLE hCff)
{
    _CFF_S *pstCff = hCff;

    BS_DBGASSERT(NULL != hCff);

    pstCff->pstFuncTbl = &g_stCffTreFuncTbl;
}

CFF_HANDLE CFF_INI_Open(IN CHAR *pcFilePath, IN UINT uiFlag)
{
    return CFF_TRE_Open(pcFilePath, uiFlag);
}

CFF_HANDLE CFF_INI_OpenBuf(IN CHAR *buf, IN UINT uiFlag)
{
    return CFF_TRE_OpenBuf(buf, uiFlag);
}

VOID CFF_INI_SetAs(IN CFF_HANDLE hCff)
{
    return CFF_TRE_SetAs(hCff);
}
