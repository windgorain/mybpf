/*********************************************************
*   Copyright (C), Xingang.Li
*   Author:      Xingang.Li  Version: 1.0
*   Description: 
*
********************************************************/
#include "bs.h"
#include "utl/args_utl.h"
#include "utl/arch_utl.h"
#include "utl/file_func.h"
#include "utl/subcmd_utl.h"
#include "utl/getopt2_utl.h"
#include "utl/mybpf_bare.h"
#include "utl/mybpf_loader_def.h"
#include "utl/mybpf_hookpoint_def.h"
#include "utl/mybpf_spf_def.h"

static MYBPF_BARE_S g_mybpf_bare;
static MYBPF_SPF_S *g_mybpf_spf_ctrl;

static void _opt_help(GETOPT2_NODE_S *opt)
{
    char buf[4096];
    printf("%s", GETOPT2_BuildHelpinfo(opt, buf, sizeof(buf)));
    return;
}

static int _load_spf(int argc, char **argv)
{
    MYBPF_LOADER_PARAM_S p = {0};
    char *filename=NULL;
    char *instance=NULL;
    GETOPT2_NODE_S opt[] = {
        {'P', 0, "instance", GETOPT2_V_STRING, &instance, "instance name", 0},
        {'P', 0, "filename", GETOPT2_V_STRING, &filename, "bpf file name", 0},
        {0} };

    if (BS_OK != GETOPT2_Parse(argc, argv, opt)) {
        _opt_help(opt);
        return -1;
    }

    p.instance = instance;
    p.filename = filename;
    p.flag = MYBPF_LOADER_FLAG_AUTO_ATTACH;

    int ret = g_mybpf_spf_ctrl->load_instance(&p);
    if (ret < 0) {
        printf("Load failed \n");
        return ret;
    }

    return 0;
}

static int _show_spf(int argc, char **argv)
{
    g_mybpf_spf_ctrl->show_all_instance();
    return 0;
}

static int _unload_spf(int argc, char **argv)
{
    char *instance = NULL;

    GETOPT2_NODE_S opt[] = {
        {'P', 0, "instance", GETOPT2_V_STRING, &instance, "instance name", 0},
        {0} };

    if (BS_OK != GETOPT2_Parse(argc, argv, opt)) {
        _opt_help(opt);
        return -1;
    }

    return g_mybpf_spf_ctrl->unload_instance(instance);
}

static int _unload_all_spf(int argc, char **argv)
{
    g_mybpf_spf_ctrl->unload_all_instance();
    return 0;
}

static int _run_spf_run_testcmd(char *params)
{
    char *argv[32];
    int argc = 0;
    MYBPF_PARAM_S p = {0};

    if (params) {
        argc = ARGS_Split(params, argv, ARRAY_SIZE(argv));
    }

    p.p[0] = argc;
    p.p[1] = (long)argv;
 
    return g_mybpf_spf_ctrl->run(MYBPF_HP_TCMD, &p);
}

static int _test_spf_testcmd(int argc, char **argv)
{
    char *params = NULL;
    GETOPT2_NODE_S opt[] = {
        {'o', 'p', "params", GETOPT2_V_STRING, &params, "params", 0},
        {0} };

    if (BS_OK != GETOPT2_Parse(argc, argv, opt)) {
        _opt_help(opt);
        return -1;
    }

    return _run_spf_run_testcmd(params);
}

static void _quit(int argc, char **argv)
{
    g_mybpf_spf_ctrl->finit();
    MYBPF_UnloadBare(&g_mybpf_bare);
    exit(0);
}

static int _run_spf(int argc, char **argv)
{
    static SUB_CMD_NODE_S subcmds[] = {
        {"load", _load_spf, "load spf file"},
        {"show", _show_spf, "show instance"},
        {"unload instance", _unload_spf, "unload spf"},
        {"unload all", _unload_all_spf, "unload all spf"},
        {"testcmd", _test_spf_testcmd, "test spf cmd"},
        {"quit", _quit, "quit"},
        {NULL}
    };

    return SUBCMD_DoParams(subcmds, argc, argv);
}

static int _load_spf_loader(FILE_MEM_S *m, char *config_file)
{
    int ret;
    MYBPF_PARAM_S p = {0};

    ret = MYBPF_LoadBare(m->data, m->len, NULL, &g_mybpf_bare);
    if (ret < 0) {
        printf("Load spfloader failed \n");
        return ret;
    }

    p.p[0] = (long)m->data;
    MYBPF_RunBareMain(&g_mybpf_bare, &p);

    g_mybpf_spf_ctrl = (void*)(long)p.bpf_ret;
    if (! g_mybpf_spf_ctrl) {
        printf("Can't get spf ctrl \n");
        return -1;
    }

    ret = g_mybpf_spf_ctrl->init();
    if (ret < 0) {
        printf("SPF init failed \n");
        return -1;
    }

    if (config_file) {
        ret = g_mybpf_spf_ctrl->config_by_file(config_file);
        if (ret < 0) {
            printf("Load spf config failed \n");
            return ret;
        }
    }

    return 0;
}

static int _load_spf_loader_file(char *spf_loader, char *config_file)
{
    FILE_MEM_S m = {0};

    int ret = FILE_Mem2m(spf_loader, &m);
    if (ret < 0) {
        printf("Can't open file %s", spf_loader);
        return -1;
    }

    ret = _load_spf_loader(&m, config_file);

    FILE_FreeMem(&m);

    return ret;
}

static int _run_cmd()
{
    char line[256];
    char *argv[32];
    int argc = 0;

	printf("> ");

    while(NULL != fgets(line, sizeof(line), stdin)){
        argc = ARGS_Split(line, argv, ARRAY_SIZE(argv));
        if (argc > 0) {
            _run_spf(argc, argv);
        }
        printf("> ");
    }

    return 0;
}

static int _run(char *spf_loader, char *config_file)
{
    if (_load_spf_loader_file(spf_loader, config_file) < 0) {
        return -1;
    }

    return _run_cmd();
}


int main(int argc, char **argv)
{
    char *spfloader = NULL;
    char *config_file = NULL;

    GETOPT2_NODE_S opt[] = {
        {'o', 's', "spfloader", GETOPT2_V_STRING, &spfloader, "spf loader file name", 0},
        {'o', 'c', "cfgfile", GETOPT2_V_STRING, &config_file, "config file name", 0},
        {0} };

    ErrCode_EnablePrint(1);

    if (BS_OK != GETOPT2_Parse(argc, argv, opt)) {
        _opt_help(opt);
        return -1;
    }

    if (! spfloader) {
        if (ARCH_LocalArch() == ARCH_TYPE_ARM64) {
            spfloader = "spf_loader.arm64.bare";
        } else if (ARCH_LocalArch() == ARCH_TYPE_X86_64) {
            spfloader = "spf_loader.x64.bare";
        } else {
            fprintf(stderr, "Can't support this arch \n");
            return -1;
        }
    }

    return _run(spfloader, config_file);
}

