/*================================================================
*   Author:      LiXingang  Version: 1.0  Date: 2015-6-23
*   Description: brace 带大括号的配置文件, '#' is comment line
*   EG:
*       debug: {
*           reqest:1;
*           log_utl:{
*              filename:fakecert_debug.txt;
*              print:0
*           }
*       };
*       errlog: {
*       }
================================================================*/
#include "bs.h"
#include "utl/txt_utl.h"
#include "utl/cff_utl.h"

#include "cff_inner.h"

static char * cff_brace_ProcCfg(IN _CFF_S *pstCff, MKV_MARK_S *pstSecCurrent, IN CHAR *pcCfg)
{
    CHAR *pcSplit;
    CHAR cSplit;
    CHAR *key;
    CHAR *value;
    MKV_MARK_S *mark;
    char *read = pcCfg;

    while ((read) && (*read != '\0')) {
        if (*read == '}') {
            return read + 1;
        }

        if (*read == ';') {
            read ++;
            continue;
        }

        pcSplit = strchr(read, ':');
        if (NULL == pcSplit) {
            return NULL;
        }

        key = read;
        *pcSplit = 0;

        if (key[0] == 0) {
            return NULL;
        }

        read = pcSplit + 1;

        if (*read== '\0') {
            return NULL;
        } else if (*read == '{') {
            mark = MKV_AddMark2Mark(pstSecCurrent, key, FALSE);
            if (NULL == mark) {
                return NULL;
            }
            read = cff_brace_ProcCfg(pstCff, mark, read + 1);
        } else if (*read == '}') {
            return read + 1;
        } else { 
            value = read;
            pcSplit = TXT_MStrnchr(value, strlen(value), ";}");
            if (NULL != pcSplit) {
                cSplit = *pcSplit;
                *pcSplit = '\0';
            }
            MKV_AddNewKey2Mark(pstSecCurrent, key, value, FALSE, pstCff->uiFlag & CFF_FLAG_SORT);
            if ((NULL != pcSplit) && (cSplit == '}')) {
                return pcSplit + 1;
            }
            if (pcSplit != NULL) {
                read = pcSplit + 1;
            } else {
                read = NULL;
            }
        }
    }

    return NULL;
}

static VOID cff_brace_Init(IN _CFF_S *pstCff)
{
    
    TXT_DelLineComment(pstCff->pcFileContent, "#", pstCff->pcFileContent);
    
    pstCff->pcFileContent = TXT_CompressLine(pstCff->pcFileContent);
    
    pstCff->pcFileContent = TXT_StrimAll(pstCff->pcFileContent);

    cff_brace_ProcCfg(pstCff, &pstCff->stCfgRoot, pstCff->pcFileContent);
 
    if (pstCff->uiFlag & CFF_FLAG_SORT) {
        MKV_SortMark(&pstCff->stCfgRoot);
    }
}

static VOID cff_brace_SaveTag(IN MKV_MARK_S *pstMark, IN UINT uiLevle, IN PF_CFF_SAVE_CB pfFunc, IN VOID *pUserData) 
{
    MKV_KEY_S  *pstKey;
    MKV_MARK_S  *pstSecTmp;
    UINT i;
    char buf[1024];

    if (pstMark->pucMarkName) {
        for (i=1; i<uiLevle; i++) {
            pfFunc("  ", pUserData);
        }
        snprintf(buf, sizeof(buf), "%s: {\r\n", pstMark->pucMarkName);
        pfFunc(buf, pUserData);
    }

    DLL_SCAN(&pstMark->stKeyValueDllHead, pstKey) {
        for (i=0; i<uiLevle; i++) {
            pfFunc("  ", pUserData);
        }

        snprintf(buf, sizeof(buf), "%s:%s;\r\n", pstKey->pucKeyName, pstKey->pucKeyValue);
        pfFunc(buf, pUserData);
    }

    DLL_SCAN(&pstMark->stSectionDllHead, pstSecTmp) {
        cff_brace_SaveTag(pstSecTmp, uiLevle + 1, pfFunc, pUserData);
    }

    if (pstMark->pucMarkName) {
        for (i=1; i<uiLevle; i++) {
            pfFunc("  ", pUserData);
        }
        pfFunc("};\r\n", pUserData);
    }

    return;
}

static BS_STATUS cff_brace_Save(IN CFF_HANDLE hCff, IN PF_CFF_SAVE_CB pfFunc, IN VOID *pUserData)
{
    _CFF_S *pstCff = hCff;

    cff_brace_SaveTag(&pstCff->stCfgRoot, 0, pfFunc, pUserData);

    return BS_OK;
}

static _CFF_FUNC_TBL_S g_stCffBraceFuncTbl = 
{
    cff_brace_Save
};

CFF_HANDLE CFF_BRACE_Open(IN CHAR *pcFilePath, IN UINT uiFlag)
{
    _CFF_S *pstCff;

    pstCff = _cff_Open(pcFilePath, uiFlag);
    if (NULL == pstCff)
    {
        return NULL;
    }

    pstCff->pstFuncTbl = &g_stCffBraceFuncTbl;

    cff_brace_Init(pstCff);

    return pstCff;
}

CFF_HANDLE CFF_BRACE_OpenBuf(IN CHAR *buf, IN UINT uiFlag)
{
    _CFF_S *pstCff;

    pstCff = _cff_OpenBuf(buf, uiFlag);
    if (NULL == pstCff) {
        return NULL;
    }

    pstCff->pstFuncTbl = &g_stCffBraceFuncTbl;

    cff_brace_Init(pstCff);

    return pstCff;
}


VOID CFF_BRACE_SetAs(IN CFF_HANDLE hCff)
{
    _CFF_S *pstCff = hCff;

    BS_DBGASSERT(NULL != hCff);

    pstCff->pstFuncTbl = &g_stCffBraceFuncTbl;
}
