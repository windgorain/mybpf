/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _SUBCMD_UTL_H
#define _SUBCMD_UTL_H
#ifdef __cplusplus
extern "C"
{
#endif

#define SUBCMD_FLAG_HIDE_HELP 0x1 

typedef int (*PF_SUBCMD_FUNC)(int argc, char **argv);

typedef struct {
    char *subcmd;
    void *func; 
    char *help;
}SUB_CMD_NODE_S;

int SUBCMD_Do(SUB_CMD_NODE_S *subcmd, int argc, char **argv);
int SUBCMD_DoParams(SUB_CMD_NODE_S *subcmd, int argc, char **argv);
int SUBCMD_DoExt(SUB_CMD_NODE_S *subcmd, int argc, char **argv, int flag);

#ifdef __cplusplus
}
#endif
#endif 
