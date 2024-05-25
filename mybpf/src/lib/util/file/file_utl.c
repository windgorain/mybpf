/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      lixingang  Version: 1.0  Date: 2007-2-8
* Description: 
* History:     
******************************************************************************/
#include "bs.h"

#include "sys/stat.h"
#include "utl/mem_utl.h"
#include "utl/file_func.h"
#include "utl/file_utl.h"
#include "utl/txt_utl.h"
#include "utl/cjson.h"
#include "utl/passwd_utl.h"
#include <stdbool.h>


#ifdef IN_WINDOWS
static BOOL_T _OS_FILE_IsAbsolutePath(IN CHAR *pszPath)
{
    if (strchr(pszPath, ':') != NULL)
    {
        return TRUE;
    }

    return FALSE;
}
#endif

#ifdef IN_UNIXLIKE
static BOOL_T _OS_FILE_IsAbsolutePath(IN CHAR *pszPath)
{
    if (pszPath[0] == '/')
    {
        return TRUE;
    }

    return FALSE;
}
#endif

BOOL_T FILE_IsAbsolutePath(IN CHAR *pszPath)
{
    return _OS_FILE_IsAbsolutePath(pszPath);
}

char * FILE_ToAbsPath(char *base_dir, char *path, OUT char *dst, int dst_size)
{
    int old_path_len = strlen(path);
    int base_dir_len;
    int iRet;

    if (base_dir == NULL) {
        base_dir = "./";
    }

    if (old_path_len >= dst_size) {
        return NULL;
    }

    if (FILE_IsAbsolutePath(path)) {
        return path;
    } else {
        base_dir_len = strlen(base_dir);
        if ((base_dir[base_dir_len - 1] == '/') || (base_dir[base_dir_len - 1] == '\\')) {
            iRet = snprintf(dst, dst_size, "%s%s", base_dir, path);
        } else {
            iRet = snprintf(dst, dst_size, "%s/%s", base_dir, path);
        }
    }

    if ((iRet < 0) || (iRet >= dst_size)) {
        return NULL;
    }

    return dst;
}

char * FILE_Dup2AbsPath(IN char *base_dir, IN char *path)
{
    char filename[FILE_MAX_PATH_LEN + 1];
    char *new_filename;

    new_filename = FILE_ToAbsPath(base_dir, path, filename, sizeof(filename));
    if (NULL == new_filename) {
        return NULL;
    }

    return strdup(new_filename);
}


void FILE_GetPathFromFilePath(char *pszFilePath, OUT char *szPath)
{
    CHAR *pszFileName;
    
    BS_DBGASSERT(NULL != pszFilePath);

    szPath[0] = '\0';

    pszFileName = FILE_GetFileNameFromPath(pszFilePath);
    if (NULL == pszFileName)
    {
        BS_DBGASSERT(0);
        return;
    }

    if (pszFileName - pszFilePath > FILE_MAX_PATH_LEN)
    {
        BS_DBGASSERT(0);
        return;
    }

    MEM_Copy(szPath, pszFilePath, pszFileName - pszFilePath);
    szPath[pszFileName - pszFilePath] = '\0';

    return;
}


CHAR * FILE_EarseExternName(IN CHAR *pcFilePath)
{
    CHAR *pcExtName;
    UINT uiFilePathLen = strlen(pcFilePath);

    pcExtName = FILE_GetExternNameFromPath(pcFilePath, uiFilePathLen);
    if (pcExtName == NULL)
    {
        return pcFilePath;
    }

    pcExtName --; 
    *pcExtName = '\0';

    return pcFilePath;
}

CHAR * FILE_ChangeExternName(CHAR *file_name, CHAR *ext_name)
{
    CHAR *pcExtName;
    UINT uiFilePathLen = strlen(file_name);

    pcExtName = FILE_GetExternNameFromPath(file_name, uiFilePathLen);
    if (pcExtName == NULL)
    {
        pcExtName = file_name + uiFilePathLen;
        *pcExtName = '.';
        pcExtName ++;
    }

    strcpy(pcExtName, ext_name);

    return file_name;
}


