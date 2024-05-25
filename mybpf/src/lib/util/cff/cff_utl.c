/******************************************************************************
* Copyright (C), LiXingang
* Author:      LiXingang  Version: 1.0  Date: 2015-6-23
* Description: config file
* History:     
******************************************************************************/
#include "bs.h"

#include "utl/txt_utl.h"
#include "utl/cff_utl.h"

#include "cff_inner.h"

typedef struct {
    char *buf;
    int buf_size;
}_CFF_SAVE_2_BUF_S;

BOOL_T _ccf_IsSort(IN _CFF_S *pstCff)
{
    if (pstCff->uiFlag & CFF_FLAG_SORT)
    {
        return TRUE;
    }

    return FALSE;
}

static VOID ccf_Close(IN _CFF_S *pstCff)
{
    if (pstCff->pcFileName) {
        MEM_Free(pstCff->pcFileName);
    }

    if (pstCff->file_mem.data) {
        FILE_FreeMem(&pstCff->file_mem);
    }

    MKV_DelAllInMark(&pstCff->stCfgRoot);

    MEM_Free(pstCff);
}

_CFF_S * _cff_Open(IN CHAR *pcFileName, IN UINT uiFlag)
{
    _CFF_S *pstCff;
    UINT uiFileNameLen;

    if (NULL == pcFileName)
    {
        return NULL;
    }

    if (uiFlag & CFF_FLAG_CREATE_IF_NOT_EXIST)
    {
        FILE_MakeFile(pcFileName);
    }

    pstCff = MEM_ZMalloc(sizeof(_CFF_S));
    if (NULL == pstCff)
    {
        return NULL;
    }

    MKV_MARK_Init(&pstCff->stCfgRoot);
    pstCff->uiFlag = uiFlag;

    uiFileNameLen = strlen(pcFileName);
    pstCff->pcFileName = MEM_Malloc(uiFileNameLen + 1);
    if (NULL == pstCff->pcFileName)
    {
        ccf_Close(pstCff);
        return NULL;
    }
    TXT_Strlcpy(pstCff->pcFileName, pcFileName, uiFileNameLen + 1);

    if (0 != FILE_Mem(pcFileName, &pstCff->file_mem)) {
        ccf_Close(pstCff);
        return NULL;
    }

    pstCff->pcFileContent = (CHAR*)pstCff->file_mem.data;

    
    if (pstCff->file_mem.len >= 3) {
        if ((pstCff->pcFileContent[0] == (CHAR)0xef)
            && (pstCff->pcFileContent[1] == (CHAR)0xbb)
            && (pstCff->pcFileContent[2] == (CHAR)0xbf)) {
            pstCff->uiFlag |= CFF_FLAG_UTF8_BOM;
            pstCff->pcFileContent += 3;
        }
    }

    pstCff->pcFileContent = TXT_Strim(pstCff->pcFileContent);

    return pstCff;
}

_CFF_S * _cff_OpenBuf(IN CHAR *buf, IN UINT flag)
{
    _CFF_S *pstCff;

    if (NULL == buf) {
        return NULL;
    }

    pstCff = MEM_ZMalloc(sizeof(_CFF_S));
    if (NULL == pstCff) {
        return NULL;
    }

    MKV_MARK_Init(&pstCff->stCfgRoot);
    pstCff->uiFlag = flag;

    pstCff->pcFileContent = TXT_Strdup(buf);
    if (NULL == pstCff->pcFileContent) {
        ccf_Close(pstCff);
        return NULL;
    }

    pstCff->pcFileContent = TXT_Strim(pstCff->pcFileContent);

    return pstCff;
}

static void cff_Save(IN char *buf, IN VOID *pUserData)
{
    FILE *fp = pUserData;
    fwrite(buf, 1, strlen(buf), fp);
}

static void cff_Show(IN char *buf, IN VOID *pUserData)
{
    printf("%s", buf);
}

static void cff_Save2Buf(IN char *buf, IN VOID *pUserData)
{
    _CFF_SAVE_2_BUF_S *save_buf = pUserData;
    int len;

    if (save_buf->buf_size <= 0) {
        return;
    }

    len = scnprintf(save_buf->buf, save_buf->buf_size, "%s", buf);

    if (len > 0) {
        save_buf->buf += len;
        save_buf->buf_size -= len;
    }
}

