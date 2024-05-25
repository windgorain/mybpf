/*********************************************************
*   Copyright (C) LiXingang
*   Description: 
*
********************************************************/
#ifndef _PCI_DMA_H
#define _PCI_DMA_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef int (*PF_PCI_DMA_WRITE)(UINT64 addr, void *data, int len, void *user_data);
typedef int (*PF_PCI_DMA_READ)(UINT64 addr, OUT void *buf, int len, void *user_data);
typedef int (*PF_PCI_SEND_DATA)(void *data, int len, void *user_data);

int PCI_DMA_ProcessMwr(void *msg, int len, PF_PCI_DMA_WRITE dma_write_func, void *user_data);
int PCI_DMA_ProcessMrd(PCIE_TLP_MEM_S *tlp, int len,
        PF_PCI_DMA_READ dma_read_func,
        PF_PCI_SEND_DATA send_func,
        void *user_data);

#ifdef __cplusplus
}
#endif
#endif 
