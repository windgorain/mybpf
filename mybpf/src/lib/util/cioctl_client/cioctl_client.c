/*================================================================
*   Created by LiXingang, Copyright LiXingang
*   Description: 
*
================================================================*/
#include "bs.h"
#include "utl/npipe_utl.h"
#include "utl/cioctl_utl.h"

int CIOCTL_Connect(char *path)
{
    if (! path) {
        path = CIOCTL_PIPE_FILE_PATH;
    }

    return NPIPE_ConnectStream(path);
}

void CIOCTL_Close(int fd)
{
    close(fd);
}


