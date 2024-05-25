/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2009-6-16
* Description: 
* History:     
******************************************************************************/
#include "bs.h"
#include "utl/num_utl.h"
#include "utl/bit_opt.h"
#include "utl/txt_utl.h"
#include "utl/mem_utl.h"
#include "utl/socket_utl.h"
#include <sys/un.h>

static VOID socket_WindowInit(void)
{
#ifdef IN_WINDOWS
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    static BOOL_T bIsInit = 0;

    if (bIsInit == 0)
    {
        bIsInit = 1;

        wVersionRequested = MAKEWORD (2, 2);
        err = WSAStartup (wVersionRequested, &wsaData);
        if (err != 0)
        {
            return;
        }
    }
#endif
}

static INT socket_GetLastErrno(void)
{
    INT iErr = SOCKET_E_ERR;

#ifdef IN_WINDOWS
    iErr = WSAGetLastError();
#else
    iErr = errno;
    if ((EINPROGRESS == iErr) || (EINTR == iErr)) {
        iErr = EAGAIN;
    }
#endif

    if (iErr > 0) {
        iErr = -iErr;
    }

    return iErr;
}


INT Socket_Read(IN INT iSocketId, OUT void *buf, IN UINT uiBufLen, IN UINT uiFlag)
{
    INT iReadLen;
    INT iRet;

    iReadLen = recv(iSocketId, buf, (INT)uiBufLen, (INT)uiFlag);

    if (iReadLen == 0) {
        iRet = 0;
    } else if (iReadLen < 0) {
        iRet = socket_GetLastErrno();
    } else {
        iRet = iReadLen;
    }

	return iRet;
}


BS_STATUS Socket_Read2(int iSocketId, OUT void *buf, UINT uiLen, OUT UINT *puiReadLen, UINT ulFlag)
{
    INT iLen;
    BS_STATUS eRet = BS_ERR;

	*puiReadLen = 0;

    iLen = Socket_Read(iSocketId, buf, uiLen, ulFlag);

	if (iLen > 0) {
        eRet = BS_OK;
		*puiReadLen = iLen;
	} else {
        if (iLen == 0) {
            eRet = BS_PEER_CLOSED;
        } else if (iLen == SOCKET_E_AGAIN) {
            eRet = BS_OK;
        }
    }

    return eRet;
}


int _Socket_Accept(int fd, OUT struct sockaddr *pstAddr, INOUT INT *piLen, char *filename, int line)
{
    
    struct sockaddr server_addr;
    INT s;
    INT iRet;
    int length = sizeof(server_addr);
    struct sockaddr *pstAddrTmp = pstAddr;
    INT *piLenTmp = piLen;

    if (NULL == pstAddrTmp)
    {
        pstAddrTmp = &server_addr;
        piLenTmp = &length;
    }

    s = (INT)accept(fd, pstAddrTmp, (UINT*)piLenTmp);

    if (s >= 0) {
        iRet = s;
        SSHOW_Add(s, filename, line);
    } else {
        iRet = socket_GetLastErrno();
    }

    return iRet;
}


INT Socket_Write(IN INT iSocketId, IN VOID *data, IN UINT ulLen, IN UINT ulFlag)
{
    INT iSendLen;
    INT iRet;

    iSendLen = send(iSocketId, data, (INT)ulLen, (INT)ulFlag);
    if (iSendLen > 0) {
        iRet = iSendLen;
    } else {
        iRet = socket_GetLastErrno();
    }

    return iRet;
}


int Socket_Write2(int fd, void *data, U32 len, U32 flag)
{
    int ret;
    
    ret = Socket_Write(fd, data, len, flag);
    if (ret > 0) {
        return ret;
    }

    if (ret == SOCKET_E_AGAIN) {
        return 0;
    }

    return ret;
}

int Socket_WriteString(int fd, char *buf, unsigned int flag)
{
    return Socket_Write(fd, buf, strlen(buf), flag);
}

