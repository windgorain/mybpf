#ifndef CERT_VERIFY_H
#define CERT_VERIFY_H
#include <openssl/x509.h>

int cert_verify_by_X509(STACK_OF(X509) *trusted, STACK_OF(X509) *untrusted, X509 *cert) ;

int cert_verify_by_file(const char *CApath, const char *CAfile, int noCApath, int noCAfile, const char *file);
#endif
