/*================================================================
*   Created by LiXingang
*   Description: OB: observer 神族叮当
*
================================================================*/
#ifndef _OB_UTL_H
#define _OB_UTL_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    DLL_NODE_S link_node;
    int pri; 
    void *func;
}OB_S;

typedef DLL_HEAD_S OB_LIST_S;
#define OB_LIST_INIT_VALUE DLL_HEAD_INIT_VALUE
#define OB_INIT            DLL_INIT

#define OB_SCAN_BEGIN(_pstObChainHead, _ob)   \
    do{ \
        DLL_SCAN ((_pstObChainHead), _ob)  { \

#define OB_SCAN_END()  }}while(0)

#define OB_NOTIFY(_pstObChainHead, _func_type, ...) \
    do { \
        OB_S *_ob;   \
        _func_type func; \
        OB_SCAN_BEGIN(_pstObChainHead, _ob) {   \
            func = _ob->func; \
            func(_ob, ##__VA_ARGS__); \
        }OB_SCAN_END();    \
    }while(0)

void OB_Add(OB_LIST_S *head, OB_S *ob);
void OB_Del(OB_LIST_S *head, OB_S *ob);

#ifdef __cplusplus
}
#endif
#endif 
