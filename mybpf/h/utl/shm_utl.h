/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _SHM_UTL_H
#define _SHM_UTL_H
#ifdef __cplusplus
extern "C"
{
#endif

#define SHM_SIZE (1024*1024)

void * SHM_At(int shmid, const void *shmaddr, int shmflag);
int SHM_Dt(const void *shmaddr);
int SHM_Ctl(int shmid, int cmd, void *buf);
int SHM_Get(key_t key, size_t size, int shmflag);

#ifdef __cplusplus
}
#endif
#endif 
