/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2007-5-13
* Description: 
* History:     
******************************************************************************/

#ifndef __UTL_TXT_H_
#define __UTL_TXT_H_

#include "utl/sprintf_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 


#define TXT_CONST_STRCMP(name, str)  ({ \
        char _str[] = (str); \
        strcmp((name), _str); \
        })

#define TXT_IS_EMPTY(_pstStr) (((_pstStr) == NULL) || ((_pstStr[0]) == '\0'))

#define TXT_GET_SELF_OR_BLANK(_pstStr) (((_pstStr) == NULL) ? "" : (_pstStr))

#define TXT_BLANK_CHARS " \t\r\n"

#define TXT_SL(_x)  (sizeof(_x)-1)


#define TXT_IS_LINE_LAST_CHAR(ucChar) ((ucChar) == '\n')

#define TXT_IS_BLANK(x) (((x) == ' ') || ((x) == '\t'))

#define TXT_IS_BLANK_OR_RN(x) (TXT_IS_BLANK(x) || ((x) == '\r') || ((x) == '\n'))

#define TXT_ARGS_BUILD(_buf,_buf_size)  \
    do {    \
        va_list __args;   \
        va_start(__args, fmt);    \
        BS_Vsnprintf(_buf, _buf_size, fmt, __args);   \
        va_end(__args);   \
    }while(0)

#define TXT_ARGS_PRINT_LEN	1023
#define TXT_ARGS_PRINT(print_func, user_data)    \
    do {    \
        va_list args;   \
        char __msg[TXT_ARGS_PRINT_LEN + 1];  \
                        \
        va_start(args, fmt);    \
        BS_Vsnprintf(__msg, sizeof(__msg), fmt, args);   \
        va_end(args);           \
        print_func(__msg, user_data);                 \
    }while(0)

#define TXT_STRNCMP(_str1, _str2) strncmp((_str1), (_str2), TXT_SL(_str2))

extern BS_STATUS TXT_Lower(INOUT CHAR *pucTxtBuf);

extern BS_STATUS TXT_Upper(INOUT CHAR *pucTxtBuf);

extern VOID TXT_DelSubStr(IN CHAR *pucTxtBuf, IN CHAR *pucSubStr, OUT CHAR *pucTxtOutBuf, IN ULONG ulSize);


extern VOID TXT_DelLineComment
(
    IN CHAR *pcInBuf,
    IN CHAR *pcCommentFlag,  
    OUT CHAR *pcOutBuf
);


extern BS_STATUS TXT_GetLine(IN CHAR *pucTxtBuf, OUT UINT *puiLineLen, OUT BOOL_T *pbIsFoundLineEnd, OUT CHAR **ppucLineNext);


extern BS_STATUS TXT_N_GetLine(IN CHAR *pucTxtBuf, IN UINT ulLen, OUT UINT *pulLineLen, OUT BOOL_T *pbIsFoundLineEndFlag, OUT CHAR **ppucLineNext);

extern CHAR *TXT_Strnchr(IN CHAR *pcBuf, IN CHAR ch2Find, IN UINT ulLen);


extern CHAR * TXT_MStrnchr
(
    IN CHAR *pcString,
    IN UINT uiStringLen,
    IN CHAR *pcToFind
);

char * TXT_MStrchr(char *string, char *to_finds);


extern char * TXT_Invert(char *in, char *out);


extern CHAR * TXT_ReverseStrchr(IN CHAR *pcBuf, IN CHAR ch2Find);


extern CHAR * TXT_ReverseStrnchr(CHAR *pcBuf, CHAR cToFind, UINT len);

extern CHAR *TXT_Strnstr(IN CHAR *s1, IN CHAR *s2, IN ULONG ulLen) ;

extern char * TXT_Strnistr(char *s1, char *s2, ULONG ulLen);

extern BS_STATUS TXT_Atoui(IN CHAR *pszBuf, OUT UINT *pulNum);

extern BS_STATUS TXT_Atoll(CHAR *pszBuf, OUT INT64 *var);

extern BS_STATUS TXT_AtouiWithCheck(IN CHAR *pszBuf, OUT UINT *puiNum);

extern UINT TXT_Str2Ui(IN CHAR *pcStr);

extern BS_STATUS TXT_XAtoui(IN CHAR *pszBuf, OUT UINT *pulNum);

extern long TXT_Strtol(char *str, int base);

extern CHAR TXT_Random(void);
extern void TXT_ReplaceChar(INOUT char *pcTxtBuf, char from, char to);
extern VOID TXT_ReplaceSubStr(IN CHAR *pcTxtBuf, IN CHAR *pcSubStrFrom, IN CHAR *pcSubStrTo, OUT CHAR *pcTxtOutBuf, IN ULONG ulSize);

extern VOID TXT_ReplaceSubStrOnce
(
    IN CHAR *pcTxtBuf,
    IN CHAR *pcSubStrFrom,
    IN CHAR *pcSubStrTo,
    OUT CHAR *pcTxtOutBuf,
    IN ULONG ulOutSize
);

extern void TXT_N2RN(char *in, char *out, int out_size);

extern char * TXT_CompressLine(INOUT char * pcString);

extern CHAR * TXT_StrimHead(IN CHAR *pcData, IN ULONG ulDataLen, IN CHAR *pcSkipChars);

extern ULONG TXT_StrimTail(IN CHAR *pcData, IN ULONG ulDataLen, IN CHAR *pcSkipChars);

extern CHAR * TXT_StrimHeadTail(CHAR *pcData, ULONG ulDataLen, CHAR *pcSkipChars, OUT ULONG *pulNewLen);

extern CHAR * TXT_StrimString(IN CHAR *pcData, IN CHAR *pcSkipChars);


