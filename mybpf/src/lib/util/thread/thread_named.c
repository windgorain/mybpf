/*================================================================
*   Created by LiXingang: 2007.2.8
*   Description: 命名线程
*
================================================================*/
#include "bs.h"

#include "utl/txt_utl.h"
#include "utl/mutex_utl.h"
#include "utl/thread_utl.h"
#include "utl/thread_named.h"

#ifdef IN_LINUX
#include <sys/prctl.h>
#endif

typedef struct {
    DLL_NODE_S link_node;

    char name[THREAD_NAMED_MAX_NAME_LEN + 1];
    USER_HANDLE_S user_data;
    PF_THREAD_NAMED_FUNC func;
    THREAD_ID thread_id;
}THREAD_NAMED_NODE_S;

static DLL_HEAD_S g_named_thread_list = DLL_HEAD_INIT_VALUE(&g_named_thread_list);
static DLL_HEAD_S g_name_thread_notify_list = DLL_HEAD_INIT_VALUE(&g_name_thread_notify_list);
static MUTEX_S g_named_thread_lock;

static inline void _threadnamed_init(void)
{
    static int inited = 0;
    if (! inited) {
        inited = 1;
        MUTEX_Init(&g_named_thread_lock);
    }
}

CONSTRUCTOR(init) {
    _threadnamed_init();
}

static void _threadnamed_FillInfo(THREAD_NAMED_NODE_S *node, THREAD_NAMED_INFO_S *info)
{
    info->func = node->func;
    info->thread_id = node->thread_id;
    info->user_data = node->user_data;
    strcpy(info->name, node->name);
}

static void _threadnamed_event_notify(UINT event, IN THREAD_NAMED_NODE_S *node)
{
    THREAD_NAMED_INFO_S info;
    THREAD_NAMED_OB_S *ob;

    _threadnamed_FillInfo(node, &info);

    DLL_SCAN(&g_name_thread_notify_list, ob) {
        ob->ob_func(event, &info);
    }
}

static int _threadnamed_Cmp(DLL_NODE_S *pstNode1, DLL_NODE_S *pstNode2, HANDLE hUserHandle)
{
    THREAD_NAMED_NODE_S *node1 = (void*)pstNode1;
    THREAD_NAMED_NODE_S *node2 = (void*)pstNode2;

    return strcmp(node1->name, node2->name);
}

static int _threadnamed_AddNode(THREAD_NAMED_NODE_S *node)
{
    int ret;

    MUTEX_P(&g_named_thread_lock);
    ret = DLL_UniqueSortAdd(&g_named_thread_list, (void*)node, _threadnamed_Cmp, NULL);
    MUTEX_V(&g_named_thread_lock);

    return ret;
}

static void _threadnamed_FreeNode(THREAD_NAMED_NODE_S *node)
{
    MUTEX_P(&g_named_thread_lock);
    DLL_DEL(&g_named_thread_list, node);
    MUTEX_V(&g_named_thread_lock);
    MEM_Free(node);
}

static THREAD_NAMED_NODE_S * _threadnamed_Find(char *name)
{
    THREAD_NAMED_NODE_S *node;
    THREAD_NAMED_NODE_S *found = NULL;
    int ret;
    
    DLL_SCAN(&g_named_thread_list, node) {
        ret = strcmp(name, node->name);
        if (ret < 0) {
            break;
        } else if (ret == 0) {
            found = node;
            break;
        }
    }

    return found;
}

static void _threadnamed_main(void *user_data)
{
    THREAD_NAMED_NODE_S *node = user_data;

#ifdef IN_LINUX
    prctl(PR_SET_NAME, node->name);
#endif

    _threadnamed_event_notify(THREAD_NAMED_EVENT_START, node);
    node->func(&node->user_data);
    _threadnamed_event_notify(THREAD_NAMED_EVENT_QUIT, node);

    _threadnamed_FreeNode(node);
}

static void * _threadnamed_GetNext(THREAD_NAMED_ITER_S *iter)
{
    THREAD_NAMED_NODE_S *node;

    DLL_SCAN(&g_named_thread_list, node) {
        if (strcmp(iter->info.name, node->name) < 0) {
            break;
        }
    }

    if (node == NULL) {
        return NULL;
    }

    _threadnamed_FillInfo(node, &iter->info);

    return &iter->info;
}

