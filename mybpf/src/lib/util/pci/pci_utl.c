/*********************************************************
*   Copyright (C) LiXingang
*   Description: 
*
********************************************************/
#include "bs.h"
#include "utl/pci_utl.h"
#include "utl/bit_opt.h"

typedef struct {
    UINT fmt:3;
    UINT type:5;
    UINT type_ign:5; 
    UINT reserved:3;
    UINT tlp_type: 8;
}PCIE_TLP_TYPE_MAP_S;

static const PCIE_TLP_TYPE_MAP_S g_pcie_tlp_type_map[] = {
    {.fmt = 0b000, .type=0b00000, .tlp_type=PCIE_TLP_TYPE_MRD},
    {.fmt = 0b001, .type=0b00000, .tlp_type=PCIE_TLP_TYPE_MRD},
    {.fmt = 0b000, .type=0b00001, .tlp_type=PCIE_TLP_TYPE_MRDLK},
    {.fmt = 0b001, .type=0b00001, .tlp_type=PCIE_TLP_TYPE_MRDLK},
    {.fmt = 0b010, .type=0b00000, .tlp_type=PCIE_TLP_TYPE_MWR},
    {.fmt = 0b011, .type=0b00000, .tlp_type=PCIE_TLP_TYPE_MWR},
    {.fmt = 0b000, .type=0b00010, .tlp_type=PCIE_TLP_TYPE_IORD},
    {.fmt = 0b010, .type=0b00010, .tlp_type=PCIE_TLP_TYPE_IOWR},
    {.fmt = 0b000, .type=0b00100, .tlp_type=PCIE_TLP_TYPE_CFGRD0},
    {.fmt = 0b010, .type=0b00100, .tlp_type=PCIE_TLP_TYPE_CFGWR0},
    {.fmt = 0b000, .type=0b00101, .tlp_type=PCIE_TLP_TYPE_CFGRD1},
    {.fmt = 0b010, .type=0b00101, .tlp_type=PCIE_TLP_TYPE_CFGWR1},
    {.fmt = 0b010, .type=0b11011, .tlp_type=PCIE_TLP_TYPE_TCFGRD},
    {.fmt = 0b001, .type=0b11011, .tlp_type=PCIE_TLP_TYPE_TCFGWR},
    {.fmt = 0b001, .type=0b10000, .type_ign =0b00111, .tlp_type=PCIE_TLP_TYPE_MSG},
    {.fmt = 0b011, .type=0b10000, .type_ign =0b00111, .tlp_type=PCIE_TLP_TYPE_MSGD},
    {.fmt = 0b000, .type=0b01010, .tlp_type=PCIE_TLP_TYPE_CPL},
    {.fmt = 0b010, .type=0b01010, .tlp_type=PCIE_TLP_TYPE_CPLD},
    {.fmt = 0b000, .type=0b01011, .tlp_type=PCIE_TLP_TYPE_CPLLK},
    {.fmt = 0b010, .type=0b01011, .tlp_type=PCIE_TLP_TYPE_CPLDLK},
    {.fmt = 0b010, .type=0b01100, .tlp_type=PCIE_TLP_TYPE_FETCHADD},
    {.fmt = 0b011, .type=0b01100, .tlp_type=PCIE_TLP_TYPE_FETCHADD},
    {.fmt = 0b010, .type=0b01101, .tlp_type=PCIE_TLP_TYPE_SWAP},
    {.fmt = 0b011, .type=0b01101, .tlp_type=PCIE_TLP_TYPE_SWAP},
    {.fmt = 0b010, .type=0b01110, .tlp_type=PCIE_TLP_TYPE_CAS},
    {.fmt = 0b011, .type=0b01110, .tlp_type=PCIE_TLP_TYPE_CAS},
    {.fmt = 0b100, .type=0b00000, .type_ign =0b01111, .tlp_type=PCIE_TLP_TYPE_LPRFX},
    {.fmt = 0b100, .type=0b10000, .type_ign =0b01111, .tlp_type=PCIE_TLP_TYPE_EPRFX},
};

