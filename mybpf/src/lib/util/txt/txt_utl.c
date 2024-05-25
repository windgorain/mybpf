/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      lixingang  Version: 1.0  Date: 2007-2-8
* Description: 
* History:     
******************************************************************************/
#include "bs.h"
#include "utl/txt_utl.h"
#include "utl/lstr_utl.h"
#include "utl/num_utl.h"
#include "utl/rand_utl.h"
#include "utl/stack_utl.h"
#include "utl/ctype_utl.h"

BS_STATUS TXT_Lower(INOUT CHAR *pucTxtBuf)
{
    CHAR *pucTmp = pucTxtBuf;

    if (pucTxtBuf == NULL)
    {
        RETURN(BS_NULL_PARA);
    }

    while (*pucTmp != '\0')
    {
        if (NUM_IN_RANGE(*pucTmp, 'A', 'Z'))
        {
            *pucTmp += ('a' - 'A');
        }
        pucTmp++;
    }

    return BS_OK;
}

BS_STATUS TXT_Upper(INOUT CHAR *pucTxtBuf)
{
    CHAR *pucTmp = pucTxtBuf;

    if (pucTxtBuf == NULL)
    {
        RETURN(BS_NULL_PARA);
    }

    while (*pucTmp != '\0')
    {
        if (NUM_IN_RANGE(*pucTmp, 'a', 'z'))
        {
            *pucTmp -= ('a' - 'A');
        }
        pucTmp++;
    }

    return BS_OK;
}

static BS_STATUS txt_DelSubStr(IN CHAR *pucTxtBuf, IN CHAR *pucSubStr, OUT CHAR *pucTxtOutBuf, IN ULONG ulSize)
{
    CHAR *pcFind;
    CHAR *pcTxtBufTmp;
    CHAR *pcTxtOutBufTmp;
    ULONG ulSizeTmp = ulSize;
    UINT ulCopyLen;

    if (strlen(pucTxtBuf) == 0)
    {
        pucTxtOutBuf[0] = '\0';
        return BS_OK;
    }

    if (strlen(pucSubStr) == 0)
    {
        strlcpy(pucTxtOutBuf, pucTxtBuf, ulSize);
        return BS_OK;
    }

    pucTxtOutBuf[ulSize - 1] = '\0';

    pcTxtBufTmp = pucTxtBuf;
    pcTxtOutBufTmp = pucTxtOutBuf;

    pcFind = strstr(pcTxtBufTmp, pucSubStr);
    if (pcFind == NULL)
    {
        strlcpy(pcTxtOutBufTmp, pcTxtBufTmp, ulSize);
        return BS_OK;
    }

    while ((pcTxtBufTmp[0] != '\0') && (ulSizeTmp > 1))
    {
        pcFind = strstr(pcTxtBufTmp, pucSubStr);
        if (pcFind == NULL)
        {
            strlcpy(pcTxtOutBufTmp, pcTxtBufTmp, ulSizeTmp);
            break;
        }
        else
        {
            if (pcFind != pcTxtBufTmp)
            {
                ulCopyLen = MIN((ULONG)pcFind - (ULONG)pcTxtBufTmp, ulSizeTmp - 1);
                strlcpy(pcTxtOutBufTmp, pcTxtBufTmp, ulCopyLen + 1);
                ulSizeTmp -= ulCopyLen;
                pcTxtOutBufTmp += ulCopyLen;
            }
            pcTxtBufTmp = pcFind + strlen(pucSubStr);
        }
    }

    return txt_DelSubStr(pucTxtOutBuf, pucSubStr, pucTxtOutBuf, ulSize);
}

VOID TXT_DelSubStr(IN CHAR *pucTxtBuf, IN CHAR *pucSubStr, OUT CHAR *pucTxtOutBuf, IN ULONG ulSize)
{
    BS_DBGASSERT(NULL != pucTxtBuf);
    BS_DBGASSERT(NULL != pucSubStr);
    BS_DBGASSERT(NULL != pucTxtOutBuf);

    if (ulSize == 0)
    {
        return;
    }

    txt_DelSubStr(pucTxtBuf, pucSubStr, pucTxtOutBuf, ulSize);
}

