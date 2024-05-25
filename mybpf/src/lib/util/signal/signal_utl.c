/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2011-7-6
* Description: 
* History:     
******************************************************************************/
#include "bs.h"

#include "utl/signal_utl.h"

#ifdef IN_UNIXLIKE
static VOID * signal_Set(IN int iSigno, IN BOOL_T bRestart, IN VOID * pfFunc)
{
    struct sigaction act, oact;

    act.sa_handler = pfFunc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    if (iSigno == SIGALRM) {
        bRestart = FALSE;
    }

    if (bRestart) {
#ifdef SA_RESTART
        act.sa_flags |= SA_RESTART;
#endif
    } else {
#ifdef SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT;
#endif
    }

    if (sigaction(iSigno, &act, &oact) < 0)
    {
        return SIG_ERR;
    }

    return oact.sa_handler;
}
#endif

#ifdef IN_WINDOWS
static VOID * signal_Set(IN int iSigno, IN BOOL_T bRestart, IN VOID * pfFunc)
{
    return signal(iSigno, pfFunc);
}
#endif

VOID * SIGNAL_Set(IN int iSigno, IN BOOL_T bRestart, IN VOID * pfFunc)
{
    return signal_Set(iSigno, bRestart, pfFunc);
}

void (*setsignal(int signum, void (*sighandler)(int, siginfo_t *, void *)))(int)
{
    struct sigaction action;
    struct sigaction old;
    memset(&action, 0, sizeof(action));
    sigemptyset(&action.sa_mask);
    action.sa_sigaction = sighandler;
    action.sa_flags = SA_SIGINFO;

    if (signum == SIGALRM) {
#ifdef SA_INTERRUPT
        action.sa_flags |= SA_INTERRUPT;
#endif
    } else {
#ifdef SA_RESTART
        action.sa_flags |= SA_RESTART;
#endif
    }

    if (sigaction(signum, &action, &old) < 0) {
        fprintf(stderr, "setsignal %d err!\n", signum);
        return (SIG_ERR);
    }

    return old.sa_handler;
}

#define MAX_SIGNAL_NUM 65
static const char *signal_list[MAX_SIGNAL_NUM] = {
    [0] = "ZERO",
    [1] = "SIGHUP", [2] = "SIGINT", [3] = "SIGQUIT", [4] = "SIGILL", [5] = "SIGTRAP",
    [6] = "SIGABRT", [7] = "SIGBUS", [8] = "SIGFPE", [9] = "SIGKILL", [10] = "SIGUSR1",
    [11] = "SIGSEGV", [12] = "SIGUSR2", [13] = "SIGPIPE", [14] = "SIGALRM", [15] = "SIGTERM",
    [16] = "SIGSTKFLT", [17] = "SIGCHLD", [18] = "SIGCONT", [19] = "SIGSTOP", [20] = "SIGTSTP",
    [21] = "SIGTTIN", [22] = "SIGTTOU", [23] = "SIGURG", [24] = "SIGXCPU", [25] = "SIGXFSZ",
    [26] = "SIGVTALRM", [27] = "SIGPROF", [28] = "SIGWINCH", [29] = "SIGIO", [30] = "SIGPWR",
    [31] = "SIGSYS", [32] = "UNKNOWN", [33] = "UNKNOWN", [34] = "SIGRTMIN", [35] = "SIGRTMIN+1",
    [36] = "SIGRTMIN+2", [37] = "SIGRTMIN+3",
    [38] = "SIGRTMIN+4", [39] = "SIGRTMIN+5", [40] = "SIGRTMIN+6", [41] = "SIGRTMIN+7", [42] = "SIGRTMIN+8",
    [43] = "SIGRTMIN+9", [44] = "SIGRTMIN+10", [45] = "SIGRTMIN+11", [46] = "SIGRTMIN+12", [47] = "SIGRTMIN+13",
    [48] = "SIGRTMIN+14", [49] = "SIGRTMIN+15", [50] = "SIGRTMAX-14", [51] = "SIGRTMAX-13", [52] = "SIGRTMAX-12",
    [53] = "SIGRTMAX-11", [54] = "SIGRTMAX-10", [55] = "SIGRTMAX-9", [56] = "SIGRTMAX-8", [57] = "SIGRTMAX-7",
    [58] = "SIGRTMAX-6", [59] = "SIGRTMAX-5", [60] = "SIGRTMAX-4", [61] = "SIGRTMAX-3", [62] = "SIGRTMAX-2",
    [63] = "SIGRTMAX-1", [64] = "SIGRTMAX"
};

const char *signal_id2name(int id)
{
    if (id < 0 || id >= MAX_SIGNAL_NUM) {
        return "errnum";
    } else {
        return signal_list[id];
    }
}

int signal_name2id(char *sig_name)
{
    int idx;
    for (idx = 0; idx <MAX_SIGNAL_NUM; idx++){
        if (signal_list[idx] && !strcasecmp(sig_name, signal_list[idx])) {
            return idx;
        }
    }

    return 0;
}

void pthread_set_ignore_sig(int *sig_array, int sig_num)
{
    sigset_t mask;
    sigemptyset(&mask);

    int idx;
    for (idx = 0; idx < sig_num; idx++) {
        sigaddset(&mask, sig_array[idx]);
    }

    pthread_sigmask(SIG_SETMASK, &mask, NULL);
}

void pthread_clear_sig(void)
{
    sigset_t mask;
    sigemptyset(&mask);

    pthread_sigmask(SIG_SETMASK, &mask, NULL);
}
