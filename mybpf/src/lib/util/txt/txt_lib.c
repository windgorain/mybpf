/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#include "bs.h"
#include "utl/txt_utl.h"
#include "utl/lstr_utl.h"
#include "utl/num_utl.h"
#include "utl/rand_utl.h"
#include "utl/stack_utl.h"
#include "utl/ctype_utl.h"

static int _txt_replace_substr(CHAR *pucTxtBuf, CHAR *pucSubStrFrom,
        CHAR *pucSubStrTo, OUT CHAR *pucTxtOutBuf, ULONG ulSize)
{
    CHAR *pcFind;
    CHAR *pcTxtBufTmp;
    CHAR *pcTxtOutBufTmp;
    UINT ulSubStrToLen;
    ULONG ulSizeTmp = ulSize;
    UINT uiCopyLen;

    if (strlen(pucTxtBuf) == 0) {
        *pucTxtOutBuf = '\0';
        return BS_OK;
    }

    if (strlen(pucSubStrFrom) == 0) {
        strlcpy(pucTxtOutBuf, pucTxtBuf, ulSize);
        return BS_OK;
    }

    pcTxtBufTmp = pucTxtBuf;
    pcTxtOutBufTmp = pucTxtOutBuf;
    ulSubStrToLen = strlen(pucSubStrTo);

    
    pcFind = strstr(pcTxtBufTmp, pucSubStrFrom);
    if (pcFind == NULL) {
        strlcpy(pcTxtOutBufTmp, pcTxtBufTmp, ulSize);
        return BS_OK;
    }

    
    while ((pcTxtBufTmp[0] != '\0') && (ulSizeTmp > 1)) {
        pcFind = strstr(pcTxtBufTmp, pucSubStrFrom);
        if (pcFind == NULL) {
            strlcpy(pcTxtOutBufTmp, pcTxtBufTmp, ulSizeTmp);
            break;
        } else {
            if (pcFind != pcTxtBufTmp) {
                uiCopyLen = MIN(ulSizeTmp - 1, (ULONG)pcFind - (ULONG)pcTxtBufTmp);

                strlcpy(pcTxtOutBufTmp, pcTxtBufTmp, uiCopyLen + 1);
                ulSizeTmp -= uiCopyLen;
                pcTxtOutBufTmp += uiCopyLen;
                pcTxtOutBufTmp[0] = '\0';

                uiCopyLen = MIN(ulSizeTmp - 1, ulSubStrToLen);
                strlcpy(pcTxtOutBufTmp, pucSubStrTo, uiCopyLen + 1);
                ulSizeTmp -= uiCopyLen;
                pcTxtOutBufTmp += ulSubStrToLen;
            }
            pcTxtBufTmp = pcFind + strlen(pucSubStrFrom);
        }
    }

    return _txt_replace_substr(pucTxtOutBuf, pucSubStrFrom, pucSubStrTo, pucTxtOutBuf, ulSize);
}



CHAR * TXT_FindOneOf(IN CHAR *pszStr, IN CHAR *pszPattern)
{
    CHAR *pt = pszStr;
    CHAR *pt1;

    CHAR cPt;

    while (*pt != '\0') {
        cPt = *pt;

        pt1 = pszPattern;
        while (*pt1 != '\0') {
            if (*pt1 == cPt) {
                return pt;
            }
            pt1 ++;
        }

        pt ++;
    }

    return NULL;
}


BOOL_T TXT_IsInRange(IN CHAR cCh, IN UCHAR *pszStringRange, IN UINT ulLen)
{
    UINT i;

    for (i=0; i<ulLen; i++) {
        if (cCh == pszStringRange[i]) {
            return TRUE;
        }
    }

    return FALSE;
}

UCHAR *TXT_FindFirstNonSuch(IN UCHAR *pszStr, IN UINT ulLen, IN UCHAR *pszNoSuch, IN UINT ulNoSuchLen)
{
    UINT i;

	
	for (i=0; i<ulLen; i++) {
		if (! TXT_IsInRange(pszStr[i], pszNoSuch, ulNoSuchLen)) {
			break;
		}
	}

    if (i == ulLen) {
        return NULL;
    }

    return pszStr + i;
}


UINT TXT_StrToToken(IN CHAR *pszStr, IN CHAR *pszPatterns, OUT CHAR *apszArgz[], IN UINT uiMaxArgz)
{
    CHAR * pt = pszStr;
    CHAR * pt1;
    UINT uiCount = 0;
    UINT uiPatternLen;

    if (*pszStr == '\0') {
        return 0;
    }

    if (uiMaxArgz == 0) {
        return 0;
    }

    uiPatternLen = strlen(pszPatterns);

    do {
        pt = (CHAR*)TXT_FindFirstNonSuch((UCHAR*)pt, strlen(pt), (UCHAR*)pszPatterns, uiPatternLen);
        if (pt == NULL) {
            return uiCount;
        }

        pt1 = TXT_FindOneOf(pt, pszPatterns);
        if (NULL != pt1) {
            *pt1 = '\0';
            pt1 ++;
        }

        apszArgz[uiCount] = pt;
        uiCount ++;

        pt = pt1;
    } while ((pt) && (uiCount < uiMaxArgz));

    return uiCount;
}


