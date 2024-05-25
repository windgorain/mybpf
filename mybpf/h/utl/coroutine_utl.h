/*================================================================
*   Created by LiXingang, Copyright LiXingang
*   Description: 
*
================================================================*/
#ifndef _COROUTINE_UTL_H
#define _COROUTINE_UTL_H
#ifdef __cplusplus
extern "C"
{
#endif

#define COROUTINE_NAME_SIZE 16

typedef struct tagCOROUTINE_S COROUTINE_S;
typedef void (*PF_COROUTINE_MAIN_FUNC)();
typedef void (*PF_COROUTINE_PRINT_FUNC)(char *fmt, ...);

typedef struct {
    int id;
    UCHAR state; 
    int stack_size;
    char task_name[COROUTINE_NAME_SIZE];
    UINT64 sleep_end_tsc;
    void *main_func;
}COROUTINE_ATTR_S;

COROUTINE_S * Coroutine_CtrlCreate(int max_tasks);
void Coroutine_CtrlDestroy(COROUTINE_S *ctrl);
int Coroutine_Create(COROUTINE_S *ctrl, char *task_name, PF_COROUTINE_MAIN_FUNC main_func);
void Coroutine_Run(COROUTINE_S *ctrl);
void Coroutine_Stop(COROUTINE_S *ctrl);
void Coroutine_Yield(COROUTINE_S *ctrl);
void Coroutine_USleep(COROUTINE_S *ctrl, UINT64 usec);
void Coroutine_MSleep(COROUTINE_S *ctrl, UINT64 msec);
void Coroutine_Sleep(COROUTINE_S *ctrl, UINT64 sec);
int Coroutine_Suspend(COROUTINE_S *ctrl, int task_id, UINT64 timeout_us);
int Coroutine_SuspendSelf(COROUTINE_S *ctrl, UINT64 timeout_us);
int Coroutine_Resume(COROUTINE_S *ctrl, int task_id);
int Coroutine_GetByName(COROUTINE_S *ctrl, char *task_name);
char * Coroutine_GetName(COROUTINE_S *ctrl, int task_id);
char * Coroutine_SelfName(COROUTINE_S *ctrl);
int Coroutine_GetAttr(COROUTINE_S *ctrl, int task_id, OUT COROUTINE_ATTR_S *attr);

int Coroutine_GetNext(COROUTINE_S *ctrl, int curr_task_id );
void Coroutine_StackTrace(COROUTINE_S *ctrl, int task_id, PF_COROUTINE_PRINT_FUNC print_func);

#ifdef __cplusplus
}
#endif
#endif 
