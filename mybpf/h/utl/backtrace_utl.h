/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _BACKTRACE_UTL_H
#define _BACKTRACE_UTL_H
#ifdef __cplusplus
extern "C"
{
#endif

void BackTrace_Print(void);
void BackTrace_WriteToFile(char *file);
void BackTrace_WriteToFp(FILE *fp);
void BackTrace_WriteToFd(int fd);
void BackTrace_WriteToBuf(OUT char *buf, int buf_size);


void CoreDump_Enable(void);

#ifdef __cplusplus
}
#endif
#endif 
