/******************************************************************************
* Copyright (C),    LiXingang
* Author:      LiXingang  Version: 1.0  Date: 2014-3-26
* Description: 
* History:     
******************************************************************************/
#include "bs.h"
#include <fcntl.h>
#include <sys/stat.h>
#include "utl/daemon_utl.h"

#ifdef IN_LINUX
int DAEMON_Init(int nochdir, int noclose)
{
    return daemon(nochdir, noclose);
}
#endif

#ifdef IN_MAC
int DAEMON_Init(int nochdir, int noclose)
{
    pid_t pid;
    struct sigaction sa;

    umask(0);

    pid = fork();
    if (pid < 0) {
        return -1;
    }

    if (pid > 0) { 
        exit(0);
    }

    setsid();

    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGHUP, &sa, NULL);
 
    pid = fork();
    if (pid  < 0) {
        return -1;
    }

    if (pid > 0)  {
        exit(0);
    }

    if (! nochdir) {
        chdir("/");
    }

    close(0);
    close(1);
    close(2);
    open("/dev/null", O_RDWR);
    dup(0);
    dup(0);

    return 0;
}
#endif

