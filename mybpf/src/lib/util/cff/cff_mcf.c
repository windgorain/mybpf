/******************************************************************************
* Copyright (C), LiXingang
* Author:      LiXingang  Version: 1.0  Date: 2015-6-26
* Description: multi conf file
* eg:   zhangsan:age=15;sex=boy
*       lisi:age=16;sex=boy
* History:     
******************************************************************************/
#include "bs.h"

#include "utl/txt_utl.h"
#include "utl/cff_utl.h"

#include "cff_inner.h"


#define MYCONF_CHAR_REMARK '#'  
#define MYCONF_CHAR_KEY_PRO_SPLIT ':'  
#define MYCONF_CHAR_PRO_SPLIT ';'  
#define MYCONF_CHAR_PRO_VALUE_SPLIT '='  


static VOID cff_mcf_ProcLine(IN _CFF_S *pstCff, IN CHAR *pcLine)
{
    CHAR *pcTag;
    CHAR *pcProp;
    CHAR *pcTmp = NULL;
    CHAR *pcValue;
    CHAR *pcSplit;
    MKV_MARK_S  *pstCurrTag = NULL;

    if ((pcLine[0] == '\0')
        || (pcLine[0] == MYCONF_CHAR_REMARK)
        || (pcLine[0] == MYCONF_CHAR_KEY_PRO_SPLIT))
    {
        return;
    }

    pcTag = pcLine;

    pcSplit = strchr(pcTag, MYCONF_CHAR_KEY_PRO_SPLIT);
    if (NULL != pcSplit)
    {
        *pcSplit = '\0';
        pcTmp = pcSplit + 1;
    }

    pstCurrTag = MKV_AddMark2Mark(pstCurrTag, pcTag, FALSE);
    if (NULL == pstCurrTag)
    {
        return;
    }

    if (pcTmp == NULL)
    {
        return;
    }

    TXT_SCAN_ELEMENT_BEGIN(pcTmp, MYCONF_CHAR_PRO_SPLIT, pcProp)
    {
        pcValue = "";
        pcSplit = strchr(pcProp, '=');
        if (NULL != pcSplit)
        {
            *pcSplit = '\0';
            pcValue = pcSplit + 1;
        }

        TXT_StrimAndMove(pcProp);
        TXT_StrimAndMove(pcValue);

        MKV_AddNewKey2Mark(pstCurrTag, pcProp, pcValue, TRUE, _ccf_IsSort(pstCff));
    }TXT_SCAN_ELEMENT_END();
}

static VOID cff_mcf_Init(IN _CFF_S *pstCff)
{
    CHAR *pcLine;
    UINT uiLineLen;
    
    TXT_SCAN_LINE_BEGIN(pstCff->pcFileContent, pcLine, uiLineLen)
    {
        pcLine[uiLineLen] = '\0';
        TXT_StrimAndMove(pcLine);
        cff_mcf_ProcLine(pstCff, pcLine);
    }TXT_SCAN_LINE_END();

    if (pstCff->uiFlag & CFF_FLAG_SORT)
    {
        MKV_SortMark(&pstCff->stCfgRoot);
    }
}

static VOID cff_mcf_SaveTag(IN MKV_MARK_S *pstMark, IN PF_CFF_SAVE_CB pfFunc, IN VOID *pUserData)
{
    MKV_KEY_S  *pstProp;
    BOOL_T bFirst = TRUE;
    char buf[1024];

    snprintf(buf, sizeof(buf), "%s:", pstMark->pucMarkName);
    pfFunc(buf, pUserData);

    DLL_SCAN(&pstMark->stKeyValueDllHead, pstProp) {
        if (bFirst) {
            bFirst = FALSE;
        } else {
            pfFunc(";", pUserData);
        }
        snprintf(buf, sizeof(buf),"%s=%s", pstProp->pucKeyName, pstProp->pucKeyValue);
        pfFunc(buf, pUserData);
    }

    return;
}

static BS_STATUS cff_mcf_Save(IN CFF_HANDLE hCff, IN PF_CFF_SAVE_CB pfFunc, IN VOID *pUserData)
{
    _CFF_S *pstCff = hCff;
    MKV_MARK_S  *pstMark;

    DLL_SCAN(&pstCff->stCfgRoot.stSectionDllHead, pstMark) {
        cff_mcf_SaveTag(pstMark, pfFunc, pUserData);
    }

    return BS_OK;
}

static _CFF_FUNC_TBL_S g_stCffMcfFuncTbl = 
{
    cff_mcf_Save
};

CFF_HANDLE CFF_MCF_Open(IN CHAR *pcFilePath, IN UINT uiFlag)
{
    _CFF_S *pstCff;

    pstCff = _cff_Open(pcFilePath, uiFlag);
    if (NULL == pstCff)
    {
        return NULL;
    }

    pstCff->pstFuncTbl = &g_stCffMcfFuncTbl;

    cff_mcf_Init(pstCff);

    return pstCff;
}

CFF_HANDLE CFF_MCF_OpenBuf(IN CHAR *buf, IN UINT uiFlag)
{
    _CFF_S *pstCff;

    pstCff = _cff_OpenBuf(buf, uiFlag);
    if (NULL == pstCff)
    {
        return NULL;
    }

    pstCff->pstFuncTbl = &g_stCffMcfFuncTbl;

    cff_mcf_Init(pstCff);

    return pstCff;
}


VOID CFF_BRACE_SetAs(IN CFF_HANDLE hCff)
{
    _CFF_S *pstCff = hCff;

    BS_DBGASSERT(NULL != hCff);

    pstCff->pstFuncTbl = &g_stCffMcfFuncTbl;
}
