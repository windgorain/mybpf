#ifndef __CTYPE_UTL_H_
#define __CTYPE_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

#define ISSPACE(c)	(((c) == ' ') || ((c) == '\t'))
#define ISDIGIT(c)	('0' <= (c) && (c) <= '9')
#define ISLOWER(c)	('a' <= (c) && (c) <= 'z')
#define TOLOWER(c)  ((c) | 0x20)
#define ISUPPER(c)  (((c) >= 'A') && ((c) <= 'Z'))

static inline int ISXDIGIT(int ch)
{
	if (ISDIGIT(ch))
		return TRUE;

	if ((ch >= 'a') && (ch <= 'f'))
		return TRUE;

	return (ch >= 'A') && (ch <= 'F');
}


static inline BOOL_T CTYPE_IsXDigit(UCHAR ucChar)
{
    if (('0' <= ucChar) && ('9' >= ucChar))
    {
        return TRUE;
    }

    if (('a' <= ucChar) && ('f' >= ucChar))
    {
        return TRUE;
    }

    if (('A' <= ucChar) && ('F' >= ucChar))
    {
        return TRUE;
    }

    return FALSE;
}

BOOL_T CTYPE_IsNumString(CHAR *pcString);

#ifdef __cplusplus
    }
#endif 

#endif 


