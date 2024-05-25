/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2008-6-5
* Description: 
* History:     
******************************************************************************/

#ifndef __XMLC_CFG_H_
#define __XMLC_CFG_H_

#include "utl/mkv_utl.h"
#include "utl/string_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 

#if 1

#define XMLC_MAX_LEVEL 32

#define XMLC_SCAN_MARK_START(_pstSecRoot, _pstSecNode)	\
    do {    \
        MKV_MARK_S *_pstSecNodeTmp;    \
        DLL_SAFE_SCAN(&_pstSecRoot->stSectionDllHead, _pstSecNode, _pstSecNodeTmp)     \
        {

#define XMLC_SCAN_KEY_START(_pstSecRoot, _pstKeyNode)	\
	do {	\
		MKV_KEY_S  *_pstKeyTmp;	\
		DLL_SAFE_SCAN(&_pstSecRoot->stKeyValueDllHead, _pstKeyNode, _pstKeyTmp)	\
		{


#define XMLC_SCAN_END()	\
		}	\
	}while (0)

typedef struct
{
    MKV_MARK_S  stSecRoot;
    BOOL_T     bIsSort;
    BOOL_T     bReadOnly;
    BOOL_T     bIsUtf8;
    UINT       uiFileSize;
    CHAR       *pucFileName;
    CHAR       *pucFileContent;
    UINT      ulMemSize;   
}XMLC_HEAD_S;

HANDLE XMLC_Open
(
    IN CHAR * pucFileName,
    IN BOOL_T bIsCreateIfNotExist,
    IN BOOL_T bSort,
    IN BOOL_T bReadOnly
);
BS_STATUS XMLC_Save(IN HANDLE hXmlcHandle);
BS_STATUS XMLC_SaveTo(IN HANDLE hXmlcHandle, IN CHAR *pcFileName);
HSTRING XMLC_ToString(IN HANDLE hXmlcHandle);
VOID XMLC_Close(IN HANDLE hXmlcHandle);
BS_STATUS XMLC_DelKeyInMark(IN MKV_MARK_S *pstMark, IN MKV_KEY_S *pstKey);
BS_STATUS XMLC_DelKey(IN HANDLE hXmlcHandle, IN MKV_X_PARA_S *pstSections, IN CHAR *pszKey);
BS_STATUS XMLC_DelAllKeyOfMark(IN HANDLE hXmlcHandle, IN MKV_X_PARA_S *pstSections);
BS_STATUS XMLC_DelMark(IN HANDLE hXmlcHandle, IN MKV_X_PARA_S *pstSections);

BS_STATUS XMLC_AddMark(IN HANDLE hXmlcHandle, IN MKV_X_PARA_S *pstSections);

MKV_MARK_S * XMLC_AddMark2Mark(IN HANDLE hXmlcHandle, IN MKV_MARK_S *pstMark, IN CHAR *pcMark);
MKV_MARK_S * XMLC_FindMarkInMark(IN HANDLE hXmlcHandle, IN MKV_MARK_S *pstMark, IN CHAR *pcMark);
MKV_MARK_S * XMLC_GetMark(IN HANDLE hXmlcHandle, IN MKV_X_PARA_S *pstSections);
BOOL_T XMLC_IsMarkExist(IN HANDLE hXmlcHandle, IN MKV_X_PARA_S *pstSections);
CHAR * XMLC_GetNextMarkInMark(IN HANDLE hXmlcHandle, IN MKV_MARK_S *pstMarkRoot, IN CHAR *pcCurSecName);
CHAR * XMLC_GetNextMark(IN HANDLE hXmlcHandle, IN MKV_X_PARA_S *pstSections, IN CHAR *pcCurSecName);
CHAR * XMLC_GetMarkByIndex(IN HANDLE hXmlcHandle, IN MKV_X_PARA_S *pstSections, IN UINT uiIndex);
BS_STATUS XMLC_GetNextKeyInMark(IN MKV_MARK_S *pstMarkRoot, INOUT CHAR **ppszKeyName);
BS_STATUS XMLC_GetNextKey(IN HANDLE hXmlcHandle, IN MKV_X_PARA_S *pstSections, INOUT CHAR **ppszKeyName);
BS_STATUS XMLC_SetKeyValueAsString(IN HANDLE hXmlcHandle, IN MKV_X_PARA_S *pstSections, IN CHAR *pszKeyName, IN CHAR *pszValue);
BS_STATUS XMLC_SetKeyValueAsUlong(IN HANDLE hXmlcHandle, IN MKV_X_PARA_S *pstSections, IN CHAR *pucKeyName, IN UINT ulKeyValue);
BS_STATUS XMLC_GetKeyValueAsString(IN HANDLE hXmlcHandle, IN MKV_X_PARA_S *pstSections, IN CHAR *pszKeyName, OUT CHAR **ppszKeyValue);
BS_STATUS XMLC_GetKeyValueAsUint(IN HANDLE hXmlcHandle, IN MKV_X_PARA_S *pstSections, IN CHAR *pucKeyName, OUT UINT *pulKeyValue);
BS_STATUS XMLC_GetKeyValueAsInt(IN HANDLE hXmlcHandle, IN MKV_X_PARA_S *pstSections, IN CHAR *pucKeyName, OUT INT *plKeyValue);
BOOL_T XMLC_IsKeyExist(IN HANDLE hXmlcHandle, IN MKV_X_PARA_S *pstSections, IN CHAR *pszKeyName);
UINT XMLC_GetMarkNumInMark(IN MKV_MARK_S *pstMark);
UINT XMLC_GetMarkNum(IN HANDLE hXmlcHandle, IN MKV_X_PARA_S *pstSections);
UINT XMLC_GetKeyNumOfMark(IN MKV_MARK_S *pstMark);
VOID XMLC_WalkMarkInMark(IN MKV_MARK_S *pstMarkRoot, IN PF_MKV_MARK_WALK_FUNC pfFunc, IN HANDLE hUserHandle);
VOID XMLC_WalkKeyInMark(IN MKV_MARK_S *pstMarkRoot, IN PF_MKV_KEY_WALK_FUNC pfFunc, IN HANDLE hUserHandle);
BS_STATUS XMLC_SetKeyValueInMark(IN MKV_MARK_S *pstMark, IN CHAR *pcKey, IN CHAR *pcValue);
CHAR * XMLC_GetKeyValueInMark(IN MKV_MARK_S *pstMark, IN CHAR *pcKey);

#endif

#if 1

#define SXMLC_SCAN_SECTION_START(_hIniId, _pszSectionName)	\
    do {    \
        MKV_MARK_S *_pstSecRoot, *_pstSecNode;  \
        MKV_X_PARA_S _stTreParam;    \
        _stTreParam.ulLevle = 0;     \
        _pstSecRoot = XMLC_GetMark(_hIniId, &_stTreParam); \
        if (NULL != _pstSecRoot)    \
        {\
            XMLC_SCAN_MARK_START(_pstSecRoot,_pstSecNode) \
            {   \
                _pszSectionName = _pstSecNode->pucMarkName;   \
                {

#define SXMLC_SCAN_KEY_START(_hIniId, _pszSectionName, _pszKeyName, _pszKeyValue)	\
    do {    \
        MKV_MARK_S *_pstSecRoot;     \
		MKV_KEY_S  *_pstKeyNode;	\
        MKV_X_PARA_S _stTreParam;    \
        _stTreParam.ulLevle = 1;     \
        _stTreParam.apszMarkName[0] = _pszSectionName;  \
        _pstSecRoot = XMLC_GetMark(_hIniId, &_stTreParam); \
        if (NULL != _pstSecRoot)    \
        {   \
            XMLC_SCAN_MARK_START(_pstSecRoot,_pstKeyNode) \
            {   \
                _pszKeyName = _pstKeyNode->pucKeyName;   \
                _pszKeyValue = _pstKeyNode->pucKeyValue; \
                {


#define SXMLC_SCAN_END()	\
                }   \
    		} XMLC_SCAN_END(); \
        }   \
	}while (0)


typedef int (*PF_SXMLC_SEC_WALK_FUNC)(HANDLE hIniHandle, CHAR *pszSecName, HANDLE hUsrHandle);
typedef int (*PF_SXMLC_KEY_WALK_FUNC)(HANDLE hIniHandle, CHAR *pszSecName, CHAR *pszKeyName, HANDLE hUsrHandle);

extern HANDLE SXMLC_Open
(
    IN CHAR * pucFileName,
    IN BOOL_T bIsCreateIfNotExist,
    IN BOOL_T bSort,
    IN BOOL_T bReadOnly
);
extern VOID SXMLC_Close(IN HANDLE hIniHandle);
extern BS_STATUS SXMLC_Save(IN HANDLE hIniHandle);
extern BS_STATUS SXMLC_GetKeyValueAsString(IN HANDLE hIniHandle, IN CHAR *pucMarkName, IN CHAR *pucKeyName, OUT CHAR **ppucKeyValue);
extern BS_STATUS SXMLC_GetKeyValueAsUint(IN HANDLE hIniHandle, IN CHAR *pucMarkName, IN CHAR *pucKeyName, OUT UINT *pulKeyValue);
extern BS_STATUS SXMLC_GetKeyValueAsInt(IN HANDLE hIniHandle, IN CHAR *pucMarkName, IN CHAR *pucKeyName, OUT INT *plKeyValue);
extern BS_STATUS SXMLC_SetKeyValueAsString(IN HANDLE hIniHandle, IN CHAR *pucMarkName, IN CHAR *pucKeyName, IN CHAR *pucKeyValue);
extern BS_STATUS SXMLC_SetKeyValueAsUlong(IN HANDLE hIniHandle, IN CHAR *pucMarkName, IN CHAR *pucKeyName, IN UINT ulKeyValue);
extern BS_STATUS SXMLC_DelKey(IN HANDLE hIniHandle, IN CHAR *pucMarkName, IN CHAR *pucKeyName);
extern BS_STATUS SXMLC_DelAllKey(IN HANDLE hIniHandle, IN CHAR *pucMarkName);
extern BS_STATUS SXMLC_AddSection(IN HANDLE hIniHandle, IN CHAR *pucMarkName);
extern BS_STATUS SXMLC_DelSection(IN HANDLE hIniHandle, IN CHAR *pucMarkName);
extern BOOL_T SXMLC_IsSecExist(IN HANDLE hIniHandle, IN CHAR *pucMarkName);
extern BOOL_T SXMLC_IsKeyExist(IN HANDLE hIniHandle, IN CHAR *pucMarkName, IN CHAR *pucKeyName);
extern CHAR * SXMLC_GetNextSec(IN HANDLE hHandle, IN CHAR *pcCruSecName);
extern CHAR * SXMLC_GetSecByIndex(IN HANDLE hHandle, IN UINT uiIndex);
extern BS_STATUS SXMLC_GetNextKey(IN HANDLE hIniHandle, IN CHAR *pucMarkName, INOUT CHAR **ppszKeyName);
extern VOID SXMLC_WalkSection(IN HANDLE hHandle, IN PF_SXMLC_SEC_WALK_FUNC pfFunc, IN HANDLE hUsrHandle);
extern VOID SXMLC_WalkKey(IN HANDLE hIniHandle, IN CHAR *pszSecName, IN PF_SXMLC_KEY_WALK_FUNC pfFunc, IN HANDLE hUsrHandle);


extern UINT SXMLC_GetSectionNum(IN HANDLE hIniHandle);


extern UINT SXMLC_GetKeyNumOfSection(IN HANDLE hIniHandle, IN CHAR *pucMarkName);

#endif

#ifdef __cplusplus
    }
#endif 

#endif 