BS_STATUS CFF_SaveAs(IN CFF_HANDLE hCffHandle, IN char *filepath)
{
    _CFF_S *pstCff = hCffHandle;
    FILE  *fp = NULL;
    BS_STATUS eRet;

    if ((NULL == pstCff) || (NULL == filepath)){
        RETURN(BS_NULL_PARA);
    }

    if (pstCff->uiFlag & CFF_FLAG_READ_ONLY) {
        RETURN(BS_NO_PERMIT);
    }

    fp = FILE_Open(filepath, FALSE, "wb+");
    if (NULL == fp) {
        RETURN(BS_CAN_NOT_OPEN);
    }

    if (pstCff->uiFlag & CFF_FLAG_UTF8_BOM) {
        fprintf(fp, "\xEF\xBB\xBF");
    }

    eRet = pstCff->pstFuncTbl->pfSave(hCffHandle, cff_Save, fp);

    fclose(fp);

    return eRet;
}

BS_STATUS CFF_Save(IN CFF_HANDLE hCffHandle)
{
    _CFF_S *pstCff = hCffHandle;

    if (pstCff->pcFileName == NULL) {
        RETURN(BS_NOT_SUPPORT);
    }
 
    return CFF_SaveAs(hCffHandle, pstCff->pcFileName);
}

BS_STATUS CFF_Show(IN CFF_HANDLE hCffHandle)
{
    _CFF_S *pstCff = hCffHandle;
    BS_STATUS eRet;

    if (NULL == pstCff) {
        RETURN(BS_NULL_PARA);
    }

    eRet = pstCff->pstFuncTbl->pfSave(hCffHandle, cff_Show, NULL);

    return eRet;
}

BS_STATUS CFF_Save2Buf(IN CFF_HANDLE hCffHandle, IN char *buf, IN int buf_size)
{
    _CFF_SAVE_2_BUF_S save_buf;
    _CFF_S *pstCff = hCffHandle;

    save_buf.buf = buf;
    save_buf.buf_size = buf_size;

    return pstCff->pstFuncTbl->pfSave(hCffHandle, cff_Save2Buf, &save_buf);
}

VOID CFF_Close(IN CFF_HANDLE hCffHandle)
{
    _CFF_S *pstCff = hCffHandle;

    if (NULL == pstCff)
    {
        return;
    }

    ccf_Close(pstCff);    
}

BS_STATUS CFF_X_DelProp(IN CFF_HANDLE hCffHandle, IN MKV_X_PARA_S *pstTags, IN CHAR *pcProp)
{
    _CFF_S *pstCff = hCffHandle;

    return MKV_DelKey(&pstCff->stCfgRoot, pstTags, pcProp);
}

BS_STATUS CFF_X_DelAllPropOfTag(IN CFF_HANDLE hCffHandle, IN MKV_X_PARA_S *pstTags)
{
    _CFF_S *pstCff = hCffHandle;

    return MKV_DelAllKeyOfMark(&pstCff->stCfgRoot, pstTags);
}

BS_STATUS CFF_X_DelTag(IN CFF_HANDLE hCffHandle, IN MKV_X_PARA_S *pstTags)
{
    _CFF_S *pstCff = hCffHandle;

    return MKV_DelMark(&pstCff->stCfgRoot, pstTags);
}

BS_STATUS CFF_X_AddTag(IN CFF_HANDLE hCffHandle, IN MKV_X_PARA_S *pstTags)
{
    _CFF_S *pstCff = hCffHandle;

    return MKV_AddMark(&pstCff->stCfgRoot, pstTags, _ccf_IsSort(pstCff));
}

MKV_MARK_S * CFF_X_GetTag(IN CFF_HANDLE hCffHandle, IN MKV_X_PARA_S *pstTags)
{
    _CFF_S *pstCff = hCffHandle;

    return MKV_GetMark(&pstCff->stCfgRoot, pstTags);
}

BOOL_T CFF_X_IsTagExist(IN CFF_HANDLE hCffHandle, IN MKV_X_PARA_S *pstTags)
{
    if (NULL == CFF_X_GetTag(hCffHandle, pstTags))
    {
        return FALSE;
    }

    return TRUE;
}

