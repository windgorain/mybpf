
#ifndef __UTL_EXTURL_H_
#define __UTL_EXTURL_H_

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

#define EXTURL_MAX_PROTOCOL_LEN 15
#define EXTURL_MAX_ADDRESS_LEN  255
#define EXTURL_MAX_PATH_LEN     255

typedef struct
{
	CHAR szProtocol[EXTURL_MAX_PROTOCOL_LEN + 1];
	CHAR szAddress[EXTURL_MAX_ADDRESS_LEN + 1];
	CHAR szPath[EXTURL_MAX_PATH_LEN + 1];
	USHORT usPort;
}EXTURL_S;




BS_STATUS EXTURL_Parse
(
	IN CHAR *pcExtUrl,
	OUT EXTURL_S *pstExtUrl
);

#ifdef __cplusplus
    }
#endif 

#endif 

