#ifndef X509_DEREPLICATON_H
#define X509_DEREPLICATON_H

/*
 * src_file: [in] location of pem either file or derectory in pem format
 * dst_file: [in] location of pem file to save merged file
 * total:  [out] the total # of x509 cert in src_file
 * unique: [out] the unique # of X509 cert after dereplication
 */

int certs_dereplication(char *src_file, char *dst_file, int *total, int *unique);

#endif
