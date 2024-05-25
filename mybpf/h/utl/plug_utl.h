/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _PLUG_UTL_H
#define _PLUG_UTL_H
#ifdef __cplusplus
extern "C"
{
#endif

#ifdef IN_UNIXLIKE
#define DLL_PROCESS_ATTACH 0 
#define DLL_THREAD_ATTACH  1 
#define DLL_THREAD_DETACH  2 
#define DLL_PROCESS_DETACH 3 
#endif

#ifdef IN_UNIXLIKE
typedef BOOL_T (*PF_PLUG_Entry)(PLUG_HDL hPlug, int reason, void *reserved);

#define PLUG_ENTRY \
    PLUG_API BOOL_T PLUG_Entry(PLUG_HDL hPlug, int reason, void *reserved) \
    {   \
        static int _inited = 0; \
        if (DLL_PROCESS_ATTACH == reason) {if (_inited == 1) return 1;_inited = 1;} \
        if (DLL_PROCESS_DETACH == reason) {_inited = 0;} \
        return DllMain(hPlug, reason, reserved); \
    }
#endif

#ifdef IN_WINDOWS
#define PLUG_ENTRY 
#endif


PLUG_HDL PLUG_LoadLib(IN CHAR *pszFilePath);
void PLUG_UnloadLib(PLUG_HDL plug);


#ifdef __cplusplus
}
#endif
#endif 
