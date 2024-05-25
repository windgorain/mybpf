/*================================================================
*   Created by LiXingang, Copyright LiXingang
*   Description: 
*
================================================================*/
#include "bs.h"
#include "utl/coroutine_utl.h"
#include "utl/rdtsc_utl.h"
#include "utl/arm64_stack.h"
#include <ucontext.h>

#define COROUTINE_IDLE_TASK_ID 0
#define COROUTINE_MAX 16 
#define COROUTINE_DFT_STACK_SIZE (1024*16)

enum {
    COROUTINE_STATE_READY = 0,
    COROUTINE_STATE_SLEEP,
    COROUTINE_STATE_SUSPEND,
    COROUTINE_STATE_FINISH,
};

typedef struct {
    int id;
    UCHAR state; 
    int stack_size;
    char *stack;
    char task_name[COROUTINE_NAME_SIZE];
    UINT64 sleep_end_tsc;
    PF_COROUTINE_MAIN_FUNC main_func;
    ucontext_t ctx;
}COROUTINE_NODE_S;

typedef struct tagCOROUTINE_S{
    int max_tasks;  
    int current_task_id; 
    int finished_task_id; 
    UINT stop:1; 
    COROUTINE_NODE_S idle_task;
    COROUTINE_NODE_S **tasks;
}COROUTINE_S;

static inline COROUTINE_NODE_S * _coroutine_self(COROUTINE_S *ctrl)
{
    return ctrl->tasks[ctrl->current_task_id];
}

static inline COROUTINE_NODE_S * _coroutine_get_by_id(COROUTINE_S *ctrl, int task_id)
{
    if (task_id >= ctrl->max_tasks) {
        return NULL;
    }
    return ctrl->tasks[task_id];
}

static void _coroutine_free_node(COROUTINE_NODE_S *node)
{
    if (node->stack) {
        MEM_Free(node->stack);
    }
    MEM_Free(node);
}

static void _coroutine_destroy_task(COROUTINE_S *ctrl, int task_id)
{
    _coroutine_free_node(ctrl->tasks[task_id]);
    ctrl->tasks[task_id] = NULL;
}


static int _coroutine_get_a_blank(COROUTINE_S *ctrl)
{
    int i;

    
    for (i=1; i<ctrl->max_tasks; i++) {
        if (! ctrl->tasks[i]) {
            return i;
        }
    }

    return -1;
}

static inline int _coroutine_is_waiting(COROUTINE_NODE_S *node)
{
    if (! node) {
        return 0;
    }

    if (node->state == COROUTINE_STATE_SLEEP) {
        return 1;
    }

    if ((node->state == COROUTINE_STATE_SUSPEND) && (node->sleep_end_tsc != 0)) {
        return 1;
    }

    return 0;
}


static void _coroutine_process_timeout_nodes(COROUTINE_S *ctrl)
{
    int i;
    COROUTINE_NODE_S *node;
    UINT64 curr_tsc = RDTSC_Get();
    INT64 diff;

    for (i=1; i<ctrl->max_tasks; i++) {
        node = ctrl->tasks[i];
        if (_coroutine_is_waiting(node)) {
            diff = curr_tsc - node->sleep_end_tsc;
            if (diff >= 0) {
                node->state = COROUTINE_STATE_READY;
            }
        }
    }
}


static COROUTINE_NODE_S * _coroutine_get_a_ready(COROUTINE_S *ctrl)
{
    int i;
    COROUTINE_NODE_S *node;
    int next_id = ctrl->current_task_id + 1;

    if (next_id >= ctrl->max_tasks) {
        next_id = 1;
    }

    for (i=next_id; i<ctrl->max_tasks; i++) {
        node = ctrl->tasks[i];
        if ((node) && (node->state == COROUTINE_STATE_READY)) {
            return node;
        }
    }

    for (i=1; i<next_id; i++) {
        node = ctrl->tasks[i];
        if ((node) && (node->state == COROUTINE_STATE_READY)) {
            return node;
        }
    }

    return ctrl->tasks[COROUTINE_IDLE_TASK_ID]; 
}

static void _coroutine_switch_to(COROUTINE_S *ctrl, COROUTINE_NODE_S *node)
{
    COROUTINE_NODE_S *curr = ctrl->tasks[ctrl->current_task_id];
    ctrl->current_task_id = node->id;
    swapcontext(&curr->ctx, &node->ctx);
}

static void _coroutine_main(COROUTINE_S *ctrl, COROUTINE_NODE_S *node)
{
    node->main_func();
    node->state = COROUTINE_STATE_FINISH;
    ctrl->finished_task_id = node->id;
}

