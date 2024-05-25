/*================================================================
*   Created by LiXingang: 2018.11.14
*   Description: 
*
================================================================*/
#ifndef _SSL_DECODE_H
#define _SSL_DECODE_H
#ifdef __cplusplus
extern "C"
{
#endif

#define SSL3_VERSION                    0x0300
#define TLS1_VERSION                    0x0301
#define TLS1_1_VERSION                  0x0302
#define TLS1_2_VERSION                  0x0303

#if 1 

# define TLSEXT_TYPE_server_name                 0
# define TLSEXT_TYPE_max_fragment_length         1
# define TLSEXT_TYPE_client_certificate_url      2
# define TLSEXT_TYPE_trusted_ca_keys             3
# define TLSEXT_TYPE_truncated_hmac              4
# define TLSEXT_TYPE_status_request              5

# define TLSEXT_TYPE_user_mapping                6

# define TLSEXT_TYPE_client_authz                7
# define TLSEXT_TYPE_server_authz                8

# define TLSEXT_TYPE_cert_type           9

# define TLSEXT_TYPE_elliptic_curves             10
# define TLSEXT_TYPE_ec_point_formats            11

# define TLSEXT_TYPE_srp                         12

# define TLSEXT_TYPE_signature_algorithms        13

# define TLSEXT_TYPE_use_srtp    14

# define TLSEXT_TYPE_heartbeat   15

# define TLSEXT_TYPE_application_layer_protocol_negotiation 16

# define TLSEXT_TYPE_signed_certificate_timestamp    18

# define TLSEXT_TYPE_padding     21

# define TLSEXT_TYPE_encrypt_then_mac    22

# define TLSEXT_TYPE_extended_master_secret      23

# define TLSEXT_TYPE_session_ticket              35

# define TLSEXT_TYPE_renegotiate                 0xff01
# endif 


# define TLSEXT_NAMETYPE_host_name 0


typedef struct {
    UCHAR handshake_type;
    UCHAR session_id_len;
    UCHAR  compression_methods_len;
    USHORT handshake_ssl_ver;
    USHORT cipher_suites_len;
    USHORT extensions_len;
    UINT len;
    UINT time;
    void *random;
    void *session_id;
    USHORT *cipher_suites;
    void *extensions;
}SSL_CLIENT_HELLO_INFO_S ;

int SSLDecode_IsRecordComplete(void *buf, int buf_len);
int SSLDecode_ParseClientHello(UCHAR *buf, int buf_len, SSL_CLIENT_HELLO_INFO_S *client_hello_info);

typedef struct {
    USHORT type;
    USHORT len;
    void *data;
}SSL_DECODE_EXTENSION_S;
void SSLDecode_InitExtensionIter(SSL_DECODE_EXTENSION_S *iteration);
SSL_DECODE_EXTENSION_S *SSLDecode_GetNextExtension(void *extensions, int extensions_len, SSL_DECODE_EXTENSION_S *iteration);
LSTR_S * SSLDecode_GetSNI(void *extensions, int extensions_len, LSTR_S *sni);
char * SSLDecode_GetSNIString(void *extensions, int extensions_len, char *sni, int sni_size);

int SSLDecode_DegreaseExtData(USHORT ext_type, SSL_DECODE_EXTENSION_S*ext,char* finger_buf, int buf_len);
BOOL_T SSLDecode_TypeInGrase(USHORT type);
BOOL_T SSLDecode_TypeInExtract(USHORT type);
BOOL_T SSLDecode_TypeInDecode(USHORT type);
USHORT SSLDecode_DegreaseTypeCode(USHORT type);

#ifdef __cplusplus
}
#endif
#endif 
