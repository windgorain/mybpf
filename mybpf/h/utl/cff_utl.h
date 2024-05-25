/******************************************************************************
* Copyright (C), LiXingang
* Author:      LiXingang  Version: 1.0  Date: 2015-6-23
* Description: 
* History:     
******************************************************************************/

#ifndef __CFF_UTL_H_
#define __CFF_UTL_H_

#include "utl/mkv_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 


typedef HANDLE CFF_HANDLE;

#define CFF_FLAG_CREATE_IF_NOT_EXIST   0x1
#define CFF_FLAG_SORT                  0x2
#define CFF_FLAG_READ_ONLY             0x4
#define CFF_FLAG_UTF8_BOM              0x8  


typedef VOID (*PF_CFF_TAG_WALK_FUNC)(IN HANDLE hIniHandle, IN CHAR *pcTag, IN HANDLE hUsrHandle);
typedef VOID (*PF_CFF_PROP_WALK_FUNC)(IN HANDLE hIniHandle, IN CHAR *pcTag, IN CHAR *pcProp, IN HANDLE hUsrHandle);


#define CFF_X_SCAN_TAG_START(_pstTagRoot, _pstTagNode)	MKV_SCAN_MARK_START(_pstTagRoot, _pstTagNode)
#define CFF_X_SCAN_PROP_START(_pstTagRoot, _pstPropNode)	MKV_SCAN_KEY_START(_pstTagRoot, _pstPropNode)
#define CFF_X_SCAN_END()	MKV_SCAN_END()