static BS_STATUS _threadnamed_GetByName(char *name, THREAD_NAMED_INFO_S *info)
{
    THREAD_NAMED_NODE_S *node;

    node = _threadnamed_Find(name);
    if (! node) {
        return BS_NOT_FOUND;
    }

    _threadnamed_FillInfo(node, info);

    return BS_OK;
}

static BS_STATUS _threadnamed_GetByID(THREAD_ID thread_id, THREAD_NAMED_INFO_S *info)
{
    THREAD_NAMED_NODE_S *node, *found=NULL;

    DLL_SCAN(&g_named_thread_list, node) {
        if (thread_id == node->thread_id) {
            found = node;
            break;
        }
    }

    if (! found) {
        return BS_NOT_FOUND;
    }

    _threadnamed_FillInfo(found, info);

    return BS_OK;
}

void ThreadNamed_RegOb(THREAD_NAMED_OB_S *ob)
{
    _threadnamed_init();

    MUTEX_P(&g_named_thread_lock);
    DLL_ADD(&g_name_thread_notify_list, &ob->link_node);
    MUTEX_V(&g_named_thread_lock);
}

void ThreadNamed_UnregOb(THREAD_NAMED_OB_S *ob)
{
    MUTEX_P(&g_named_thread_lock);
    DLL_DEL(&g_name_thread_notify_list, &ob->link_node);
    MUTEX_V(&g_named_thread_lock);
}


THREAD_ID ThreadNamed_Create(char *name, THREAD_CREATE_PARAM_S *param, PF_THREAD_NAMED_FUNC func, USER_HANDLE_S *user_data)
{
    THREAD_ID thread_id;
    THREAD_NAMED_NODE_S *node;
    UINT pri = 0, stack_size = 0;

    _threadnamed_init();

    node = MEM_ZMalloc(sizeof(THREAD_NAMED_NODE_S));
    if (node == NULL) {
        ERROR_SET(BS_NO_MEMORY);
        return THREAD_ID_INVALID;
    }

    node->func = func;
    if (NULL != user_data) {
        node->user_data = *user_data;
    }
    strlcpy(node->name, name, sizeof(node->name));

    if (0 != _threadnamed_AddNode(node)) {
        MEM_Free(node);
        ERROR_SET(BS_ALREADY_EXIST);
        return THREAD_ID_INVALID;
    }

    if (NULL != param) {
        pri = param->uiPriority;
        stack_size = param->uiStackSize;
    }

    thread_id = ThreadUtl_Create(_threadnamed_main, pri, stack_size, node);
    if (THREAD_ID_INVALID == thread_id) {
        _threadnamed_FreeNode(node);
        ERROR_SET(BS_ERR);
        return thread_id;
    }

    node->thread_id = thread_id;

    return thread_id;
}

BS_STATUS ThreadNamed_GetByName(char *name, THREAD_NAMED_INFO_S *info)
{
    BS_STATUS ret;

    MUTEX_P(&g_named_thread_lock);
    ret = _threadnamed_GetByName(name, info);
    MUTEX_V(&g_named_thread_lock);

    return ret;
}

BS_STATUS ThreadNamed_GetByID(THREAD_ID thread_id, THREAD_NAMED_INFO_S *info)
{
    BS_STATUS ret;

    MUTEX_P(&g_named_thread_lock);
    ret = _threadnamed_GetByID(thread_id, info);
    MUTEX_V(&g_named_thread_lock);

    return ret;
}

void ThreadNamed_InitIter(THREAD_NAMED_ITER_S *iter)
{
    memset(iter, 0, sizeof(THREAD_NAMED_ITER_S));
}

THREAD_NAMED_INFO_S * ThreadNamed_GetNext(THREAD_NAMED_ITER_S *iter)
{
    void *ret;

    MUTEX_P(&g_named_thread_lock);
    ret = _threadnamed_GetNext(iter);
    MUTEX_V(&g_named_thread_lock);

    return ret;
}

