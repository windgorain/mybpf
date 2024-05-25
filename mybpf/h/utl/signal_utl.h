/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2011-7-6
* Description: 
* History:     
******************************************************************************/

#ifndef __SIGNAL_UTL_H_
#define __SIGNAL_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

#ifdef IN_UNIXLIKE

VOID * SIGNAL_Set(IN int iSigno, IN BOOL_T bRestart, IN VOID * pfFunc);

typedef void (*SHC_HANDLE_FUNC)(int sig, void *user_data);

typedef struct {
    DLL_NODE_S link_node;
    SHC_HANDLE_FUNC pfHandler;
    void * user_data;
}SIGNAL_HANDLER_S;

void SHC_Reg(int sig, IN SIGNAL_HANDLER_S *node);
void SHC_Unreg(IN SIGNAL_HANDLER_S *node);
void * SHC_Watch(int sig);

void (*setsignal(int signum, void (*sighandler)(int, siginfo_t *, void *)))(int);
const char *signal_id2name(int id);
int signal_name2id(char *sig_name);
void pthread_set_ignore_sig(int *sig_array, int sig_num);
void pthread_clear_sig(void);

#define PTHREAD_SET_IGNORE_SIG()                                    \
    do {                                                            \
        int TMPV(sig)[] = {SIGINT, SIGTERM};                        \
        pthread_set_ignore_sig(TMPV(sig), ARRAY_LENGTH(TMPV(sig))); \
    } while (0)

#define PTHREAD_SET_IGNORE_SIG_1(_special_sig)                      \
    do {                                                            \
        int TMPV(sig)[] = {SIGINT, SIGTERM, _special_sig};          \
        pthread_set_ignore_sig(TMPV(sig), ARRAY_LENGTH(TMPV(sig))); \
    } while (0)

#define PTHREAD_SET_IGNORE_ONE_SIG(_special_sig)                    \
    do {                                                            \
        int TMPV(sig)[] = {_special_sig};                           \
        pthread_set_ignore_sig(TMPV(sig), 1);                       \
    } while (0)

#endif

#ifdef __cplusplus
    }
#endif 

#endif 


