/*********************************************************
*   Copyright (C) LiXingang
*   Description: 
*
********************************************************/
#ifndef _PCI_UTL_H
#define _PCI_UTL_H

#include "utl/bit_opt.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define PCIE_TLP_3DW 12
#define PCIE_TLP_4DW 16
#define PCIE_TLP_DATA_MAX (1024 * 4)
#define PCIE_TLP_SIZE_MAX (1028 * 4)

enum {
    PCIE_TLP_TYPE_MRD = 0,
    PCIE_TLP_TYPE_MRDLK,
    PCIE_TLP_TYPE_MWR,
    PCIE_TLP_TYPE_IORD,
    PCIE_TLP_TYPE_IOWR,
    PCIE_TLP_TYPE_CFGRD0,
    PCIE_TLP_TYPE_CFGWR0,
    PCIE_TLP_TYPE_CFGRD1,
    PCIE_TLP_TYPE_CFGWR1,
    PCIE_TLP_TYPE_TCFGRD,
    PCIE_TLP_TYPE_TCFGWR,
    PCIE_TLP_TYPE_MSG,
    PCIE_TLP_TYPE_MSGD,
    PCIE_TLP_TYPE_CPL,
    PCIE_TLP_TYPE_CPLD,
    PCIE_TLP_TYPE_CPLLK,
    PCIE_TLP_TYPE_CPLDLK,
    PCIE_TLP_TYPE_FETCHADD,
    PCIE_TLP_TYPE_SWAP,
    PCIE_TLP_TYPE_CAS,
    PCIE_TLP_TYPE_LPRFX,
    PCIE_TLP_TYPE_EPRFX,
};

typedef struct {
    unsigned char fmt_type; 
    unsigned char flag; 
    unsigned short len_attr; 
}PCIE_TLP_HEADER_S;

typedef struct {
    unsigned char fmt_type; 
    unsigned char flag; 
    unsigned short len_attr; 

    unsigned int bytes[3]; 

    unsigned int payload[1024];
}PCIE_TLP_S;

#define PCIE_TLP_FMT(tlp) ((tlp)->fmt_type >> 5)
#define PCIE_TLP_TYPE(tlp) ((tlp)->fmt_type & 0x1f)
#define PCIE_TLP_TH(tlp) ((tlp)->flag & 0x1)
#define PCIE_TLP_ATTR2(tlp) (((tlp)->flag >> 2) & 0x1)
#define PCIE_TLP_TC(tlp) (((tlp)->flag >> 4) & 0x7)
#define PCIE_TLP_LENGTH(tlp) ((tlp)->len_attr & 0x3ff)
#define PCIE_TLP_AT(tlp) (((tlp)->len_attr >> 10) & 0x3)
#define PCIE_TLP_ATTR(tlp) (((tlp)->len_attr >> 12) & 0x3)
#define PCIE_TLP_EP(tlp) (((tlp)->len_attr >> 14) & 0x1)
#define PCIE_TLP_TD(tlp) (((tlp)->len_attr >> 15) & 0x1)

#define PCIE_SET_TLP_FMT(tlp, v) BIT_OFF_SETTO((tlp)->fmt_type, 0x3, v, 5)
#define PCIE_SET_TLP_TYPE(tlp, v) BIT_OFF_SETTO((tlp)->fmt_type, 0x1f, v, 0)
#define PCIE_SET_TLP_TH(tlp, v) BIT_OFF_SETTO((tlp)->flag, 0x1, v, 0)
#define PCIE_SET_TLP_ATTR2(tlp, v) BIT_OFF_SETTO((tlp)->flag , 0x1, v, 2)
#define PCIE_SET_TLP_TC(tlp, v) BIT_OFF_SETTO((tlp)->flag , 0x3, v, 4)
#define PCIE_SET_TLP_LENGTH(tlp, v) BIT_OFF_SETTO((tlp)->len_attr, 0x3ff, v, 0)
#define PCIE_SET_TLP_AT(tlp, v) BIT_OFF_SETTO((tlp)->len_attr, 0x3, v, 10)
#define PCIE_SET_TLP_ATTR(tlp, v) BIT_OFF_SETTO((tlp)->len_attr, 0x3, v, 12)
#define PCIE_SET_TLP_EP(tlp, v) BIT_OFF_SETTO((tlp)->len_attr, 0x1, v, 14)
#define PCIE_SET_TLP_TD(tlp, v) BIT_OFF_SETTO((tlp)->len_attr, 0x1, v, 15)


typedef struct {
    unsigned char fmt_type;
    unsigned char flag;
    unsigned short len_attr;

    unsigned short request_id;
    unsigned char tag;
    unsigned char dw;

    unsigned short bdf;
    unsigned short reg_num;
}PCIE_TLP_CFG_S;

#define PCIE_TLP_CFG_FIRST_BE(tlp) ((tlp)->dw & 0xf)
#define PCIE_TLP_CFG_LAST_BE(tlp) (((tlp)->dw >> 4) & 0xf)
#define PCIE_TLP_CFG_REG_NUM(tlp) (((tlp)->reg_num >> 2) & 0x3ff)
#define PCIE_TLP_CFG_BDF(tlp) ((tlp)->bdf)

