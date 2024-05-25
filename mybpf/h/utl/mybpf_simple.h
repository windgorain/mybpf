/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _MYBPF_SIMPLE_H
#define _MYBPF_SIMPLE_H
#include "utl/mybpf_utl.h"
#include "utl/vbuf_utl.h"
#include "utl/umap_def.h"
#include "utl/mybpf_spf_sec.h"
#include "utl/mybpf_simple_def.h"
#include "utl/mybpf_depend_def.h"
#ifdef __cplusplus
extern "C"
{
#endif

#define MYBPF_SIMPLE_CONVERT_FLAG_HAVE_NAME 0x1 

typedef struct {
    int jit_arch; 

    UINT helper_mode: 4;

    UINT with_func_name: 1; 
    UINT with_map_name: 1; 
    UINT keep_prog_order: 1; 

    UINT translate_mode_aot: 1; 
    UINT param_6th: 1; 
    UINT raw_bare: 1; 

    U32 app_ver;
}MYBPF_SIMPLE_CONVERT_PARAM_S;

typedef struct {
	unsigned int type:1;
	unsigned int size_key:1;
	unsigned int size_value:1;
	unsigned int max_elem:1;
	unsigned int flags:1;
}MYBPF_SIMPLE_MAP_MASK_S;

int MYBPF_SPF_ConvertBuf(FILE_MEM_S *m, OUT VBUF_S *vbuf, MYBPF_SIMPLE_CONVERT_PARAM_S *p);
int MYBPF_SPF_ConvertFile2File(char *src_file, char *dst_file, MYBPF_SIMPLE_CONVERT_PARAM_S *p);
int MYBPF_SPF_ConvertFile2Buf(char *src_file, OUT VBUF_S *vbuf, MYBPF_SIMPLE_CONVERT_PARAM_S *p);

int MYBPF_SPF_OpenExt(char *file, OUT FILE_MEM_S *m, MYBPF_SIMPLE_CONVERT_PARAM_S *p);
int MYBPF_SPF_Open(char *file, OUT FILE_MEM_S *m);
int MYBPF_SPF_OpenFileRaw(char *file, OUT FILE_MEM_S *m);
void MYBPF_SPF_Close(FILE_MEM_S *m);
BOOL_T MYBPF_SPF_IsSpfFormat(FILE_MEM_S *m);
BOOL_T MYBPF_SPF_IsSpfFile(char *filename);
Elf64_Shdr * MYBPF_SPF_GetDependsSec(LLDATA_S *d);
U32 MYBPF_SPF_GetDependsCount(LLDATA_S *d);
int * MYBPF_SPF_GetDepends(LLDATA_S *d);
U64 MYBPF_SPF_GetProgsSize(LLDATA_S *d);
void * MYBPF_SPF_GetProgs(LLDATA_S *d);
int MYBPF_SPF_GetJitArch(FILE_MEM_S *m);
int MYBPF_SPF_GetMaps(FILE_MEM_S *m, OUT MYBPF_MAPS_SEC_S *map_sec);
char * MYBPF_SPF_GetMapNameByID(FILE_MEM_S *m, U32 map_id);
int MYBPF_SPF_GetMapIDByName(FILE_MEM_S *m, char *map_name);
int MYBPF_SPF_ModifyMap(FILE_MEM_S *m, int map_id, UMAP_ELF_MAP_S *mapdef, MYBPF_SIMPLE_MAP_MASK_S *msk);
int MYBPF_SPF_ReplaceArchPattern(char *src_filename, OUT char *dst_filename, int size);

#ifdef __cplusplus
}
#endif
#endif 