#define CFF_SCAN_TAG_START(_hCffHandle, _pcTagName)	\
    do {    \
        MKV_MARK_S *_pstTagRoot, *_pstTagNode;  \
        MKV_X_PARA_S _stTreParam;    \
        _stTreParam.ulLevle = 0;     \
        _pstTagRoot = CFF_X_GetTag(_hCffHandle, &_stTreParam); \
        if (NULL != _pstTagRoot)    \
        {\
            CFF_X_SCAN_TAG_START(_pstTagRoot,_pstTagNode) \
            {   \
                _pcTagName = _pstTagNode->pucMarkName;   \
                {


#define CFF_SCAN_PROP_START(_hCffHandle, _pcTagName, _pcProp, _pcValue)	\
    do {    \
        MKV_MARK_S *_pstTagRoot;     \
		MKV_KEY_S  *_pstPropNode;	\
        MKV_X_PARA_S _stTreParam;    \
        _stTreParam.ulLevle = 1;     \
        _stTreParam.apszMarkName[0] = _pcTagName;  \
        _pstTagRoot = CFF_X_GetTag(_hCffHandle, &_stTreParam); \
        if (NULL != _pstTagRoot)    \
        {   \
            CFF_X_SCAN_PROP_START(_pstTagRoot,_pstPropNode) \
            {   \
                _pcProp = _pstPropNode->pucKeyName;   \
                _pcValue = _pstPropNode->pucKeyValue; \
                {


#define CFF_SCAN_END()	\
                }   \
    		} CFF_X_SCAN_END(); \
        }   \
	}while (0)

CFF_HANDLE CFF_INI_Open(IN CHAR *pcFilePath, IN UINT uiFlag);
CFF_HANDLE CFF_TRE_Open(IN CHAR *pcFilePath, IN UINT uiFlag);
CFF_HANDLE CFF_MCF_Open(IN CHAR *pcFilePath, IN UINT uiFlag);
CFF_HANDLE CFF_BRACE_Open(IN CHAR *pcFilePath, IN UINT uiFlag);

CFF_HANDLE CFF_INI_OpenBuf(IN CHAR *buf, IN UINT uiFlag);
CFF_HANDLE CFF_TRE_OpenBuf(IN CHAR *buf, IN UINT uiFlag);
CFF_HANDLE CFF_MCF_OpenBuf(IN CHAR *buf, IN UINT uiFlag);
CFF_HANDLE CFF_BRACE_OpenBuf(IN CHAR *buf, IN UINT uiFlag);


VOID CFF_INI_SetAs(IN CFF_HANDLE hCff);
VOID CFF_TRE_SetAs(IN CFF_HANDLE hCff);
VOID CFF_MCF_SetAs(IN CFF_HANDLE hCff);
VOID CFF_BRACE_SetAs(IN CFF_HANDLE hCff);

VOID CFF_Close(IN CFF_HANDLE hCffHandle);

BS_STATUS CFF_Show(IN CFF_HANDLE hCffHandle);
BS_STATUS CFF_Save2Buf(IN CFF_HANDLE hCffHandle, IN char *buf, IN int buf_size);
BS_STATUS CFF_Save(IN CFF_HANDLE hCffHandle);
BS_STATUS CFF_SaveAs(IN CFF_HANDLE hCffHandle, IN char *filePath);

BS_STATUS CFF_DelProp(IN CFF_HANDLE hCffHandle, IN CHAR *pcTag, IN CHAR *pcProp);
BS_STATUS CFF_DelAllProp(IN CFF_HANDLE hCffHandle, IN CHAR *pcTag);
BS_STATUS CFF_DelTag(IN CFF_HANDLE hCffHandle, IN CHAR *pcTag);
BS_STATUS CFF_AddTag(IN CFF_HANDLE hCffHandle, IN CHAR *pcTag);
char * CFF_GetTagDuplicate(IN CFF_HANDLE hCffHandle);
BS_STATUS CFF_GetPropAsString(IN CFF_HANDLE hCffHandle, IN CHAR *pcTag, IN CHAR *pcProp, OUT CHAR **ppcValue);
int CFF_CopyPropAsString(CFF_HANDLE hCffHandle, CHAR *pcTag, CHAR *pcProp, OUT CHAR *value, int value_size);
BOOL_T CFF_IsPropExist(IN CFF_HANDLE hCffHandle, IN CHAR *pcTag, IN CHAR *pcProp);
CHAR * CFF_GetNextTag(IN CFF_HANDLE hCffHandle, IN CHAR *pcCurrentTag);
BS_STATUS CFF_GetNextProp(IN CFF_HANDLE hCffHandle, IN CHAR *pcTag, INOUT CHAR **ppcProp);
BOOL_T CFF_IsTagExist(IN CFF_HANDLE hCffHandle, IN CHAR *pcTag);
BS_STATUS CFF_SetPropAsString(IN CFF_HANDLE hCffHandle, IN CHAR *pcTag, IN CHAR *pcProp, IN CHAR *pcValue);
BS_STATUS CFF_SetPropAsUint(IN CFF_HANDLE hCffHandle, IN CHAR *pcTag, IN CHAR *pcProp, IN UINT uiValue);
BS_STATUS CFF_SetPropAsUint64(IN CFF_HANDLE hCffHandle, IN CHAR *pcTag, IN CHAR *pcProp, IN UINT64 uiValue);
BS_STATUS CFF_GetPropAsUint64(IN CFF_HANDLE hCffHandle, IN CHAR *pcTag, IN CHAR *pcProp, OUT UINT64 *puiValue);
BS_STATUS CFF_GetPropAsUint(IN CFF_HANDLE hCffHandle, IN CHAR *pcTag, IN CHAR *pcProp, OUT UINT *puiValue);
BS_STATUS CFF_GetPropAsInt(IN CFF_HANDLE hCffHandle, IN CHAR *pcTag, IN CHAR *pcProp, OUT INT *piValue);


UINT CFF_GetPropAsUintDft(CFF_HANDLE hCffHandle, char *pcTag, char *pcProp, UINT dft);
int CFF_GetPropAsIntDft(CFF_HANDLE hCffHandle, char *pcTag, char *pcProp, int dft);
UINT64 CFF_GetPropAsUint64Dft(CFF_HANDLE hCffHandle, char *pcTag, char *pcProp, UINT64 dft);
char * CFF_GetPropAsStringDft(CFF_HANDLE hCffHandle, char *pcTag, char *pcProp, char *dft);


UINT CFF_GetTagNum(IN CFF_HANDLE hCffHandle);
VOID CFF_WalkTag(IN CFF_HANDLE hCffHandle, IN PF_CFF_TAG_WALK_FUNC pfFunc, IN HANDLE hUsrHandle);
VOID CFF_WalkProp(IN CFF_HANDLE hCffHandle, IN CHAR *pcTag, IN PF_CFF_PROP_WALK_FUNC pfFunc, IN HANDLE hUsrHandle);

UINT CFF_GetPorpNumOfTag(IN CFF_HANDLE hCffHandle, IN CHAR *pcTag);


BS_STATUS CFF_X_DelProp(IN CFF_HANDLE hCffHandle, IN MKV_X_PARA_S *pstTags, IN CHAR *pcProp);
BS_STATUS CFF_X_DelAllPropOfTag(IN CFF_HANDLE hCffHandle, IN MKV_X_PARA_S *pstTags);
BS_STATUS CFF_X_DelTag(IN CFF_HANDLE hCffHandle, IN MKV_X_PARA_S *pstTags);
BS_STATUS CFF_X_AddTag(IN CFF_HANDLE hCffHandle, IN MKV_X_PARA_S *pstTags);
MKV_MARK_S * CFF_X_GetTag(IN CFF_HANDLE hCffHandle, IN MKV_X_PARA_S *pstTags);
BOOL_T CFF_X_IsTagExist(IN CFF_HANDLE hCffHandle, IN MKV_X_PARA_S *pstTags);
CHAR * CFF_X_GetNextTag(IN CFF_HANDLE hCffHandle, IN MKV_X_PARA_S *pstTags, IN CHAR *pcCurTag);
BS_STATUS CFF_X_GetNextProp(IN CFF_HANDLE hCffHandle, IN MKV_X_PARA_S *pstTags, INOUT CHAR **ppcPropName);
BS_STATUS CFF_X_SetPropAsString
(
    IN CFF_HANDLE hCffHandle,
    IN MKV_X_PARA_S *pstTags,
    IN CHAR *pcPropName,
    IN CHAR *pcValue
);
BS_STATUS CFF_X_SetPropAsUint
(
    IN CFF_HANDLE hCffHandle,
    IN MKV_X_PARA_S *pstTags,
    IN CHAR *pcPropName,
    IN UINT uiValue
);
BS_STATUS CFF_X_SetPropAsUint64
(
    IN CFF_HANDLE hCffHandle,
    IN MKV_X_PARA_S *pstTags,
    IN CHAR *pcPropName,
    IN UINT64 uiValue
);
BS_STATUS CFF_X_GetPropAsString
(
    IN CFF_HANDLE hCffHandle,
    IN MKV_X_PARA_S *pstSections,
    IN CHAR *pcPropName,
    OUT CHAR **ppcValue
);
BS_STATUS CFF_X_GetPropAsUint64
(
    IN CFF_HANDLE hCffHandle,
    IN MKV_X_PARA_S *pstTags,
    IN CHAR *pcPropName,
    OUT UINT64 *puiKeyValue
);
BS_STATUS CFF_X_GetPropAsUint
(
    IN CFF_HANDLE hCffHandle,
    IN MKV_X_PARA_S *pstTags,
    IN CHAR *pcPropName,
    OUT UINT *puiKeyValue
);
BS_STATUS CFF_X_GetPropAsInt
(
    IN CFF_HANDLE hCffHandle,
    IN MKV_X_PARA_S *pstTags,
    IN CHAR *pcPropName,
    OUT INT *piValue
);
BOOL_T CFF_X_IsPropExist(IN CFF_HANDLE hCffHandle, IN MKV_X_PARA_S *pstTags, IN CHAR *pcPropName);
UINT CFF_X_GetTagNum(IN CFF_HANDLE hCffHandle, IN MKV_X_PARA_S *pstTags);
char * CFF_X_GetTagDuplicate(IN CFF_HANDLE hCffHandle, IN MKV_X_PARA_S *pstTags);

#ifdef __cplusplus
    }
#endif 

#endif 


