/*================================================================
*   Created by LiXingang, Copyright LiXingang
*   Description: 
*
================================================================*/
#ifndef _CIOCTL_UTL_H
#define _CIOCTL_UTL_H
#ifdef __cplusplus
extern "C"
{
#endif

#define CIOCTL_MODULE_NAME_SIZE 32
#define CIOCTL_PIPE_FILE_PATH "/tmp/cioctl_pipe"

typedef struct {
    UINT size; 
    char name[CIOCTL_MODULE_NAME_SIZE];
    
}CIOCTL_REQUEST_S;

typedef struct {
    UINT size; 
    int ret;
    
}CIOCTL_REPLY_S;

#ifdef __cplusplus
}
#endif
#endif 