static UINT _pcie_get_tlp_length(void *tlp)
{
    PCIE_TLP_HEADER_S *tlp_hdr = tlp;
    UINT length;

    length = PCIE_TLP_LENGTH(tlp_hdr);
    if (length == 0) {
        length = 1024;
    }

    length *= 4;

    return length;
}

static void * _pcie_get_data_ptr(void *tlp, int is4dw)
{
    char *d = tlp;
    d += 12;
    if (is4dw) {
        d += 4;
    }
    return d;
}

static BOOL_T _pcie_tlp_is_4dw(void *tlp)
{
    PCIE_TLP_HEADER_S *tlp_hdr = tlp;
    UCHAR fmt = PCIE_TLP_FMT(tlp_hdr);

    if (fmt & 0x1) {
        return TRUE;
    }

    return FALSE;
}

static void _pcie_change_common_tlp_order(INOUT PCIE_TLP_HEADER_S *tlp) 
{
    tlp->len_attr = htons(tlp->len_attr);
}

static void _pcie_change_cfg_tlp_order(INOUT PCIE_TLP_CFG_S *tlp) 
{
    tlp->request_id = htons(tlp->request_id);
    tlp->bdf = htons(tlp->bdf);
    tlp->reg_num = htons(tlp->reg_num);
}

static void _pcie_change_mem_tlp_order(INOUT PCIE_TLP_MEM_S *tlp) 
{
    tlp->request_id = htons(tlp->request_id);
    tlp->addr1 = htonl(tlp->addr1);
    if (_pcie_tlp_is_4dw(tlp)) {
        tlp->addr2 = htonl(tlp->addr2);
    }
}

static void _pcie_change_cpl_tlp_order(INOUT PCIE_TLP_COMPLETE_S *tlp) 
{
    tlp->completer_id = htons(tlp->completer_id);
    tlp->status_count = htons(tlp->status_count);
    tlp->request_id = htons(tlp->request_id);
}

int PCIE_GetTlpType(PCIE_TLP_HEADER_S *tlp)
{
    int i;

    UCHAR fmt = PCIE_TLP_FMT(tlp);
    UCHAR type = PCIE_TLP_TYPE(tlp);
    UCHAR tmp;

    for (i=0; i<ARRAY_SIZE(g_pcie_tlp_type_map); i++) {
        tmp = type;
        BIT_CLR(tmp, g_pcie_tlp_type_map[i].type_ign);
        if ((fmt == g_pcie_tlp_type_map[i].fmt) 
                && (tmp == g_pcie_tlp_type_map[i].type)) {
            return g_pcie_tlp_type_map[i].tlp_type;
        }
    }

    return -1;
}

BOOL_T PCIE_TLPIs4DW(void *tlp)
{
    return _pcie_tlp_is_4dw(tlp);
}

static int _pcie_check_tlp_hdr(void *tlp, int tlp_len)
{
    if (tlp_len < PCIE_TLP_3DW) {
        RETURN(BS_TOO_SMALL);
    }

    int is4dw = _pcie_tlp_is_4dw(tlp);
    if ((is4dw) && (tlp_len < PCIE_TLP_4DW)) {
        RETURN(BS_TOO_SMALL);
    }

    return 0;
}


int PCIE_CheckTlpCommonHdr(void *tlp, int tlp_len)
{
    return _pcie_check_tlp_hdr(tlp, tlp_len);
}


int PCIE_CheckTlp(void *tlp, int tlp_len)
{
    int data_len;
    int length;
    int ret;
    UCHAR fmt;
    PCIE_TLP_HEADER_S *tlp_hdr = tlp;

    ret = _pcie_check_tlp_hdr(tlp, tlp_len);
    if (ret < 0) {
        return ret;
    }

    int is4dw = _pcie_tlp_is_4dw(tlp);

    if (is4dw) {
        data_len = tlp_len - PCIE_TLP_4DW;
    } else {
        data_len = tlp_len - PCIE_TLP_3DW;
    }

    fmt = PCIE_TLP_FMT(tlp_hdr);
    if (fmt & 0b010) { 
        length = _pcie_get_tlp_length(tlp);
        if (length > data_len) {
            RETURN(BS_TOO_SMALL);
        }
    }

    return 0;
}