CHAR * CFF_X_GetNextTag(IN CFF_HANDLE hCffHandle, IN MKV_X_PARA_S *pstTags, IN CHAR *pcCurTag)
{
    _CFF_S *pstCff = hCffHandle;

    BS_DBGASSERT(pstCff->uiFlag & CFF_FLAG_SORT);

    return MKV_GetNextMark(&pstCff->stCfgRoot, pstTags, pcCurTag);
}

BS_STATUS CFF_X_GetNextProp(IN CFF_HANDLE hCffHandle, IN MKV_X_PARA_S *pstTags, INOUT CHAR **ppcPropName)
{
    _CFF_S *pstCff = hCffHandle;

    BS_DBGASSERT(pstCff->uiFlag & CFF_FLAG_SORT);

    return MKV_GetNextKey(&pstCff->stCfgRoot, pstTags, ppcPropName);
}

BS_STATUS CFF_X_SetPropAsString
(
    IN CFF_HANDLE hCffHandle,
    IN MKV_X_PARA_S *pstTags,
    IN CHAR *pcPropName,
    IN CHAR *pcValue
)
{
    _CFF_S *pstCff = hCffHandle;

    return MKV_SetKeyValueAsString(&pstCff->stCfgRoot, pstTags, pcPropName, pcValue, _ccf_IsSort(pstCff));
}

BS_STATUS CFF_X_SetPropAsUint
(
    IN CFF_HANDLE hCffHandle,
    IN MKV_X_PARA_S *pstTags,
    IN CHAR *pcPropName,
    IN UINT uiValue
)
{
    _CFF_S *pstCff = hCffHandle;

    return MKV_SetKeyValueAsUint(&pstCff->stCfgRoot, pstTags, pcPropName, uiValue, _ccf_IsSort(pstCff));
}

BS_STATUS CFF_X_SetPropAsUint64
(
    IN CFF_HANDLE hCffHandle,
    IN MKV_X_PARA_S *pstTags,
    IN CHAR *pcPropName,
    IN UINT64 uiValue
)
{
    _CFF_S *pstCff = hCffHandle;

    return MKV_SetKeyValueAsUint64(&pstCff->stCfgRoot, pstTags, pcPropName, uiValue, _ccf_IsSort(pstCff));
}

BS_STATUS CFF_X_GetPropAsString
(
    IN CFF_HANDLE hCffHandle,
    IN MKV_X_PARA_S *pstTags,
    IN CHAR *pcPropName,
    OUT CHAR **ppcValue
)
{
    _CFF_S *pstCff = hCffHandle;
    return MKV_GetKeyValueAsString(&pstCff->stCfgRoot, pstTags, pcPropName, ppcValue);
}

BS_STATUS CFF_X_GetPropAsUint64
(
    IN CFF_HANDLE hCffHandle,
    IN MKV_X_PARA_S *pstTags,
    IN CHAR *pcPropName,
    OUT UINT64 *puiKeyValue
)
{
    _CFF_S *pstCff = hCffHandle;

    return MKV_GetKeyValueAsUint64(&pstCff->stCfgRoot, pstTags, pcPropName, puiKeyValue);
}

BS_STATUS CFF_X_GetPropAsUint
(
    IN CFF_HANDLE hCffHandle,
    IN MKV_X_PARA_S *pstTags,
    IN CHAR *pcPropName,
    OUT UINT *puiKeyValue
)
{
    _CFF_S *pstCff = hCffHandle;

    return MKV_GetKeyValueAsUint(&pstCff->stCfgRoot, pstTags, pcPropName, puiKeyValue);
}

BS_STATUS CFF_X_GetPropAsInt
(
    IN CFF_HANDLE hCffHandle,
    IN MKV_X_PARA_S *pstTags,
    IN CHAR *pcPropName,
    OUT INT *piValue
)
{
    _CFF_S *pstCff = hCffHandle;
    return MKV_GetKeyValueAsInt(&pstCff->stCfgRoot, pstTags, pcPropName, piValue);
}