BS_STATUS Socket_WriteUntilFinish(IN INT iSocketId, IN UCHAR *pucBuf, IN UINT ulLen, IN UINT ulFlag)
{
    INT iLen;
    UINT uiSendLen = 0;

    while (uiSendLen < ulLen)
    {
        iLen = Socket_Write(iSocketId, pucBuf + uiSendLen, ulLen - uiSendLen, ulFlag);
        if (iLen <= 0) {
            RETURN(BS_ERR);
        }
        uiSendLen += (UINT)iLen;
    }

    return BS_OK;
}


int Socket_Connect(IN INT iSocketID, IN UINT ulIp, IN USHORT usPort)
{
    struct sockaddr_in server_addr;     

    if ((ulIp == 0) || (usPort == 0)) {
        return 0;
    }

    Mem_Zero (&server_addr, sizeof(struct sockaddr_in));
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(ulIp);
    server_addr.sin_port = htons(usPort);

    if (connect(iSocketID, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0 ) {
        return socket_GetLastErrno();
    }

    return BS_OK;
}


int Socket_Connect2(int fd, UINT ulIp, USHORT usPort)
{
    int ret;
    
    ret = Socket_Connect(fd, ulIp, usPort);
    if (ret == SOCKET_E_AGAIN) {
        return 0;
    }

    return ret;
}

int Socket_ConnectUnixSocket(int fd, char *path)
{
    struct sockaddr_un server_addr;

    server_addr.sun_family = AF_LOCAL;
    strlcpy(server_addr.sun_path, path, sizeof(server_addr.sun_path));

    if (connect(fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_un)) < 0) {
        return socket_GetLastErrno();
    }

    return 0;
}

int Socket_UDPClient(UINT ip, USHORT port)
{
    int fd;

    if ((fd = Socket_Create(AF_INET, SOCK_DGRAM)) < 0 ) {
        return -1;
    }

    if (Socket_Connect(fd, ip, port) < 0) {
        Socket_Close(fd);
    }

    return fd;
}

int Socket_UnixSocketClient(char *path, int type, int flags)
{
    int fd;

    if ((fd = Socket_Create(AF_LOCAL, type)) < 0 ) {
        return -1;
    }

    if (flags & O_NONBLOCK) {
        Socket_SetNoBlock(fd, 1);
    }

    int ret = Socket_ConnectUnixSocket(fd, path);
    if (ret < 0) {
        if (ret == EAGAIN) {
            return fd;
        }
        Socket_Close(fd);
        return -1;
    }

    return fd;
}

BOOL_T Socket_N_IsIPv4(IN CHAR *pcName, IN UINT uiLen)
{
    CHAR szTmp[INET_ADDRSTRLEN + 1];

    if (uiLen >= sizeof(szTmp))
    {
        return FALSE;
    }

    memcpy(szTmp, pcName, uiLen);
    szTmp[uiLen] = '\0';

    return Socket_IsIPv4(szTmp);
}

UINT Socket_Ipsz2IpNetWitchCheck(IN CHAR *pcIP)
{
    if (FALSE == Socket_IsIPv4(pcIP))
    {
        return 0;
    }

    return inet_addr(pcIP);
}