int PCIE_TlpCheckAndChangeOrder(void *msg, int len)
{
    int ret;

    ret = PCIE_CheckTlpCommonHdr(msg, len);
    if (ret < 0) {
        return ret;
    }

    PCIE_ChangeTlpOrder(msg);

    ret = PCIE_CheckTlp(msg, len);
    if (ret < 0) {
        return ret;
    }

    return 0;
}


void PCIE_BuildCfgTLP(UCHAR fmt, UCHAR type, USHORT bdf, UINT addr, int size, OUT PCIE_TLP_CFG_S *tlp)
{
    UINT v = 0;
    int i;
    int count = 0;

    PCIE_SET_TLP_FMT(tlp, fmt);
    PCIE_SET_TLP_TYPE(tlp, type);
    PCIE_SET_TLP_CFG_BDF(tlp, bdf);
    tlp->request_id = 0;
    PCIE_SET_TLP_LENGTH(tlp, 1);
    PCIE_SET_TLP_CFG_REG_NUM(tlp, addr/4);

    for (i=(addr % 4); i<4; i++) {
        if (count < size) {
            v |= (1 << i);
        }
        count ++;
    }
    PCIE_SET_TLP_CFG_FIRST_BE(tlp, v);
}



void PCIE_BuildCfgReadTLP(int is_ep, USHORT bdf, UINT addr, int size, OUT PCIE_TLP_CFG_S *tlp)
{
    if (is_ep) {
        PCIE_BuildCfgTLP(0, 0b00100, bdf, addr, size, tlp);
    } else {
        PCIE_BuildCfgTLP(0, 0b00101, bdf, addr, size, tlp);
    }
}


void PCIE_BuildCfgWriteTLP(int is_ep, USHORT bdf, UINT addr, int size, UINT val, OUT PCIE_TLP_CFG_S *tlp)
{
    if (is_ep) {
        PCIE_BuildCfgTLP(0b010, 0b00100, bdf, addr, size, tlp);
    } else {
        PCIE_BuildCfgTLP(0b010, 0b00101, bdf, addr, size, tlp);
    }

    PCIE_WriteTlpData(&val, 4, 0, (void*)tlp);
}


void PCIE_BuildMemTLP(UCHAR fmt, UCHAR type, USHORT bdf, UINT64 addr, BOOL_T is64, int size, OUT PCIE_TLP_MEM_S *tlp)
{
    int i;
    UINT v;
    UINT end_addr = addr + size;
    UINT len;
    int count = 0;

    PCIE_SET_TLP_FMT(tlp, fmt);
    PCIE_SET_TLP_TYPE(tlp, type);
    tlp->request_id = 0;

    len = (size+3)/4;
    if (len == 0) {
        len = 1;
    }

    PCIE_SET_TLP_LENGTH(tlp, len);

    if (is64) {
        tlp->addr1 = (addr >> 32);
        tlp->addr2 = addr & 0xfffffffc;
    } else {
        tlp->addr1 = addr & 0xfffffffc;
    }

    v = 0;
    for (i=(addr % 4); i<4; i++) {
        if (count < size) {
            v |= (1 << i);
        }
        count ++;
    }
    PCIE_SET_TLP_CFG_FIRST_BE(tlp, v);

    if ((addr & 0xfffffffc) + 4 < end_addr) {
        v = 0;
        for (i=0; i<(end_addr % 4); i++) {
            v |= (1 << i);
        }
        PCIE_SET_TLP_CFG_LAST_BE(tlp, v);
    }
}


void PCIE_BuildEpMemReadTLP(USHORT bdf, UINT64 addr, BOOL_T is64, int size, OUT PCIE_TLP_MEM_S *tlp)
{
    if (is64) {
        PCIE_BuildMemTLP(1, 0, bdf, addr, is64, size, tlp);
    } else {
        PCIE_BuildMemTLP(0, 0, bdf, addr, is64, size, tlp);
    }
}


void PCIE_BuildEpMemWriteTLP(USHORT bdf, UINT64 addr, BOOL_T is64, int size, void *data, OUT PCIE_TLP_MEM_S *tlp)
{
    if (is64) {
        PCIE_BuildMemTLP(3, 0, bdf, addr, is64, size, tlp);
    } else {
        PCIE_BuildMemTLP(2, 0, bdf, addr, is64, size, tlp);
    }
    PCIE_WriteTlpData(data, NUM_ALIGN(size, 4), is64, (void*)tlp);
}

