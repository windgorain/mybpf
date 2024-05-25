/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2007-9-6
* Description: 
* History:     
******************************************************************************/

#ifndef __EVENT_UTL_H_
#define __EVENT_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

#define EVENT_FLAG_CARE_ALL 0x1 
#define EVENT_FLAG_WAIT     0x2 

#define EVENT_ALL (0xffffffffffffffffULL)

typedef HANDLE EVENT_HANDLE;

extern EVENT_HANDLE Event_Create(void);
extern VOID Event_Delete (IN EVENT_HANDLE hEventID);
extern BS_STATUS Event_Write(EVENT_HANDLE hEventID, UINT64 events);
extern BS_STATUS Event_Read(EVENT_HANDLE hEventID, UINT64 events,
        OUT UINT64 *events_out, UINT ulFlag, UINT ulTimeToWait);

#ifdef __cplusplus
    }
#endif 

#endif 