BOOL_T CFF_X_IsPropExist(IN CFF_HANDLE hCffHandle, IN MKV_X_PARA_S *pstTags, IN CHAR *pcPropName)
{
    _CFF_S *pstCff = hCffHandle;
    return MKV_IsKeyExist(&pstCff->stCfgRoot, pstTags, pcPropName);
}

UINT CFF_X_GetTagNum(IN CFF_HANDLE hCffHandle, IN MKV_X_PARA_S *pstTags)
{
    _CFF_S *pstCff = hCffHandle;
    return MKV_GetSectionNum(&pstCff->stCfgRoot, pstTags);
}

char * CFF_X_GetTagDuplicate(IN CFF_HANDLE hCffHandle, IN MKV_X_PARA_S *pstTags)
{
    _CFF_S *pstCff = hCffHandle;
    return MKV_GetMarkDuplicate(&pstCff->stCfgRoot, pstTags);
}

BS_STATUS CFF_DelProp(IN CFF_HANDLE hCffHandle, IN CHAR *pcTag, IN CHAR *pcProp)
{
    MKV_X_PARA_S stTreParam;

    stTreParam.apszMarkName[0] = pcTag;
    stTreParam.ulLevle = 1;
    
    return CFF_X_DelProp(hCffHandle, &stTreParam, pcProp);
}

BS_STATUS CFF_DelAllProp(IN CFF_HANDLE hCffHandle, IN CHAR *pcTag)
{
    MKV_X_PARA_S stTreParam;

    stTreParam.apszMarkName[0] = pcTag;
    stTreParam.ulLevle = 1;

    return CFF_X_DelAllPropOfTag(hCffHandle, &stTreParam);
}

BS_STATUS CFF_DelTag(IN CFF_HANDLE hCffHandle, IN CHAR *pcTag)
{
    MKV_X_PARA_S stTreParam;

    stTreParam.apszMarkName[0] = pcTag;
    stTreParam.ulLevle = 1;

    return CFF_X_DelTag(hCffHandle, &stTreParam);
}

BS_STATUS CFF_AddTag(IN CFF_HANDLE hCffHandle, IN CHAR *pcTag)
{
    MKV_X_PARA_S stTreParam;

    stTreParam.apszMarkName[0] = pcTag;
    stTreParam.ulLevle = 1;

    return CFF_X_AddTag(hCffHandle, &stTreParam);
}

char * CFF_GetTagDuplicate(IN CFF_HANDLE hCffHandle)
{
    MKV_X_PARA_S stTreParam;

    stTreParam.ulLevle = 0;

    return CFF_X_GetTagDuplicate(hCffHandle, &stTreParam);
}

BS_STATUS CFF_GetPropAsString(IN CFF_HANDLE hCffHandle, IN CHAR *pcTag, IN CHAR *pcProp, OUT CHAR **ppcValue)
{
    MKV_X_PARA_S stTreParam;

    stTreParam.apszMarkName[0] = pcTag;
    stTreParam.ulLevle = 1;

    return CFF_X_GetPropAsString(hCffHandle, &stTreParam, pcProp, ppcValue);
}

int CFF_CopyPropAsString(CFF_HANDLE hCffHandle, CHAR *pcTag, CHAR *pcProp, OUT CHAR *value, int value_size)
{
    char *val = NULL;
    int ret;

    ret = CFF_GetPropAsString(hCffHandle, pcTag, pcProp, &val);
    if (ret < 0) {
        return  ret;
    }

    if (strlcpy(value, val, value_size) >= value_size) {
        RETURN(BS_FULL);
    }

    return 0;
}

BOOL_T CFF_IsPropExist(IN CFF_HANDLE hCffHandle, IN CHAR *pcTag, IN CHAR *pcProp)
{
    MKV_X_PARA_S stTreParam;

    stTreParam.apszMarkName[0] = pcTag;
    stTreParam.ulLevle = 1;

    return CFF_X_IsPropExist(hCffHandle, &stTreParam, pcProp);
}

CHAR * CFF_GetNextTag(IN CFF_HANDLE hCffHandle, IN CHAR *pcCurrentTag)
{
    MKV_X_PARA_S stTreParam;

    stTreParam.ulLevle = 0;

    return CFF_X_GetNextTag(hCffHandle, &stTreParam, pcCurrentTag);
}

