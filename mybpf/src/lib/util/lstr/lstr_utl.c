/******************************************************************************
* Copyright (C), LiXingang
* Author:      LiXingang  Version: 1.0  Date: 2015-6-6
* Description: string with length.
* History:     
******************************************************************************/
#include "bs.h"

#include "utl/txt_utl.h"
#include "utl/lstr_utl.h"


VOID LSTR_Strim(IN LSTR_S *pstString, IN CHAR *pcSkipChars, OUT LSTR_S *pstStringOut)
{
    ULONG ulDataLenTemp;
    CHAR *pucNew;
    
    pucNew = TXT_StrimHead(pstString->pcData, pstString->uiLen, pcSkipChars);
    ulDataLenTemp = (ULONG)pstString->uiLen - ((ULONG)pucNew - (ULONG)pstString->pcData);
    pstStringOut->uiLen = (UINT)TXT_StrimTail(pucNew, ulDataLenTemp, pcSkipChars);
    pstStringOut->pcData = pucNew;
}

CHAR * LSTR_Strchr(IN LSTR_S *pstLstr, IN CHAR cChr)
{
    return TXT_Strnchr(pstLstr->pcData, cChr, pstLstr->uiLen);
}

CHAR * LSTR_ReverseStrchr(IN LSTR_S *pstLstr, IN CHAR cChr)
{
    UINT uiLen, i;
    char *pcBuf = pstLstr->pcData;

    uiLen = pstLstr->uiLen;
    if (0 == uiLen) {
        return NULL;
    }

    for (i=uiLen; i>0; i--) {
        if (pcBuf[i-1] == cChr) {
            return &pcBuf[i-1];
        }
    }

    return NULL;
}

CHAR *LSTR_Strstr(IN LSTR_S *pstLstr, IN CHAR *pcToFind)
{
    return TXT_Strnstr(pstLstr->pcData, pcToFind, pstLstr->uiLen);
}


VOID LSTR_RemoveQuotation(INOUT LSTR_S *pstString)
{
    if (pstString->uiLen == 0)
    {
        return;
    }

    if ((pstString->pcData[0] == '\"') || (pstString->pcData[0] == '\''))
    {
        pstString->pcData ++;
        pstString->uiLen --;
    }

    if (pstString->uiLen == 0)
    {
        return;
    }

    if ((pstString->pcData[pstString->uiLen - 1] == '\"') || (pstString->pcData[pstString->uiLen - 1] == '\''))
    {
        pstString->uiLen --;
    }
}



VOID LSTR_MSplit
(
    IN LSTR_S *pstString,
    IN CHAR *pcSplitChar, 
    OUT LSTR_S *pstStr1,
    OUT LSTR_S *pstStr2
)
{
    CHAR *pcSplit;
    LONG lTemp;

    pcSplit = TXT_MStrnchr(pstString->pcData, pstString->uiLen, pcSplitChar);
    if (NULL == pcSplit)
    {
        pstStr1->pcData = pstString->pcData;
        pstStr1->uiLen = pstString->uiLen;
        pstStr2->pcData = NULL;
        pstStr2->uiLen = 0;
    }
    else
    {
        pstStr1->pcData = pstString->pcData;
        lTemp = pcSplit - pstString->pcData;
        pstStr1->uiLen = (UINT)lTemp;
        pstStr2->pcData = pcSplit + 1;
        pstStr2->uiLen = pstString->uiLen - (pstStr1->uiLen + 1);
    }

    return;
}


VOID LSTR_Split
(
    IN LSTR_S *pstString,
    IN CHAR cSplitChar, 
    OUT LSTR_S *pstStr1,
    OUT LSTR_S *pstStr2
)
{
    CHAR *pcSplit;
    UINT uiStringLen;

    pcSplit = LSTR_Strchr(pstString, cSplitChar);
    if (NULL == pcSplit)
    {
        pstStr1->pcData = pstString->pcData;
        pstStr1->uiLen = pstString->uiLen;
        pstStr2->pcData = NULL;
        pstStr2->uiLen = 0;
    }
    else
    {
        uiStringLen = pstString->uiLen;
        pstStr1->pcData = pstString->pcData;
        pstStr1->uiLen = (UINT)(pcSplit - pstString->pcData);
        pstStr2->pcData = pcSplit + 1;
        pstStr2->uiLen = uiStringLen - (pstStr1->uiLen + 1);
    }

    return;
}


UINT LSTR_XSplit
(
    IN LSTR_S *pstString,
    IN CHAR cSplitChar, 
    OUT LSTR_S *pstStr,
    IN UINT uiCount 
)
{
    LSTR_S stReserved;
    UINT i = 0;

    stReserved = *pstString;

    while ((i < uiCount) && (stReserved.uiLen > 0))
    {
        LSTR_Split(&stReserved, cSplitChar, &pstStr[i], &stReserved);
        i++;
    }

    return i;
}