UINT FILE_GetPathDeep(IN CHAR *pcFilePath, IN UINT uiPathLen)
{
    UINT uiDeep = 0;
    CHAR *pcEnd = pcFilePath + uiPathLen;
    CHAR *pcPtr = pcFilePath;

    if (*pcPtr == '/') 
    {
        pcPtr ++;
    }

    while (pcPtr < pcEnd)
    {
        if (*pcPtr == '/')
        {
            uiDeep ++;
        }

        pcPtr++;
    }

    return uiDeep;
}


BOOL_T FILE_IsFileExist(IN CHAR *pcFilePath)
{

    
    if (0 == access(pcFilePath, 0))
    {
        return TRUE;
    }

    return FALSE;
}


BOOL_T FILE_IsDirExist(IN CHAR *pcDirName)
{
    
    struct stat stBuf;
    BOOL_T bRet;
    INT iRet;

    BS_DBGASSERT(NULL != pcDirName);

    
    iRet = stat(pcDirName, &stBuf);
    if (0 == iRet)
    {
        bRet = S_ISDIR(stBuf.st_mode);
    }
    else 
    {
        bRet = FALSE;
    }

    return bRet;    
}



BOOL_T FILE_IsDir(IN CHAR *pcPath)
{
    struct stat f_stat;
    CHAR szPath[FILE_MAX_PATH_LEN + 1];

    if (NULL == pcPath)
    {
        return FALSE;
    }

    strlcpy(szPath, pcPath, sizeof(szPath));
    FILE_PATH_TO_HOST(szPath);

    if (stat (szPath, &f_stat ) == -1)
    {
        return FALSE;
    }

    if (S_IFDIR & f_stat.st_mode)
    {
        return TRUE;
    }

    return FALSE;
}

S64 FILE_GetSize(char *pszFileName)
{
    struct stat f_stat;
    CHAR szPath[FILE_MAX_PATH_LEN + 1];

    if (! pszFileName) {
        return -1;
    }

    strlcpy(szPath, pszFileName, sizeof(szPath));
    FILE_PATH_TO_HOST(szPath);

    if (stat (szPath, &f_stat ) == -1) {
        return -1;
    }

    return (long)f_stat.st_size;
}

BS_STATUS FILE_GetUtcTime
(
 IN CHAR *pszFileName,
 OUT time_t *pulCreateTime,   
    OUT time_t *pulModifyTime,   
    OUT time_t *pulAccessTime    
)
{
    struct stat f_stat; 
    CHAR szPath[FILE_MAX_PATH_LEN + 1];

    if (NULL == pszFileName)
    {
        RETURN(BS_NULL_PARA);
    }

    strlcpy(szPath, pszFileName, sizeof(szPath));
    FILE_PATH_TO_HOST(szPath);

    if (stat (szPath, &f_stat ) == -1)
    {
        RETURN(BS_CAN_NOT_OPEN);
    }

    if (NULL != pulCreateTime)
    {
        *pulCreateTime = f_stat.st_ctime;
    }

    if (NULL != pulModifyTime)
    {
        *pulModifyTime = f_stat.st_mtime;
    }

    if (NULL != pulAccessTime)
    {
        *pulAccessTime = f_stat.st_atime;
    }

    return BS_OK;
}

