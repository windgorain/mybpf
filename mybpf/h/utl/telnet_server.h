/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _TELNET_SERVER_H
#define _TELNET_SERVER_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
    UINT uiState;
    INT iSocketId;
    HANDLE hCmdRunner;
}TEL_CTRL_S;

void TELS_Init(TEL_CTRL_S *ctrl, int fd, HANDLE hCmdRunner);
void TELS_Hsk(int fd);
int TELS_Run(TEL_CTRL_S *ctrl, UCHAR *data, int data_len);

#ifdef __cplusplus
}
#endif
#endif 