BS_STATUS CFF_GetNextProp(IN CFF_HANDLE hCffHandle, IN CHAR *pcTag, INOUT CHAR **ppcProp)
{
    _CFF_S *pstCff = hCffHandle;
    MKV_X_PARA_S stTreParam;

    stTreParam.apszMarkName[0] = pcTag;
    stTreParam.ulLevle = 1;

    return CFF_X_GetNextProp(&pstCff->stCfgRoot, &stTreParam, ppcProp);
}

BOOL_T CFF_IsTagExist(IN CFF_HANDLE hCffHandle, IN CHAR *pcTag)
{
    MKV_X_PARA_S stTreParam;

    stTreParam.apszMarkName[0] = pcTag;
    stTreParam.ulLevle = 1;

    return CFF_X_IsTagExist(hCffHandle, &stTreParam);
}

BS_STATUS CFF_SetPropAsString(IN CFF_HANDLE hCffHandle, IN CHAR *pcTag, IN CHAR *pcProp, IN CHAR *pcValue)
{
    MKV_X_PARA_S stTreParam;

    stTreParam.apszMarkName[0] = pcTag;
    stTreParam.ulLevle = 1;

    return CFF_X_SetPropAsString(hCffHandle, &stTreParam, pcProp, pcValue);
}

BS_STATUS CFF_SetPropAsUint(IN CFF_HANDLE hCffHandle, IN CHAR *pcTag, IN CHAR *pcProp, IN UINT uiValue)
{
    MKV_X_PARA_S stTreParam;

    stTreParam.apszMarkName[0] = pcTag;
    stTreParam.ulLevle = 1;

    return CFF_X_SetPropAsUint(hCffHandle, &stTreParam, pcProp, uiValue);
}

BS_STATUS CFF_SetPropAsUint64(IN CFF_HANDLE hCffHandle, IN CHAR *pcTag, IN CHAR *pcProp, IN UINT64 uiValue)
{
    MKV_X_PARA_S stTreParam;

    stTreParam.apszMarkName[0] = pcTag;
    stTreParam.ulLevle = 1;

    return CFF_X_SetPropAsUint64(hCffHandle, &stTreParam, pcProp, uiValue);
}

BS_STATUS CFF_GetPropAsUint64(IN CFF_HANDLE hCffHandle, IN CHAR *pcTag, IN CHAR *pcProp, OUT UINT64 *puiValue)
{
    MKV_X_PARA_S stTreParam;

    stTreParam.apszMarkName[0] = pcTag;
    stTreParam.ulLevle = 1;

    return CFF_X_GetPropAsUint64(hCffHandle, &stTreParam, pcProp, puiValue);
}

BS_STATUS CFF_GetPropAsUint(IN CFF_HANDLE hCffHandle, IN CHAR *pcTag, IN CHAR *pcProp, OUT UINT *puiValue)
{
    MKV_X_PARA_S stTreParam;

    stTreParam.apszMarkName[0] = pcTag;
    stTreParam.ulLevle = 1;

    return CFF_X_GetPropAsUint(hCffHandle, &stTreParam, pcProp, puiValue);
}

BS_STATUS CFF_GetPropAsInt(IN CFF_HANDLE hCffHandle, IN CHAR *pcTag, IN CHAR *pcProp, OUT INT *piValue)
{
    MKV_X_PARA_S stTreParam;

    stTreParam.apszMarkName[0] = pcTag;
    stTreParam.ulLevle = 1;

    return CFF_X_GetPropAsInt(hCffHandle, &stTreParam, pcProp, piValue);
}


UINT CFF_GetPropAsUintDft(CFF_HANDLE hCffHandle, char *pcTag, char *pcProp, UINT dft)
{
    UINT val;
    return CFF_GetPropAsUint(hCffHandle, pcTag, pcProp, &val) == 0 ? val : dft;
}

int CFF_GetPropAsIntDft(CFF_HANDLE hCffHandle, char *pcTag, char *pcProp, int dft)
{
    int val;
    return CFF_GetPropAsInt(hCffHandle, pcTag, pcProp, &val) == 0 ? val : dft;
}

