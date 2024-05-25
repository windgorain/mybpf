/* Added by lixg */

#include <common.h>
#include <command.h>
#include <display_options.h>
#include <timestamp.h>
#include <version.h>
#include <version_string.h>
#include <linux/compiler.h>
#ifdef CONFIG_SYS_COREBOOT
#include <asm/cb_sysinfo.h>
#endif

#include <vsprintf.h>
#include "bs.h"
#include "utl/mybpf_bare.h"
#include "utl/mybpf_loader_def.h"
#include "utl/mybpf_hookpoint_def.h"
#include "utl/mybpf_spf_def.h"

static MYBPF_BARE_S g_mybpf_bare;
static MYBPF_SPF_S *g_mybpf_spf_ctrl;

static int do_load_spf(struct cmd_tbl *cmdtp, int flag, int argc, char *const argv[])
{
    MYBPF_LOADER_PARAM_S p = {0};

    if (argc < 3) {
		return CMD_RET_USAGE;
    }

    p.instance = argv[1];
    p.flag = MYBPF_LOADER_FLAG_AUTO_ATTACH;
    p.simple_mem.data = (void*)hextoul(argv[2], NULL);

    int ret = g_mybpf_spf_ctrl->load_instance(&p);
    if (ret < 0) {
        printf("Load failed \n");
        return ret;
    }

    return 0;
}

static int do_unload_spf(struct cmd_tbl *cmdtp, int flag, int argc, char *const argv[])
{
    if (argc < 2) {
        return CMD_RET_USAGE;
    }

    return g_mybpf_spf_ctrl->unload_instance(argv[1]);
}

static int do_unload_all_spf(struct cmd_tbl *cmdtp, int flag, int argc, char *const argv[])
{
    g_mybpf_spf_ctrl->unload_all_instance();
    return 0;
}

static int do_test_spf(struct cmd_tbl *cmdtp, int flag, int argc, char *const argv[])
{
    MYBPF_PARAM_S p = {0};

    p.p[0] = argc - 1;
    p.p[1] = (long)(argv + 1);

    return g_mybpf_spf_ctrl->run(MYBPF_HP_TCMD, &p);
}

static int do_load_spfloader(struct cmd_tbl *cmdtp, int flag, int argc, char *const argv[])
{
    void *mem;
	MYBPF_PARAM_S p = {0};

    if (argc < 2) {
        return CMD_RET_USAGE;
    }

	mem = (void*)hextoul(argv[1], NULL);

    int ret = MYBPF_LoadBare(mem, NULL, &g_mybpf_bare);
    if (ret < 0) {
        printf("Load loader failed \n");
        return ret;
    }

    p.p[0] = (long)mem;
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
	
    return 0;
}

U_BOOT_CMD(load_loader, 2, 0, do_load_spfloader, "load_loader", "address");
U_BOOT_CMD(load_spf, 3, 0, do_load_spf, "load_spf", "instance address");
U_BOOT_CMD(unload_spf, 2, 0, do_unload_spf, "unload_spf", "instance");
U_BOOT_CMD(unload_allspf, 1, 0, do_unload_all_spf, "unload_allspf", "");
U_BOOT_CMD(test_spf, 6, 0, do_test_spf, "test_spf", "args");

