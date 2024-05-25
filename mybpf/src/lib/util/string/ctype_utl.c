/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#include "bs.h"
#include "utl/ctype_utl.h"

BOOL_T CTYPE_IsNumString(CHAR *pcString)
{
    ULONG ulLen;
    ULONG i;

    if (NULL == pcString) {
        return FALSE;
    }

    ulLen = strlen(pcString);

    for (i=0; i<ulLen; i++) {
        if ((pcString[i] < '0') || (pcString[i] > '9')) {
            return FALSE;
        }
    }

    return TRUE;
}