static void _coroutine_schedule(COROUTINE_S *ctrl)
{
    _coroutine_process_timeout_nodes(ctrl);

    COROUTINE_NODE_S *node = _coroutine_get_a_ready(ctrl);
    if (node->id == ctrl->current_task_id) {
        return;
    }

    _coroutine_switch_to(ctrl, node);
}

static void _coroutine_usleep(COROUTINE_S *ctrl, UINT64 usec)
{
    COROUTINE_NODE_S *node = _coroutine_self(ctrl);
    node->sleep_end_tsc = RDTSC_Get() + usec * RDTSC_US_HZ;
    node->state = COROUTINE_STATE_SLEEP;
    _coroutine_schedule(ctrl);
}

static int _coroutine_suspend(COROUTINE_S *ctrl, COROUTINE_NODE_S *node, UINT64 timeout_us)
{
    node->state = COROUTINE_STATE_SUSPEND;
    if (timeout_us) {
        node->sleep_end_tsc = RDTSC_Get() + timeout_us * RDTSC_US_HZ;
    } else {
        node->sleep_end_tsc = 0;
    }

    
    if (ctrl->current_task_id == node->id) {
        _coroutine_schedule(ctrl);
    }

    return 0;
}

static int _coroutine_resume(COROUTINE_S *ctrl, COROUTINE_NODE_S *node)
{
    if (node->state == COROUTINE_STATE_SUSPEND) {
        node->sleep_end_tsc = 0;
        node->state = COROUTINE_STATE_READY;
    }
    return 0;
}

static int _coroutine_find_by_name(COROUTINE_S *ctrl, char *name)
{
    int i;
    COROUTINE_NODE_S *node;

    for (i=0; i<ctrl->max_tasks; i++) {
        node = ctrl->tasks[i];
        if (node && (0==strcmp(name, node->task_name))) {
            return i;
        }
    }

    return -1;
}

COROUTINE_S * Coroutine_CtrlCreate(int max_tasks)
{
    COROUTINE_S *ctrl;

    ctrl = MEM_ZMalloc(sizeof(COROUTINE_S));
    if (! ctrl) {
        return NULL;
    }

    ctrl->tasks = MEM_ZMalloc(sizeof(COROUTINE_S *) * max_tasks);
    if (! ctrl->tasks) {
        MEM_Free(ctrl);
        return NULL;
    }

    ctrl->max_tasks = max_tasks;
    ctrl->current_task_id = -1;
    ctrl->finished_task_id = -1;
    ctrl->tasks[0] = &ctrl->idle_task;
    strlcpy(ctrl->idle_task.task_name, "idle", sizeof(ctrl->idle_task.task_name));

    return ctrl;
}

void Coroutine_CtrlDestroy(COROUTINE_S *ctrl)
{
    int i;

    if (ctrl->tasks) {
        for (i=0; i<ctrl->max_tasks; i++) {
            if (ctrl->tasks[i]) {
                _coroutine_free_node(ctrl->tasks[i]);
                ctrl->tasks[i] = NULL;
            }
        }
        MEM_Free(ctrl->tasks);
    }

    MEM_Free(ctrl);
}

int Coroutine_Create(COROUTINE_S *ctrl, char *task_name, PF_COROUTINE_MAIN_FUNC main_func)
{
    COROUTINE_NODE_S *node;

    if ((! ctrl) || (! task_name) || (! main_func)) {
        RETURN(BS_NULL_PARA);
    }

    if (strlen(task_name) >= COROUTINE_NAME_SIZE) {
        RETURN(BS_BAD_PARA);
    }

    if (_coroutine_find_by_name(ctrl, task_name) >= 0) {
        RETURN(BS_ALREADY_EXIST);
    }

    int index = _coroutine_get_a_blank(ctrl);
    if (index < 0) {
        RETURN(BS_NO_RESOURCE);
    }

    node = MEM_ZMalloc(sizeof(COROUTINE_NODE_S));
    if (! node) {
        RETURN(BS_NO_MEMORY);
    }

    node->stack = MEM_ZMalloc(COROUTINE_DFT_STACK_SIZE);
    if (! node->stack) {
        _coroutine_free_node(node);
        RETURN(BS_NO_MEMORY);
    }

    node->stack_size = COROUTINE_DFT_STACK_SIZE;
    node->id = index;
    node->main_func = main_func;
    strlcpy(node->task_name, task_name, sizeof(node->task_name));

    if (getcontext(&node->ctx) < 0) {
        _coroutine_free_node(node);
        RETURN(BS_CAN_NOT_OPEN);
    }

    ctrl->tasks[index] = node;

    node->ctx.uc_stack.ss_sp = node->stack;
    node->ctx.uc_stack.ss_size = node->stack_size;
    node->ctx.uc_link = &ctrl->idle_task.ctx;

    makecontext(&node->ctx, (void*)_coroutine_main, 2, ctrl, node);

    return index;
}