INT LSTR_StrCmp(IN LSTR_S *pstStr, IN CHAR *pcString)
{
    LSTR_S stStr2;

    stStr2.pcData = pcString;
    stStr2.uiLen = strlen(pcString);

    return LSTR_Cmp(pstStr, &stStr2);
}


INT LSTR_StrCaseCmp(IN LSTR_S *pstStr, IN CHAR *pcString)
{
    LSTR_S stStr2;

    stStr2.pcData = pcString;
    stStr2.uiLen = strlen(pcString);

    return LSTR_CaseCmp(pstStr, &stStr2);
}


INT LSTR_Cmp(IN LSTR_S *pstStr, IN LSTR_S *pstStr2)
{
    UINT   uiLen   = MIN(pstStr->uiLen, pstStr2->uiLen);
    UINT   uiLoop  = 0;
    CHAR  *pcStr   = pstStr->pcData;

    while (uiLoop < uiLen)
    {
        if (pcStr[uiLoop] != pstStr2->pcData[uiLoop])
        {
            return pcStr[uiLoop] - pstStr2->pcData[uiLoop];
        }
        uiLoop++;
    }

    if (pstStr->uiLen == pstStr2->uiLen)
    {
        return 0;
    }

    if (pstStr->uiLen > pstStr2->uiLen)
    {
        return 1;
    }

    return -1;
}


INT LSTR_CaseCmp(IN LSTR_S *pstStr, IN LSTR_S *pstStr2)
{
    int c1;
    int c2;
    UINT   uiLen   = MIN(pstStr->uiLen, pstStr2->uiLen);
    UINT   uiLoop  = 0;
    CHAR  *pcStr   = pstStr->pcData;
    CHAR  *pcStr2  = pstStr2->pcData;

    while (uiLoop < uiLen)
    {
        c1 = tolower(pcStr[uiLoop]);
        c2 = tolower(pcStr2[uiLoop]);

        if (c1 != c2)
        {
            return (c1 - c2);
        }

        uiLoop++;
    }

    if (pstStr->uiLen == pstStr2->uiLen)
    {
        return 0;
    }

    if (pstStr->uiLen > pstStr2->uiLen)
    {
        return 1;
    }

    return -1;
}


CHAR *LSTR_Strlcpy(IN LSTR_S *pstStr, IN UINT uiSzSize, OUT CHAR *pcSz)
{
    ULONG ulLen = (ULONG)pstStr->uiLen;

    if (NULL == pstStr->pcData)
    {
        pcSz[0] = 0;
        return pcSz;
    }

    if (ulLen >=  uiSzSize)
    {
        ulLen = uiSzSize - 1;
    }

    memcpy(pcSz, pstStr->pcData, ulLen);
    pcSz[ulLen] = 0;

    return pcSz;
}

VOID LSTR_Cat(IN LSTR_S *pstStr1, IN LSTR_S *pstStr2)
{
    memcpy(pstStr1->pcData + pstStr1->uiLen, pstStr2->pcData, pstStr2->uiLen);
    pstStr1->uiLen += pstStr2->uiLen;
}

BS_STATUS LSTR_Atoui(IN LSTR_S *pstStr, OUT UINT *puiNum)
{
    CHAR szNum[12];

    LSTR_Strlcpy(pstStr, sizeof(szNum), szNum);

    return TXT_Atoui(szNum, puiNum);
}

UINT LSTR_A2ui(IN LSTR_S *pstStr)
{
    UINT uiRet = 0;

    LSTR_Atoui(pstStr, &uiRet);

    return uiRet;
}

BS_STATUS LSTR_XAtoui(IN LSTR_S *pstStr, OUT UINT *puiNum)
{
    CHAR szNum[12];

    LSTR_Strlcpy(pstStr, sizeof(szNum), szNum);

    return TXT_XAtoui(szNum, puiNum);
}


BS_STATUS LSTR_GetExt(IN LSTR_S *pstFilePath, OUT LSTR_S *pstExt)
{
    UINT uiCount;
    CHAR cChar;
    UINT uiOffset = 0;

    LSTR_Init(pstExt);

    if (pstFilePath->uiLen == 0)
    {
        return BS_ERR;
    }

    for (uiCount=pstFilePath->uiLen; uiCount>0; uiCount--)
    {
        cChar = pstFilePath->pcData[uiCount - 1];
        if (cChar == '.')
        {
            uiOffset = uiCount;
            break;
        }
    }

    if ((uiOffset == 0) || (uiOffset >= pstFilePath->uiLen))
    {
        return BS_ERR;
    }

    pstExt->pcData = pstFilePath->pcData + uiOffset;
    pstExt->uiLen = pstFilePath->uiLen - uiOffset;

    return BS_OK;
}

