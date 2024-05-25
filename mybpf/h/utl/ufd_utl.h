/*================================================================
*   Created by LiXingang, Copyright LiXingang
*   Description: 
*
================================================================*/
#ifndef _UFD_UTL_H
#define _UFD_UTL_H
#ifdef __cplusplus
extern "C"
{
#endif

enum {
    UFD_FD_TYPE_MAP = 0,
    UFD_FD_TYPE_PROG,

    UFD_FD_TYPE_MAX
};

typedef void (*PF_UFD_FREE)(void *ufd_ctx, void *file);

typedef struct {
    int ref_cnt;
    UCHAR type;
    void *data;
    PF_UFD_FREE free_func;
}UFD_FILE_S;

typedef struct {
    UINT capacity;
    UFD_FILE_S fds[0];
}UFD_S;

int UFD_Init(INOUT UFD_S *ctx, UINT capacity);
UFD_S * UFD_Create(UINT capacity);
void UFD_Destroy(UFD_S *ctx);
int UFD_Open(UFD_S *ctx, int type, void *data, PF_UFD_FREE free_func);
int UFD_Ref(UFD_S *ctx, int fd, int ref_count);
int UFD_IncRef(UFD_S *ctx, int fd);
int UFD_DecRef(UFD_S *ctx, int fd);
int UFD_Close(UFD_S *ctx, int fd);
int UFD_GetFileType(UFD_S *ctx, int fd);
int UFD_GetFileSize(UFD_S *ctx, int fd);
UFD_FILE_S * UFD_GetFile(UFD_S *ctx, int fd);
void * UFD_GetFileData(UFD_S *ctx, int fd);
UFD_FILE_S * UFD_RefFile(UFD_S *ctx, int fd);
void * UFD_RefFileData(UFD_S *ctx, int fd);

int UFD_GetNext(UFD_S *ctx, int curr );

int UFD_GetNextOfType(UFD_S *ctx, int type, int curr );

#ifdef __cplusplus
}
#endif
#endif 