BS_STATUS FILE_SetUtcTime
(
    IN CHAR *pszFileName,
    IN FILE_TIME_MODE_E eModifyTimeMode,
    IN time_t ulModifyTime,
    IN FILE_TIME_MODE_E eAccessTimeMode,
    IN time_t ulAccessTime
)
{
    struct utimbuf stTime;
    time_t ulOldAccessTime;
    time_t ulOldModifyTime;
    time_t ulNowAccessTime;
    time_t ulNowModifyTime;
    time_t ulNewAccessTime;
    time_t ulNewModifyTime;

    
    if ((eAccessTimeMode == FILE_TIME_MODE_NOTMODIFY) || (eModifyTimeMode == FILE_TIME_MODE_NOTMODIFY))
    {
        if ((eAccessTimeMode == FILE_TIME_MODE_NOTMODIFY) && (eModifyTimeMode == FILE_TIME_MODE_NOTMODIFY))
        {
            return BS_OK;
        }
        
        if (BS_OK != FILE_GetUtcTime(pszFileName, NULL, &ulOldModifyTime, &ulOldAccessTime))
        {
            RETURN(BS_ERR);
        }
    }

    if ((eAccessTimeMode == FILE_TIME_MODE_NOW) || (eModifyTimeMode == FILE_TIME_MODE_NOW))
    {
        if (0 != utime(pszFileName, NULL))
        {
            RETURN(BS_ERR);
        }

        if ((eAccessTimeMode == FILE_TIME_MODE_NOW) && (eModifyTimeMode == FILE_TIME_MODE_NOW))
        {
            return BS_OK;
        }

        if (BS_OK != FILE_GetUtcTime(pszFileName, NULL, &ulNowModifyTime, &ulNowAccessTime))
        {
            RETURN(BS_ERR);
        }
    }

    switch (eAccessTimeMode)
    {
        case FILE_TIME_MODE_NOTMODIFY:
            ulNewAccessTime = ulOldAccessTime;
            break;
        case FILE_TIME_MODE_SET:
            ulNewAccessTime = ulAccessTime;
            break;
        case FILE_TIME_MODE_NOW:
            ulNewAccessTime = ulNowAccessTime;
            break;
        default:
            ulNewAccessTime = 0;
            break;
    }

    switch (eModifyTimeMode)
    {
        case FILE_TIME_MODE_NOTMODIFY:
            ulNewModifyTime = ulOldModifyTime;
            break;
        case FILE_TIME_MODE_SET:
            ulNewModifyTime = ulModifyTime;
            break;
        case FILE_TIME_MODE_NOW:
            ulNewModifyTime = ulNowModifyTime;
            break;
        default:
            ulNewModifyTime = 0;
            break;
    }

    memset(&stTime, 0, sizeof(stTime));
    stTime.actime = ulNewAccessTime;
    stTime.modtime = ulNewModifyTime;

    if (0 != utime(pszFileName, &stTime))
    {
        RETURN(BS_ERR);
    }

    return BS_OK;
}


BS_STATUS FILE_MakeDir(char *path)
{
    int ret = mkdir(path, 0777);
    if (ret < 0) {
        if (errno == EEXIST) {
            return BS_ALREADY_EXIST;
        }
    }

    return ret;
}


BS_STATUS FILE_MakeDir2(char *path)
{
    int ret = mkdir(path, 0777);
    if (ret < 0) {
        if (errno == EEXIST) {
            return 0;
        }
    }

    return ret;
}



BS_STATUS FILE_MakeDirs(IN CHAR *pszPath)
{
    CHAR *pcSplit, *pszDir;
    CHAR szPath[FILE_MAX_PATH_LEN + 1];
    INT lRet = BS_OK;

    BS_DBGASSERT(NULL != pszPath);

    if (strlen(pszPath) > FILE_MAX_PATH_LEN)
    {
        RETURN(BS_TOO_LONG);
    }

    strlcpy(szPath, pszPath, sizeof(szPath));

    FILE_PATH_TO_UNIX(szPath);

    pszDir = szPath;
    while (NULL != (pcSplit = strchr(pszDir, '/')))
    {
        *pcSplit = '\0';
        lRet = mkdir(szPath, 0777);
        *pcSplit = '/';
        pszDir = pcSplit + 1;
    }

    if (lRet  == EEXIST)
    {
        RETURN(BS_ALREADY_EXIST);
    }

    return BS_OK;
}