VOID LSTR_Lstr2Str(IN LSTR_S *pstLstr, OUT CHAR *pcStr, IN UINT uiStrSize)
{
    UINT uiCopyLen;

    if (uiStrSize == 0)
    {
        return;
    }

    pcStr[0] = '\0';

    if (NULL == pstLstr)
    {
        return;
    }

    uiCopyLen = MIN(uiStrSize - 1, pstLstr->uiLen);
    if (uiCopyLen > 0)
    {
        MEM_Copy(pcStr, pstLstr->pcData, uiCopyLen);
    }

    pcStr[uiCopyLen] = '\0';

    return;
}


VOID LSTR_CompressLine(INOUT LSTR_S *pstLstr)
{
    UINT uiLen;
    char *read;
    char *write;
    char *end;

    read = pstLstr->pcData;
    write = pstLstr->pcData;
    end = pstLstr->pcData + pstLstr->uiLen;
    uiLen = pstLstr->uiLen;

    while (read < end) {
        if ((*read == '\r') || (*read == '\n')) {
            read ++;
            uiLen --;
            continue;
        }

        *write = *read;
        read ++;
        write ++;
    }

    pstLstr->uiLen = uiLen;

    return;
}


VOID LSTR_DelElement(INOUT LSTR_S *pstLstr, IN CHAR cSplitChar, IN LSTR_S *pstElement)
{
    LSTR_S stEleTmp;
    LSTR_S stTmp;

    stTmp.pcData = pstLstr->pcData;
    stTmp.uiLen = 0;
    
    LSTR_SCAN_ELEMENT_BEGIN(pstLstr->pcData, pstLstr->uiLen, cSplitChar, &stEleTmp)
    {
        if (LSTR_Cmp(&stEleTmp, pstElement) ==0)
        {
            continue;
        }

        if (stTmp.uiLen != 0)
        {
            stTmp.pcData[stTmp.uiLen] = cSplitChar;
            stTmp.uiLen++;
        }

        LSTR_Cat(&stTmp, &stEleTmp);
    }LSTR_SCAN_ELEMENT_END();

    pstLstr->uiLen = stTmp.uiLen;

    return;
}

CHAR * LSTR_FindElement(INOUT LSTR_S *pstLstr, IN CHAR cSplitChar, IN LSTR_S *pstElement)
{
    LSTR_S stEleTmp;

    LSTR_SCAN_ELEMENT_BEGIN(pstLstr->pcData, pstLstr->uiLen, cSplitChar, &stEleTmp)
    {
        if (LSTR_Cmp(&stEleTmp, pstElement) ==0)
        {
            return stEleTmp.pcData;
        }
    }LSTR_SCAN_ELEMENT_END();

    return NULL;
}


BS_STATUS LSTR_GetValueByKey
(
    IN LSTR_S *pstString,
    IN CHAR cSplitChar,
    IN CHAR *pcKey,
    OUT LSTR_S *pstStrValue
)
{
    BS_STATUS eRet = BS_ERR;
    LSTR_S stStrKey;
    LSTR_S stStrValue;

    LSTR_Init(pstStrValue);

    LSTR_Split(pstString, cSplitChar, &stStrKey, &stStrValue);
    if (0 == LSTR_StrCmp(&stStrKey, pcKey) && 0 != stStrValue.uiLen)
    {
        *pstStrValue = stStrValue;
        eRet = BS_OK;
    }

    return eRet;
}

void LSTR_KvIterInit(IN LSTR_S *pstStr, OUT LSTR_KV_ITER_S *iter)
{
    iter->stLstr.pcData = pstStr->pcData;
    iter->stLstr.uiLen = pstStr->uiLen;
}

LSTR_KV_S * LSTR_GetNextKV(INOUT LSTR_KV_ITER_S *iter, IN char split, IN char eque)
{
    LSTR_S ele1, ele2;

    if (iter->stLstr.uiLen == 0) {
        return NULL;
    }

    LSTR_Split(&iter->stLstr, split, &ele1, &ele2);

    iter->stLstr = ele2;

    LSTR_Split(&ele1, eque, &iter->kv.key, &iter->kv.value);

    return &iter->kv;
}

int LSTR_ScanMultiKV(IN LSTR_S *pstStr, IN char cSplit, IN char cEque, PF_LSTR_MULTI_KV_SCAN_CB pfCallBack, IN void *pUserHandle)
{
    LSTR_S stEle;
    LSTR_S stKey;
    LSTR_S stValue;

    LSTR_SCAN_ELEMENT_BEGIN(pstStr->pcData, pstStr->uiLen, cSplit, &stEle) {
        LSTR_Split(&stEle, cEque, &stKey, &stValue);
        if (stKey.uiLen > 0) {
            pfCallBack(&stKey, &stValue, pUserHandle);
        }
    }LSTR_SCAN_ELEMENT_END();

    return 0;
}