static BS_STATUS txt_DelLineComment
(
    IN CHAR *pcInBuf,
    IN CHAR *pcCommentFlag,  
    OUT CHAR *pcOutBuf
)
{
    CHAR *pcFind;
    CHAR *read;
    CHAR *write;
    UINT ulCopyLen;

    if (strlen(pcInBuf) == 0) {
        pcOutBuf[0] = '\0';
        return BS_OK;
    }

    if (strlen(pcCommentFlag) == 0) {
        if (pcInBuf != pcOutBuf) {
            strlcpy(pcOutBuf, pcInBuf, strlen(pcInBuf) + 1);
        }
        return BS_OK;
    }

    read = pcInBuf;
    write = pcOutBuf;

    while (read[0] != '\0') {
        pcFind = strstr(read, pcCommentFlag);
        if (pcFind == NULL) {
            if (read != write) {
                strlcpy(write, read, strlen(read) + 1);
            }
            break;
        } else {
            if (pcFind != read) {
                
                ulCopyLen = (ULONG)pcFind - (ULONG)read;
                if (read != write) {
                    strlcpy(write, read, ulCopyLen + 1);
                }
                write += ulCopyLen;
            }

            read = pcFind + strlen(pcCommentFlag);

            
            pcFind = TXT_MStrnchr(read, strlen(read), (void*)"\r\n");
            if (NULL == pcFind) {
                break;
            }

            read = pcFind + 1;
            if ((*read == '\r') || (*read == '\n')) {
                read ++;
            }
        }
    }

    return BS_OK;
}


VOID TXT_DelLineComment
(
    IN CHAR *pcInBuf,
    IN CHAR *pcCommentFlag,  
    OUT CHAR *pcOutBuf
)
{
    BS_DBGASSERT(NULL != pcInBuf);
    BS_DBGASSERT(NULL != pcCommentFlag);
    BS_DBGASSERT(NULL != pcOutBuf);

    txt_DelLineComment(pcInBuf, pcCommentFlag, pcOutBuf);
}

void TXT_ReplaceChar(INOUT char *pcTxtBuf, char from, char to)
{
    char *c = pcTxtBuf;
    while (*c) {
        if (*c == from) {
            *c = to;
        }
        c++;
    }
}


void TXT_N2RN(char *in, char *out, int out_size)
{
    char *i = in;
    int r = 0;
    char *end = (out + out_size) - 1;

    if (out_size == 0) {
        return;
    }

    while (*i) {
        if (out >= end) {
            break;
        }

        if ((*i == '\n') && (r == 0)) {
            *out = '\r';
            out ++;
            if (out >= end) {
                break;
            }
        }

        r = 0;
        if (*i == '\r') {
            r = 1;
        } 

        *out = *i;
        i ++;
        out ++;
    }

    *out = '\0';
}

VOID TXT_ReplaceSubStrOnce
(
 IN CHAR *pcTxtBuf,
 IN CHAR *pcSubStrFrom,
 IN CHAR *pcSubStrTo,
 OUT CHAR *pcTxtOutBuf,
 IN ULONG ulOutSize
 )
{
    CHAR *pcFind;

    BS_DBGASSERT(NULL != pcTxtBuf);
    BS_DBGASSERT(NULL != pcSubStrFrom);
    BS_DBGASSERT(NULL != pcSubStrTo);
    BS_DBGASSERT(NULL != pcTxtOutBuf);

    if (strlen(pcTxtBuf) == 0)
    {
        *pcTxtOutBuf = '\0';
        return;
    }

    if (strlen(pcSubStrFrom) == 0)
    {
        strlcpy(pcTxtOutBuf, pcTxtBuf, ulOutSize);
        return;
    }

    
    pcFind = strstr(pcTxtBuf, pcSubStrFrom);
    if (pcFind == NULL)
    {
        strlcpy(pcTxtOutBuf, pcTxtBuf, ulOutSize);
        return;
    }

    memcpy(pcTxtOutBuf, pcTxtBuf, pcFind - pcTxtBuf);
    TXT_Strlcat(pcTxtOutBuf, pcFind + strlen(pcSubStrFrom), ulOutSize);
}

char * TXT_CompressLine(INOUT char * pcString)
{
    LSTR_S stStr;

    stStr.pcData = pcString;
    stStr.uiLen = strlen(pcString);

    LSTR_CompressLine(&stStr);

    stStr.pcData[stStr.uiLen] = '\0';

    return stStr.pcData;
}

CHAR * TXT_StrimHeadTail(CHAR *pcData, ULONG ulDataLen, CHAR *pcSkipChars, OUT ULONG *pulNewLen)
{
    CHAR *head = TXT_StrimHead(pcData, ulDataLen, pcSkipChars);
    ULONG len = ulDataLen - (head - pcData);
    *pulNewLen = TXT_StrimTail(head, len, pcSkipChars);

    return head;
}