BS_STATUS FILE_MakePath(IN CHAR *pszPath)
{
    CHAR *pcSplit, *pszDir;
    CHAR szPath[FILE_MAX_PATH_LEN + 1];
    int ret = 0;

    BS_DBGASSERT(NULL != pszPath);

    if (strlen(pszPath) > FILE_MAX_PATH_LEN) {
        RETURN(BS_TOO_LONG);
    }

    strlcpy(szPath, pszPath, sizeof(szPath));

    FILE_PATH_TO_UNIX(szPath);

    pszDir = szPath;
    while (NULL != (pcSplit = strchr(pszDir, '/'))) {
        *pcSplit = '\0';
        if (szPath[0]) {
            ret = FILE_MakeDir2(szPath);
            if (ret != 0) {
                break;
            }
        }
        *pcSplit = '/';
        pszDir = pcSplit + 1;
    }

    return FILE_MakeDir2(pszPath);
}

BS_STATUS FILE_MakeFile(IN CHAR *pszFilePath)
{
    FILE * fp;

    fp = FILE_Open(pszFilePath, TRUE, "ab+");
    if (NULL == fp)
    {
        return BS_ERR;
    }
    fclose(fp);

    return BS_OK;
}

BOOL_T FILE_DelFile(IN CHAR *pszFilePath)
{
    if (0 == remove(pszFilePath))
    {
        return TRUE;
    }

    return FALSE;
}

BS_STATUS FILE_DelDir(IN CHAR *pszPath)
{
    CHAR szPath[FILE_MAX_PATH_LEN + 1];
    INT ulLen;
    CHAR *pszName;
    CHAR szSubPath[FILE_MAX_PATH_LEN + 1];

    strlcpy(szPath, pszPath, sizeof(szPath));

    ulLen = strlen(szPath);

    FILE_PATH_TO_UNIX(szPath);

    if (szPath[ulLen-1] != '/')
    {
        szPath[ulLen] = '/';
        szPath[ulLen+1] = '\0';
    }

#ifdef IN_WINDOWS
    TXT_Strlcat(szPath, "*", sizeof(szPath));
#endif

    FILE_PATH_TO_HOST(szPath);

    DIR_SCAN_START(szPath, pszName)
    {
        strlcpy(szSubPath, szPath, sizeof(szSubPath));
#ifdef IN_WINDOWS
        szSubPath[strlen(szSubPath)-1] = '\0';
#endif
		TXT_Strlcat(szSubPath, pszName, sizeof(szSubPath));
        FILE_DelDir(szSubPath);
    }
    DIR_SCAN_END();

    FILE_SCAN_START(szPath, pszName)
    {
        strlcpy(szSubPath, szPath, sizeof(szSubPath));
#ifdef IN_WINDOWS
		szSubPath[strlen(szSubPath)-1] = '\0';
#endif
        TXT_Strlcat(szSubPath, pszName, sizeof(szSubPath));
        FILE_DelFile(szSubPath);
    }
    FILE_SCAN_END();

    strlcpy(szPath, pszPath, sizeof(szPath));
    FILE_PATH_TO_HOST(szPath);

    if (0 != rmdir(pszPath))
    {
        RETURN(BS_ERR);
    }

    return BS_OK;
}


FILE * FILE_Open(IN CHAR *pszFilePath, IN BOOL_T bIsCreateDirIfNotExist, IN CHAR *pszOpenMode)
{
    CHAR szPath[FILE_MAX_PATH_LEN + 1];

    if (strlen(pszFilePath) > FILE_MAX_PATH_LEN)
    {
        return NULL;
    }

    if (bIsCreateDirIfNotExist == TRUE)
    {
        FILE_MakeDirs (pszFilePath);
    }

    strlcpy(szPath, pszFilePath, sizeof(szPath));
    FILE_PATH_TO_HOST(szPath);
    
    return FOPEN(szPath, pszOpenMode);
}

VOID FILE_Close(IN FILE *fp)
{
    fclose(fp);
}

