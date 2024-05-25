/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _CMD_LST_H
#define _CMD_LST_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    UCHAR type;
    UCHAR level;
    UINT property;
    char *view;
    char *cmd;
    char *view_name;
    char *func_name;
    char *param;
}CMDLST_ELE_S;

typedef int (*PF_CMDLST_LINE)(void *cmdlist, CMDLST_ELE_S *ele);

typedef struct {
    PF_CMDLST_LINE line_func;
}CMDLST_S;


BS_STATUS CMDLST_Scan(CMDLST_S *ctrl, char *buf);
BS_STATUS CMDLST_ScanByFile(CMDLST_S *ctrl, char *filename);


#ifdef __cplusplus
}
#endif
#endif 
