/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2007-10-25
* Description: 
* History:     
******************************************************************************/
#include "bs.h"
#include "utl/mem_utl.h"
#include "utl/data2hex_utl.h"
#include "utl/ulc_user.h"

static inline void _mem_swap(void *buf1, void *buf2, int len)
{
    unsigned char *d1 = buf1;
    unsigned char *d2 = buf2;
    unsigned char tmp;
    int i;

    for (i=0; i<len; i++) {
        tmp = d1[i];
        d1[i] = d2[i];
        d2[i] = tmp;
    }
}

void * MEM_FindOneOf(void *mem, UINT mem_len, void *to_finds, UINT to_finds_len)
{
    UCHAR *data = mem;
    UCHAR *end = data + mem_len;

    BS_DBGASSERT(NULL != data);

    while (data < end) {
        if (MEM_FindOne(to_finds, to_finds_len, *data)) {
            return data;
        }
        data ++;
    }

    return NULL;
}

void * MEM_FindOne(void *mem, UINT mem_len, UCHAR to_find)
{
    UCHAR *data = mem;
    UCHAR *end = data + mem_len;

    BS_DBGASSERT(NULL != data);

    while (data < end) {
        if (*data == to_find) {
            return data;
        }
        data ++;
    }

    return NULL;
}

void * MEM_Find(IN VOID *pMem, IN UINT ulMemLen, IN VOID *pMemToFind, IN UINT ulMemToFindLen)
{
    UCHAR *data = pMem;
    UCHAR *end;

    BS_DBGASSERT(NULL != pMem);
    BS_DBGASSERT(NULL != pMemToFind);
    BS_DBGASSERT(ulMemToFindLen > 0);
    
    if (ulMemToFindLen > ulMemLen) {
        return NULL;
    }

    end = data + (ulMemLen - ulMemToFindLen) + 1;

    for (; data < end; data++) {
        if (*data != *(UCHAR*)pMemToFind) {
            continue;
        }

        if (memcmp(data, pMemToFind, ulMemToFindLen) == 0) {
            return data;
        }
    }

    return NULL;
}

void * MEM_CaseFind(void *pMem, UINT ulMemLen, void *pMemToFind, UINT ulMemToFindLen)
{
    UCHAR *data = pMem;
    UCHAR *end;
    UCHAR ch;

    BS_DBGASSERT(NULL != pMem);
    BS_DBGASSERT(NULL != pMemToFind);
    BS_DBGASSERT(ulMemToFindLen > 0);
    
    if (ulMemToFindLen > ulMemLen) {
        return NULL;
    }

    end = data + (ulMemLen - ulMemToFindLen) + 1;

    UCHAR first = *(UCHAR*)pMemToFind;
    first = tolower(first);

    for (; data < end; data++) {
        ch = tolower(*data);
        if (ch != first) {
            continue;
        }

        if (MEM_CaseCmp(data, ulMemToFindLen, pMemToFind, ulMemToFindLen) == 0) {
            return data;
        }
    }

    return NULL;
}

int MEM_CaseCmp(UCHAR *pucMem1, UINT uiMem1Len, UCHAR *pucMem2, UINT uiMem2Len)
{
    int c1;
    int c2;
    int i;
    UINT   uiLen   = MIN(uiMem1Len, uiMem2Len);
    UCHAR  *tmp1 = pucMem1;
    UCHAR  *tmp2 = pucMem2;

    for (i=0; i<uiLen; i++) {
        c1 = tolower(tmp1[i]);
        c2 = tolower(tmp2[i]);
        if (c1 != c2) {
            return (c1 - c2);
        }
    }

    if (uiMem1Len == uiMem2Len) {
        return 0;
    }

    if (uiMem1Len > uiMem2Len) {
        return 1;
    }

    return -1;
}


int MEM_SprintCFromat(void *mem, UINT mem_len, OUT char *buf, int buf_size)
{
    UCHAR *d = mem;
    char info[64];
    int len;
    int reserved_size = buf_size;
    int copyed_len = 0;
    int print_len = 0;

    
    while ((mem_len > print_len) && (reserved_size > 6)) {
        sprintf(info, "0x%02x,", *d);
        len = strlcpy(buf + copyed_len, info, reserved_size);
        reserved_size -= len;
        copyed_len += len;
        print_len ++;
        d ++;
        if ((print_len) && ((print_len % 16) == 0)) {
            len = strlcpy(buf + copyed_len, (char*)"\n", reserved_size);
            reserved_size -= len;
            copyed_len += len;
        }
    }

    if ((print_len) && ((print_len % 16) != 0)) {
        strlcpy(buf + copyed_len, (char*)"\n", reserved_size);
    }

    return print_len;
}


int MEM_Sprint(void *mem, UINT mem_len, OUT char *buf, int buf_size)
{
    UCHAR *d = mem;
    char info[64];
    int len = 0;
    int reserved_size = buf_size;
    int copyed_len = 0;
    int print_len = 0;

    
    while ((mem_len > print_len) && (reserved_size > 3)) {
        sprintf(info, "%02x ", *d);
        len = strlcpy(buf + copyed_len, info, reserved_size);
        reserved_size -= len;
        copyed_len += len;
        print_len ++;
        d ++;
        if ((print_len) && ((print_len % 16) == 0)) {
            buf[copyed_len - 1] = '\n';
        }
    }

    if ((print_len) && ((print_len % 16) != 0)) {
        buf[copyed_len - 1] = '\n';
    }

    return print_len;
}