UINT FILE_Read(IN FILE *fp, OUT UCHAR *pucBuf, IN UINT uiBufSize)
{
    return fread(pucBuf, 1, uiBufSize, fp);
}

BOOL_T FILE_IsHaveUtf8Bom(IN FILE *fp)
{
    UCHAR aucBom[3];
    UINT uiReadLen;

    uiReadLen = FILE_Read(fp, aucBom, sizeof(aucBom));
    fseek(fp, 0, SEEK_SET);

    if (uiReadLen < 3)
    {
        return FALSE;
    }

    if ((aucBom[0] == 0xEF) && (aucBom[1] == 0xBB) && (aucBom[2] == 0xBF))
    {
        return TRUE;
    }

    return FALSE;
}


BS_STATUS FILE_CopyTo(IN CHAR *pszSrcFileName, IN CHAR *pszDstFileName)
{
    FILE *fp, *fq;
    UCHAR ucChar;
    time_t ulModifyTime;
    time_t ulAccessTime;
    
    BS_DBGASSERT(NULL != pszSrcFileName);
    BS_DBGASSERT(NULL != pszDstFileName);

    fp = FILE_Open(pszSrcFileName, FALSE, "rb+");
    if (NULL == fp)
    {
        RETURN(BS_CAN_NOT_OPEN);
    }

    fq = FILE_Open(pszDstFileName, TRUE, "wb+");
    if (fq == NULL)
    {
        fclose(fp);
        RETURN(BS_CAN_NOT_OPEN);
    }

    for (ucChar = fgetc(fp); !feof(fp); ucChar = fgetc(fp))
    {
        fputc(ucChar, fq);
    }

    fclose(fp);
    fclose(fq);

    FILE_GetUtcTime(pszSrcFileName, NULL, &ulModifyTime, &ulAccessTime);
    FILE_SetUtcTime(pszDstFileName, FILE_TIME_MODE_SET, ulModifyTime, FILE_TIME_MODE_SET, ulAccessTime);

    return BS_OK;
}


BS_STATUS FILE_MoveTo(IN CHAR *pszSrcFileName, IN CHAR *pszDstFileName, IN BOOL_T bOverWrite)
{
    CHAR szBakFileName[FILE_MAX_PATH_LEN + 1];
    BS_DBGASSERT(NULL != pszSrcFileName);
    BS_DBGASSERT(NULL != pszDstFileName);

    if (strlen(pszDstFileName) > FILE_MAX_PATH_LEN - 4)
    {
        RETURN(BS_ERR);
    }

    if (bOverWrite == TRUE)
    {
        
        strlcpy(szBakFileName, pszDstFileName, sizeof(szBakFileName));
        TXT_Strlcat(szBakFileName, ".cnjia1", sizeof(szBakFileName));
        FILE_DelFile(szBakFileName);
        rename(pszDstFileName, szBakFileName);
    }

    
    if (0 != rename(pszSrcFileName, pszDstFileName))
    {
        if (bOverWrite == TRUE)
        {
            
            rename(szBakFileName, pszDstFileName);
        }

        RETURN(BS_CAN_NOT_OPEN);
    }

    if (bOverWrite == TRUE)
    {
        FILE_DelFile(szBakFileName);
    }

    return BS_OK;
}

VOID FILE_WriteStr(IN FILE *fp, IN CHAR *pszString)
{
    fwrite(pszString, 1, strlen(pszString), fp);
}

int FILE_MemByData(void *data, int data_len, OUT FILE_MEM_S *m)
{
    m->len = data_len;

    m->data = MEM_Malloc(data_len + 1);
    if (! m->data) {
        RETURN(BS_NO_MEMORY);
    }

    memcpy(m->data, data, data_len);

    m->data[data_len] = '\0';

    return 0;
}