extern CHAR *TXT_Strim(IN CHAR *pszStr);


extern VOID TXT_StrimAndMove(IN CHAR *pszStr);

extern char * TXT_StrimAll(IN CHAR *pcStr);

extern char * TXT_StrimTo(char *in, char *out);

extern char * TXT_StrimAllTo(char *in, char *out);


extern UCHAR * TXT_FindFirstNonBlank(IN UCHAR *pszStr, IN UINT ulLen);

extern UCHAR *TXT_FindFirstNonSuch(IN UCHAR *pszStr, IN UINT ulLen, IN UCHAR *pszNoSuch, IN UINT ulNoSuchLen);

extern CHAR * TXT_FindOneOf(IN CHAR *pszStr, IN CHAR *pszPattern);

extern BOOL_T TXT_IsInRange(IN CHAR cCh, IN UCHAR *pszStringRange, IN UINT ulLen);

extern UINT TXT_StrToToken(IN CHAR *pszStr, IN CHAR *pszPatterns, OUT CHAR *apszArgz[], IN UINT uiMaxArgz);

extern UINT TXT_StrToToken2(IN CHAR *pszStr, IN CHAR *pszPatterns, OUT CHAR *apszArgz[], IN UINT uiMaxArgz);

extern UINT TXT_GetTokenNum(IN CHAR *pszStr, IN CHAR *pszPatterns);


extern HANDLE TXT_StrToDynamicToken(IN CHAR *pszStr, IN CHAR *pszPatterns);


extern CHAR * TXT_StrchrX(IN CHAR *pszStr, IN CHAR pcToFind, IN UINT ulNum);

extern UINT TXT_CountCharNum(IN CHAR *pszString, IN CHAR cCharToCount);

extern VOID TXT_Strlwr(INOUT CHAR *pszString);

extern char *TXT_Strdup(IN CHAR *pcStr);

extern BOOL_T TXT_EndcharMatch(char *string, char *pattern, int pattern_len, char end_char);

extern BS_STATUS TXT_StrCpy(IN CHAR *pszDest, IN CHAR *pszSrc);

extern UINT TXT_Strlcpy(IN CHAR *pcDest, IN CHAR *pcSrc, IN UINT uiSize);

extern ULONG TXT_Strlcat(char *dst, const char *src, ULONG siz);

extern VOID TXT_StrSplit(IN CHAR *pcString, IN CHAR cSplitChar, OUT LSTR_S * pstStr1, OUT LSTR_S * pstStr2);

extern VOID TXT_MStrSplit(IN CHAR *pcString, IN CHAR *pcSplitChar, OUT LSTR_S * pstStr1, OUT LSTR_S * pstStr2);

extern BOOL_T TXT_InsertChar(IN CHAR *pcDest, IN UINT uiOffset, IN CHAR cInsertChar);

extern BOOL_T TXT_RemoveChar(IN CHAR *pcDest, IN UINT uiOffset);

extern ULONG TXT_GetSamePrefixLen(IN CHAR *pcStr1, IN CHAR *pcStr2);

extern BS_STATUS TXT_FindBracket
(
    IN CHAR *pszString,
    IN UINT ulLen,
    IN CHAR *pszBracket,
    OUT CHAR **ppcStart,
    OUT CHAR **ppcEnd
);

extern char * TXT_Str2Translate(char *str, char *trans_char_sets, char *out, int out_size);


char * TXT_Num2BitString(uint64_t v, int min_len, OUT char *str);

#define TXT_SCAN_N_LINE_BEGIN(pszTxtBuf, uiBufLen, pszLine, uiLineLen) \
    do { \
        BOOL_T _bIsFoundLineEnd; \
        CHAR *_pucLineNext = NULL; \
        UINT _uiReservedLen; \
        UINT _uiBufLenTmp = uiBufLen; \
        for (pszLine=pszTxtBuf; pszLine!=NULL; pszLine=_pucLineNext) { \
            _uiReservedLen = _uiBufLenTmp - (pszLine - pszTxtBuf); \
            TXT_N_GetLine(pszLine, _uiReservedLen, &uiLineLen, &_bIsFoundLineEnd, &_pucLineNext);

#define TXT_SCAN_LINE_BEGIN(pszTxtBuf, pszLine, uiLineLen)  \
    do{   \
        BOOL_T _bIsFoundLineEnd; \
        CHAR *_pucLineNext = NULL;           \
        for (pszLine=pszTxtBuf; pszLine!=NULL; pszLine=_pucLineNext)  \
        {   \
            TXT_GetLine(pszLine, &uiLineLen, &_bIsFoundLineEnd, &_pucLineNext);

#define TXT_SCAN_LINE_END()     }}while(0)


#define TXT_SCAN_ELEMENT_BEGIN(pszTxtBuf, cSplitChar, pszElement)  \
    do {    \
        CHAR *_pc = pszTxtBuf;    \
        CHAR *_pcChanged;   \
        CHAR _cSaved = 0;   \
        do {    \
            pszElement = _pc;   \
            _pc = strchr(_pc, cSplitChar); \
            if (NULL != _pc)  { \
                _cSaved = *_pc; \
                *_pc = '\0';    \
                _pcChanged = _pc;   \
                _pc++;  \
            }else{_pcChanged = NULL;}   \
            if (*pszElement != '\0') {

#define TXT_SCAN_ELEMENT_STOP()   {if (_pcChanged != NULL){*_pcChanged = _cSaved;}}
#define TXT_SCAN_ELEMENT_END()    }TXT_SCAN_ELEMENT_STOP()}while(_pc != NULL);}while(0)

#ifdef strlcpy
#undef strlcpy
#endif

#define strlcpy TXT_Strlcpy


#ifdef __cplusplus
    }
#endif 

#endif 


