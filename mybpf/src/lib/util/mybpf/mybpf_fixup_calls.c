/*================================================================
*   Created by LiXingang, Copyright LiXingang
*   Date: 2017.10.2
*   Description: 
*
================================================================*/
#include "bs.h"
#include "utl/ufd_utl.h"
#include "utl/umap_utl.h"
#include "utl/mybpf_loader.h"
#include "utl/mybpf_prog.h"
#include "utl/mybpf_insn.h"
#include "utl/bpf_helper_utl.h"
#include "mybpf_osbase.h"
#include "mybpf_def_inner.h"


static int _mybpf_prog_get_helper_offset(int imm, void *ud)
{
    PF_BPF_HELPER_FUNC helper_func = BpfHelper_GetFuncExt(imm, ud);
    if (! helper_func) {
        return 0;
    }

    PF_BPF_HELPER_FUNC base_func = (void*)BpfHelper_BaseHelper;
    return helper_func - base_func;
}



int MYBPF_PROG_FixupExtCalls(void *insts, int len, const void **tmp_helpers)
{
    return MYBPF_INSN_FixupExtCalls(insts, len, _mybpf_prog_get_helper_offset, tmp_helpers);
}

