/******************************************************************
*   Created by LiXingang
*   Description: 
*
******************************************************************/
#include "bs.h"
#include "utl/mmap_utl.h"
#include "utl/arch_utl.h"
#include "utl/mybpf_prog_def.h"
#include "utl/bpf_helper_utl.h"
#include "utl/file_utl.h"
#include "utl/mybpf_bare.h"

int MYBPF_LoadBareFile(char *file, const void **tmp_helpers, OUT MYBPF_BARE_S *bare)
{
    int ret;
    FILE_MEM_S m = {0};

    ret = FILE_Mem(file, &m);
    if (ret < 0) {
        RETURNI(BS_CAN_NOT_OPEN, "Can't open file");
    }

    ret = MYBPF_LoadBare(m.data, m.len, tmp_helpers, bare);

    FILE_FreeMem(&m);

    return ret;
}


U64 MYBPF_RunBareFile(char *file, const void **tmp_helpers, MYBPF_PARAM_S *p)
{
    MYBPF_BARE_S bare;
    int ret;

    ret = MYBPF_LoadBareFile(file, tmp_helpers, &bare);
    if (ret < 0) {
        return ret;
    }

    U64 bpf_ret = MYBPF_RunBareMain(&bare, p);

    MYBPF_UnloadBare(&bare);

    return bpf_ret;
}

BOOL_T MYBPF_IsBareFile(char *filename)
{
    MYBPF_BARE_HDR_S hdr = {0};

    int len = FILE_MemTo(filename, &hdr, sizeof(hdr));
    if (len != sizeof(hdr)) {
        return FALSE;
    }

    if (hdr.magic != htonl(MYBPF_BARE_MAGIC)) {
        return FALSE;
    }

    return TRUE;
}