CHAR * TXT_StrimString(IN CHAR *pcData, IN CHAR *pcSkipChars)
{
    ULONG ulLen;

    if (NULL == pcData)
    {
        return NULL;
    }

    ulLen = TXT_StrimTail(pcData, strlen(pcData), pcSkipChars);
    pcData[ulLen] = '\0';

    return TXT_StrimHead(pcData, ulLen, pcSkipChars);
}


CHAR *TXT_Strim(IN CHAR *pszStr)
{
    LSTR_S stString;

    stString.pcData = pszStr;
    stString.uiLen = strlen(pszStr);
    
    LSTR_Strim(&stString, (void*)" \t\n\r", &stString);
    stString.pcData[stString.uiLen] = '\0';

    return stString.pcData;
}


VOID TXT_StrimAndMove(IN CHAR *pszStr)
{
	CHAR *pt = pszStr;
	INT len, idx, b = 0;

	
	for (; *pt != '\0'; pt++ )
	{
		if (! TXT_IS_BLANK_OR_RN(*pt))
			break;
	}

    memmove(pszStr, pt, strlen(pt) + 1);

	len = strlen(pt);
	if (len)
	{
		for ( idx = (len - 1); idx >= 0; idx-- )
		{
			switch (pt[idx])
			{
				case ' ':
				case '\t':
				case '\r':
				case '\n':
					pt[idx] = '\0';
					break;
					
				default:
					b++;
					break;
			}
			
			if (b)
				break;
		}
	}
}


char * TXT_StrimAll(IN CHAR *pcStr)
{
    CHAR *pcRead;
    CHAR *pcWrite;
    CHAR c;

    pcRead = pcStr;
    pcWrite = pcStr;

    while (*pcRead != '\0')
    {
        c = *pcRead;
        pcRead ++;

        if (TXT_IS_BLANK_OR_RN(c))  {
            continue;
        }

        *pcWrite = c;
        pcWrite ++;
    }

    *pcWrite = '\0';

    return pcStr;
}


char * TXT_StrimTo(char *in, char *out)
{
    in = TXT_StrimHead(in, strlen(in), (void*)" \t\r\n");
    strcpy(out, in);
    TXT_Strim(out);

    return out;
}


char * TXT_StrimAllTo(char *in, char *out)
{
    in = TXT_StrimHead(in, strlen(in), (void*)" \t\r\n");
    strcpy(out, in);
    TXT_StrimAll(out);

    return out;
}


UCHAR * TXT_FindFirstNonBlank(IN UCHAR *pszStr, IN UINT ulLen)
{
    UINT i;

	
	for (i=0; i<ulLen; i++)
	{
		if (! TXT_IS_BLANK_OR_RN(pszStr[i]))
		{
			break;
		}
	}

    if (i == ulLen)
    {
        return NULL;
    }

    return pszStr + i;
}