void Coroutine_Run(COROUTINE_S *ctrl)
{
    ctrl->current_task_id = COROUTINE_IDLE_TASK_ID; 

    while(ctrl->stop == 0) {
        _coroutine_schedule(ctrl);
        ctrl->current_task_id = COROUTINE_IDLE_TASK_ID;
        if (ctrl->finished_task_id > 0) {
            _coroutine_destroy_task(ctrl, ctrl->finished_task_id);
            ctrl->finished_task_id = -1;
        }
    }
}

void Coroutine_Stop(COROUTINE_S *ctrl)
{
    ctrl->stop = 1;
}

void Coroutine_Yield(COROUTINE_S *ctrl)
{
    _coroutine_schedule(ctrl);
}

void Coroutine_USleep(COROUTINE_S *ctrl, UINT64 usec)
{
    _coroutine_usleep(ctrl, usec);
}

void Coroutine_MSleep(COROUTINE_S *ctrl, UINT64 msec)
{
    _coroutine_usleep(ctrl, msec * 1000);
}

void Coroutine_Sleep(COROUTINE_S *ctrl, UINT64 sec)
{
    _coroutine_usleep(ctrl, sec * 1000 * 1000);
}

int Coroutine_Suspend(COROUTINE_S *ctrl, int task_id, UINT64 timeout_us)
{
    COROUTINE_NODE_S *node = _coroutine_get_by_id(ctrl, task_id);
    if (! node) {
        RETURN(BS_NOT_FOUND);
    }

    return _coroutine_suspend(ctrl, node, timeout_us);
}

int Coroutine_SuspendSelf(COROUTINE_S *ctrl, UINT64 timeout_us)
{
    return _coroutine_suspend(ctrl, _coroutine_self(ctrl), timeout_us);
}

int Coroutine_Resume(COROUTINE_S *ctrl, int task_id)
{
    COROUTINE_NODE_S *node = _coroutine_get_by_id(ctrl, task_id);
    if (! node) {
        RETURN(BS_NOT_FOUND);
    }

    return _coroutine_resume(ctrl, node);
}

int Coroutine_GetByName(COROUTINE_S *ctrl, char *task_name)
{
    return _coroutine_find_by_name(ctrl, task_name);
}

char * Coroutine_GetName(COROUTINE_S *ctrl, int task_id)
{
    COROUTINE_NODE_S *node = _coroutine_get_by_id(ctrl, task_id);
    if (! node) {
        return NULL;
    }

    return node->task_name;
}

char * Coroutine_SelfName(COROUTINE_S *ctrl)
{
    COROUTINE_NODE_S *node = _coroutine_self(ctrl);
    if (! node) {
        return NULL;
    }

    return node->task_name;
}

int Coroutine_GetAttr(COROUTINE_S *ctrl, int task_id, OUT COROUTINE_ATTR_S *attr)
{
    COROUTINE_NODE_S *node = _coroutine_get_by_id(ctrl, task_id);
    if (! node) {
        RETURN(BS_NO_SUCH);
    }

    attr->id = node->id;
    attr->main_func = node->main_func;
    attr->sleep_end_tsc = node->sleep_end_tsc;
    attr->stack_size = node->stack_size;
    attr->state = node->state;
    strlcpy(attr->task_name, node->task_name, sizeof(attr->task_name));

    return 0;
}


int Coroutine_GetNext(COROUTINE_S *ctrl, int curr_task_id )
{
    int i;

    for (i=curr_task_id + 1; i<ctrl->max_tasks; i++) {
        if (ctrl->tasks[i]) {
            return i;
        }
    }

    return -1;
}

void Coroutine_StackTrace(COROUTINE_S *ctrl, int task_id, PF_COROUTINE_PRINT_FUNC print_func)
{
#ifdef __ARM64__
    COROUTINE_NODE_S *node = _coroutine_get_by_id(ctrl, task_id);
    if (! node) {
        return;
    }
    CONTEXT_REGS_S *ctx_regs = (void*)node->ctx.uc_mcontext.sp;
    print_func("Stack Trace:\n");
    while(ctx_regs) {
        print_func("0x%llx\n",ctx_regs->x30);
        ctx_regs = (void*)ctx_regs->x29;
    }
#endif
}


