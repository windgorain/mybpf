/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _UBPF_UTL_H
#define _UBPF_UTL_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef void * UBPF_VM_HANDLE;
typedef uint64_t (*ubpf_jit_fn)(void* mem, size_t mem_len);

typedef struct {
    UBPF_VM_HANDLE vm;
    ubpf_jit_fn jit_func;
}UBPF_JIT_S;



int UBPF_S2c(int linktype, char *cbpf_string, OUT void *bpf_prog);

UBPF_VM_HANDLE UBPF_S2e(int linktype, char *cbpf_string);
int UBPF_S2j(int linktype, char *cbpf_string, OUT UBPF_JIT_S *jit);


UBPF_VM_HANDLE UBPF_C2e(void *bpf_prog);

ubpf_jit_fn UBPF_C2j(void *bpf_prog, OUT UBPF_JIT_S *jit);


ubpf_jit_fn UBPF_E2j(UBPF_VM_HANDLE vm);

int UBPF_GetJittedSize(UBPF_VM_HANDLE vm);

UBPF_VM_HANDLE UBPF_Create();
void UBPF_Destroy(UBPF_VM_HANDLE vm);
int UBPF_Load(UBPF_VM_HANDLE vm, void *ebpf_code, int ebpf_len);
UBPF_VM_HANDLE UBPF_CreateLoad(void *ebpf_code, int ebpf_len, void **funcs, int funcs_count);
int BPF_Check(void *insts, int num_insts, OUT char * error_msg, int error_msg_size);

#ifdef __cplusplus
}
#endif
#endif 