void MEM_Print(void *mem, int len, PF_MEM_PRINT_FUNC print_func)
{
    char info[3*16+1];
    int print_len = 0;

    while (len > print_len) {
        print_len += MEM_Sprint(mem + print_len, len - print_len, info, sizeof(info));
        if (print_func) {
            print_func(info);
        } else {
            printf("%s", info);
        }
    }
}

void MEM_PrintCFormat(void *mem, int len, PF_MEM_PRINT_FUNC print_func)
{
    char info[5*16+2];
    int print_len = 0;

    while (len > print_len) {
        print_len += MEM_SprintCFromat(mem + print_len, len - print_len, info, sizeof(info));
        if (print_func) {
            print_func(info);
        } else {
            printf("%s", info);
        }
    }
}

VOID MEM_DiscreteFindInit(INOUT MEM_FIND_INFO_S *pstFindInfo, IN UCHAR *pucPattern, IN UINT uiPatternLen)
{
    Mem_Zero(pstFindInfo, sizeof(MEM_FIND_INFO_S));

    pstFindInfo->pucPattern = pucPattern;
    pstFindInfo->uiPatternLen = uiPatternLen;
}


BS_STATUS MEM_DiscreteFind
(
    INOUT MEM_FIND_INFO_S *pstFindInfo,
    IN UCHAR *pucData,
    IN UINT uiDataLen,
    OUT UINT *puiFindOffset
)
{
    UCHAR *pucTmp;
    UCHAR *pucEnd;
    BOOL_T bFound = FALSE;

    pucTmp = pucData;
    pucEnd = pucData + uiDataLen;

    while (pucTmp < pucEnd)
    {
        if (*pucTmp != pstFindInfo->pucPattern[pstFindInfo->uiPatternCmpLen])
        {
            pstFindInfo->uiPatternCmpLen = 0;
        }
        else
        {
            if (pstFindInfo->uiPatternCmpLen == 0)
            {
                pstFindInfo->uiCmpMemOffset = pstFindInfo->uiPreMemTotleSize + (pucTmp - pucData);
            }

            pstFindInfo->uiPatternCmpLen ++;

            if (pstFindInfo->uiPatternCmpLen == pstFindInfo->uiPatternLen)
            {
                *puiFindOffset = pstFindInfo->uiCmpMemOffset;
                bFound = TRUE;
                break;
            }
        }

        pucTmp ++;
    }

    if (bFound == TRUE)
    {
        return BS_OK;
    }

    pstFindInfo->uiPreMemTotleSize += uiDataLen;

    return BS_NOT_FOUND;
}


void MEM_Invert(void *in, int len, void *out)
{
    int i;
    unsigned char * inc = (unsigned char*)in + (len -1);
    unsigned char * outc = out;

    for (i=0; i<len; i++) {
        *outc = *inc;
        outc ++;
        inc --;
    }
}


int MEM_IsZero(void *data, int size)
{
    int i;
    unsigned char *tmp = data;

    for (i=0; i<size; i++) {
        if (tmp[i] != 0) {
            return 0;
        }
    }

    return 1;
}


int MEM_IsFF(void *data, int size)
{
    int i;
    unsigned char *tmp = data;

    for (i=0; i<size; i++) {
        if (tmp[i] != 0xff) {
            return 0;
        }
    }

    return 1;
}


int MEM_ReplaceChar(void *data, int len, UCHAR src, UCHAR dst)
{
    int i;
    int count = 0;
    UCHAR *tmp = data;

    for (i=0; i<len; i++) {
        if (tmp[i] == src) {
            tmp[i] = dst;
            count ++;
        }
    }

    return count;
}


int MEM_ReplaceOneChar(void *data, int len, UCHAR src, UCHAR dst)
{
    int i;
    UCHAR *tmp = data;

    for (i=0; i<len; i++) {
        if (tmp[i] == src) {
            tmp[i] = dst;
            return 1;
        }
    }

    return 0;
}


void MEM_Swap(void *buf1, void *buf2, int len)
{
    _mem_swap(buf1, buf2, len);
}



int MEM_SwapByOff(void *buf, int buf_len, int off)
{
    if (off >= buf_len) {
        return 0;
    }

    if (off * 2 == buf_len) { 
        _mem_swap(buf, (char*)buf + off, off);
        return 0;
    }

    void *tmp = MEM_Malloc(off);
    if (! tmp) {
        RETURN(BS_NO_MEMORY);
    }

    memcpy(tmp, buf, off);

    memmove(buf, (char*)buf + off, buf_len - off);
    memcpy((char*)buf + (buf_len - off), tmp, off);

    MEM_Free(tmp);

    return 0;
}




int MEM_MoveData(void *data, S64 len, S64 offset)
{
    if (offset == 0) {
        return 0;
    }

    void *tmp = MEM_Malloc(len);
    if (! tmp) {
        RETURN(BS_NO_MEMORY);
    }

    memcpy(tmp, data, len);

    if (offset < 0) { 
        char *d = (char*)data + offset;
        memmove(d + len, d, -offset);
    } else { 
        char *d = (char*)data + len;
        memmove(data, d, offset);
    }

    memcpy((char*)data + offset, tmp, len);

    MEM_Free(tmp);

    return 0;
}




int MEM_MoveDataTo(void *data, U64 len, void *dst)
{
    return MEM_MoveData(data, len, (S64)dst - (S64)data);
}

void MEM_CopyWithCheck(void *dst, void *src, U32 len)
{
    
    {
        char *d1_min = dst;
        char *d1_max = (d1_min + len) - 1;
        char *d2_min = src;
        char *d2_max = (d2_min + len) - 1;
        if (NUM_AREA_IS_OVERLAP(d1_min, d1_max, d2_min, d2_max) != FALSE) {
            BS_DBGASSERT(0);
        }
    }
    memcpy(dst, src, len);
}

