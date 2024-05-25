/*================================================================
*   Created：2018.11.02
*   Description：
*
================================================================*/
#ifndef _FAKE_CERT_H
#define _FAKE_CERT_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    void *ca_cert;
    void *ca_key;
    void *self_key;
}FAKECERT_CTRL_S;

int FakeCert_init(FAKECERT_CTRL_S *ctrl, char *ca_certfile, char *ca_keyfile, char *self_keyfile, pem_password_cb *cb, void *u);
void FakeCdrt_Finit(FAKECERT_CTRL_S *ctrl);
int FakeCert_BuildByCert(FAKECERT_CTRL_S *ctrl, X509 *real_cert);

X509 * FakeCert_Build(FAKECERT_CTRL_S *ctrl);
#ifdef __cplusplus
}
#endif
#endif 
