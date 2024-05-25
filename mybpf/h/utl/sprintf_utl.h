/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2012-4-29
* Description: 
* History:     
******************************************************************************/

#ifndef __SPRINTF_UTL_H_
#define __SPRINTF_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

struct printf_spec {
	UCHAR	type;		
	UCHAR	flags;		
	UCHAR	base;		
	UCHAR	qualifier;
    SHORT	field_width;
	SHORT	precision;	
};

typedef struct {
    struct printf_spec spec;
    char *fmt;
    int read;
}FormatCompile_ELE_S;

#define SPRINTF_COMPILE_ELE_NUM 128

typedef struct {
    FormatCompile_ELE_S ele[SPRINTF_COMPILE_ELE_NUM];
}FormatCompile_S;

INT BS_Printf(const char *fmt, ...);
INT BS_Sprintf(char * buf, const char *fmt, ...);
INT BS_Snprintf(char * buf, IN INT iLen, const char *fmt, ...);
INT BS_Vsnprintf(char *buf, size_t size, const char *fmt, va_list args);
int BS_Scnprintf(char *buf, int size, const char *fmt, ...);

int BS_FormatCompile(FormatCompile_S *fc, char *fmt);
int BS_VFormat(FormatCompile_S *fc, char *buf, size_t size, va_list args);
int BS_Format(FormatCompile_S *fc, char * buf, ...);
int BS_FormatN(FormatCompile_S *fc, char *buf, int size, ...);


#define scnprintf BS_Scnprintf


#define SNPRINTF(buf,size, ...) ({ \
        int _nlen = snprintf((buf), (size), ##__VA_ARGS__); \
        if (_nlen >= (size)) _nlen = -1; \
        _nlen; })

#ifdef __cplusplus
    }
#endif 

#endif 


