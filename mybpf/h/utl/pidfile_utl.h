/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _PIDFILE_UTL_H
#define _PIDFILE_UTL_H
#ifdef __cplusplus
extern "C"
{
#endif

int PIDFILE_Create(char *filename);
int PIDFILE_Lock(char *filename);
int PIDFile_ReadPID(char *filename);

#ifdef __cplusplus
}
#endif
#endif 
