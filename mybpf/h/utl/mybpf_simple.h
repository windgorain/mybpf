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
    UINT keep_text_pos: 1; 

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

int MYBPF_SIMPLE_Elf2SpfBuf(void *elf, MYBPF_SIMPLE_CONVERT_PARAM_S *p, OUT VBUF_S *vbuf);
int MYBPF_SIMPLE_Bpf2SpfBuf(char *bpf_file, MYBPF_SIMPLE_CONVERT_PARAM_S *p, OUT VBUF_S *vbuf);
int MYBPF_SIMPLE_DelSec(VBUF_S *vbuf, int type, int id);
int MYBPF_SIMPLE_Merge(VBUF_S *src1, VBUF_S *src2, OUT VBUF_S *dst);
int MYBPF_SIMPLE_Convert(FILE_MEM_S *spf, OUT VBUF_S *dst, MYBPF_SIMPLE_CONVERT_PARAM_S *p);
int MYBPF_SIMPLE_Convert2Buf(char *src_filename, MYBPF_SIMPLE_CONVERT_PARAM_S *p, OUT VBUF_S *vbuf);
int MYBPF_SIMPLE_Convert2File(char *src_filename, char *dst_filename, MYBPF_SIMPLE_CONVERT_PARAM_S *p);
int MYBPF_SIMPLE_ModifyMap(FILE_MEM_S *m, int map_id, UMAP_ELF_MAP_S *mapdef, MYBPF_SIMPLE_MAP_MASK_S *msk);
int MYBPF_SIMPLE_BuildInfo(FILE_MEM_S *m, OUT char *buf, int buf_size);
int MYBPF_SIMPLE_BuildFileInfo(char *file, OUT char *buf, int buf_size);

BOOL_T MYBPF_SIMPLE_IsSpfFile(char *simple_file);

int MYBPF_SIMPLE_OpenSpf(char *file, OUT FILE_MEM_S *m);
int MYBPF_SIMPLE_OpenFile(char *file, OUT FILE_MEM_S *m);
int MYBPF_SIMPLE_OpenFileRaw(char *file, OUT FILE_MEM_S *m);
void MYBPF_SIMPLE_Close(FILE_MEM_S *m);

int MYBPF_SIMPLE_WriteFile(FILE_MEM_S *m, char *filename);
int MYBPF_SIMPLE_GetMapCount(FILE_MEM_S *m);
U32 MYBPF_SIMPLE_GetMapFlags(FILE_MEM_S *m);
int MYBPF_SIMPLE_GetMapsSection(FILE_MEM_S *m, OUT MYBPF_MAPS_SEC_S *map_sec);
void * MYBPF_SIMPLE_GetMap(FILE_MEM_S *m, int index);
char * MYBPF_SIMPLE_GetMapName(FILE_MEM_S *m, int id);
int MYBPF_SIMPLE_GetMapIDByName(FILE_MEM_S *m, char *name);
int MYBPF_SIMPLE_GetJitArch(FILE_MEM_S *m);

int MYBPF_SIMPLE_GetTypeSecCount(FILE_MEM_S *m, int type);
void * MYBPF_SIMPLE_GetSec(FILE_MEM_S *m, int type, int id);
void * MYBPF_SIMPLE_GetSecByName(FILE_MEM_S *m, int type, char *name);
char * MYBPF_SIMPLE_GetSecName(void *sec);
void * MYBPF_SIMPLE_GetSecData(void *sec);
int MYBPF_SIMPLE_GetSecDataSize(void *sec);
int MYBPF_SIMPLE_CopySecData(void *sec, OUT void *mem, int mem_size);
void * MYBPF_SIMPLE_DupSecData(void *sec);

int MYBPF_SIMPLE_GetProgsCount(FILE_MEM_S *m);
int MYBPF_SIMPLE_GetProgsSize(FILE_MEM_S *m);
void * MYBPF_SIMPLE_GetProgs(FILE_MEM_S *m);
int MYBPF_SIMPLE_CopyPorgs(FILE_MEM_S *m, OUT void *mem, int mem_size);
void * MYBPF_SIMPLE_DupProgs(FILE_MEM_S *m);
int MYBPF_SIMPLE_GetProgsInfo(FILE_MEM_S *m, OUT ELF_PROG_INFO_S *progs, int max_prog_count);
int MYBPF_SIMPLE_GetProgsInfoSize(FILE_MEM_S *m);
int MYBPF_SIMPLE_GetMainProgsCount(FILE_MEM_S *m);
int MYBPF_SIMPLE_GetMainProgInfo(FILE_MEM_S *m, int id, OUT ELF_PROG_INFO_S *off);
int MYBPF_SIMPLE_GetMainProgsInfo(FILE_MEM_S *m, OUT ELF_PROG_INFO_S *progs, int max_prog_count);
int MYBPF_SIMPLE_WalkProg(FILE_MEM_S *m, PF_ELF_WALK_PROG walk_func, void *ud);
int MYBPF_SIMPLE_CopyDepends(FILE_MEM_S *m, OUT MYBPF_HELPER_DEPENDS_S *d, int max_count);

UINT MYBPF_SIMPLE_GetSimpleSizeByHdr(void *hdr);

#ifdef __cplusplus
}
#endif
#endif 
