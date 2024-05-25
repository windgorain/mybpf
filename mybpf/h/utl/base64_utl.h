#ifndef __BASE64_UTL_H_
#define __BASE64_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */


#define BASE64_LEN(ulDataLen) (((ulDataLen) + 2) / 3 * 4)


#define BASE64_CLEAR_LEN(ulBase64Len)  ((((ulBase64Len) + 3) / 4) * 3)

int BASE64_Encode(IN UCHAR *pucData, IN UINT uiDataLen, OUT CHAR *pcOut);
int BASE64_Decode(IN CHAR *pcInput, IN UINT uiDataLen, OUT UCHAR *pucOutput);

#ifdef __cplusplus
    }
#endif 

#endif 



