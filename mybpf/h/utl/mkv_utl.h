/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2010-5-22
* Description: makr_key_value 树管理
* History:     
******************************************************************************/
#ifndef __MKV_UTL_H_
#define __MKV_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

#define MKV_MAX_LEVEL 32

#define MKV_SCAN_MARK_START(_pstMarkRoot, _pstMarkNode)	\
    do {    \
        MKV_MARK_S *_pstNodeTmp;    \
        DLL_SAFE_SCAN(&_pstMarkRoot->stSectionDllHead, _pstMarkNode, _pstNodeTmp)     \
        {

#define MKV_SCAN_KEY_START(_pstMarkRoot, _pstKeyNode)	\
	do {	\
		MKV_KEY_S  *_pstKeyTmp;	\
		DLL_SAFE_SCAN(&_pstMarkRoot->stKeyValueDllHead, _pstKeyNode, _pstKeyTmp)	\
		{


#define MKV_SCAN_END()	\
		}	\
	}while (0)

typedef struct
{
    ULONG ulLevle;  
    CHAR *apszMarkName[MKV_MAX_LEVEL];
}MKV_X_PARA_S;

typedef struct
{
    DLL_NODE_S  stDllNode;      
    CHAR        *pucMarkName;
    BOOL_T      bIsCopy;
    DLL_HEAD_S  stSectionDllHead;   
    DLL_HEAD_S  stKeyValueDllHead;   
}MKV_MARK_S;

static inline VOID MKV_MARK_Init(IN MKV_MARK_S *pstMark)
{
    DLL_INIT(&(pstMark)->stSectionDllHead);
    DLL_INIT(&(pstMark)->stKeyValueDllHead);
}

typedef struct
{
    DLL_NODE_S stDllNode;       
    CHAR *pucKeyName;
    CHAR *pucKeyValue;
    BOOL_T bIsCopy;         
}MKV_KEY_S;

typedef BS_STATUS (*PF_MKV_MarkProcess)(IN MKV_MARK_S *pstMarkNode, IN VOID *pUserPointer);

typedef struct
{
    CHAR *pszMarkName;
    PF_MKV_MarkProcess pfFunc;
}MKV_MARK_PROCESS_S;

typedef int (*PF_MKV_MARK_WALK_FUNC)(IN MKV_MARK_S *pstRoot, IN MKV_MARK_S *pstMark, IN HANDLE hUserHandle);
typedef int (*PF_MKV_KEY_WALK_FUNC)(IN MKV_MARK_S *pstMarkRoot, IN MKV_KEY_S *pstKey, IN HANDLE hUserHandle);

MKV_MARK_S * MKV_GetLastMarkOfLevel(MKV_MARK_S *pstRoot, IN UINT ulLevel);
MKV_MARK_S * MKV_AddMark2Mark(IN MKV_MARK_S *pstRoot, IN CHAR *pszMarkName, IN BOOL_T bCopy);
MKV_MARK_S * MKV_AddMark2MarkWithSort(IN MKV_MARK_S *pstRoot, IN CHAR *pszMarkName, IN BOOL_T bCopy);
MKV_KEY_S * MKV_AddNewKey2Mark
(
    IN MKV_MARK_S *pstMark,
    IN CHAR *pszKeyName,
    IN CHAR *pszValue,
    IN BOOL_T bCopy,
    IN BOOL_T bSort
);
VOID MKV_SortMark(IN MKV_MARK_S *pstRoot);
MKV_MARK_S * MKV_FindMarkInMark(IN MKV_MARK_S *pstRoot, IN CHAR *pszMarkName);
VOID MKV_DelMarkInMark(IN MKV_MARK_S *pstMarkRoot, IN MKV_MARK_S *pstMark);
VOID MKV_DelAllMarkInMark(IN MKV_MARK_S *pstMarkRoot);
VOID MKV_DelAllInMark(IN MKV_MARK_S *pstMarkRoot);
MKV_KEY_S* MKV_FindKeyInMark(IN MKV_MARK_S* pstMark, IN CHAR *pucKeyName);
MKV_KEY_S * MKV_SetKeyValueInMark
(
    IN MKV_MARK_S *pstMark,
    IN CHAR *pszKeyName,
    IN CHAR *pszValue,
    IN BOOL_T bCopy,
    IN BOOL_T bSort
);
BS_STATUS MKV_DelKeyInMark(IN MKV_MARK_S *pstMark, IN MKV_KEY_S *pstKey);
VOID MKV_DelAllKeyInMark(IN MKV_MARK_S *pstMark);
MKV_MARK_S * MKV_FindMarkByLevel(IN MKV_MARK_S *pstRoot, IN UINT ulLevel, IN CHAR ** apszArgs);
BS_STATUS MKV_DelKey(IN MKV_MARK_S *pstRoot, IN MKV_X_PARA_S *pstMarks, IN CHAR *pszKey);
BS_STATUS MKV_DelAllKeyOfMark(IN MKV_MARK_S *pstRoot, IN MKV_X_PARA_S *pstMarks);
BS_STATUS MKV_DelMark(IN MKV_MARK_S *pstRoot, IN MKV_X_PARA_S *pstMarks);
BS_STATUS MKV_AddMark(IN MKV_MARK_S *pstRoot, IN MKV_X_PARA_S *pstMarks, IN BOOL_T bSort);
MKV_MARK_S * MKV_GetMark(IN MKV_MARK_S *pstRoot, IN MKV_X_PARA_S *pstMarks);
BOOL_T MKV_IsMarkExist(IN MKV_MARK_S *pstRoot, IN MKV_X_PARA_S *pstMarks);
CHAR * MKV_GetNextMarkInMark(IN MKV_MARK_S *pstRoot, IN CHAR *pcCurMarkName);
CHAR * MKV_GetMarkByIndexInMark(IN MKV_MARK_S *pstRoot, IN UINT uiIndex);
CHAR * MKV_GetNextMark(IN MKV_MARK_S *pstRoot, IN MKV_X_PARA_S *pstMarks, IN CHAR *pcCurMarkName);
CHAR * MKV_GetMarkByIndex(IN MKV_MARK_S *pstRoot, IN MKV_X_PARA_S *pstMarks, IN UINT uiIndex);
BS_STATUS MKV_GetNextKeyInMark(IN MKV_MARK_S *pstMark, INOUT CHAR **ppszKeyName);
BS_STATUS MKV_GetNextKey(IN MKV_MARK_S *pstRoot, IN MKV_X_PARA_S *pstMarks, INOUT CHAR **ppszKeyName);
BS_STATUS MKV_SetKeyValueAsString
(
    IN MKV_MARK_S *pstRoot,
    IN MKV_X_PARA_S *pstMarks,
    IN CHAR *pszKeyName,
    IN CHAR *pszValue,
    IN BOOL_T bMarkSort
);
BS_STATUS MKV_SetKeyValueAsUint
(
    IN MKV_MARK_S *pstRoot,
    IN MKV_X_PARA_S *pstMarks,
    IN CHAR *pucKeyName,
    IN UINT uiKeyValue,
    IN BOOL_T bMarkSort
);
BS_STATUS MKV_SetKeyValueAsUint64
(
    IN MKV_MARK_S *pstRoot,
    IN MKV_X_PARA_S *pstMarks,
    IN CHAR *pucKeyName,
    IN UINT64 uiKeyValue,
    IN BOOL_T bMarkSort
);
BS_STATUS MKV_GetKeyValueAsString
(
    IN MKV_MARK_S *pstRoot,
    IN MKV_X_PARA_S *pstMarks,
    IN CHAR *pszKeyName,
    OUT CHAR **ppszKeyValue
);
BS_STATUS MKV_GetKeyValueAsUint64
(
    IN MKV_MARK_S *pstRoot,
    IN MKV_X_PARA_S *pstMarks,
    IN CHAR *pucKeyName,
    OUT UINT64 *puiKeyValue
);
BS_STATUS MKV_GetKeyValueAsUint
(
    IN MKV_MARK_S *pstRoot,
    IN MKV_X_PARA_S *pstMarks,
    IN CHAR *pucKeyName,
    OUT UINT *puiKeyValue
);
BS_STATUS MKV_GetKeyValueAsInt
(
    IN MKV_MARK_S *pstRoot,
    IN MKV_X_PARA_S *pstMarks,
    IN CHAR *pucKeyName,
    OUT INT *plKeyValue
);
BOOL_T MKV_IsKeyExist(IN MKV_MARK_S *pstRoot, IN MKV_X_PARA_S *pstMarks, IN CHAR *pszKeyName);

UINT MKV_GetMarkNumInMark(IN MKV_MARK_S *pstMark);
UINT MKV_GetSectionNum(IN MKV_MARK_S *pstRoot, IN MKV_X_PARA_S *pstMarks);

UINT MKV_GetKeyNumOfMark(IN MKV_MARK_S *pstMark);
VOID MKV_WalkMarkInMark(IN MKV_MARK_S *pstRoot, IN PF_MKV_MARK_WALK_FUNC pfFunc, IN HANDLE hUserHandle);

char * MKV_GetMarkDuplicate(IN MKV_MARK_S *pstRoot, IN MKV_X_PARA_S *pstMarks);
VOID MKV_WalkKeyInMark(IN MKV_MARK_S *pstRoot, IN PF_MKV_KEY_WALK_FUNC pfFunc, IN HANDLE hUserHandle);


VOID MKV_ScanProcess
(
    IN MKV_MARK_S *pstMarkRoot,
    IN MKV_MARK_PROCESS_S *pstFuncTbl,
    IN UINT uiFuncTblCount,
    IN VOID *pUserPointer
);

#ifdef __cplusplus
    }
#endif 

#endif 


