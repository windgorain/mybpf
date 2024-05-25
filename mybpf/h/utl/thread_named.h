/*================================================================
*   Created by LiXingang: 2018.11.16
*   Description: 
*
================================================================*/
#ifndef _THREAD_NAMED_H
#define _THREAD_NAMED_H
#ifdef __cplusplus
extern "C"
{
#endif

#define THREAD_NAMED_MAX_NAME_LEN 31

typedef void (*PF_THREAD_NAMED_FUNC)(USER_HANDLE_S *user_data);

typedef struct {
    char name[THREAD_NAMED_MAX_NAME_LEN + 1];
    USER_HANDLE_S user_data;
    PF_THREAD_NAMED_FUNC func;
    THREAD_ID thread_id;
}THREAD_NAMED_INFO_S;

typedef struct {
    UINT uiPriority;
    UINT uiStackSize;
    UINT uiNearTId;     
}THREAD_CREATE_PARAM_S;

typedef struct {
    THREAD_NAMED_INFO_S info;
}THREAD_NAMED_ITER_S;

typedef void (*PF_THREAD_NAMED_OB_FUNC)(UINT event, THREAD_NAMED_INFO_S *info);

enum {
    THREAD_NAMED_EVENT_START = 0,
    THREAD_NAMED_EVENT_QUIT
};

typedef struct {
    DLL_NODE_S link_node;
    PF_THREAD_NAMED_OB_FUNC ob_func;
}THREAD_NAMED_OB_S;

THREAD_ID ThreadNamed_Create(char *name, THREAD_CREATE_PARAM_S *param, PF_THREAD_NAMED_FUNC func, USER_HANDLE_S *user_data);
void ThreadNamed_RegOb(THREAD_NAMED_OB_S *ob);
void ThreadNamed_UnregOb(THREAD_NAMED_OB_S *ob);
void ThreadNamed_InitIter(THREAD_NAMED_ITER_S *iter);
THREAD_NAMED_INFO_S * ThreadNamed_GetNext(THREAD_NAMED_ITER_S *iter);
BS_STATUS ThreadNamed_GetByName(char *name, THREAD_NAMED_INFO_S *info);
BS_STATUS ThreadNamed_GetByID(THREAD_ID thread_id, THREAD_NAMED_INFO_S *info);

#ifdef __cplusplus
}
#endif
#endif 
