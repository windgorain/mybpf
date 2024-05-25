/*********************************************************
*   Copyright (C) LiXingang
*   Description: 
*
********************************************************/
#include "bs.h"
#include "utl/pci_utl.h"
#include "utl/pci_dma.h"
#include "utl/bit_opt.h"

#define PCIE_RCB 512

static void _pci_dma_process_mwr_be(UINT64 addr, UCHAR *data, UCHAR be,
        PF_PCI_DMA_WRITE dma_write_func, void *user_data)
{
    int i;

    for (i=0; i<4; i++) {
        if (be & (1 << i)) {
            dma_write_func(addr + i, data + i, 1, user_data);
        }
    }
}

int PCI_DMA_ProcessMwr(void *msg, int len, PF_PCI_DMA_WRITE dma_write_func, void *user_data)
{
    PCIE_TLP_MEM_S *tlp = msg;
    UINT64 addr;
    UINT64 last_dw_addr;
    UCHAR first_be;
    UCHAR last_be;
    int length;
    UCHAR *data;

    length = PCIE_GetTlpDataLength(msg);
    first_be = PCIE_TLP_CFG_FIRST_BE(tlp);
    last_be = PCIE_TLP_CFG_LAST_BE(tlp);
    data = PCIE_GetTlpDataPtr(tlp);
    addr = PCIE_GetMemTlpAddr(msg);
    last_dw_addr = PCIE_GetLastDwAddr(addr, length);

    
    if ((addr & 0xfff00000) == 0xfee00000) {
        return dma_write_func(addr, data, length, user_data);
    }

    if (first_be != 0xf) {
        _pci_dma_process_mwr_be(addr, data, first_be, dma_write_func, user_data);
        length -= 4;
        if (length <= 0) {
            return 0;
        }

        addr += 4;
        data += 4;
    }

    if ((last_be) && (last_be != 0xf)) {
        length -= 4;
        _pci_dma_process_mwr_be(last_dw_addr, data + length, last_be, dma_write_func, user_data);
    }

    if (length > 0) {
        dma_write_func(addr, data, length, user_data);
    }

    return 0;
}


int PCI_DMA_ProcessMrd(PCIE_TLP_MEM_S *tlp, int len,
        PF_PCI_DMA_READ dma_read_func,
        PF_PCI_SEND_DATA send_func,
        void *user_data)
{
    UINT64 addr;
    UINT64 last_dw_addr;
    UCHAR first_be;
    UCHAR last_be;
    int length;
    PCIE_TLP_S reply_msg = {0};
    PCIE_TLP_COMPLETE_S *cpld = (void*)&reply_msg;
    UINT low_addr;
    UINT last_drop = 0;
    UINT byte_count;
    UINT size;
    UINT copy_size;
    char buf[PCIE_RCB];
    UCHAR tag;

    length = PCIE_GetTlpDataLength((void*)tlp);
    first_be = PCIE_TLP_CFG_FIRST_BE(tlp);
    last_be = PCIE_TLP_CFG_LAST_BE(tlp);
    addr = PCIE_GetMemTlpAddr(tlp);
    last_dw_addr = PCIE_GetLastDwAddr(addr, length);
    low_addr = PCIE_FirstBe2LowAddr(addr, first_be);
    tag = tlp->tag;

    
    if (last_be) {
        last_drop = 3 - BIT_GetHighIndexFrom(last_be, 3);
    }

    byte_count = length;
    byte_count -= last_drop;
    byte_count -= PCIE_BeUnsetCount(first_be);

    while (length > 0) {
        size = PCIE_RCB - (addr % PCIE_RCB);
        size = MIN(size, length);
        copy_size = size - (low_addr & 0x3);
        copy_size = MIN(copy_size, byte_count);
        if (addr + copy_size > last_dw_addr) {
            copy_size -= last_drop;
        }
        dma_read_func(addr + (low_addr & 0x3), &buf[low_addr & 0x3], copy_size, user_data);
        memset(&reply_msg, 0, PCIE_TLP_4DW);
        PCIE_BuildCpldTLP(0, tlp->request_id, byte_count, low_addr, size, buf, (void*)&reply_msg);
        cpld->tag = tag;
        send_func(&reply_msg, PCIE_TLP_3DW + size, user_data);
        length -= size;
        byte_count -= copy_size;
        addr += size;
        low_addr = 0;
    }

    return 0;
}