UINT TXT_GetTokenNum(IN CHAR *pszStr, IN CHAR *pszPatterns)
{
    CHAR * pt = pszStr;
    CHAR * pt1;
    UINT uiCount = 0;
    UINT uiPatternLen;

    if (*pszStr == '\0') {
        return 0;
    }

    uiPatternLen = strlen(pszPatterns);

    do {
        pt = (CHAR*)TXT_FindFirstNonSuch((UCHAR*)pt, strlen(pt), (UCHAR*)pszPatterns, uiPatternLen);
        if (pt == NULL) {
            return uiCount;
        }

        pt1 = TXT_FindOneOf(pt, pszPatterns);
        if (NULL != pt1) {
            pt1 ++;
        }

        uiCount ++;

        pt = pt1;
    } while (pt != NULL);

    return uiCount;
}

void TXT_ReplaceSubStr(IN CHAR *pcTxtBuf, IN CHAR *pcSubStrFrom, IN CHAR *pcSubStrTo, OUT CHAR *pcTxtOutBuf, IN ULONG ulSize)
{
    BS_DBGASSERT(NULL != pcTxtBuf);
    BS_DBGASSERT(NULL != pcSubStrFrom);
    BS_DBGASSERT(NULL != pcSubStrTo);
    BS_DBGASSERT(NULL != pcTxtOutBuf);

    if (ulSize == 0) {
        return;
    }

    _txt_replace_substr(pcTxtBuf, pcSubStrFrom, pcSubStrTo, pcTxtOutBuf, ulSize);
}

CHAR * TXT_StrimHead(IN CHAR *pcData, IN ULONG ulDataLen, IN CHAR *pcSkipChars)
{
    CHAR *pcTemp = pcData;
    CHAR *pcEnd = pcData + ulDataLen;

    
    if((NULL == pcData) || (NULL == pcSkipChars)) {
        return NULL;
    }

    if (0 == ulDataLen) {
        return pcData;
    }

    while (pcTemp < pcEnd) {
        if(NULL == strchr(pcSkipChars, *pcTemp)) {
            break;
        }
        pcTemp ++;
    }

    return pcTemp;
}


ULONG TXT_StrimTail(IN CHAR *pcData, IN ULONG ulDataLen, IN CHAR *pcSkipChars)
{
    CHAR *pcTemp;

    
    if ((NULL == pcData) || (NULL == pcSkipChars)) {
        return 0;
    }

    if (0 == ulDataLen) {
        return 0;
    }

    pcTemp = (pcData + ulDataLen) - 1;

    while (pcTemp >= pcData) {
        if (NULL == strchr(pcSkipChars, *pcTemp)) {
            break;
        }
        pcTemp --;
    }

    return ((ULONG)(pcTemp + 1) - (ULONG)pcData);
}

CHAR * TXT_Strnchr(IN CHAR *pcBuf, IN CHAR ch2Find, IN UINT ulLen)
{
    UINT i;
    
    if (pcBuf == NULL) {
        return NULL;
    }

    for (i=0; i<ulLen; i++) {
        if (ch2Find == pcBuf[i]) {
            return (pcBuf + i);
        } else if ('\0' == pcBuf[i]) {
            return NULL;
        }
    }

    return NULL;
}

BS_STATUS TXT_Atoui(IN CHAR *pszBuf, OUT UINT *puiNum)
{
    if ((pszBuf == NULL) || (pszBuf[0] == '\0')) {
        RETURN(BS_BAD_PTR);
    }

	if (FALSE == CTYPE_IsNumString(pszBuf)) {
		RETURN(BS_ERR);
	}

    *puiNum = strtoul(pszBuf, NULL, 10);

    return BS_OK;
}

UINT TXT_Str2Ui(IN CHAR *pcStr)
{
    UINT uiNum = 0;
    TXT_Atoui(pcStr, &uiNum);
    return uiNum;
}

void TXT_StrSplit(IN CHAR *pcString, IN CHAR cSplitChar, OUT LSTR_S * pstStr1, OUT LSTR_S * pstStr2)
{
    LSTR_S stString;

    stString.pcData = pcString;
    stString.uiLen = strlen(pcString);

    LSTR_Split(&stString, cSplitChar, pstStr1, pstStr2);
}

char * TXT_Strdup(IN CHAR *pcStr)
{
    UINT uiLen;
    CHAR *pcDup;

    if (NULL == pcStr) {
        return NULL;
    }

    uiLen = strlen(pcStr);
    pcDup = MEM_Malloc(uiLen + 1);
    if (NULL == pcDup) {
        return NULL;
    }
    strlcpy(pcDup, pcStr, uiLen + 1);

    return pcDup;
}


BOOL_T TXT_EndcharMatch(char *string, char *pattern, int pattern_len, char end_char)
{
    int string_len = strlen(string);
    
    if (string_len < pattern_len) {
        return FALSE;
    }

    if ((string_len > pattern_len) && (string[pattern_len] != end_char)) {
        return FALSE;
    }

    if (strncmp(string, pattern, pattern_len) == 0) {
        return TRUE;
    }

    return FALSE;
}