void PCIE_BuildCpldTLP(USHORT comp_id, USHORT req_id, UINT byte_count,
        UINT low_addr, int size, void *data, OUT PCIE_TLP_COMPLETE_S *tlp)
{
    PCIE_SET_TLP_FMT(tlp, 0b010);
    PCIE_SET_TLP_TYPE(tlp, 0b01010);
    PCIE_SET_TLP_LENGTH(tlp, (size + 3)/4);
    PCIE_SET_TLP_COMP_BYTE_COUNT(tlp, byte_count);
    PCIE_SET_TLP_COMP_LOWER_ADDR(tlp, low_addr);
    tlp->completer_id = comp_id;
    tlp->request_id = req_id;

    PCIE_WriteTlpData(data, size, 0, (void*)tlp);
}

void PCIE_BuildCplTLP(USHORT comp_id, USHORT req_id, OUT PCIE_TLP_COMPLETE_S *tlp)
{
    PCIE_SET_TLP_FMT(tlp, 0b000);
    PCIE_SET_TLP_TYPE(tlp, 0b01010);
    PCIE_SET_TLP_LENGTH(tlp, 1);
    tlp->completer_id = comp_id;
    tlp->request_id = req_id;
}


UINT64 PCIE_GetMemTlpAddr(PCIE_TLP_MEM_S *tlp)
{
    UINT64 addr;
    int is64 = _pcie_tlp_is_4dw(tlp);

    if (is64) {
        addr = tlp->addr1;
        addr = addr << 32;
        addr |= (tlp->addr2 & 0xfffffffc);
    } else {
        addr = (tlp->addr1 & 0xfffffffc);
    }

    return addr;
}

void * PCIE_GetTlpDataPtr(void *tlp)
{
    return _pcie_get_data_ptr(tlp, _pcie_tlp_is_4dw(tlp));
}


UINT PCIE_GetTlpDataLength(PCIE_TLP_HEADER_S *tlp)
{
    return _pcie_get_tlp_length(tlp);
}

void PCIE_WriteTlpData(void *data, int data_len, BOOL_T is4dw, OUT PCIE_TLP_S *tlp)
{
    char *d = _pcie_get_data_ptr(tlp, is4dw);
    memcpy(d, data, data_len);
}


int PCIE_ReadTlpData(UINT64 addr, OUT void *data, int data_size, BOOL_T is4dw, PCIE_TLP_S *tlp)
{
    char *d;
    int length;
    int len;
    char *out = data;
    PCIE_TLP_COMPLETE_S *comp_tlp = (void*)tlp;

    length = _pcie_get_tlp_length(tlp);
    len = MIN(length, data_size);

    d = _pcie_get_data_ptr(tlp, is4dw);

    UINT low_addr = PCIE_TLP_COMP_LOWER_ADDR(comp_tlp);

    if (low_addr) {
        low_addr &= 0x3;
        if (len <= low_addr) {
            return -1;
        }
        len -= low_addr;
        d += low_addr;
    } else {
        len -= (addr % 4);
        d += (addr % 4);
    }

    memcpy(out, d, len);

    return len;
}


void PCIE_ChangeTlpOrder(INOUT void *tlp)
{
    UCHAR type;

    _pcie_change_common_tlp_order(tlp);

    type = PCIE_GetTlpType(tlp);

    switch (type) {
        case PCIE_TLP_TYPE_MWR:
        case PCIE_TLP_TYPE_MRD:
        case PCIE_TLP_TYPE_MRDLK:
            _pcie_change_mem_tlp_order(tlp);
            break;
        case PCIE_TLP_TYPE_CFGRD0:
        case PCIE_TLP_TYPE_CFGWR0:
        case PCIE_TLP_TYPE_CFGRD1:
        case PCIE_TLP_TYPE_CFGWR1:
            _pcie_change_cfg_tlp_order(tlp);
            break;
        case PCIE_TLP_TYPE_CPL:
        case PCIE_TLP_TYPE_CPLD:
            _pcie_change_cpl_tlp_order(tlp);
            break;
    }
}