UINT64 CFF_GetPropAsUint64Dft(CFF_HANDLE hCffHandle, char *pcTag, char *pcProp, UINT64 dft)
{
    UINT64 val;
    return CFF_GetPropAsUint64(hCffHandle, pcTag, pcProp, &val) == 0 ? val : dft;
}

char * CFF_GetPropAsStringDft(CFF_HANDLE hCffHandle, char *pcTag, char *pcProp, char *dft)
{
    char *val;
    return CFF_GetPropAsString(hCffHandle, pcTag, pcProp, &val) == 0 ? val : dft;
}


UINT CFF_GetTagNum(IN CFF_HANDLE hCffHandle)
{
    MKV_X_PARA_S stTreParam;

    stTreParam.ulLevle = 0;

    return CFF_X_GetTagNum(hCffHandle, &stTreParam);
}

static int cff_WalkTagFunc(IN MKV_MARK_S *pstMarkRoot, IN MKV_MARK_S *pstMark, IN USER_HANDLE_S *pstUserHandle)
{
    PF_CFF_TAG_WALK_FUNC pfFunc;
    
    pfFunc = (PF_CFF_TAG_WALK_FUNC) (pstUserHandle->ahUserHandle[1]);
    pfFunc(pstUserHandle->ahUserHandle[0], pstMark->pucMarkName, pstUserHandle->ahUserHandle[2]);

    return 0;
}

VOID CFF_WalkTag(IN CFF_HANDLE hCffHandle, IN PF_CFF_TAG_WALK_FUNC pfFunc, IN HANDLE hUsrHandle)
{
    USER_HANDLE_S stUserHandle;
    _CFF_S *pstCff = hCffHandle;

    stUserHandle.ahUserHandle[0] = hCffHandle;
    stUserHandle.ahUserHandle[1] = pfFunc;
    stUserHandle.ahUserHandle[2] = hUsrHandle;

    MKV_WalkMarkInMark(&pstCff->stCfgRoot, (PF_MKV_MARK_WALK_FUNC)cff_WalkTagFunc, &stUserHandle);
}

static int cff_WalkPropFunc(IN MKV_MARK_S *pstMarkRoot, IN MKV_KEY_S *pstProp, IN USER_HANDLE_S *pstUserHandle)
{
    PF_CFF_PROP_WALK_FUNC pfFunc;
    
    pfFunc = (PF_CFF_PROP_WALK_FUNC) (pstUserHandle->ahUserHandle[1]);
    pfFunc(pstUserHandle->ahUserHandle[0], pstMarkRoot->pucMarkName, pstProp->pucKeyName, pstUserHandle->ahUserHandle[2]);

    return 0;
}

VOID CFF_WalkProp(IN CFF_HANDLE hCffHandle, IN CHAR *pcTag, IN PF_CFF_PROP_WALK_FUNC pfFunc, IN HANDLE hUsrHandle)
{
    MKV_MARK_S *pstMarkRoot;
    MKV_X_PARA_S stTreParam;
    USER_HANDLE_S stUserHandle;

    stTreParam.apszMarkName[0] = pcTag;
    stTreParam.ulLevle = 1;

    pstMarkRoot = CFF_X_GetTag(hCffHandle, &stTreParam);
    if (NULL == pstMarkRoot)
    {
        return;
    }

    stUserHandle.ahUserHandle[0] = hCffHandle;
    stUserHandle.ahUserHandle[1] = pfFunc;
    stUserHandle.ahUserHandle[2] = hUsrHandle;

    MKV_WalkKeyInMark(pstMarkRoot, (PF_MKV_KEY_WALK_FUNC)cff_WalkPropFunc, &stUserHandle);
}


UINT CFF_GetPorpNumOfTag(IN CFF_HANDLE hCffHandle, IN CHAR *pcTag)
{
    MKV_MARK_S *pstMarkRoot;
    MKV_X_PARA_S stTreParam;

    stTreParam.apszMarkName[0] = pcTag;
    stTreParam.ulLevle = 1;

    pstMarkRoot = CFF_X_GetTag(hCffHandle, &stTreParam);

    return MKV_GetKeyNumOfMark(pstMarkRoot);
}


