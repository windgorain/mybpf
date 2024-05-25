/*================================================================
*   Created by LiXingang: 2018.11.09
*   Description: 
*
================================================================*/
#include "bs.h"
#include "utl/subcmd_utl.h"
#include "utl/txt_utl.h"
#include "utl/ulc_user.h"

#define SUBCMD_MATCH_NUM 128

typedef struct {
    SUB_CMD_NODE_S *matched[128];
    int matched_count;
    int shuld_help;
}SUBCMD_MATCHED_S;

static int subcmd_prefix_match(SUB_CMD_NODE_S *node, int argc, char **argv)
{
    char tmp[512];
    char *tok[32];
    int tok_num;
    int cmp_num;
    int i;

    if (argc == 0) {
        return BS_PART_MATCH;
    }

    strlcpy(tmp, node->subcmd, sizeof(tmp));

    tok_num = TXT_StrToToken(tmp, " ", tok, 32);
    if (tok_num <= 0) {
        return BS_NOT_MATCH;
    }

    cmp_num = MIN(tok_num, argc);

    for (i=0; i<cmp_num; i++) {
        if (strncmp(tok[i], argv[i], strlen(argv[i])) != 0) {
            return BS_NOT_MATCH;
        }
    }

    if (argc >= tok_num) {
        return BS_MATCH;
    }

    return BS_PART_MATCH;
}

static int subcmd_search(SUB_CMD_NODE_S *root, int argc, char **argv, OUT SUBCMD_MATCHED_S *matched)
{
    SUB_CMD_NODE_S *tmp = root;
    int count = 0;
    int ret;

    if (! root) {
        return 0;
    }

    matched->shuld_help = 0;

    while(tmp->subcmd) {
        ret = subcmd_prefix_match(tmp, argc, argv);
        if (ret != BS_NOT_MATCH) {
            matched->matched[count] = tmp;
            count ++;
            if (count >= SUBCMD_MATCH_NUM) {
                break;
            }
            if (ret == BS_PART_MATCH) {
                matched->shuld_help = 1;
            }
        }
        tmp ++;
    }

    if (count != 1) {
        matched->shuld_help = 1;
    }

    return count;
}

static int subcmd_search_subcmds(SUB_CMD_NODE_S *root, int argc, char **argv, OUT SUBCMD_MATCHED_S *matched)
{
    int count = -1;

    while (argc >= 0) {
        count = subcmd_search(root, argc, argv, matched);
        if (count > 0) {
            break;
        }
        argc --;
    }

    matched->matched_count = count;

    return 0;
}

static char * subcmd_build_help_info(SUBCMD_MATCHED_S *matched, OUT char *buf, int buf_size)
{
    SUB_CMD_NODE_S *node;
    int len = 0;
    int i;

    buf[0] = '\0';

    len += snprintf(buf, buf_size-len, "Commands:\r\n");

    for (i=0; i<matched->matched_count; i++) {
        node = matched->matched[i];
        len += snprintf(buf+len, buf_size-len, " ");
        len += snprintf(buf+len, buf_size-len, "%-32s    ", node->subcmd);
        len += snprintf(buf+len, buf_size-len, "- %s\r\n", node->help == NULL ? "": node->help);
    }

    return buf;
}

static void subcmd_help(SUBCMD_MATCHED_S *matched)
{
    char buf[4096];
    printf("%s", subcmd_build_help_info(matched, buf, sizeof(buf)));
}


int SUBCMD_DoExt(SUB_CMD_NODE_S *subcmd, int argc, char **argv, int flag)
{
    PF_SUBCMD_FUNC func;
    int tok_num;
    SUBCMD_MATCHED_S matched;

    memset(&matched, 0, sizeof(matched));

    if (argc < 0) {
        subcmd_help(&matched);
        return -1;
    }

    subcmd_search_subcmds(subcmd, argc, argv, &matched);
    if ((matched.shuld_help) && ((flag & SUBCMD_FLAG_HIDE_HELP) == 0)){
        subcmd_help(&matched);
        return -1;
    }

    tok_num = TXT_GetTokenNum(matched.matched[0]->subcmd, " ");
    if (! tok_num) {
        subcmd_help(&matched);
        return -1;
    }

    func = matched.matched[0]->func;

    return func((argc + 1) - tok_num, argv + (tok_num - 1));
}


int SUBCMD_Do(SUB_CMD_NODE_S *subcmd, int argc, char **argv)
{
    return SUBCMD_DoExt(subcmd, argc - 1, argv + 1, 0);
}


int SUBCMD_DoParams(SUB_CMD_NODE_S *subcmd, int argc, char **argv)
{
    return SUBCMD_DoExt(subcmd, argc, argv, 0);
}

