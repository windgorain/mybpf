/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2011-7-12
* Description: 
* History:     
******************************************************************************/

#ifndef __NPIPE_UTL_H_
#define __NPIPE_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

#ifdef IN_UNIXLIKE

int NPIPE_OpenDgram(IN CHAR *name);


int NPIPE_OpenStream(IN CHAR *name);
int NPIPE_ConnectStream(const char *name);

int NPIPE_OpenSeqpacket(IN CHAR *name);
int NPIPE_ConnectSeqpacket(const char *name);

int NPIPE_Accept(int listenfd);

#endif

#ifdef __cplusplus
    }
#endif 

#endif 


