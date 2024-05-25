/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2010-1-14
* Description: 
* History:     
******************************************************************************/

#ifndef __PATH_UTL_H_
#define __PATH_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

#include "tree_utl.h"

#define PATH_WALK_DIRS_START(_pszPath, _pszDir, _ulLen)   \
    do{   \
        CHAR *_pcSplit, *_pcEnd; \
        _pcSplit = strchr(_pszPath, '/');    \
        if (NULL != _pcSplit)    {   \
            _pcSplit ++;    \
            while ((_pcSplit != NULL) && (*_pcSplit != '\0'))   {   \
                _pcEnd = strchr(_pcSplit, '/'); \
                _pszDir = _pcSplit; \
                if (NULL == _pcEnd){_ulLen = strlen(_pcSplit); _pcSplit = NULL;} \
                else {_ulLen = _pcEnd - _pcSplit; _pcSplit = _pcEnd + 1;} \
                {
        

#define PATH_WALK_END()    }}}}while(0)

#ifdef __cplusplus
    }
#endif 

#endif 


