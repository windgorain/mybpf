/*********************************************************
*   Copyright (C), Xingang.Li
*   Author:      Xingang.Li  Version: 1.0
*   Description: 
*
********************************************************/
#include "bs.h"
#include "utl/args_utl.h"
#include "utl/subcmd_utl.h"
#include "utl/getopt2_utl.h"
#include "utl/mybpf_bare.h"

static MYBPF_BARE_S g_mybpf_bare;

static void _opt_help(GETOPT2_NODE_S *opt)
{
    char buf[4096];
    printf("%s", GETOPT2_BuildHelpinfo(opt, buf, sizeof(buf)));
    return;
}

static int _load_bare(int argc, char **argv)
{
    char *filename=NULL;
    GETOPT2_NODE_S opt[] = {
        {'P', 0, "filename", GETOPT2_V_STRING, &filename, "bpf file name", 0},
        {0} };

    if (BS_OK != GETOPT2_Parse(argc, argv, opt)) {
        _opt_help(opt);
        return -1;
    }

    if (filename == NULL) {
        _opt_help(opt);
        return -1;
    }

    int ret = MYBPF_LoadBareFile(filename, NULL, &g_mybpf_bare);
    if (ret < 0) {
        ErrCode_PrintErrInfo();
        return ret;
    }

    return 0;
}

static int _unload_bare(int argc, char **argv)
{
    MYBPF_UnloadBare(&g_mybpf_bare);
    return 0;
}

static int _run_bare_params(char *params)
{
    char *argv[32];
    int argc = 0;
    MYBPF_PARAM_S p = {0};

    if (! g_mybpf_bare.prog) {
        fprintf(stderr, "Error: Bare not loaded \n");
        return -1;
    }

    if (params) {
        argc = ARGS_Split(params, argv, ARRAY_SIZE(argv));
    }

    p.p[0] = argc;
    p.p[1] = (long)argv;
 
    return MYBPF_RunBareMain(&g_mybpf_bare, &p);
}

static int _run_bare(int argc, char **argv)
{
    char *params = NULL;
    GETOPT2_NODE_S opt[] = {
        {'o', 'p', "params", GETOPT2_V_STRING, &params, "params", 0},
        {0} };

    if (BS_OK != GETOPT2_Parse(argc, argv, opt)) {
        _opt_help(opt);
        return -1;
    }

    return _run_bare_params(params);
}

static int _quit(int argc, char **argv)
{
    exit(0);
}

static int _run(int argc, char **argv)
{
    static SUB_CMD_NODE_S subcmds[] = {
        {"load", _load_bare, "load bare file"},
        {"unload", _unload_bare, "unload bare"},
        {"run", _run_bare, "run bare"},
        {"quit", _quit, "quit"},
        {NULL}
    };

    return SUBCMD_DoParams(subcmds, argc, argv);
}

int main(void)
{
    char line[256];
    char *argv[32];
    int argc = 0;

	printf("> ");

    while(NULL != fgets(line, sizeof(line), stdin)){
        argc = ARGS_Split(line, argv, ARRAY_SIZE(argv));
        if (argc > 0) {
            _run(argc, argv);
        }
        printf("> ");
    }

    return 0;
}

