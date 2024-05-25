#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/pkcs12.h>
#include <errno.h>
#include <stdio.h>

# define B_FORMAT_TEXT   0x8000
# define FORMAT_TEXT    (1 | B_FORMAT_TEXT)     
# define FORMAT_BINARY   2                      
# define FORMAT_BASE64  (3 | B_FORMAT_TEXT)     
# define FORMAT_ASN1     4                      
# define FORMAT_PEM     (5 | B_FORMAT_TEXT)

BIO *dup_bio_in(int format);
BIO *dup_bio_out(int format);
BIO *dup_bio_err(int format);

X509 *load_cert(const char *file, int format, const char *cert_descrip);


int load_certs_crls(const char *file, int format, const char *pass, 
        const char *desc, STACK_OF(X509) **pcerts, STACK_OF(X509_CRL) **pcrls);