UINT TXT_StrToToken2(IN CHAR *pszStr, IN CHAR *pszPatterns, OUT CHAR *apszArgz[], IN UINT uiMaxArgz)
{
    CHAR * pt = pszStr;
    CHAR * pt1;
    UINT uiCount = 0;

    if (*pszStr == '\0') {
        return 0;
    }

    if (uiMaxArgz == 0) {
        return 0;
    }

    do {
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


HANDLE TXT_StrToDynamicToken(IN CHAR *pszStr, IN CHAR *pszPatterns)
{
    CHAR *pt1, *pt = pszStr;
    HANDLE hStack;
    UINT uiPatternLen;

    if (*pszStr == '\0')
    {
        return NULL;
    }

    hStack = HSTACK_Create(0);
    if (NULL == hStack)
    {
        return NULL;
    }

    uiPatternLen = strlen(pszPatterns);

    do {
        pt = (CHAR*)TXT_FindFirstNonSuch((void*)pt, strlen(pt), (void*)pszPatterns, uiPatternLen);
        if (pt == NULL)
        {
            return hStack;
        }

        pt1 = TXT_FindOneOf(pt, pszPatterns);
        if (NULL != pt1)
        {
            *pt1 = '\0';
            pt1 ++;
        }

        if (BS_OK != HSTACK_Push(hStack, pt))
        {
            HSTACK_Destory(hStack);
            return NULL;
        }

        pt = pt1;
    }while (pt != NULL);

    return hStack;
}


BS_STATUS TXT_GetLine(IN CHAR *pucTxtBuf, OUT UINT *puiLineLen, OUT BOOL_T *pbIsFoundLineEndFlag, OUT CHAR **ppucLineNext)
{
    CHAR *spucLineEnd;
    
    BS_DBGASSERT(NULL != pucTxtBuf);
    BS_DBGASSERT(NULL != puiLineLen);
    BS_DBGASSERT(NULL != ppucLineNext);

    spucLineEnd = strchr(pucTxtBuf, '\n');
    if (spucLineEnd == NULL)
    {
        spucLineEnd = strchr(pucTxtBuf, '\r');
    }

    if (spucLineEnd == NULL)    
    {
        *pbIsFoundLineEndFlag = FALSE;
        *puiLineLen = strlen(pucTxtBuf);
        *ppucLineNext = NULL;
        return BS_OK;
    }

    *pbIsFoundLineEndFlag = TRUE;

    
    if (spucLineEnd > pucTxtBuf)    
    {
        if ((spucLineEnd[-1] == '\r') || (spucLineEnd[-1] == '\n'))
        {
            *puiLineLen = spucLineEnd - pucTxtBuf - 1;
        }
        else
        {
            *puiLineLen = spucLineEnd - pucTxtBuf;
        }
    }
    else
    {
        *puiLineLen = 0;
    }

    if (spucLineEnd[1] == '\0')
    {
        *ppucLineNext = NULL;
    }
    else
    {
        *ppucLineNext = spucLineEnd + 1;
    }

    return BS_OK;
}


BS_STATUS TXT_N_GetLine(IN CHAR *pucTxtBuf, IN UINT ulLen, OUT UINT *pulLineLen, OUT BOOL_T *pbIsFoundLineEndFlag, OUT CHAR **ppucLineNext)
{
    CHAR *spucLineEnd;
    
    BS_DBGASSERT(NULL != pucTxtBuf);
    BS_DBGASSERT(NULL != pulLineLen);
    BS_DBGASSERT(NULL != ppucLineNext);

    spucLineEnd = TXT_Strnchr(pucTxtBuf, '\n', ulLen);

    if (spucLineEnd == NULL)    
    {
        *pbIsFoundLineEndFlag = FALSE;
        *pulLineLen = ulLen;
        *ppucLineNext = NULL;
        return BS_OK;
    }

    *pbIsFoundLineEndFlag = TRUE;

    
    if (spucLineEnd > pucTxtBuf)    
    {
        if (spucLineEnd[-1] == '\r')
        {
            *pulLineLen = spucLineEnd - pucTxtBuf - 1;
        }
        else
        {
            *pulLineLen = spucLineEnd - pucTxtBuf;
        }
    }

    if (spucLineEnd[1] == '\0')
    {
        *ppucLineNext = NULL;
    }
    else
    {
        *ppucLineNext = spucLineEnd + 1;
    }

    return BS_OK;
}


CHAR * TXT_MStrnchr
(
    IN CHAR *pcString,
    IN UINT uiStringLen,
    IN CHAR *pcToFind
)
{
    CHAR *pcStringEnd;
    CHAR *pcChar;
    CHAR *pcFound = NULL;
    UINT uiPatternLen;

    pcStringEnd = pcString + uiStringLen;
    pcChar = pcString;
    uiPatternLen = (UINT)strlen(pcToFind);

    while (pcChar < pcStringEnd)
    {
        if (TXT_Strnchr(pcToFind, *pcChar, uiPatternLen) != NULL)
        {
            pcFound = pcChar;
            break;
        }

        pcChar ++;
    }

    return pcFound;
}

char * TXT_MStrchr(char *string, char *to_finds)
{
    return TXT_MStrnchr(string, strlen(string), to_finds);
}


char * TXT_Invert(char *in, char *out)
{
    int len = strlen(in);

    MEM_Invert(in, len, out);
    out[len] = '\0';

    return out;
}


CHAR * TXT_ReverseStrnchr(CHAR *pcBuf, CHAR ch2Find, UINT uiLen)
{
    LSTR_S lstr;
    lstr.pcData = pcBuf;
    lstr.uiLen = uiLen;
    return LSTR_ReverseStrchr(&lstr, ch2Find);
}


CHAR * TXT_ReverseStrchr(IN CHAR *pcBuf, IN CHAR ch2Find)
{
    LSTR_S lstr;
    lstr.pcData = pcBuf;
    lstr.uiLen = strlen(pcBuf);
    return LSTR_ReverseStrchr(&lstr, ch2Find);
}

CHAR *TXT_Strnstr(IN CHAR *s1, IN CHAR *s2, IN ULONG ulLen) 
{
	ULONG ulLen2;

	ulLen2 = strlen(s2);

	if (ulLen2 == 0)
	{
		return s1;
	}

	while (ulLen >= ulLen2)
    {
		ulLen--;

		if (!memcmp(s1, s2, ulLen2))
		{
			return s1;
		}

		s1++;
	}

	return NULL;
}

char * TXT_Strnistr(char *s1, char *s2, ULONG ulLen) 
{
	ULONG ulLen2;

	ulLen2 = strlen(s2);

	if (ulLen2 == 0) {
		return s1;
	}

	while (ulLen >= ulLen2) {
		ulLen--;

        if (0 == strnicmp(s1, s2, ulLen2)) {
            return s1;
        }

		s1++;
	}

	return NULL;
}

BS_STATUS TXT_Atoll(CHAR *pszBuf, OUT INT64 *var)
{
    if ((pszBuf == NULL) || (pszBuf[0] == '\0')) {
        RETURN(BS_BAD_PTR);
    }

	if (FALSE == CTYPE_IsNumString(pszBuf)) {
		RETURN(BS_ERR);
	}

    *var = strtoll(pszBuf, NULL, 10);

    return BS_OK;
}

BS_STATUS TXT_AtouiWithCheck(IN CHAR *pszBuf, OUT UINT *puiNum)
{
    BS_STATUS eRet;

    
    if (strlen(pszBuf) >= sizeof("4294967295")) {
        RETURN(BS_OUT_OF_RANGE);
    }

    eRet = TXT_Atoui(pszBuf, puiNum);
    if (eRet != BS_OK) {
        return eRet;
    }

    return BS_OK;
}


BS_STATUS TXT_XAtoui(IN CHAR *pszBuf, OUT UINT *pulNum)
{
    if ((pszBuf == NULL) || (pszBuf[0] == '\0')) {
        RETURN(BS_BAD_PTR);
    }

    *pulNum = strtoul(pszBuf, NULL, 16);

    return BS_OK;
}

long TXT_Strtol(char *str, int base)
{
    return strtol(str, NULL, base);
}

CHAR TXT_Random(void)
{
    
    UINT uiRandom;

    uiRandom = RAND_Get() % 62;

    if (uiRandom < 26)
    {
        return uiRandom + 'A';
    }

    if (uiRandom < 52)
    {
        return uiRandom - 26 + 'a';
    }

    return uiRandom - 52 + '0';
}


CHAR * TXT_StrchrX(IN CHAR *pszStr, IN CHAR pcToFind, IN UINT ulNum)
{
    CHAR *pcFind = NULL;
    
    if (NULL == pszStr)
    {
        return NULL;
    }

    pcFind = pszStr;
    while (ulNum > 0)
    {
        pcFind = strchr(pcFind, pcToFind);
        if (NULL == pcFind)
        {
            break;
        }
		ulNum--;
		if (ulNum > 0)
		{
			pcFind++;
		}        
    }

    return pcFind;
}

BS_STATUS TXT_FindBracket
(
    IN CHAR *pszString,
    IN UINT ulLen,
    IN CHAR *pszBracket,
    OUT CHAR **ppcStart,
    OUT CHAR **ppcEnd
)
{
    UINT ulCount = 0;
    UINT i;
    CHAR *pcStart, *pcEnd;
    
    BS_DBGASSERT(NULL != pszString);

    pcStart = NULL;
    pcEnd = NULL;

    for (i=0; i<ulLen; i++)
    {
        if (pszString[i] == pszBracket[0])
        {
            if (pcStart == NULL)
            {
                pcStart = pszString + i;
            } 
            ulCount++;
        }
        else if (pszString[i] == pszBracket[1])
        {
            if (ulCount == 0)
            {
                RETURN(BS_ERR);
            }
            
            ulCount--;

            if (ulCount == 0)
            {
                pcEnd = pszString + i;
                break;
            }
        }
    }

    if (pcEnd == NULL)
    {
        RETURN(BS_ERR);
    }

    *ppcStart = pcStart;
    *ppcEnd = pcEnd;

    return BS_OK;
}

UINT TXT_CountCharNum(IN CHAR *pszString, IN CHAR cCharToCount)
{
    UINT ulCount = 0;
    UINT i = 0;

    while (pszString[i] != '\0')
    {
        if (pszString[i] == cCharToCount)
        {
            ulCount ++;
        }
        i++;
    }

    return ulCount;    
}

VOID TXT_Strlwr(INOUT CHAR *pszString)
{
    CHAR *pcTemp;
    
    BS_DBGASSERT(NULL != pszString);

    pcTemp = pszString;

    while (*pcTemp != '\0')
    {
        if (NUM_IN_RANGE(*pcTemp, 'A', 'Z'))
        {
            *pcTemp += 'a' - 'A';
        }

        pcTemp ++;
    }
}


UINT TXT_Strlcpy(IN CHAR *pcDest, IN CHAR *pcSrc, IN UINT uiSize)
{
    ULONG n;
    CHAR *p;

    for (p = pcDest, n = 0; n + 1 < uiSize && *pcSrc != '\0';  ++p, ++pcSrc, ++n)
    {
        *p = *pcSrc;
    }
    *p = '\0';
    if(*pcSrc == '\0')
    {
        return n;
    }
    else
    {
        return n + strlen (pcSrc);
    }
}



ULONG TXT_Strlcat(char *dst, const char *src, ULONG siz)
{
    char *d = dst;
    const char *s = src;
    ULONG n = siz;
    ULONG dlen;

    
    while (n-- != 0 && *d != '\0')
    {
        d++;
    }
    
    dlen = d - dst;
    n = siz - dlen;

    if (n == 0)
    {
        return(dlen + strlen(s));
    }

    while (*s != '\0')
    {
        if (n != 1)
        {
            *d++ = *s;
            n--;
        }
        s++;
    }

    *d = '\0';

    return (dlen + (s - src));    
}



BOOL_T TXT_InsertChar(IN CHAR *pcDest, IN UINT uiOffset, IN CHAR cInsertChar)
{
    UINT uiLen;
    UINT i;

    uiLen = strlen(pcDest);
    if (uiLen < uiOffset)
    {
        return FALSE;
    }

    for (i=uiLen; i>uiOffset; i--)
    {
        pcDest[i] = pcDest[i-1];
    }

    pcDest[uiOffset] = cInsertChar;
    pcDest[uiLen + 1] = '\0';

    return TRUE;
}


BOOL_T TXT_RemoveChar(IN CHAR *pcDest, IN UINT uiOffset)
{
    UINT uiLen;
    UINT i;

    uiLen = strlen(pcDest);
    if (uiLen <= uiOffset)
    {
        return FALSE;
    }

    for (i=uiOffset; i<uiLen; i++)
    {
        pcDest[i] = pcDest[i+1];
    }

    return TRUE;    
}


ULONG TXT_GetSamePrefixLen(IN CHAR *pcStr1, IN CHAR *pcStr2)
{
    ULONG ulLen = 0;

    while ((pcStr1[ulLen] != '\0') && (pcStr1[ulLen] == pcStr2[ulLen]))
    {
        ulLen ++;
    }

    return ulLen;
}

BS_STATUS TXT_StrCpy(IN CHAR *pszDest, IN CHAR *pszSrc)
{
    strlcpy(pszDest, pszSrc, strlen(pszSrc) + 1);

    return BS_OK;
}

VOID TXT_MStrSplit(IN CHAR *pcString, IN CHAR *pcSplitChar, OUT LSTR_S * pstStr1, OUT LSTR_S * pstStr2)
{
    LSTR_S stString;

    stString.pcData = pcString;
    stString.uiLen = strlen(pcString);

    LSTR_MSplit(&stString, pcSplitChar, pstStr1, pstStr2);
}


char * TXT_Str2Translate(char *str, char *trans_char_sets, char *out, int out_size)
{
    char *c;
    char *dst;
    char *dst_end = out + out_size - 1;
    int count = strlen(trans_char_sets);

    c = str;
    dst = out;

    while (*c != '\0') {
        if (dst >= dst_end) {
            return NULL;
        }

        if (TXT_IsInRange(*c, (UCHAR*)trans_char_sets, count)) {
            *dst = '\\';
            dst++;
            if (dst >= dst_end) {
                return NULL;
            }
        }
        *dst = *c;
        dst++;
        c++;
    }

    *dst = '\0';

    return out;
}


char * TXT_Num2BitString(uint64_t v, int min_len, OUT char *str)
{
    int i, j = 0;
    int flag = 0;

    for(i=63; i>=0; i--) {
        if (i < min_len) {
            flag = 1;
        }
        if (v & (1ULL << i)) {
            flag = 1;
        }
        if (flag) {
            str[j] = '0';
            if (v & (1ULL << i)) {
                str[j] = '1';
            }
            j++;
        }
    }

    str[j] = '\0';

    return str;
}


