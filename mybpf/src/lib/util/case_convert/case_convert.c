/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#include "bs.h"
#include "utl/case_convert.h"

static UCHAR g_case_convert_to_upper[256];
static UCHAR g_case_convert_to_lower[256];

static void caseconvert_Init()
{
    int i;
    for (i = 0; i < 256; i++) {
        g_case_convert_to_upper[i] = (UCHAR)toupper (i);
        g_case_convert_to_lower[i] = (UCHAR)tolower(i);
    }
}

void CaseConvert_ToUpper(UCHAR *dst, UCHAR *src, int len)
{
    int i;
    for (i = 0; i < len; i++) {
        dst[i] = g_case_convert_to_upper[src[i]];
    }
}

void CaseConvert_ToLower(UCHAR *dst, UCHAR *src, int len)
{
    int i;
    for (i = 0; i < len; i++) {
        dst[i] = g_case_convert_to_lower[src[i]];
    }
}

CONSTRUCTOR(init) {
    caseconvert_Init();
}
