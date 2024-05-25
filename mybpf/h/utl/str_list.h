/******************************************************************************
* Copyright (C), 2000-2006,  Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2016-8-30
* Description: 
* History:     
******************************************************************************/

#ifndef __STR_LIST_H_
#define __STR_LIST_H_

#ifdef __cplusplus
    extern "C" {
#endif 

typedef HANDLE STRLIST_HANDLE;

typedef struct
{
    DLL_NODE_S stLinkNode;
    LSTR_S stStr;
}STRLIST_NODE_S;


#define STRLIST_FLAG_CASE_SENSITIVE 0x1  
#define STRLIST_FLAG_CHECK_REPEAT   0x2  


STRLIST_HANDLE StrList_Create(IN UINT uiFlag);
BS_STATUS StrList_Add(IN STRLIST_HANDLE hStrList, IN CHAR *pcStr);
VOID StrList_Del(IN STRLIST_HANDLE hStrList, IN CHAR *pcStr);
CHAR * StrList_Find(IN STRLIST_HANDLE hStrList, IN CHAR *pcStr);
CHAR * StrList_FindByLstr(IN STRLIST_HANDLE hStrList, IN LSTR_S *pstLstr);
STRLIST_NODE_S * StrList_GetNext(IN STRLIST_HANDLE hStrList, IN STRLIST_NODE_S *pstCurr);

#ifdef __cplusplus
    }
#endif 

#endif 