#define PCIE_SET_TLP_CFG_FIRST_BE(tlp, v) BIT_OFF_SETTO((tlp)->dw, 0xf, v, 0)
#define PCIE_SET_TLP_CFG_LAST_BE(tlp, v) BIT_OFF_SETTO((tlp)->dw, 0xf, v, 4)
#define PCIE_SET_TLP_CFG_REG_NUM(tlp, v) BIT_OFF_SETTO((tlp)->reg_num, 0x3ff, v, 2)
#define PCIE_SET_TLP_CFG_BDF(tlp, v) BIT_OFF_SETTO((tlp)->bdf, 0xffff, v, 0)

typedef struct {
    unsigned char fmt_type;
    unsigned char flag;
    unsigned short len_attr;

    unsigned short request_id;
    unsigned char tag;
    unsigned char dw;

    unsigned int addr1;
    unsigned int addr2;
}PCIE_TLP_MEM_S;

typedef struct {
    unsigned char fmt_type;
    unsigned char flag;
    unsigned short len_attr;

    unsigned short completer_id;
    unsigned short status_count;

    unsigned short request_id;
    unsigned char tag;
    unsigned char lower_addr;
}PCIE_TLP_COMPLETE_S;

#define PCIE_TLP_COMP_STATUS(tlp) (((tlp)->status_count >> 13) & 0x7)
#define PCIE_TLP_COMP_BCM(tlp) (((tlp)->status_count >> 12) & 0x1)
#define PCIE_TLP_COMP_BYTE_COUNT(tlp) (((tlp)->status_count) & 0xfff)
#define PCIE_TLP_COMP_LOWER_ADDR(tlp) (((tlp)->lower_addr) & 0x7f)

#define PCIE_SET_TLP_COMP_STATUS(tlp, v) BIT_OFF_SETTO((tlp)->status_count, 0x7, v, 13)
#define PCIE_SET_TLP_COMP_BCM(tlp, v) BIT_OFF_SETTO((tlp)->status_count, 0x1, v, 12)
#define PCIE_SET_TLP_COMP_BYTE_COUNT(tlp, v) BIT_OFF_SETTO((tlp)->status_count, 0xfff, v, 0)
#define PCIE_SET_TLP_COMP_LOWER_ADDR(tlp, v) BIT_OFF_SETTO((tlp)->lower_addr, 0x7f, v, 0)

typedef struct {
    PCIE_TLP_COMPLETE_S tlp;
    UINT data1;
    UINT data2;
}PCIE_TLP_CFG_CPLD_S;

int PCIE_CheckTlpCommonHdr(void *tlp, int tlp_len);
int PCIE_CheckTlp(void *tlp, int tlp_len);
int PCIE_TlpCheckAndChangeOrder(void *msg, int len);
int PCIE_GetTlpType(PCIE_TLP_HEADER_S *tlp);
BOOL_T PCIE_TLPIs4DW(void *tlp);

void PCIE_BuildCfgTLP(UCHAR fmt, UCHAR type, USHORT bdf, UINT addr, int size, OUT PCIE_TLP_CFG_S *tlp);
void PCIE_BuildCfgReadTLP(int is_ep, USHORT bdf, UINT addr, int size, OUT PCIE_TLP_CFG_S *tlp);
void PCIE_BuildCfgWriteTLP(int is_ep, USHORT bdf, UINT addr, int size, UINT val, OUT PCIE_TLP_CFG_S *tlp);
void PCIE_BuildMemTLP(UCHAR fmt, UCHAR type, USHORT bdf, UINT64 addr, BOOL_T is64, int size, OUT PCIE_TLP_MEM_S *tlp);
void PCIE_BuildEpMemReadTLP(USHORT bdf, UINT64 addr, BOOL_T is64, int size, OUT PCIE_TLP_MEM_S *tlp);
void PCIE_BuildEpMemWriteTLP(USHORT bdf, UINT64 addr, BOOL_T is64, int size, void *data, OUT PCIE_TLP_MEM_S *tlp);
void PCIE_BuildCplTLP(USHORT comp_id, USHORT req_id, OUT PCIE_TLP_COMPLETE_S *tlp);
void PCIE_BuildCpldTLP(USHORT comp_id, USHORT req_id, UINT byte_count,
        UINT low_addr, int size, void *data, OUT PCIE_TLP_COMPLETE_S *tlp);

UINT64 PCIE_GetMemTlpAddr(PCIE_TLP_MEM_S *tlp);
void * PCIE_GetTlpDataPtr(void *tlp);
UINT PCIE_GetTlpDataLength(PCIE_TLP_HEADER_S *tlp);
void PCIE_WriteTlpData(void *data, int data_len, BOOL_T is4dw, OUT PCIE_TLP_S *tlp);
int PCIE_ReadTlpData(UINT64 addr, OUT void *data, int data_size, BOOL_T is4dw, PCIE_TLP_S *tlp);


void PCIE_ChangeTlpOrder(INOUT void *tlp);

static inline UINT PCIE_FirstBe2LowAddr(UINT addr, UCHAR first_be)
{
    UINT low_addr = addr & 0x7f;
    low_addr += BIT_GetLowIndex(first_be);
    return low_addr;
}


static inline UCHAR PCIE_BeSettedCount(UCHAR be)
{
    int i;
    int count = 0;
    for (i=0; i<4; i++) {
        if (be & (1 << i)) {
            count ++;
        }
    }
    return count;
}


static inline UCHAR PCIE_BeUnsetCount(UCHAR be)
{
    return 4 - PCIE_BeSettedCount(be);
}

static inline UINT64 PCIE_GetLastDwAddr(UINT64 begin_addr, UINT length)
{
    return (begin_addr + length) - 4;
}

#ifdef __cplusplus
}
#endif
#endif 