typedef struct {
    CHAR szUserScanDir[FILE_MAX_PATH_LEN + 1];    
    CHAR *pcPattern; 
    PF_ScanFile_Output pfOutput;
    VOID *pUserData;
    CHAR szCurrentScanDir[FILE_MAX_PATH_LEN + 1]; 
    CHAR szScanPattern[FILE_MAX_PATH_LEN + 1]; 
}FILE_SCANFILE_S;

static VOID file_ScanFile(IN FILE_SCANFILE_S *pstScanFile)
{
    CHAR *pcFileName;
    CHAR *pcSubDirName;
    UINT uiCurrentScanDirLen;

    uiCurrentScanDirLen = strlen(pstScanFile->szCurrentScanDir);

    if (uiCurrentScanDirLen != 0)
    {
    	if (snprintf(pstScanFile->szScanPattern, sizeof(pstScanFile->szScanPattern), "%s/%s%s",
                    pstScanFile->szUserScanDir, pstScanFile->szCurrentScanDir, pstScanFile->pcPattern) < 0) {
            return;
        }
    }
    else
    {
        if (snprintf(pstScanFile->szScanPattern, sizeof(pstScanFile->szScanPattern),
                    "%s/%s", pstScanFile->szUserScanDir, pstScanFile->pcPattern) < 0) {
            return;
        }
    }

    FILE_SCAN_START(pstScanFile->szScanPattern, pcFileName)
	{
        pstScanFile->pfOutput(pstScanFile->szCurrentScanDir, pcFileName, pstScanFile->pUserData);
	}FILE_SCAN_END();

    if (uiCurrentScanDirLen == 0)
    {
        if (snprintf(pstScanFile->szScanPattern, sizeof(pstScanFile->szScanPattern),
                    "%s/*", pstScanFile->szUserScanDir) < 0) {
            return;
        }
    }
    else
    {
        if (snprintf(pstScanFile->szScanPattern, sizeof(pstScanFile->szScanPattern),
                    "%s/%s*", pstScanFile->szUserScanDir, pstScanFile->szCurrentScanDir) < 0) {
            return;
        }
    }

	DIR_SCAN_START(pstScanFile->szScanPattern, pcSubDirName)
	{
        sprintf(&pstScanFile->szCurrentScanDir[uiCurrentScanDirLen], "%s/", pcSubDirName);
		file_ScanFile(pstScanFile);
	}DIR_SCAN_END();

    return;
}

VOID FILE_ScanFile
(
    IN CHAR *pcScanDir,
    IN CHAR *pcPattern, 
    IN PF_ScanFile_Output pfOutput,
    IN VOID *pUserData
)
{
    FILE_SCANFILE_S stScanFile;
    UINT uiScanDirLen;

    if (NULL == pcPattern)
    {
        pcPattern = "*";
    }

    uiScanDirLen = strlen(pcScanDir);
    if (uiScanDirLen == 0)
    {
        BS_DBGASSERT(0);
        return;
    }

    memset(&stScanFile, 0, sizeof(stScanFile));
    strlcpy(stScanFile.szUserScanDir, pcScanDir, sizeof(stScanFile.szUserScanDir));
    stScanFile.pcPattern = pcPattern;
    stScanFile.pfOutput = pfOutput;
    stScanFile.pUserData = pUserData;
    stScanFile.szCurrentScanDir[0] = '\0';

    if (stScanFile.szUserScanDir[uiScanDirLen - 1] == '/')
    {
        stScanFile.szUserScanDir[uiScanDirLen - 1] = '\0';
    }

    file_ScanFile(&stScanFile);

    return;
}


int FILE_ReadLine(FILE *fp, char *line, int size, char end_char)
{
    char *tmp = line;
    int count = 0;
    int c;

    BS_DBGASSERT(size > 0);

    line[size - 1] = 0;

    do {
        c = fgetc(fp);
        if( feof(fp) ) {
            break ;
        }
        count ++;
        if (count < size) {
            *tmp = c;
            tmp ++;
        }
    }while(c != end_char);

    if (count < size) {
        line[count] = 0;
    }

    return count;
}

