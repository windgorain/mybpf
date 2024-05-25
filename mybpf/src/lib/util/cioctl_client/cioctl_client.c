/*================================================================
*   Created by LiXingang, Copyright LiXingang
*   Description: 
*
================================================================*/
#include "bs.h"
#include "utl/cioctl_utl.h"

int CIOCTL_Connect(char *path)
{
    int fd;
    struct sockaddr_un dst;

    if (! path) {
        path = CIOCTL_PIPE_FILE_PATH;
    }

    return NPIPE_ConnectStream(path);
}

int CIOCTL_Close(int fd)
{
    close(fd);
}