UINT Socket_NameToIpNet(IN CHAR *szIpOrHostName)
{
    socket_WindowInit();

    if (NULL == szIpOrHostName)
    {
        return 0;
    }

    if(stricmp(szIpOrHostName, "LOCALHOST") == 0)
    {
        return inet_addr("127.0.0.1");
    }
    else if (! Socket_IsIPv4(szIpOrHostName))
    {
        struct hostent* remoteHost;
        remoteHost = gethostbyname(szIpOrHostName);
        if (remoteHost != NULL)
        {
            return *((UINT*)remoteHost->h_addr_list[0]);
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return inet_addr(szIpOrHostName);
    }
}


UINT Socket_NameToIpHost (IN CHAR *szIpOrHostName)
{
    UINT uiIP;

    uiIP = Socket_NameToIpNet(szIpOrHostName);

    return ntohl(uiIP);
}


CHAR * Socket_IpToName (IN UINT ulIp)
{
    struct in_addr stAddr;

#ifdef IN_WINDOWS
    stAddr.S_un.S_addr = htonl(ulIp);
#endif
#ifdef IN_UNIXLIKE
    stAddr.s_addr = htonl(ulIp);
#endif
    return inet_ntoa (stAddr);
}

CHAR * Socket_Ip2Name(IN UINT ip, OUT char *buf, IN int buf_size)
{
#ifdef IN_UNIXLIKE
    inet_ntop(AF_INET, &ip, buf, buf_size);
    return buf;
#endif
#ifdef IN_WINDOWS
    strlcpy(buf, Socket_IpToName(ntohl(ip), buf_size));
    return buf;
#endif
}

BS_STATUS Socket_SetSockOpt(IN INT iSocketId, IN INT iLevel, IN INT iOpt, IN VOID *pOpt, IN UINT uiOptLen)
{
    if (setsockopt(iSocketId, iLevel, iOpt, pOpt, uiOptLen) < 0)
    {
        return BS_ERR;
    }

    return BS_OK;
}

BS_STATUS Socket_Ioctl(INT iSocketId, INT lCmd, void *argp)
{
    INT lRet;
    
#ifdef IN_WINDOWS
    lRet = ioctlsocket(iSocketId, lCmd, argp);
#else
    lRet = ioctl(iSocketId, lCmd, argp);
#endif

    if (lRet != 0)
    {
        RETURN(BS_ERR);
    }

    return BS_OK;
}


BS_STATUS Socket_GetLocalIpPort(IN INT iSocketId, OUT UINT *pulIp, OUT USHORT *pusPort)
{
    socklen_t iAddrLen;
    struct sockaddr_storage stSockAddr;
    struct sockaddr_in *pstSin;

    Mem_Zero(&stSockAddr, sizeof(stSockAddr));
    iAddrLen = sizeof(stSockAddr);
    
    getsockname(iSocketId, (struct sockaddr *)&stSockAddr, &iAddrLen);

    pstSin = (struct sockaddr_in*)&stSockAddr;

    *pusPort = ntohs(pstSin->sin_port);
#ifdef IN_WINDOWS
    *pulIp = ntohl(pstSin->sin_addr.S_un.S_addr);
#endif
#ifdef IN_UNIXLIKE
    *pulIp = ntohl(pstSin->sin_addr.s_addr);
#endif
    return BS_OK;
}


BS_STATUS Socket_GetPeerIpPort(IN INT iSocketId, OUT UINT *pulIp, OUT USHORT *pusPort)
{
    socklen_t iAddrLen;
    struct sockaddr_storage stSockAddr;
    struct sockaddr_in *pstSin;

    Mem_Zero(&stSockAddr, sizeof(stSockAddr));
    iAddrLen = sizeof(stSockAddr);
    
    getpeername(iSocketId, (struct sockaddr *)&stSockAddr, &iAddrLen);

    pstSin = (struct sockaddr_in*)&stSockAddr;

    *pusPort = ntohs(pstSin->sin_port);
#ifdef IN_WINDOWS
    *pulIp = ntohl(pstSin->sin_addr.S_un.S_addr);
#endif
#ifdef IN_UNIXLIKE
    *pulIp = ntohl(pstSin->sin_addr.s_addr);
#endif

    return BS_OK;
}


USHORT Socket_GetHostPort(IN INT iSocketId)
{
    UINT uiIp;
    USHORT usPort;
    if (BS_OK != Socket_GetLocalIpPort(iSocketId, &uiIp, &usPort))
    {
        return 0;
    }

    return usPort;
}

UINT Socket_GetFamily(IN INT iSocketId)
{
    struct sockaddr_storage stSockAddrLocal;
    struct sockaddr_in *pstSinLocal;
    socklen_t iAddrLen;

    Mem_Zero(&stSockAddrLocal, sizeof(stSockAddrLocal));
    iAddrLen = sizeof(stSockAddrLocal);
    getsockname(iSocketId, (struct sockaddr *)&stSockAddrLocal, &iAddrLen);

    pstSinLocal = (struct sockaddr_in*)&stSockAddrLocal;

    return pstSinLocal->sin_family;
}

BS_STATUS Socket_Bind(IN INT iSocketId, IN UINT ulIp, IN USHORT usPort)
{
    struct sockaddr_in server_addr;

    Mem_Zero (&server_addr, sizeof(struct sockaddr_in));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = usPort;
    server_addr.sin_addr.s_addr= ulIp;

    if (bind(iSocketId, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        RETURN(BS_ERR);
    }

    return BS_OK;
}

BS_STATUS Socket_SetRecvBufSize(IN INT iSocketId, IN UINT ulBufLen)
{
    int ulLen = sizeof(int);
    
    if (setsockopt(iSocketId, SOL_SOCKET, SO_RCVBUF, (VOID *)&ulBufLen, ulLen) < 0)
    {
        RETURN(BS_ERR);
    }

    return BS_OK;
}

BS_STATUS Socket_SetSendBufSize(IN INT iSocketId, IN UINT ulBufLen)
{
    int ulLen = sizeof(int);
    
    if (setsockopt(iSocketId, SOL_SOCKET, SO_SNDBUF, (VOID *)&ulBufLen, ulLen) < 0)
    {
        RETURN(BS_ERR);
    }

    return BS_OK;
}

BS_STATUS Socket_Close(IN INT iSocketId)
{
    SSHOW_Del(iSocketId);
    closesocket(iSocketId);
    return BS_OK;
}

int _Socket_Create(int iFamily, UINT ulType, const char *filename, int line)
{
    INT s;

    socket_WindowInit();

    
    s = socket (iFamily, (INT)ulType, 0);

    if (s < 0) {
        return s;
    }

    SSHOW_Add(s, filename, line);

    return s;
}

BS_STATUS Socket_Listen(IN INT iSocketID, UINT ulLocalIp, IN USHORT usPort, IN UINT uiBacklog)
{
    INT iReuse = 1;

#ifndef IN_WINDOWS 
    (VOID) setsockopt(iSocketID, SOL_SOCKET, SO_REUSEADDR, (CHAR*)&iReuse, sizeof(INT));
#endif

    if (BS_OK != Socket_Bind(iSocketID, ulLocalIp, usPort))
    {
        RETURN(BS_ERR);
    }

    if(listen(iSocketID, uiBacklog)!=0)
    {
        RETURN(BS_ERR);
    }

    return BS_OK;
}

BS_STATUS Socket_SendTo
(
    IN INT iSocketId,
    IN VOID *pBuf,
    IN UINT ulBufLen,
    IN UINT ulToIp,
    IN USHORT usToPort
)
{
    struct sockaddr_in stSockAddr;
    INT lSize;

    Mem_Zero(&stSockAddr, sizeof(stSockAddr));

    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port   = usToPort;
    stSockAddr.sin_addr.s_addr = ulToIp;

    lSize = sendto(iSocketId, pBuf, ulBufLen, 0, (struct sockaddr*)&stSockAddr, sizeof(stSockAddr));

    if (lSize < 0)
    {
        RETURN(BS_ERR);
    }

    return BS_OK;
}

BS_STATUS Socket_RecvFrom
(
    IN INT iSocketId,
    OUT VOID *pBuf,
    IN UINT ulBufLen,
    OUT UINT *pulRecvLen,
    OUT UINT *pulFromIp,
    OUT USHORT *pusFromPort
)
{
    struct sockaddr_in stSockAddr;
    socklen_t  ulAddrLen = 0;
    INT lSize;

    *pulRecvLen = 0;

    Mem_Zero(&stSockAddr,sizeof(sizeof(stSockAddr)));
    ulAddrLen = sizeof(stSockAddr);

    lSize = recvfrom(iSocketId, pBuf, ulBufLen, 0, (struct sockaddr*)&stSockAddr, &ulAddrLen);
    if (lSize < 0)
    {
        RETURN(BS_ERR);
    }

    *pulFromIp = stSockAddr.sin_addr.s_addr;
    *pusFromPort = stSockAddr.sin_port;

    *pulRecvLen = lSize;

    return BS_OK;
}

BS_STATUS _Socket_Pair(UINT uiType, OUT INT aiFd[2], const char *filename, int line)
{
#ifndef IN_WINDOWS
	int fd[2];

    if (socketpair(AF_UNIX, uiType, 0, fd) < 0)
    {
        RETURN(BS_ERR);
    }

    SSHOW_Add(fd[0], filename, line);
    SSHOW_Add(fd[1], filename, line);

    aiFd[0] = (UINT)fd[0];
    aiFd[1] = (UINT)fd[1];

    return BS_OK;
#else
    
	INT iListenFd, iAcceptFd, iConnectFd;
    UINT uiIp;
    USHORT usPort;

    if ((iListenFd = _Socket_Create(AF_INET, uiType, filename, line)) < 0)
    {
        RETURN(BS_ERR);
    }

    if ((iConnectFd = _Socket_Create(AF_INET, uiType, filename, line)) < 0)
    {
        Socket_Close(iListenFd);
        RETURN(BS_ERR);
    }

    if (BS_OK != Socket_Listen(iListenFd, htonl(LOOPBACK_IP), 0, 1))
    {
        Socket_Close(iListenFd);
        Socket_Close(iConnectFd);
        RETURN(BS_ERR);
    }

    Socket_GetLocalIpPort(iListenFd, &uiIp, &usPort);
    if (BS_OK != Socket_Connect(iConnectFd, LOOPBACK_IP, usPort))
    {
        Socket_Close(iListenFd);
        Socket_Close(iConnectFd);
        RETURN(BS_ERR);
    }

    iAcceptFd = Socket_Accept(iListenFd, NULL, NULL);
    if (iAcceptFd < 0)
    {
        Socket_Close(iListenFd);
        Socket_Close(iConnectFd);
        RETURN(BS_ERR);
    }

    Socket_Close(iListenFd);

    aiFd[0] = iConnectFd;
    aiFd[1] = iAcceptFd;

    return BS_OK;
#endif
}

BS_STATUS Socket_SetNoBlock(IN INT iSocketID, IN BOOL_T bNoBlock)
{
#ifdef IN_WINDOWS
    int uiValue = (int)bNoBlock;
    return Socket_Ioctl(iSocketID, FIONBIO, &uiValue);
#endif

#ifdef IN_UNIXLIKE
    int flag = -1;
    flag = fcntl(iSocketID,F_GETFL);
    BIT_CLR(flag, O_NONBLOCK);
    if (bNoBlock) {
        flag |= O_NONBLOCK;
    }
    fcntl(iSocketID,F_SETFL,flag);
    return 0;
#endif
}

BS_STATUS Socket_SetNoDelay(IN INT iSocketID, IN BOOL_T bNoDelay)
{
    INT iOn = 0;

    if (bNoDelay) {
        iOn = 1;
    }
    
    return Socket_SetSockOpt(iSocketID, IPPROTO_TCP, TCP_NODELAY, &iOn, sizeof(INT));
}

int Socket_SetReuseAddr(int fd, int reuse)
{
    return Socket_SetSockOpt(fd, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(int));
}

INT Socket_Dup(IN INT iFd)
{
    INT iNewFd;
    
#ifdef IN_UNIXLIKE
    iNewFd = dup(iFd);
#endif

#ifdef IN_WINDOWS
    HANDLE hDupFd;
    HANDLE hCurrentProcess;

    hCurrentProcess = GetCurrentProcess();
    
    if (! DuplicateHandle(hCurrentProcess,(HANDLE)iFd, hCurrentProcess, &hDupFd, 0, TRUE, DUPLICATE_SAME_ACCESS))
    {
        iNewFd = -1;
    }

    iNewFd = (INT)(UINT)hDupFd;
#endif

    if (iNewFd >= 0)
    {
        SSHOW_Add(iNewFd, __FILE__, __LINE__);
    }

    return iNewFd;
}


INT Socket_Inheritable(IN INT iFd)
{
#ifdef IN_UNIXLIKE
    return iFd;
#endif

#ifdef IN_WINDOWS
    INT iNewFd;
    iNewFd = Socket_Dup(iFd);
    if (iNewFd >= 0)
    {
        Socket_Close(iFd);
    }

    return iNewFd;
#endif
}

int _Socket_OpenUdp(UINT ip, USHORT port, const char *file, int line)
{
    int fd;

    fd = _Socket_Create(AF_INET, SOCK_DGRAM, file, line);
    if ((ip == 0) && (port == 0)) {
        return fd;
    }

    if (ip || port) {
        int ret = Socket_Bind(fd, ip, port);
        if (ret != 0) {
            Socket_Close(fd);
            return ret;
        }
    }

    return fd;
}

int _Socket_UdpClient(UINT ip, USHORT port, const char *file, int line)
{
    int fd;
    int ret;

    fd = _Socket_Create(AF_INET, SOCK_DGRAM, file, line);
    if (fd < 0) {
        return fd;
    }

    ret = Socket_Connect(fd, ip, port);
    if (ret != 0) {
        Socket_Close(fd);
        return ret;
    }

    return fd;
}

int _Socket_TcpServer(UINT ip, USHORT port, const char *file, int line)
{
    int fd;
    int ret;

    fd = _Socket_Create(AF_INET, SOCK_STREAM, file, line);
    if (fd < 0) {
        RETURN(BS_ERR);
    }

    ret = Socket_Listen(fd, ip, port, 5);
    if (0 != ret) {
        Socket_Close(fd);
        return ret;
    }

    return fd;
}

int _Socket_UnixServer(char *path, int type, const char *file, int line)
{
	struct sockaddr_un un;
    int fd;
    int ret;

	if ((fd = _Socket_Create(AF_UNIX, type, file, line)) < 0) {
        return fd;
    }

	unlink(path);	

	memset(&un, 0, sizeof(un));
	un.sun_family = AF_UNIX;
	strlcpy(un.sun_path, path, sizeof(un.sun_path));
	int len = BS_OFFSET(struct sockaddr_un, sun_path) + strlen(path);

    ret = bind(fd, (struct sockaddr *)&un, len); 
	if (ret < 0) {
        Socket_Close(fd);
        return ret;
	}

    if (type == SOCK_STREAM) {
        if ((ret = listen(fd, 5)) < 0) {
            Socket_Close(fd);
            return ret;
        }
    }

    return fd;
}

int _Socket_UnixClient(char *path, int type, int no_block, const char *file, int line)
{
    struct sockaddr_un un;
    int fd;
    int ret, len;

    fd = _Socket_Create(AF_UNIX, type, file, line);
    if (fd < 0) {
        return fd;
    }

	memset(&un, 0, sizeof(un));
    un.sun_family = AF_UNIX;
	strlcpy(un.sun_path, path, sizeof(un.sun_path));
	len = BS_OFFSET(struct sockaddr_un, sun_path) + strlen(path);

    if (no_block) {
        Socket_SetNoBlock(fd, 1);
    }

    ret = connect(fd, (void*)&un, len);

    if ((ret < 0) && (errno != EINPROGRESS)) {
        Socket_Close(fd);
        return ret;
    }

    return fd;
}
