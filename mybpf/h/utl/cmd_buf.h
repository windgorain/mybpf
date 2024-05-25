/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _CMD_BUF_H
#define _CMD_BUF_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef void* CMD_BUF_HDL;

CMD_BUF_HDL CmdBuf_Create();
void CmdBuf_Destroy(CMD_BUF_HDL hdl);
int CmdBuf_RunCmd(CMD_BUF_HDL hdl, char *cmd);
int CmdBuf_Ret(CMD_BUF_HDL hdl);
char * CmdBuf_Buf(CMD_BUF_HDL hdl);

#ifdef __cplusplus
}
#endif
#endif 
