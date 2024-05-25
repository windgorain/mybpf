/*********************************************************
*   Copyright (C) LiXingang
*   Description: 
*
********************************************************/
#ifndef _PCI_TAG_H
#define _PCI_TAG_H
#include "utl/pci_utl.h"
#include "utl/mutex_utl.h"
#include "utl/sem_utl.h"
#ifdef __cplusplus
extern "C"
{
#endif

#define PCIE_TAG_MAX 256

typedef struct {
    unsigned int used:1;
    unsigned int reserved1: 7;
    unsigned int tag:8;
    SEM_HANDLE sem;
    PCIE_TLP_S tlp;
}PCIE_TAG_NODE_S;

typedef struct {
    PCIE_TAG_NODE_S node[PCIE_TAG_MAX];
    MUTEX_S lock;
    UCHAR tag_begin; 
}PCIE_TAG_S;

void PCIE_TAG_Init(OUT PCIE_TAG_S *tags);
int PCIE_TAG_AllocTag(PCIE_TAG_S *tags);
void PCIE_TAG_FreeTag(PCIE_TAG_S *tags, int tag);
int PCIE_TAG_WriteTlp(PCIE_TAG_S *tags, void *tlp, int tlp_len);
PCIE_TLP_S * PCIE_TAG_WaitTlp(PCIE_TAG_S *tags, int tag, UINT timeout_ms);

#ifdef __cplusplus
}
#endif
#endif 
